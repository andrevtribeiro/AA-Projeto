#pragma once
#include "structures/baseutils/assertions.hpp"

// This disjoint set assumes that 1 <= i <= N
//

// This is a simple efficient implementation of a union-find disjoint set.
// We are actually going to use the Disjoint Set class, which has many
// more utilities, but it is basically a complex version of this structure
//
struct SimpleDisjointSet {
  uint64_t N;  // Number of sets
  vector<uint64_t> id;
  vector<uint64_t> rank;
  SimpleDisjointSet() : N(0) { ; }
  SimpleDisjointSet(uint64_t _N)
      : N(_N),
        id(vector<uint64_t>(N + 1, 0)),
        rank(vector<uint64_t>(N + 1, 1)) {
    for (uint64_t i = 1; i <= N; i++) {
      id[i] = i;
    }
  }
  uint64_t Find(uint64_t i) {
    assert(i > 0);
    assert(i <= N);
    if (id[i] != i) id[i] = Find(id[i]);
    return id[i];
  }
  void Union(uint64_t i, uint64_t j) {
    i = Find(i);
    j = Find(j);
    if (rank[i] < rank[j]) swap(i, j);
    id[j] = i;
    rank[i]++;
  }
  bool AreConnected(uint64_t i, uint64_t j) { return Find(i) == Find(j); }
};
