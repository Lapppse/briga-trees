#include <climits>
#include <fstream>
#include <iostream>
#include <random>
#include <utility>

using namespace std;

random_device rng;
mt19937 gen(rng());
uniform_int_distribution<int> dst(INT_MIN, INT_MAX);

struct node {
  int x, pr;
  node *l = 0, *r = 0;
  node(int _x) {
    x = _x;
    pr = rand();
  }
};

pair<node *, node *> split(node *t, int k) {
  if (!t)
    return {0, 0};
  if (t->x < k) {
    pair<node *, node *> e = split(t->r, k);
    t->r = e.first;
    return {t, e.second};
  } else {
    pair<node *, node *> e = split(t->l, k);
    t->l = e.second;
    return {e.first, t};
  }
}

node *merge(node *t1, node *t2) {
  if (!t1)
    return t2;
  if (!t2)
    return t1;
  if (t1->pr > t2->pr) {
    node *e = merge(t1->r, t2);
    t1->r = e;
    return t1;
  } else {
    node *e = merge(t1, t2->l);
    t2->l = e;
    return t2;
  }
}

node *insert(node *t, int x) {
  pair<node *, node *> e = split(t, x);
  node *n = new node(x);
  e.first = merge(e.first, n);
  return merge(e.first, e.second);
}

node *erase(node *t, int x) {
  pair<node *, node *> e = split(t, x);
  node *d = e.second;
  if (!d)
    return merge(e.first, e.second);
  if (d->l) {
    while (d->l->l)
      d = d->l;
    if (x == d->l->x)
      d->l = d->l->r;
  } else {
    if (x == d->x)
      e.second = d->r;
  }
  return merge(e.first, e.second);
}

node *eraseall(node *t, int x) {
  pair<node *, node *> e = split(t, x);
  e.second = split(e.second, x + 1).second;
  return merge(e.first, e.second);
}

bool find(node *t, int x) {
  while (t) {
    if (t->x == x)
      return true;
    if (x < t->x)
      t = t->l;
    else
      t = t->r;
  }
  return false;
}

int main(int argc, char **argv) {
  ios::sync_with_stdio(0);
  cin.tie(0);

  ifstream fin("input.txt");
  ofstream fout("output.txt");
  int n, k, x;
  fin >> n;
  node *t = 0;
  while (n--) {
    fin >> k >> x;
    if (k == 1) {
      t = insert(t, x);
    } else if (k == 2) {
      t = erase(t, x);
    } else if (k == 3) {
      find(t, x);
    } else {
      break;
    }
  }
  clock_t start_time = clock();
  while (n--) {
    fin >> k >> x;
    if (k == 1) {
      t = insert(t, x);
    } else if (k == 2) {
      t = erase(t, x);
    } else {
      bool res = find(t, x);
    }
  }
  fout << double(clock() - start_time) / CLOCKS_PER_SEC;

  return 0;
}
