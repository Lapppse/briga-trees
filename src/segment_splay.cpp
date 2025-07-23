#include <algorithm>
#include <climits>

using namespace std;

const int inf = INT_MAX;

struct node {
  int x, size = 1, min;
  node *l = 0, *r = 0, *p = 0;
  node(int _x) { min = x = _x; }
};

int size(node *t) { return t ? t->size : 0; }

void updnode(node *t) {
  if (t) {
    t->size = size(t->l) + size(t->r) + 1;
    t->min = min(min(t->l ? t->l->min : inf, t->r ? t->r->min : inf), t->x);
  }
}

node *grandp(node *t) {
  if (t && t->p)
    return t->p->p;
  return 0;
}

void rleft(node *t) {
  node *g = grandp(t), *p = t->p, *b = t->l;
  p->r = b, p->p = t;
  updnode(p);
  t->l = p, t->p = g;
  updnode(t);
  if (g) {
    if (p == g->l)
      g->l = t;
    else
      g->r = t;
    updnode(g);
  }
  if (b)
    b->p = p;
}

void rright(node *t) {
  node *g = grandp(t), *p = t->p, *b = t->r;
  p->l = b, p->p = t;
  updnode(p);
  t->r = p, t->p = g;
  updnode(t);
  if (g) {
    if (p == g->r)
      g->r = t;
    else
      g->l = t;
    updnode(g);
  }
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

pair<node *, node *> split(node *t, int k) {
  if (k == size(t))
    return {t, 0};
  int i = 0;
  while (true) {
    i += size(t->l);
    if (i < k) {
      ++i;
      t = t->r;
    } else if (i > k) {
      i -= size(t->l);
      t = t->l;
    } else
      break;
  }
  splay(t);
  node *l = t->l;
  if (l) {
    l->p = 0;
    t->l = 0;
    updnode(t);
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
  updnode(t1);
  t2->p = t1;
  return t1;
}

node *insert(node *t, int k, int x) {
  pair<node *, node *> e = split(t, k);
  t = new node(x);
  if (e.first) {
    t->l = e.first;
    t->l->p = t;
  }
  if (e.second) {
    t->r = e.second;
    t->r->p = t;
  }
  updnode(t);
  return t;
}

node *erase(node *t, int k) {
  int i = 0;
  while (true) {
    i += size(t->l);
    if (i < k) {
      ++i;
      t = t->r;
    } else if (i > k) {
      i -= size(t->l);
      t = t->l;
    } else
      break;
  }
  splay(t);
  if (t->l)
    t->l->p = 0;
  if (t->r)
    t->r->p = 0;
  return merge(t->l, t->r);
}

int minquery(node *&t, int l, int r) {
  pair<node *, node *> ab_c = split(t, r + 1);
  pair<node *, node *> a_b = split(ab_c.first, l);
  int ret = a_b.second->min;
  ab_c.first = merge(a_b.first, a_b.second);
  t = merge(ab_c.first, ab_c.second);
  return ret;
}
