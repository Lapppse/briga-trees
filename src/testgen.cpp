#include <array>
#include <climits>
#include <ctime>
#include <filesystem>
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
enum Direction { DIR_DOWN = -1, DIR_UP = 1 };

struct ContiguousStep {
  int n, start_value;
  Direction dir;
  Operation op;

  ContiguousStep(int n, int start_value, Direction dir, Operation op)
      : start_value(start_value), n(n), dir(dir), op(op) {}

  ContiguousStep() : start_value(0), n(1), dir(DIR_UP), op(OP_START) {}
};

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

void generate_contiguous_test(const string &filename,
                              const vector<ContiguousStep> &steps) {
  ofstream fout(filename);
  size_t q_tot = 0;
  for (const ContiguousStep &s : steps) {
    q_tot += s.n;
  }
  fout << q_tot << '\n';
  for (const ContiguousStep &s : steps) {
    // cout << s.start_value << endl;
    for (int i = s.start_value, j = 0; j < s.n; i += 1 * s.dir, j++) {
      fout << s.op << ' ' << i << '\n';
    }
  }
  fout.flush();
  fout.close();
}

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
  const int queries;
  TestGroup(string name, int queries) : name(name), queries(queries) {}
};

int main() {
  const string base_filename = "./test/1_random";
  // const array<TestGroup, 3> groups = {TestGroup("small", 1'000),
  //                                     TestGroup("medium", 100'000),
  //                                     TestGroup("large", 10'000'000)};
  // for (const auto &i : groups) {
  // 1_random
  // const vector<Step> steps = {Step(), Step(i.queries, INT_MAX - 1,
  //                                          {OP_INSERT, OP_SEARCH, OP_DELETE},
  //                                          {1, 1, 1}, false, false)};
  // // 2_small_k_random
  // const vector<Step> steps = {Step(), Step(i.queries, 1'000,
  //                                          {OP_INSERT, OP_DELETE,
  //                                          OP_SEARCH}, {1, 1, 1}, false,
  //                                          false)};
  // 3_eff_random
  // const vector<Step> steps = {Step(), Step(i.queries, INT_MAX - 1,
  //                                          {OP_INSERT, OP_DELETE,
  //                                          OP_SEARCH}, {1, 1, 1}, true,
  //                                          true)};
  // 4_only_inserts
  // const vector<Step> steps = {
  //     Step(), Step(i.queries, INT_MAX - 1, {OP_INSERT}, {1}, false,
  //     false)};
  // 5_only_searches
  // const vector<Step> steps = {
  //     Step(i.queries, INT_MAX - 1, {OP_INSERT}, {1}, true, true), Step(),
  //     Step(i.queries, INT_MAX - 1, {OP_SEARCH}, {1}, true, true)};
  // 6_only_deletes
  // const vector<Step> steps = {
  //     Step(i.queries, INT_MAX - 1, {OP_INSERT}, {1}, true, true), Step(),
  //     Step(3 * i.queries, INT_MAX - 1, {OP_DELETE}, {1}, true, true)};

  // generate_test
  // const string prep = format("{}/{}", base_filename, i.name);
  // filesystem::create_directories(prep);
  // for (int j = 1; j <= 20; j++) {
  //   string filename = format("{}/{}.txt", prep, j);
  //   generate_test(steps, filename);
  //   cout << "generated " << filename << '\n';
  // }

  // Operation op_first = OP_INSERT;
  // int n = i.queries / 5;
  // 7_contiguous_inserts
  // vector<ContiguousStep> steps = {
  //     ContiguousStep(),
  //     ContiguousStep(n, INT_MAX / 2, DIR_UP, op_first),
  //     ContiguousStep(n, INT_MAX / 2 - 1, DIR_DOWN, op_first),
  //     ContiguousStep(n, 1, DIR_UP, op_first),
  //     ContiguousStep(n, INT_MAX / 4, DIR_UP, op_first),
  //     ContiguousStep(n, INT_MAX / 4 * 3, DIR_DOWN, op_first)};
  //  8_contiguous_deletes
  // Operation op_second = OP_DELETE;
  // vector<ContiguousStep> steps = {
  //     ContiguousStep(3 * n, INT_MAX / 2, DIR_UP, op_first),
  //     ContiguousStep(3 * n, INT_MAX / 2 - 1, DIR_DOWN, op_first),
  //     ContiguousStep(3 * n, 1, DIR_UP, op_first),
  //     ContiguousStep(3 * n, INT_MAX / 4, DIR_UP, op_first),
  //     ContiguousStep(3 * n, INT_MAX / 4 * 3, DIR_DOWN, op_first),
  //     ContiguousStep(),
  //     ContiguousStep(n, INT_MAX / 2, DIR_UP, op_second),
  //     ContiguousStep(n, INT_MAX / 2 - 1, DIR_DOWN, op_second),
  //     ContiguousStep(n, 1, DIR_UP, op_second),
  //     ContiguousStep(n, INT_MAX / 4, DIR_UP, op_second),
  //     ContiguousStep(n, INT_MAX / 4 * 3, DIR_DOWN, op_second)};
  // 9_contiguous_searches
  // Operation op_second = OP_SEARCH;
  // vector<ContiguousStep> steps = {
  //     ContiguousStep(n, INT_MAX / 2, DIR_UP, op_first),
  //     ContiguousStep(n, INT_MAX / 2 - 1, DIR_DOWN, op_first),
  //     ContiguousStep(n, 1, DIR_UP, op_first),
  //     ContiguousStep(n, INT_MAX / 4, DIR_UP, op_first),
  //     ContiguousStep(n, INT_MAX / 4 * 3, DIR_DOWN, op_first),
  //     ContiguousStep(),
  //     ContiguousStep(n, INT_MAX / 2, DIR_UP, op_second),
  //     ContiguousStep(n, INT_MAX / 2 - 1, DIR_DOWN, op_second),
  //     ContiguousStep(n, 1, DIR_UP, op_second),
  //     ContiguousStep(n, INT_MAX / 4, DIR_UP, op_second),
  //     ContiguousStep(n, INT_MAX / 4 * 3, DIR_DOWN, op_second)};

  // generate_contiguous_test
  // filesystem::create_directories(format("{}/{}", base_filename, i.name));
  // const string filename = format("{}/{}/1.txt", base_filename, i.name);
  // generate_contiguous_test(filename, steps);
  // cout << "generated " << filename << '\n';
}
}
