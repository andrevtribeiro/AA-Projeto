#pragma once
#include "structures/graph/edge_list.hpp"
#include "structures/ufds/disjoint_set.hpp"
using namespace std;

// UNDONE: decide between modules or moving this to a .cpp
//

static uint64_t CalcMinCut(vector<Edge>& edges, uint64_t E, DisjointSet& ds,
                           uint64_t comp) {
  // Shuffle the array
  // https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
  // for i from 0 to n−2 do
  for (uint64_t i = 0; i < E - 1; i++) {
    // j ← random integer such that i ≤ j < n
    uint64_t j = (rand() % (E - i - 1)) + i;
    if (i != j) swap(edges[i], edges[j]);  // exchange a[i] and a[j]
  }

  for (uint64_t i = 0; i < E && ds.components > comp; i++) {
    if (ds.AreConnected(edges[i].u + 1, edges[i].v + 1)) continue;
    // not connected
    ds.Union(edges[i].u + 1, edges[i].v + 1);
  }

  // components == comp
  uint64_t cut = 0;
  for (uint64_t i = 0; i < E; i++) {
    if (ds.AreConnected(edges[i].u + 1, edges[i].v + 1)) continue;
    // not connected
    cut++;
  }

  return cut;
}

// Probabilistic algorithm for mincut
//
// Time: O(N^2 E log N)
// Space: O(N+E)
//
uint64_t Karger(EdgeList& Graph) {
  uint64_t& E = Graph.E;
  uint64_t& N = Graph.N;
  vector<Edge>& edges = Graph.edges;
  uint64_t mincut = E + 1;

  uint64_t log = 1;
  uint64_t auxN = N;
  while (auxN != 0) {
    log++;
    auxN /= 2;
  }

  uint64_t runs = (N * (N - 1) * log) / 2 + 2;

  for (uint64_t i = 0; i < runs; i++) {
    DisjointSet ds = DisjointSet(N);
    mincut = std::min(mincut, CalcMinCut(edges, E, ds, 2));
  }

  return mincut;
}