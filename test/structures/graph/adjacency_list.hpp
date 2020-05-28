#pragma once
#include "graph.hpp"

/*
  Adjacency list
*/
struct AdjacencyList : Graph {
 public:
  AdjacencyList() : Graph() { ; }
  AdjacencyList(NodeId N, vector<Edge> edges) : Graph(N, edges) {
    for (NodeId i = 0; i < N; i++) adjlist.push_back(vector<NodeId>());
    for (const Edge& edge : edges) {
      adjlist[edge.u].push_back(edge.v);
    }
  }

 public:
  vector<vector<NodeId>> adjlist;

 public:
  vector<Edge> GetEdges() {
    vector<Edge> ret;
    for (NodeId i = 0; i < N; i++) {
      for (const NodeId& v : adjlist[i]) {
        ret.push_back({i, v});
      }
    }
    return ret;
  }

  vector<NodeId> GetNeighboors(NodeId i) { return adjlist[i]; }

  vector<NodeId> GetReverseNeighboors(NodeId i) {
    vector<NodeId> ret;

    for (NodeId j = 0; j < N; j++) {
      for (NodeId& k : adjlist[j]) {
        if (k == i) {
          ret.push_back(j);
        }
      }
    }

    return ret;
  }

  bool IsAdjacent(NodeId i, NodeId j) {
    for (NodeId& k : adjlist[i]) {
      if (k == j) {
        return true;
      }
    }
    return false;
  }

  uint64_t GetMemoryUsage() {
    uint64_t ret = 0;
    ret += 2 * sizeof(uint64_t);  // N,E
    ret += sizeof(vector<vector<NodeId>>);
    ret += adjlist.capacity() * sizeof(vector<NodeId>);
    for (NodeId i = 0; i < N; i++) {
      ret += adjlist[i].capacity() * sizeof(NodeId);
    }
    return ret;
  }
};

// Compiled in utils.cpp
AdjacencyList ReadGraphAsAdjacencyList();