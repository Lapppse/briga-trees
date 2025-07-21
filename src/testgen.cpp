#include <array>
#include <climits>
#include <format>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <random>
#include <vector>

using namespace std;

enum Operation { OP_INSERT = 1, OP_DELETE, OP_SEARCH, OP_START };

struct Step {
  size_t n;
  int kmin, kmax;
  const vector<short int> ops;
  const vector<int> probs;
  bool pure_deletes, pure_searches;

  Step(size_t n, int kmin, int kmax, const vector<short int> &ops,
       const vector<int> &probs, bool pure_deletes, bool pure_searches)
      : n(n), kmin(kmin), kmax(kmax), ops(ops), probs(probs),
        pure_deletes(pure_deletes), pure_searches(pure_searches) {}

  Step(size_t n, int kmax, const vector<short int> &ops,
       const vector<int> &probs, bool pure_deletes, bool pure_searches)
      : n(n), kmin(0), kmax(kmax), ops(ops), probs(probs),
        pure_deletes(pure_deletes), pure_searches(pure_searches) {}

  Step()
      : n(1), kmin(0), kmax(0), ops({4}), probs({1}), pure_deletes(false),
        pure_searches(false) {}
};

class RandomPriorityQueue {
private:
  struct Item {
    int value;
    unsigned long long
        priority; // 64-bit priority for low collision probability

    // Compare items by priority (higher priority = larger value)
    bool operator<(const Item &other) const {
      return priority < other.priority;
    }
  };

  priority_queue<Item> pq;
  mt19937_64 generator;
  uniform_int_distribution<unsigned long long> distribution;

public:
  RandomPriorityQueue()
      : distribution(0, numeric_limits<unsigned long long>::max()) {
    // Seed with true random value if available, otherwise use fallback
    random_device rd;
    if (rd.entropy() != 0) {
      generator.seed(rd());
    } else {
      // Fallback seed (using current time)
      generator.seed(time(0));
    }
  }

  // Insert value with random priority
  void push(int value) {
    unsigned long long priority = distribution(generator);
    pq.push(Item{value, priority});
  }

  // Get top value (highest random priority)
  int top() const { return pq.top().value; }

  // Remove top element
  void pop() { pq.pop(); }

  // Check if empty
  bool empty() const { return pq.empty(); }

  // Get number of elements
  size_t size() const { return pq.size(); }
};

void generate_test(const vector<Step> &steps, const string &filename) {
  random_device dev;
  mt19937 rng(dev());
  ofstream fout(filename);
  auto comp = [&rng](int a, int b) { return rng() & 1; };

  size_t q_tot = 0;
  for (const Step &s : steps) {
    q_tot += s.n;
  }
  fout << q_tot << '\n';

  RandomPriorityQueue added;
  for (const Step &s : steps) {
    int probsum = reduce(s.probs.cbegin(), s.probs.cend());
    vector<short int> ops_choice(probsum);
    auto curprob = ops_choice.begin();
    for (size_t i = 0; i < s.ops.size(); i++) {
      fill_n(curprob, s.probs[i], s.ops[i]);
      curprob += s.probs[i];
    }
    uniform_int_distribution<short int> opdist(0, probsum - 1);
    uniform_int_distribution<int> kdist(s.kmin, s.kmax);

    for (size_t i = 0; i < s.n; i++) {
      int cur_op = ops_choice[opdist(rng)], cur_k = kdist(rng);

      if ((s.pure_deletes || s.pure_searches) && cur_op == OP_INSERT) {
        added.push(cur_k);
      } else if (s.pure_deletes && cur_op == OP_DELETE) {
        if (added.empty()) {
          fout << cur_op << ' ' << cur_k << '\n';
          continue;
        }
        fout << cur_op << ' ' << added.top() << '\n';
        added.pop();
        continue;
      } else if (s.pure_searches && cur_op == OP_SEARCH) {
        if (!added.empty()) {
          cur_k = added.top();
        }
      }

      fout << cur_op << ' ' << cur_k << '\n';
    }
  }
  fout.flush();
  fout.close();
}

struct TestGroup {
  const string name;
  const int tests;
  TestGroup(string name, int tests) : name(name), tests(tests) {}
};

int main() {
  const string base_filename = "./test/5_only_searches";
  const array<TestGroup, 3> groups = {TestGroup("small", 1'000),
                                      TestGroup("medium", 100'000),
                                      TestGroup("large", 10'000'000)};
  for (const auto &i : groups) {
    // 5_only_searches
    const vector<Step> steps = {
        Step(i.tests, INT_MAX - 1, {OP_INSERT}, {1}, true, true), Step(),
        Step(i.tests, INT_MAX - 1, {OP_SEARCH}, {1}, true, true)};
    // const vector<Step> steps = {
    //     Step(i.tests, INT_MAX - 1, {OP_INSERT}, {1}, false, false)};
    // const vector<Step> steps = {
    //     Step(i.tests, INT_MAX - 1, {OP_INSERT}, {1}, false, false)};
    const string prep = format("{}/{}/", base_filename, i.name);
    for (int j = 1; j <= 20; j++) {
      string filename = prep + format("{}.txt", j);
      generate_test(steps, filename);
      cout << "generated " << filename << '\n';
    }
  }
}
