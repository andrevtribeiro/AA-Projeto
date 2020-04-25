#pragma once
#include "graph.hpp"

/*
  Adjacency Matrix
*/
struct AdjacencyMatrix : Graph {
 public:
  AdjacencyMatrix() : Graph() { ; }
  AdjacencyMatrix(NodeId N, vector<Edge> edges) : Graph(N, edges) {
    for (NodeId i = 0; i < N; i++) matrix.push_back(vector<bool>(N, false));
    for (const Edge& edge : edges) {
      matrix[edge.u][edge.v] = true;
    }
  }

 public:
  vector<vector<bool> > matrix;

 public:
  vector<Edge> GetEdges() {
    vector<Edge> ret;

    for (NodeId i = 0; i < N; i++) {
      for (NodeId j = 0; j < N; j++) {
        if (matrix[i][j]) {
          ret.push_back(Edge{i, j});
        }
      }
    }

    return ret;
  }

  vector<NodeId> GetNeighboors(NodeId i) {
    vector<NodeId> ret;

    for (NodeId j = 0; j < N; j++) {
      if (matrix[i][j]) {
        ret.push_back(j);
      }
    }

    return ret;
  }

  vector<NodeId> GetReverseNeighboors(NodeId i) {
    vector<NodeId> ret;

    for (NodeId j = 0; j < N; j++) {
      if (matrix[j][i]) {
        ret.push_back(j);
      }
    }

    return ret;
  }

  bool IsAdjacent(NodeId i, NodeId j) { return matrix[i][j]; }
};

AdjacencyMatrix ReadGraphAsAdjacencyMatrix();