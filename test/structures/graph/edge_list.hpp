#pragma once
#include "graph.hpp"


struct EdgeList : Graph {
  vector<Edge> edges;
  EdgeList() : Graph() {;}
  EdgeList(uint64_t N, vector<Edge> edges) : Graph(N, edges), edges(edges) {;}

public:
  vector<Edge> GetEdges() {return edges;}
  vector<NodeId> GetNeighboors(NodeId i) {
      vector<NodeId> ret;
      for(Edge& e: edges) {
          if (e.u == i) {
              ret.push_back(e.v);
          }
      }
      return ret;
  }
  vector<NodeId> GetReverseNeighboors(NodeId i)  {
      vector<NodeId> ret;
      for(Edge& e: edges) {
          if (e.v == i) {
              ret.push_back(e.u);
          }
      }
      return ret;
  }
  bool IsAdjacent(NodeId i, NodeId j) {
      return 0 != count_if(edges.begin(), edges.end(), [&i,&j](const Edge& e) { return e.u == i && e.v == j;});
  }
  uint64_t GetMemoryUsage() {
    uint64_t ret = 0;
    ret += 2*sizeof(uint64_t); //N,E
    ret += sizeof(vector<Edge>);
    ret += edges.capacity() * sizeof(Edge);
    return ret;
  }
};



struct EdgeListW : GraphW {
  vector<EdgeW> edges;
  EdgeListW() : GraphW() {;}
  EdgeListW(uint64_t N, vector<EdgeW> edges) : GraphW(N, edges), edges(edges) {;}

public:
  vector<EdgeW> GetEdges() {return edges;}
  vector<NeighW> GetNeighboors(NodeId i) {
      vector<NeighW> ret;
      for(EdgeW& e: edges) {
          if (e.u == i) {
              ret.push_back({e.v, e.w});
          }
      }
      return ret;
  }
  vector<NeighW> GetReverseNeighboors(NodeId i)  {
      vector<NeighW> ret;
      for(EdgeW& e: edges) {
          if (e.v == i) {
              ret.push_back({e.u, e.w});
          }
      }
      return ret;
  }
  bool IsAdjacent(NodeId i, NodeId j) {
      return 0 != count_if(edges.begin(), edges.end(), [&i,&j](const EdgeW& e) { return e.u == i && e.v == j;});
  }
  WeightType GetWeight(NodeId i, NodeId j) {
    WeightType ret = WEIGHT_INVALID;
    for(EdgeW& e: edges) {
      if (e.u == i && e.v == j) {
        ASSERT(e.w != WEIGHT_INVALID);
        return e.w;
      }
    }
    return ret;
  }
  uint64_t GetMemoryUsage() {
    uint64_t ret = 0;
    ret += 2*sizeof(uint64_t); //N,E
    ret += sizeof(vector<Edge>);
    ret += edges.capacity() * sizeof(Edge);
    return ret;
  }
};

// Compiled in utils.cpp:
EdgeList  ReadGraphAsEdgeList();
EdgeListW ReadGraphWAsEdgeListW();