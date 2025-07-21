#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

struct node {
  int x;
  node *l = 0, *r = 0, *p = 0;
  node(int _x) { x = _x; }
};

node *grandp(node *t) {
  if (t && t->p)
    return t->p->p;
  return 0;
}

void rleft(node *t) {
  node *g = grandp(t), *p = t->p, *b = t->l;
  if (g) {
    if (p == g->l)
      g->l = t;
    else
      g->r = t;
  }
  t->l = p, t->p = g;
  p->r = b, p->p = t;
  if (b)
    b->p = p;
}

void rright(node *t) {
  node *g = grandp(t), *p = t->p, *b = t->r;
  if (g) {
    if (p == g->r)
      g->r = t;
    else
      g->l = t;
  }
  t->r = p, t->p = g;
  p->l = b, p->p = t;
  if (b)
    b->p = p;
}

node *splay(node *t) {
  while (t->p) {
    node *g = grandp(t);
    if (!g) {
      if (t == t->p->l)
        rright(t);
      else
        rleft(t);
      break;
    }
    if (t == t->p->l) {
      if (t->p == g->l) {
        rright(t->p);
        rright(t);
      } else {
        rright(t);
        rleft(t);
      }
    } else {
      if (t->p == g->l) {
        rleft(t);
        rright(t);
      } else {
        rleft(t->p);
        rleft(t);
      }
    }
  }
  return t;
}

pair<node *, node *> split(node *t, int x) {
  if (!t)
    return {0, 0};
  int m = 2e9;
  node *mnode = 0;
  while (true) {
    if (x <= t->x)
      if (t->x <= m)
        m = t->x, mnode = t;
    if (x <= t->x) {
      if (t->l)
        t = t->l;
      else
        break;
    } else if (x > t->x) {
      if (t->r)
        t = t->r;
      else
        break;
    }
  }
  if (m == 2e9) {
    while (t->p)
      t = t->p;
    return {t, 0};
  }
  t = mnode;
  splay(t);
  node *l = t->l;
  if (l) {
    l->p = 0;
    t->l = 0;
  }
  return {l, t};
}

node *merge(node *t1, node *t2) {
  if (!t1)
    return t2;
  if (!t2)
    return t1;
  while (t1->r)
    t1 = t1->r;
  splay(t1);
  t1->r = t2;
  t2->p = t1;
  return t1;
}

node *insert(node *t, int x) {
  pair<node *, node *> e = split(t, x);
  t = new node(x);
  if (e.first) {
    t->l = e.first;
    t->l->p = t;
  }
  if (e.second) {
    t->r = e.second;
    t->r->p = t;
  }
  return t;
}

node *erase(node *t, int x) {
  node *tt = t;
  while (t) {
    if (x < t->x)
      t = t->l;
    else if (x > t->x)
      t = t->r;
    else {
      splay(t);
      if (t->l)
        t->l->p = 0;
      if (t->r)
        t->r->p = 0;
      return merge(t->l, t->r);
    }
  }
  return tt;
}

bool find(node *&tt, int x) {
  node *t = tt;
  while (t) {
    if (x < t->x)
      t = t->l;
    else if (x > t->x)
      t = t->r;
    else {
      splay(t);
      tt = t;
      return true;
    }
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
      find(t, x);
    }
  }
  fout << double(clock() - start_time) / CLOCKS_PER_SEC;

  return 0;
}
