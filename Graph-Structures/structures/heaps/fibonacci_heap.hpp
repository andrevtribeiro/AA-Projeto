#pragma once
#define FH_MAX_RANK 16

// UNDONE(test): test this
// UNDONE(refactor): refactor this

// HEAP suporta no maximo 2^(FH_MAX_RANK-1) elementos
template <class T>
class FibHeap;

template <class T>
struct Node {
  T key;
  Node *l, *r, *p, *c;  // left, right, parent, son
  int rank;
  bool mark;
  Node(const T &x) {
    l = r = this;
    p = c = NULL;
    rank = 0;
    mark = false;
    key = x;
  }

 private:
  // Merges two circular lists
  void merge(Node<T> *o) {
    Node<T> *aux1, *aux2;
    aux1 = o->l;
    aux2 = this->r;
    o->l = this;
    this->r = o;
    aux2->l = aux1;
    aux1->r = aux2;
  }

  // Appends 1 Node to the list of childs (increases rank and ignore lists)
  void add_child(Node<T> *o) {
    o->p = this;
    o->mark = false;
    if (this->c) {
      o->l = this->c;
      o->r = this->c->r;
      this->c->r->l = o;
      this->c->r = o;
    } else {
      o->l = o->r = o;
      this->c = o;
    }
    this->rank++;
  }

  // Appens one Node to the same list (ignores lists)
  void add_bro(Node<T> *o) {
    o->p = this->p;
    o->l = this;
    o->r = this->r;
    this->r->l = o;
    this->r = o;
  }

  // Clears parent information on the given list
  void clearInfo() {
    Node<T> *p = this;
    do {
      this->p = NULL;
      this->mark = false;
    } while (p != this);
  }
  friend class FibHeap<T>;
};

template <class T>
class FibHeap {
 public:
  // Nossa implementação de fibonacci Heap
 private:
  Node<T> *Hmin;
  size_t N;

  void consolidate() {
    Node<T> *Ranks[FH_MAX_RANK] = {NULL};
    for (int i = 0; i < FH_MAX_RANK; i++) Ranks[i] = NULL;  //?
    Node<T> *h = Hmin, *prox, *aux = NULL;
    do {
      prox = h->r;
      while (Ranks[h->rank]) {
        aux = Ranks[h->rank];
        if (aux->key < h->key) {
          aux->add_child(h);
          Ranks[h->rank] = NULL;
          h = aux;
        } else {
          h->add_child(aux);
          Ranks[h->rank - 1] = NULL;  // Add child updates rank
        }
      }
      Ranks[h->rank] = h;
      h = prox;
    } while (h != Hmin);

    Hmin = NULL;
    aux = NULL;
    for (size_t i = 0; i < FH_MAX_RANK; i++) {
      if (Ranks[i]) {
        if (Hmin != NULL) {
          Hmin->add_bro(Ranks[i]);
          if (Ranks[i]->key < Hmin->key) {
            Hmin = Ranks[i];
          }
        } else {
          Hmin = Ranks[i];
          Hmin->r = Hmin->l = Hmin;
          Hmin->p = NULL;
        }
      }
    }
  }

  void cut(Node<T> *x, Node<T> *y) {
    if (x != x->r) {
      x->r->l = x->l;
      x->l->r = x->r;
      y->c = x->r;
    } else {
      y->c = NULL;
    }
    y->rank--;
    Hmin->add_bro(x);
    x->mark = false;
  }

  void cascadingCut(Node<T> *x) {
    Node<T> *y = x->p;
    if (y != NULL) {
      if (x->mark) {
        cut(x, y);
        cascadingCut(y);
      } else {
        x->mark = true;
      }
    }
  }
  void _delete_recursive(Node<T> *r) {
    if (r == NULL) return;
    Node<T> *aux = r;
    do {
      _delete_recursive(aux->c);
      aux = aux->r;
    } while (aux != r);
    delete r;
  }

 public:
  FibHeap() {
    Hmin = NULL;
    N = 0;
  }
  ~FibHeap() { _delete_recursive(Hmin); }

  Node<T> *insert(const T &x) {
    Node<T> *n = new Node<T>(x);
    if (Hmin == NULL) {
      Hmin = n;
    } else {
      if (Hmin != NULL) {
        if (n->key < Hmin->key) {
          n->merge(this->Hmin);
          Hmin = n;
        } else {
          Hmin->merge(n);
        }
      }
    }
    N++;
    return n;
  }

  T top() {
    if (Hmin) {
      return Hmin->key;
    } else {
      return T();  // this should be a throw
    }
  }

  T pop() {
    T ret = top();
    Node<T> *aux = Hmin;
    if (Hmin != NULL) {
      if (Hmin->c != NULL) {
        Hmin->merge(Hmin->c);
        Hmin->c->clearInfo();  // ok to apear after, cuz we are on the root
      }
      if (Hmin->l == Hmin) {
        Hmin = NULL;
      } else {
        Hmin->l->r = Hmin->r;
        Hmin->r->l = Hmin->l;
        Hmin = Hmin->r;
        consolidate();
      }
      N = N - 1;
    }
    delete aux;
    return ret;
  }

  Node<T> *decrease_key(Node<T> *x, const T &key) {
    if (key < x->key) {
      Node<T> *y = x->p;
      x->key = key;
      if (y != NULL && x->key < y->key) {
        cut(x, y);
        cascadingCut(y);
      }
      if (x->key < Hmin->key) {
        Hmin = x;
      }
    }
    return x;
  }

  bool empty() { return N == 0; }
  int size() { return N; }
  Node<T> *getTop() { return Hmin; }
};