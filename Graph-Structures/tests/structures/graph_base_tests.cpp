#include <gtest/gtest.h>
#include "structures/baseutils/assertions.hpp"
#include "structures/graph/adjacency_list.hpp"
#include "structures/graph/adjacency_matrix.hpp"
#include "structures/graph/compressed_sparse_rows.hpp"
#include "structures/graph/edge_list.hpp"
#include "tests/structures/test_utils.hpp"

#ifndef GTEST_HAS_TYPED_TEST
ERROR("NO typed tests")
#endif

using testing::Types;

#define TEST_HEADER() using GraphType = typename TestFixture::GraphType;

template <class T, int _GraphSwitch = 0>
struct BGT_GEN {
  using GraphType = T;
  const static int GraphSwitch = _GraphSwitch;
};

template <class T>
struct BaseGraphTests : public testing::Test {
  typedef typename T::GraphType GraphType;

 protected:
  BaseGraphTests() {
    switch (T::GraphSwitch) {
      case 0:
        N = 1;
        E = 0;
        edges = {};
        break;

      case 1:
        N = 3;
        E = 9;
        edges = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1},
                 {1, 2}, {2, 0}, {2, 1}, {2, 2}};
        break;

      case 2:
        N = 5;
        E = 7;
        edges = {{0, 4}, {1, 2}, {1, 3}, {1, 4}, {2, 3}, {3, 4}, {3, 3}};
        break;

      default:
        assert(false);
    }
  }

  vector<Edge> edges;
  NodeId N;
  NodeId E;
};
using GraphImplementations =
    ::testing::Types<BGT_GEN<EdgeList, 0>, BGT_GEN<AdjacencyList, 0>,
                     BGT_GEN<AdjacencyMatrix, 0>, BGT_GEN<CSRGraph, 0>,
                     BGT_GEN<EdgeList, 1>, BGT_GEN<AdjacencyList, 1>,
                     BGT_GEN<AdjacencyMatrix, 1>, BGT_GEN<CSRGraph, 1>,
                     BGT_GEN<EdgeList, 2>, BGT_GEN<AdjacencyList, 2>,
                     BGT_GEN<AdjacencyMatrix, 2>, BGT_GEN<CSRGraph, 2> >;

TYPED_TEST_SUITE(BaseGraphTests, GraphImplementations);

TYPED_TEST(BaseGraphTests, EmptyConstructor) {
  TEST_HEADER();

  GraphType graph;
  ASSERT_EQ(graph.N, 0);
  ASSERT_EQ(graph.E, 0);
  ASSERT_EQ(graph.EdgeCount(), 0);
  ASSERT_EQ(graph.NodeCount(), 0);
}

TYPED_TEST(BaseGraphTests, Constructor) {
  TEST_HEADER();

  GraphType graph;
  graph = GraphType(this->N, this->edges);
  ASSERT_EQ(graph.N, this->N);
  EXPECT_EQ(graph.E, this->E);
  ASSERT_EQ(graph.NodeCount(), this->N);
  EXPECT_EQ(graph.EdgeCount(), this->E);
  for (NodeId i = 0; i < this->N; i++) {
    for (NodeId j = 0; j < this->N; j++) {
      EXPECT_EQ(graph.IsAdjacent(i, j), contains(this->edges, {i, j}));
    }
  }
}

TYPED_TEST(BaseGraphTests, IsAdjacent) {
  TEST_HEADER();

  GraphType graph;
  graph = GraphType(this->N, this->edges);
  ASSERT_EQ(graph.NodeCount(), this->N);
  EXPECT_EQ(graph.EdgeCount(), this->E);
  for (NodeId i = 0; i < this->N; i++) {
    for (NodeId j = 0; j < this->N; j++) {
      EXPECT_EQ(graph.IsAdjacent(i, j), contains(this->edges, {i, j}));
    }
  }
}

TYPED_TEST(BaseGraphTests, GetEdges) {
  TEST_HEADER();

  GraphType graph;
  graph = GraphType(this->N, this->edges);
  ASSERT_EQ(graph.NodeCount(), this->N);
  EXPECT_EQ(graph.EdgeCount(), this->E);

  vector<Edge> graph_edges = graph.GetEdges();

  // Check no repetitions:
  //
  for (size_t i = 0; i < graph_edges.size(); i++) {
    EXPECT_FALSE(contains(graph_edges, graph_edges[i], i + 1));
  }

  // Check size is correct:
  //
  EXPECT_EQ(this->edges.size(), graph_edges.size());

  // Check content is correct:
  //
  for (NodeId i = 0; i < this->N; i++) {
    for (NodeId j = 0; j < this->N; j++) {
      EXPECT_EQ(graph.IsAdjacent(i, j), contains(graph_edges, {i, j}));
    }
  }
}

TYPED_TEST(BaseGraphTests, GetNeighboors) {
  TEST_HEADER();

  GraphType graph;
  graph = GraphType(this->N, this->edges);
  ASSERT_EQ(graph.NodeCount(), this->N);
  EXPECT_EQ(graph.EdgeCount(), this->E);

  for (NodeId i = 0; i < this->N; i++) {
    vector<NodeId> graph_edges = graph.GetNeighboors(i);

    // Check no repetitions:
    //
    for (size_t j = 0; j < graph_edges.size(); j++) {
      EXPECT_FALSE(contains(graph_edges, graph_edges[j], j + 1));
    }

    // Check content is correct:
    //
    for (NodeId j = 0; j < this->N; j++) {
      EXPECT_EQ(graph.IsAdjacent(i, j), contains(graph_edges, j));
    }
  }
}

TYPED_TEST(BaseGraphTests, GetReverseNeighboors) {
  TEST_HEADER();

  GraphType graph;
  graph = GraphType(this->N, this->edges);
  ASSERT_EQ(graph.NodeCount(), this->N);
  EXPECT_EQ(graph.EdgeCount(), this->E);

  for (NodeId i = 0; i < this->N; i++) {
    vector<NodeId> graph_edges = graph.GetReverseNeighboors(i);

    // Check no repetitions:
    //
    for (size_t j = 0; j < graph_edges.size(); j++) {
      EXPECT_FALSE(contains(graph_edges, graph_edges[j], j + 1));
    }

    // Check content is correct:
    //
    for (NodeId j = 0; j < this->N; j++) {
      EXPECT_EQ(graph.IsAdjacent(j, i), contains(graph_edges, j));
    }
  }
}