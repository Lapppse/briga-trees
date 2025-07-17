#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <cstdlib>

using namespace std;

struct node
{
    int key, pr;
    node *l = 0, *r = 0;
    node(int _key)
    {
        key = _key;
        pr = rand();
    }
};

pair<node*, node*> split(node* t, int k)
{
    if (!t)
        return {0, 0};
    if (t->key < k)
    {
        pair<node*, node*> e = split(t->r, k);
        t->r = e.first;
        return {t, e.second};
    }
    else
    {
        pair<node*, node*> e = split(t->l, k);
        t->l = e.second;
        return {e.first, t};
    }
}

node* merge(node* t1, node* t2)
{
    if (!t1)
        return t2;
    if (!t2)
        return t1;
    if (t1->pr > t2->pr)
    {
        node* e = merge(t1->r, t2);
        t1->r = e;
        return t1;
    }
    else
    {
        node* e = merge(t1, t2->l);
        t2->l = e;
        return t2;
    }
}

node* insert(node* t, int x)
{
    pair<node*, node*> e = split(t, x);
    node* n = new node(x);
    e.first = merge(e.first, n);
    return merge(e.first, e.second);
}

node* erase(node* t, int x)
{
    pair<node*, node*> e = split(t, x);
    e.second = split(e.second, x + 1).second;
    return merge(e.first, e.second);
}

bool find(node* t, int x)
{
    while (t)
    {
        if (t->key == x)
            return true;
        if (x < t->key)
            t = t->l;
        else
            t = t->r;
    }
    return false;
}



int main()
{
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout << "start";
  cout.flush();

  ifstream fin("test.txt");
  int _t, n, x;
  fin >> _t;

  clock_t clock_start = clock();
  node *t = new node(0);
  while (_t--) {
    fin >> n >> x;
    if (n == 1) {
      insert(t, x);
    } else if (n == 2) {
      erase(t, x);
    } else {
      bool found = find(t, x);
    }
  }
  cout << double(clock() - clock_start) / CLOCKS_PER_SEC;
}

