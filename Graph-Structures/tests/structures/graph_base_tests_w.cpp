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
struct BaseGraphWTests : public testing::Test {
  typedef typename T::GraphType GraphType;

 protected:
  BaseGraphWTests() {
    switch (T::GraphSwitch) {
      case 0:
        N = 1;
        E = 0;
        edges = {};
        break;

      case 1:
        N = 3;
        E = 9;
        edges = {{0, 0, 2}, {0, 1, 1}, {0, 2, 3}, {1, 0, 5}, {1, 1, 6},
                 {1, 2, 1}, {2, 0, 7}, {2, 1, 9}, {2, 2, -4}};
        break;

      case 2:
        N = 5;
        E = 7;
        edges = {{0, 4, 3},  {1, 2, 1},  {1, 3, 10000}, {1, 4, 42},
                 {2, 3, 72}, {3, 4, 74}, {3, 3, -68}};
        break;

      default:
        assert(false);
    }
  }

  vector<EdgeW> edges;
  NodeId N;
  NodeId E;
};
using GraphWImplementations =
    ::testing::Types<BGT_GEN<EdgeListW, 0>, BGT_GEN<EdgeListW, 1>,
                     BGT_GEN<EdgeListW, 2> >;

TYPED_TEST_SUITE(BaseGraphWTests, GraphWImplementations);

TYPED_TEST(BaseGraphWTests, EmptyConstructor) {
  TEST_HEADER();

  GraphType graph;
  ASSERT_EQ(graph.N, 0);
  ASSERT_EQ(graph.E, 0);
  ASSERT_EQ(graph.EdgeCount(), 0);
  ASSERT_EQ(graph.NodeCount(), 0);
}

TYPED_TEST(BaseGraphWTests, Constructor) {
  TEST_HEADER();

  GraphType graph;
  graph = GraphType(this->N, this->edges);
  ASSERT_EQ(graph.N, this->N);
  EXPECT_EQ(graph.E, this->E);
  ASSERT_EQ(graph.NodeCount(), this->N);
  EXPECT_EQ(graph.EdgeCount(), this->E);
  for (NodeId i = 0; i < this->N; i++) {
    for (NodeId j = 0; j < this->N; j++) {
      EXPECT_EQ(graph.IsAdjacent(i, j), contains(this->edges, Edge{i, j}));
    }
  }
  for (auto& e : this->edges) {
    if (graph.IsAdjacent(e.u, e.v)) {
      EXPECT_EQ(graph.GetWeight(e.u, e.v), e.w);
    } else {
      EXPECT_EQ(graph.GetWeight(e.u, e.v), WEIGHT_INVALID);
    }
  }
}

TYPED_TEST(BaseGraphWTests, IsAdjacent) {
  TEST_HEADER();

  GraphType graph;
  graph = GraphType(this->N, this->edges);
  ASSERT_EQ(graph.NodeCount(), this->N);
  EXPECT_EQ(graph.EdgeCount(), this->E);
  for (NodeId i = 0; i < this->N; i++) {
    for (NodeId j = 0; j < this->N; j++) {
      EXPECT_EQ(graph.IsAdjacent(i, j), contains(this->edges, Edge{i, j}));
    }
  }
}

TYPED_TEST(BaseGraphWTests, GetWeight) {
  TEST_HEADER();

  GraphType graph;
  graph = GraphType(this->N, this->edges);
  ASSERT_EQ(graph.NodeCount(), this->N);
  EXPECT_EQ(graph.EdgeCount(), this->E);
  for (NodeId i = 0; i < this->N; i++) {
    for (NodeId j = 0; j < this->N; j++) {
      EXPECT_EQ(graph.IsAdjacent(i, j), contains(this->edges, Edge{i, j}));
    }
  }
  for (auto& e : this->edges) {
    if (graph.IsAdjacent(e.u, e.v)) {
      EXPECT_EQ(graph.GetWeight(e.u, e.v), e.w);
    } else {
      EXPECT_EQ(graph.GetWeight(e.u, e.v), WEIGHT_INVALID);
    }
  }

  if (graph.IsAdjacent(0, 0)) {
    EXPECT_NE(graph.GetWeight(0, 0), WEIGHT_INVALID);
  } else {
    EXPECT_EQ(graph.GetWeight(0, 0), WEIGHT_INVALID);
  }
}

TYPED_TEST(BaseGraphWTests, GetEdges) {
  TEST_HEADER();

  GraphType graph;
  graph = GraphType(this->N, this->edges);
  ASSERT_EQ(graph.NodeCount(), this->N);
  EXPECT_EQ(graph.EdgeCount(), this->E);

  vector<EdgeW> graph_edges = graph.GetEdges();

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
      EXPECT_EQ(graph.IsAdjacent(i, j), contains(graph_edges, Edge{i, j}));
    }
  }
  for (auto& e : graph_edges) {
    if (graph.IsAdjacent(e.u, e.v)) {
      EXPECT_EQ(graph.GetWeight(e.u, e.v), e.w);
    } else {
      EXPECT_EQ(graph.GetWeight(e.u, e.v), WEIGHT_INVALID);
    }
  }
}

TYPED_TEST(BaseGraphWTests, GetNeighboors) {
  TEST_HEADER();

  GraphType graph;
  graph = GraphType(this->N, this->edges);
  ASSERT_EQ(graph.NodeCount(), this->N);
  EXPECT_EQ(graph.EdgeCount(), this->E);

  for (NodeId i = 0; i < this->N; i++) {
    vector<NeighW> graph_edges = graph.GetNeighboors(i);

    // Check no repetitions:
    //
    for (size_t j = 0; j < graph_edges.size(); j++) {
      EXPECT_FALSE(contains(graph_edges, graph_edges[j], j + 1));
    }

    // Check content is correct:
    //
    for (NodeId j = 0; j < this->N; j++) {
      EXPECT_EQ(graph.IsAdjacent(i, j), contains(graph_edges, j));
      if (graph.IsAdjacent(i, j)) {
        EXPECT_TRUE(contains(graph_edges, NeighW{j, graph.GetWeight(i, j)}));
      }
    }
  }
}

TYPED_TEST(BaseGraphWTests, GetReverseNeighboors) {
  TEST_HEADER();

  GraphType graph;
  graph = GraphType(this->N, this->edges);
  ASSERT_EQ(graph.NodeCount(), this->N);
  EXPECT_EQ(graph.EdgeCount(), this->E);

  for (NodeId i = 0; i < this->N; i++) {
    vector<NeighW> graph_edges = graph.GetReverseNeighboors(i);

    // Check no repetitions:
    //
    for (size_t j = 0; j < graph_edges.size(); j++) {
      EXPECT_FALSE(contains(graph_edges, graph_edges[j], j + 1));
    }

    // Check content is correct:
    //
    for (NodeId j = 0; j < this->N; j++) {
      EXPECT_EQ(graph.IsAdjacent(j, i), contains(graph_edges, j));
      if (graph.IsAdjacent(j, i)) {
        EXPECT_TRUE(contains(graph_edges, NeighW{j, graph.GetWeight(j, i)}));
      }
    }
  }
}
