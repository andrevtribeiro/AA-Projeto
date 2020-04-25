#pragma once
#include "structures/graph/compressed_sparse_rows.hpp"
#include <math.h>

#define ROT(x,k) (((x)<<(k)) | ((x)>>(32-(k))))
#define MAX(x,y) (x < y ? x : y)

// 0.7213/(1+(1.079)/m) -> m = 4096
const double alpha = 0.7211100396160289;
const m_square = 16777216;

using namespace std;
const int B = 12;
// b = 12
// length for hashing = 8 

// Let h : D → [0, 1] ≡ {0, 1}∞ hash data from domain D to the binary domain. Let ρ(s), for s ∈ {0, 1}∞ , be the position of the leftmost 1-bit (ρ(0001 · · · ) = 4).
// Algorithm HYPERLOGLOG (input M : multiset of items from domain D). assumem=2b withb∈Z>0;
// initialize a collection of m registers, M [1], . . . , M [m], to −∞;
// for v ∈ M do
// set x := h(v);
// set j = 1 + ⟨x1x2 · · · xb⟩2; {the binary address determined by the first b bits of x} set w := xb+1xb+2 · · · ; set M[j] := max(M[j], ρ(w));
// !−1
// m computeZ:= X2−M[j]
// j=1
// ;{the“indicator”function} return E := αmm2Z with αm as given by Equation (3).

uint64_t ro(uint64_t w) {

    // Keep shifting x by one until leftmost bit 
    // does not become 1. 
    int total_bits = sizeof(w) * 8; 
    int res = 0; 
    while ( !(w & (1 << (total_bits - 1)))) 
    { 
        w = (w << 1); 
        res++; 
    } 
  
    return res-B+1; 	
}


uint64_t size(Counter& counter) {
	uint64_t m = pow(2, b);
	double res = 0;
	for(uint64_t i = 0; i < m; i++) {
		res += pow(2, -counter[i]);
	}
	res = 1/res;
	
	return round(alpha*m_square*res);
}


uint64_t add(Counter& counter, uint64_t v) {
	uint64_t x;
	uint64_t j;
	hashlittle2(&v, sizeof(v), &x, (BYTE*)&x + 32);

	j = (x >> (64-B))+1;
	w = x & ((uint64_t)-1 >> B);
	counter[j] = MAX(counter[j], ro(&w));
}


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
uint64_t Karger(CSRGraph& Graph) {
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