#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

struct node {
  int x = -1;
  node *l = 0, *r = 0, *p;
  bool c = false;
  node(node *_p) { p = _p; }
};

node *root(node *t) {
  while (t->p)
    t = t->p;
  return t;
}

node *grandp(node *t) {
  if (t && t->p)
    return t->p->p;
  return 0;
}

node *uncle(node *t) {
  node *g = grandp(t);
  if (g)
    return t->p == g->l ? g->r : g->l;
  return 0;
}

node *brother(node *t) {
  if (t->p)
    return t == t->p->l ? t->p->r : t->p->l;
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
  b->p = p;
}

node *insert(node *t, int x) {
  while (t->l) {
    if (x < t->x)
      t = t->l;
    else
      t = t->r;
  }
  t->x = x;
  t->c = true;
  t->l = new node(t);
  t->r = new node(t);
  while (true) {
    if (!t->p) {
      t->c = false;
      return t;
    }
    if (!t->p->c)
      return root(t);
    node *g = grandp(t), *u = uncle(t);
    if (u->c) {
      t->p->c = u->c = false;
      g->c = true;
      t = g;
      continue;
    }
    if (t->p == g->l && t == t->p->r) {
      rleft(t);
      t = t->l;
    } else if (t->p == g->r && t == t->p->l) {
      rright(t);
      t = t->r;
    }
    if (t == t->p->l)
      rright(t->p);
    else
      rleft(t->p);
    t->p->c = false;
    g->c = true;
    return root(t);
  }
}

node *erase(node *t, int x) {
  while (t->l) {
    if (t->x == x)
      break;
    if (x < t->x)
      t = t->l;
    else
      t = t->r;
  }
  if (!t->l)
    return root(t);
  if (t->l->l && t->r->l) {
    node *tt = t->r;
    while (tt->l->l)
      tt = tt->l;
    t->x = tt->x;
    t = tt;
  }
  node *c = t->l->l ? t->l : t->r;
  if (!t->p) {
    c->p = 0;
    c->c = false;
    return c;
  }
  if (t == t->p->l)
    t->p->l = c;
  else
    t->p->r = c;
  c->p = t->p;
  if (t->c)
    return root(t);
  if (c->c) {
    c->c = false;
    return root(t);
  }
  t = c;
  while (true) {
    if (!t->p)
      return t;
    node *s = brother(t);
    if (s->c) {
      if (t == t->p->l)
        rleft(s);
      else
        rright(s);
      t->p->c = true;
      s->c = false;
      s = brother(t);
    }
    if (!s->l->c && !s->r->c) {
      if (t->p->c) {
        t->p->c = false;
        s->c = true;
        return root(t);
      } else {
        s->c = true;
        t = t->p;
        continue;
      }
    }
    if (t == t->p->l && s->l->c) {
      rright(s->l);
      s->c = true;
      s->p->c = false;
      s = s->p;
    } else if (t == t->p->r && s->r->c) {
      rleft(s->r);
      s->c = true;
      s->p->c = false;
      s = s->p;
    }
    if (t == t->p->l && s->r->c) {
      rleft(s);
      swap(s->c, t->p->c);
      s->r->c = false;
      return root(t);
    } else {
      rright(s);
      swap(s->c, t->p->c);
      s->l->c = false;
      return root(t);
    }
  }
}

bool find(node *t, int x) {
  while (t->l) {
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
  node *t = new node(0);
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
