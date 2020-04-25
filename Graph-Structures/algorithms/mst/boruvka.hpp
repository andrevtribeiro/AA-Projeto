#include <algorithm>
#include "structures/graph/edge_list.hpp"
#include "structures/ufds/disjoint_set.hpp"
using namespace std;

// UNDONE: decide between modules or moving this to a .cpp
//

EdgeListW Boruvka(EdgeListW G) {
  vector<EdgeW> ret;
  DisjointSet ds = DisjointSet(G.N);
  int components = G.N;
  while (components != 1) {
    vector<WeightType> minWeight =
        vector<WeightType>(G.N, numeric_limits<WeightType>::max());
    vector<EdgeW> minEdge = vector<EdgeW>(G.N, EdgeW{0, 0, 0});
    for (EdgeW& e : G.edges) {
      if (ds.AreConnected(e.u + 1, e.v + 1)) continue;
      NodeId id1 = ds.Find(e.u + 1) - 1;
      NodeId id2 = ds.Find(e.v + 1) - 1;
      if (minWeight[id1] > e.w) {
        minWeight[id1] = e.w;
        minEdge[id1] = e;
      }
      if (minWeight[id2] > e.w) {
        minWeight[id2] = e.w;
        minEdge[id2] = e;
      }
    }
    for (NodeId i = 0; i < G.N; i++) {
      if (!ds.AreConnected(minEdge[i].u + 1, minEdge[i].v + 1)) {
        ds.Union(minEdge[i].u + 1, minEdge[i].v + 1);
        ret.push_back(minEdge[i]);
        components--;
      }
    }
  }

  return EdgeListW(G.N, ret);
}