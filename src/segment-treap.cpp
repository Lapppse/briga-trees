#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

const int inf = 2e9;

struct node {
  int size = 1, pr, x, min;
  node *l = 0, *r = 0;
  node(int _x) {
    min = x = _x;
    pr = rand();
  }
};

int size(node *t) { return t ? t->size : 0; }

void updnode(node *t) {
  if (t) {
    t->size = size(t->l) + size(t->r) + 1;
    t->min = min(min(t->l ? t->l->min : inf, t->r ? t->r->min : inf), t->x);
  }
}

pair<node *, node *> split(node *t, int k) {
  if (!k)
    return {0, t};
  if (k <= size(t->l)) {
    pair<node *, node *> e = split(t->l, k);
    t->l = e.second;
    updnode(t);
    return {e.first, t};
  } else {
    pair<node *, node *> e = split(t->r, k - size(t->l) - 1);
    t->r = e.first;
    updnode(t);
    return {t, e.second};
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
    updnode(t1);
    return t1;
  } else {
    node *e = merge(t1, t2->l);
    t2->l = e;
    updnode(t2);
    return t2;
  }
}

node *insert(node *t, int k, int x) {
  pair<node *, node *> e = split(t, k);
  node *n = new node(x);
  e.first = merge(e.first, n);
  return merge(e.first, e.second);
}

node *erase(node *t, int k) {
  pair<node *, node *> e = split(t, k);
  e.second = split(e.second, 1).second;
  return merge(e.first, e.second);
}

int minquery(node *t, int l, int r) {
  pair<node *, node *> ab_c = split(t, r + 1);
  pair<node *, node *> a_b = split(ab_c.first, l);
  int ret = a_b.second->min;
  ab_c.first = merge(a_b.first, a_b.second);
  t = merge(ab_c.first, ab_c.second);
  return ret;
}

int main(int argc, char **argv) {
  ios::sync_with_stdio(0);
  cin.tie(0);

  ifstream fin("input.txt");
  ofstream fout("output.txt");
  int n, k, x;
  fin >> n;
  node *t = 0;
  clock_t start_time = clock();
  // cout << "start" << endl;
  while (n--) {
    fin >> k >> x;
    if (k == 1) {
      // cout << "insert" << endl;
      t = insert(t, x);
    } else if (k == 2) {
      // cout << "erase" << endl;
      t = erase(t, x);
    } else {
      // cout << "find" << endl;
      find(t, x);
    }
  }
  fout << double(clock() - start_time) / CLOCKS_PER_SEC;

  return 0;
}
