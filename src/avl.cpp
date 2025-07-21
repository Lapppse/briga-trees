#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

struct node {
  int x, d = 1;
  node *l = 0, *r = 0, *p = 0;
  int diff = 0;
  node(node *_p, int _x) {
    p = _p;
    x = _x;
  }
};

void updbal(node *t) {
  int l = t->l ? t->l->d : 0, r = t->r ? t->r->d : 0;
  t->d = max(l, r) + 1;
  t->diff = l - r;
}

node *grandp(node *t) {
  if (t && t->p)
    return t->p->p;
  return 0;
}

void rleft(node *t) {
  node *g = grandp(t), *p = t->p, *b = t->l;
  p->r = b, p->p = t;
  updbal(p);
  t->l = p, t->p = g;
  updbal(t);
  if (g) {
    if (p == g->l)
      g->l = t;
    else
      g->r = t;
    updbal(g);
  }
  if (b)
    b->p = p;
}

void rright(node *t) {
  node *g = grandp(t), *p = t->p, *b = t->r;
  p->l = b, p->p = t;
  updbal(p);
  t->r = p, t->p = g;
  updbal(t);
  if (g) {
    if (p == g->r)
      g->r = t;
    else
      g->l = t;
    updbal(g);
  }
  if (b)
    b->p = p;
}

void bigrleft(node *t) {
  rright(t);
  rleft(t);
}

void bigrright(node *t) {
  rleft(t);
  rright(t);
}

node *balance(node *t) {
  if (t->diff == 2) {
    node *b = t->l;
    if (b->diff == 1) {
      rright(b);
      return b;
    } else if (b->diff == 0) {
      rright(b);
      return b;
    } else {
      node *c = b->r;
      bigrright(c);
      return c;
    }
  } else if (t->diff == -2) {
    node *b = t->r;
    if (b->diff == -1) {
      rleft(b);
      return b;
    } else if (b->diff == 0) {
      rleft(b);
      return b;
    } else {
      node *c = b->l;
      bigrleft(c);
      return c;
    }
  }
  return t;
}

node *insert(node *t, int x) {
  if (!t)
    return new node(0, x);
  node *p = 0;
  while (t) {
    p = t;
    if (x < t->x)
      t = t->l;
    else
      t = t->r;
  }
  t = new node(p, x);
  if (x < p->x)
    p->l = t;
  else
    p->r = t;
  while (t->p) {
    t = t->p;
    updbal(t);
    t = balance(t);
  }
  return t;
}

node *erase(node *t, int x) {
  node *root = t;
  while (t) {
    if (x < t->x)
      t = t->l;
    else if (x > t->x)
      t = t->r;
    else
      break;
  }
  if (!t)
    return root;
  if (t->l && t->r) {
    node *tt = t;
    t = t->r;
    while (t->l)
      t = t->l;
    tt->x = t->x;
  }
  if (t->p) {
    bool l = t == t->p->l;
    node *s = t->l ? t->l : t->r;
    if (s)
      s->p = t->p;
    if (l)
      t->p->l = s;
    else
      t->p->r = s;
    while (t->p) {
      t = t->p;
      updbal(t);
      t = balance(t);
    }
    return t;
  } else {
    if (t->l) {
      t->l->p = 0;
      return t->l;
    } else if (t->r) {
      t->r->p = 0;
      return t->r;
    } else
      return 0;
  }
}

bool find(node *t, int x) {
  while (t) {
    if (x < t->x)
      t = t->l;
    else if (x > t->x)
      t = t->r;
    else
      return true;
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
