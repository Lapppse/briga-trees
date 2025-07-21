#include <climits>
#include <fstream>
#include <iostream>
#include <random>
#include <set>

using namespace std;

random_device rng;
mt19937 gen(rng());
uniform_int_distribution<int> dst(INT_MIN, INT_MAX);

int main(int argc, char **argv) {
  ios::sync_with_stdio(0);
  cin.tie(0);

  ifstream fin("input.txt");
  ofstream fout("output.txt");
  int n, k, x;
  fin >> n;
  multiset<int> t;
  while (n--) {
    fin >> k >> x;
    if (k == 1) {
      t.insert(x);
    } else if (k == 2) {
      auto it = t.find(x);
      if (it != t.end()) {
        t.erase(it);
      }
    } else if (k == 3) {
      bool res = t.find(x) != t.end();
    } else {
      break;
    }
  }
  vector<int> v;
  v.reserve(n);
  clock_t start_time = clock();
  while (n--) {
    fin >> k >> x;
    if (k == 1) {
      t.insert(x);
    } else if (k == 2) {
      auto it = t.find(x);
      if (it != t.end()) {
        t.erase(it);
      }
    } else {
      v.push_back(t.find(x) != t.end());
    }
  }
  for (int i : v) {
    cout << i;
  }
  fout << double(clock() - start_time) / CLOCKS_PER_SEC;

  return 0;
}

// static void testt(benchmark::State &state) {
//   ios::sync_with_stdio(0);
//   cin.tie(0);
//
//   ifstream fin("input.txt");
//   ofstream fout("output.txt");
//   int n, k, x;
//   fin >> n;
//   multiset<int> t;
//   while (n--) {
//     fin >> k >> x;
//     if (k == 1) {
//       t.insert(x);
//     } else if (k == 2) {
//       auto it = t.find(x);
//       if (it != t.end()) {
//         t.erase(it);
//       }
//     } else if (k == 3) {
//       bool res = t.find(x) != t.end();
//     } else {
//       break;
//     }
//   }
//   vector<int> v;
//   // v.reserve(n);
//   for (auto _ : state) {
//     while (n--) {
//       fin >> k >> x;
//       if (k == 1) {
//         t.insert(x);
//       } else if (k == 2) {
//         auto it = t.find(x);
//         if (it != t.end()) {
//           t.erase(it);
//         }
//       } else {
//         v.push_back(t.find(x) != t.end());
//       }
//     }
//   }
//   for (int i : v) {
//     fout << i << ' ';
//   }
// }
//
// BENCHMARK(testt);
//
// BENCHMARK_MAIN();
