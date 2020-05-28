#pragma once
#include "graph.hpp"

/* Compressed sparse row */
struct CSRGraph : Graph {
 public:
  CSRGraph() : Graph() { ; }
  CSRGraph(uint64_t N, vector<Edge> edges) : Graph(N, edges) {
    sort(edges.begin(), edges.end(), [](const Edge& left, const Edge& right) {
      return left.u < right.u || (left.u == right.u && left.v < right.v);
    });

    NodeId currentNode = 0;
    sizes.push_back(0);

    for (NodeId i = 0; i < E; i++) {
      const NodeId& u = edges[i].u;
      const NodeId& v = edges[i].v;

      while (currentNode != u) {
        sizes.push_back(adjacents.size());
        currentNode++;
      }

      adjacents.push_back(v);
    }

    while (sizes.size() <= N) {
      sizes.push_back(adjacents.size());
    }
  }

 public:
  // UNDONE("Invariant: Each array adjacents[sizes[i]:sizes[i+1]-1] is sorted")
  // (the invariant is implemented, but we are not making use of it)
  //
  vector<NodeId> adjacents;
  vector<NodeId> sizes;

 public:
  vector<Edge> GetEdges() {
    vector<Edge> ret;

    NodeId i = 0;
    for (NodeId j = 0; j < adjacents.size(); j++) {
      while (sizes[i + 1] <= j) {
        i++;
      }

      ret.push_back(Edge{i, adjacents[j]});
    }

    return ret;
  }

  vector<NodeId> GetNeighboors(NodeId i) {
    vector<NodeId> ret;

    for (NodeId j = sizes[i]; j < adjacents.size() && j < sizes[i + 1]; j++) {
      ret.push_back(adjacents[j]);
    }

    return ret;
  }

  vector<NodeId> GetReverseNeighboors(NodeId k) {
    vector<NodeId> ret;

    NodeId i = 0;
    for (NodeId j = 0; j < adjacents.size(); j++) {
      while (sizes[i + 1] <= j) {
        i++;
      }

      if (adjacents[j] == k) {
        ret.push_back(i);
      }
    }

    return ret;
  }

  bool IsAdjacent(NodeId i, NodeId k) {
    vector<NodeId> ret;

    for (NodeId j = sizes[i]; j < adjacents.size() && j < sizes[i + 1]; j++) {
      if (adjacents[j] == k) return true;
    }

    return false;
  }

 public:
  void dump_debug() {
    for (NodeId i = 0; i < N; i++) {
      cout << sizes[i] << " ";
    }
    cout << endl;
    for (NodeId i = 0; i < E; i++) {
      cout << adjacents[i] << " ";
    }
    cout << endl;
  }
};


CSRGraph ReadGraphAsCSR();
CSRGraph ReadUndirectedGraphAsCSR();