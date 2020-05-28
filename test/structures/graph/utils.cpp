#include "adjacency_list.hpp"
#include "adjacency_matrix.hpp"
#include "compressed_sparse_rows.hpp"
#include "edge_list.hpp"
#include "graph.hpp"

AdjacencyList ReadGraphAsAdjacencyList() {
  AdjacencyList ret;
  cin >> ret.N >> ret.E;
  for (NodeId i = 0; i < ret.N; i++) {
    ret.adjlist.emplace_back(vector<NodeId>());
  }

  for (NodeId i = 0; i < ret.E; i++) {
    NodeId a, b;
    cin >> a >> b;
    ret.adjlist[a].emplace_back(b);
  }
  return ret;
}

AdjacencyMatrix ReadGraphAsAdjacencyMatrix() {
  AdjacencyMatrix ret;
  cin >> ret.N >> ret.E;
  // UNDONE(gcc-pragma-bug): assert( 0 <= ret.N );
  // UNDONE(gcc-pragma-bug): assert( 0 <= ret.E );

  for (NodeId i = 0; i < ret.N; i++) {
    ret.matrix.emplace_back(vector<bool>(ret.N, false));
  }

  for (NodeId i = 0; i < ret.E; i++) {
    NodeId a, b;
    cin >> a >> b;
    ASSERT(static_cast<NodeId>(-1) != a);
    // UNDONE(gcc-pragma-bug): ASSERT(0 <= a);
    ASSERT(a <= ret.N);
    ASSERT(static_cast<NodeId>(-1) != b);
    // UNDONE(gcc-pragma-bug): ASSERT(0 <= b)
    ASSERT(b <= ret.N);
    ret.matrix[a][b] = true;
  }

  return ret;
}

CSRGraph ReadGraphAsCSR() {
  NodeId N, E;
  vector<Edge> edges;
  cin >> N >> E;
  for (NodeId i = 0; i < E; i++) {
    NodeId a, b;
    cin >> a >> b;
    edges.push_back({a, b});
  }
  return CSRGraph(N, edges);
}
CSRGraph ReadUndirectedGraphAsCSR() {
  NodeId N, E;
  vector<Edge> edges;
  cin >> N >> E;
  for (NodeId i = 0; i < E; i++) {
    NodeId a, b;
    cin >> a >> b;
    edges.push_back({a, b});
    edges.push_back({b, a});
  }
  return CSRGraph(N, edges);
}

EdgeList ReadGraphAsEdgeList() {
  NodeId N, E;
  vector<Edge> edges;
  cin >> N >> E;
  for (NodeId i = 0; i < E; i++) {
    NodeId a, b;
    cin >> a >> b;
    a--;
    b--;
    edges.push_back({a, b});
  }
  return EdgeList(N, edges);
}

EdgeListW ReadGraphWAsEdgeListW() {
  NodeId N, E;
  vector<EdgeW> edges;
  cin >> N >> E;
  for (NodeId i = 0; i < E; i++) {
    NodeId a, b;
    WeightType c;
    cin >> a >> b >> c;
    a--;
    b--;
    edges.push_back({a, b, c});
  }
  return EdgeListW(N, edges);
}