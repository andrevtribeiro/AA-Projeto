#include <algorithm>
#include "structures/graph/edge_list.hpp"
#include "structures/ufds/undoable_disjoint_set.hpp"
using namespace std;

// UNDONE: decide between modules or moving this to a .cpp
//

bool UpdateBoruvka(EdgeListW& G, UndoableDisjointSet& ds, int start,
                   vector<EdgeW>& ret) {
  bool updated = false;
  vector<WeightType> minWeight =
      vector<WeightType>(G.N, numeric_limits<WeightType>::max());
  vector<EdgeW> minEdge = vector<EdgeW>(G.N, {0, 0, 0});
  for (size_t i = start; i < G.edges.size(); i++) {
    EdgeW& e = G.edges[i];
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
  for (NodeId i : ds.GetRoots()) {
    if (!ds.AreConnected(minEdge[i - 1].u + 1, minEdge[i - 1].v + 1)) {
      ds.Union(minEdge[i - 1].u + 1, minEdge[i - 1].v + 1);
      ret.push_back(minEdge[i - 1]);
      updated = true;
    }
  }
  return updated;
}

void SelectHalfTheEdgesAtRandomFrom(EdgeListW& G, UndoableDisjointSet& ds,
                                    int& start) {
  for (size_t i = start; i < G.edges.size(); i++) {
    bool isSelfEdge = false;
    {
      EdgeW& e = G.edges[i];
      if (ds.AreConnected(e.u + 1, e.v + 1)) isSelfEdge = true;
    }
    if (isSelfEdge == true) {
      swap(G.edges[start], G.edges[i]);
      start++;
    }
  }

  for (size_t i = start; i < G.edges.size(); i++) {
    if (random() % 2 == 0) {
      swap(G.edges[start], G.edges[i]);
      start++;
    }
  }
}

void ELTBoruvka(EdgeListW& G, UndoableDisjointSet& ds, int start,
                vector<EdgeW>& ret) {
  while (true) {
    if (!UpdateBoruvka(G, ds, start, ret)) break;
    if (!UpdateBoruvka(G, ds, start, ret)) break;

    int oldStart = start;
    SelectHalfTheEdgesAtRandomFrom(G, ds, oldStart);
    ds.SaveState();
    vector<EdgeW> msf = ret;  // Do we really need to copy ret now?
    ELTBoruvka(G, ds, oldStart, msf);
    // UNDONE(RemoveFHeavy(G, msf, start););
    ds.RestoreState();
  }
}

// Warning this hash NlogN complexity
// because we have not implemented RemoveFHeavy
//
EdgeListW ELTBoruvka(EdgeListW G) {
  vector<EdgeW> ret;
  UndoableDisjointSet ds = UndoableDisjointSet(G.N);
  ELTBoruvka(G, ds, 0, ret);
  return EdgeListW(G.N, ret);
}
