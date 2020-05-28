#pragma once
// UNDONE(test): test this
// UNDONE(refactor): refactor this

// This is a binaryHeap that is kind of good for Dijkstra
//
template <typename T>
class BinaryHeap {
  typedef std::pair<int, T> pit;
  typedef typename std::list<std::pair<int, T> >::iterator lit;
  std::list<pit> c;  // Not the ideal structure(its a double linked list)
  std::vector<lit> ind;
  void fix_up(int n) {
    while (n > 1 && std::less<T>()(ind[n]->second, ind[n / 2]->second)) {
      ind[n]->first = n / 2;
      ind[n / 2]->first = n;
      std::swap(ind[n], ind[n / 2]);
      n = n / 2;
    }
  }
  void fix_down(size_t n) {
    size_t i;
    while (2 * n < ind.size()) {
      i = 2 * n;
      if (i + 1 < ind.size() &&
          std::less<T>()(ind[i + 1]->second, ind[i]->second))
        i++;
      if (!std::less<T>()(ind[i]->second, ind[n]->second)) break;
      ind[n]->first = i;
      ind[i]->first = n;
      swap(ind[n], ind[i]);
      n = i;
    }
  }

 public:
  BinaryHeap() {
    c = std::list<pit>();
    ind = std::vector<lit>();
    ind.push_back(lit());
  }
  bool empty() { return ind.size() == 1; }
  T getMin() {
    if (empty()) return T();
    return ind[1]->second;
  }

  lit insert(const T& t) {
    lit ret;
    c.push_back(pit(ind.size(), t));
    ret = c.end();
    ret--;
    ind.push_back(ret);
    fix_up(ind.size() - 1);
    return ret;
  }

  void deleteMin() {
    if (empty()) return;
    c.erase(ind[1]);
    std::swap(ind[1], ind[ind.size() - 1]);
    ind[1]->first = 1;
    ind.pop_back();
    fix_down(1);
  }

  void decrease_key(lit p, T key) {
    if (std::less<T>()(p->second, key)) return;
    p->second = key;
    fix_up(p->first);
  }
};