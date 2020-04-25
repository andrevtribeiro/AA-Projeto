#include <gtest/gtest.h>
#include <structures/baseutils/assertions.hpp>
#include "algorithms/mst/boruvka.hpp"
#include "algorithms/mst/eltmst.hpp"
#include "structures/graph/edge_list.hpp"

const EdgeListW GRAPH1 = EdgeListW(7, {{0, 1, 7},
                                       {0, 3, 4},
                                       {1, 2, 11},
                                       {1, 3, 9},
                                       {1, 4, 10},
                                       {2, 4, 5},
                                       {3, 4, 15},
                                       {3, 5, 6},
                                       {4, 5, 12},
                                       {4, 6, 8},
                                       {5, 6, 13}});
const EdgeListW GRAPH2 = EdgeListW(
    12, {{0, 1, 13}, {0, 2, 6},   {1, 2, 7},  {1, 3, 1},   {2, 3, 14},
         {2, 4, 8},  {2, 7, 20},  {3, 4, 9},  {3, 5, 3},   {4, 5, 2},
         {4, 9, 18}, {6, 7, 15},  {6, 8, 5},  {6, 9, 19},  {6, 10, 10},
         {7, 9, 17}, {8, 10, 11}, {9, 11, 4}, {9, 10, 16}, {10, 11, 12}});

TEST(Boruvka, simple1) {
  EdgeListW graphW = GRAPH1;

  for (int i = 0; i < 10; i++) {
    // UNDONE: graphW.edges.shuffle();
    EdgeListW mst = Boruvka(graphW);
    WeightType totalW = 0;
    for (auto& e : mst.edges) {
      totalW += e.w;
    }
    // UNDONE: EXPECT_TRUE(isAcyclicU(mst));
    // UNDONE: EXPECT_TRUE(isMST(mst,graphw));
    EXPECT_EQ(totalW, 40);
  }
}

TEST(Boruvka, simple2) {
  EdgeListW graphW = GRAPH2;

  for (int i = 0; i < 10; i++) {
    // UNDONE: graphW.edges.shuffle();
    EdgeListW mst = Boruvka(graphW);
    WeightType totalW = 0;
    for (auto& e : mst.edges) {
      totalW += e.w;
    }
    // UNDONE: EXPECT_TRUE(isAcyclicU(mst));
    // UNDONE: EXPECT_TRUE(isMST(mst,graphw));
    ASSERT_EQ(totalW, 83);
  }
}

TEST(ELTBoruvka, simple1) {
  EdgeListW graphW = GRAPH1;

  for (int i = 0; i < 10; i++) {
    // UNDONE: graphW.edges.shuffle();
    EdgeListW mst = ELTBoruvka(graphW);
    WeightType totalW = 0;
    for (auto& e : mst.edges) {
      totalW += e.w;
    }
    // UNDONE: EXPECT_TRUE(isAcyclicU(mst));
    // UNDONE: EXPECT_TRUE(isMST(mst,graphw));
    EXPECT_EQ(totalW, 40);
  }
}

TEST(ELTBoruvka, simple2) {
  EdgeListW graphW = GRAPH2;

  for (int i = 0; i < 10; i++) {
    // UNDONE: graphW.edges.shuffle();
    EdgeListW mst = ELTBoruvka(graphW);
    WeightType totalW = 0;
    for (auto& e : mst.edges) {
      totalW += e.w;
    }
    // UNDONE: EXPECT_TRUE(isAcyclicU(mst));
    // UNDONE: EXPECT_TRUE(isMST(mst,graphw));
    ASSERT_EQ(totalW, 83);
  }
}

/*
UNDONE() : Implement ELTBoruvka
TEST(ELTBoruvka, simple) {
  vector<Edge> _edges = {{0, 1}, {0, 5}, {1, 2}, {1, 5}, {2, 3},
                         {2, 4}, {2, 5}, {3, 4}, {4, 5}};
  EdgeList G = EdgeList(6, _edges);

  for (int i = 0; i < 1000; i++) {
    int mincut = ELTBoruvka(G);
    ASSERT_LE(mincut, 3);
    ASSERT_GE(mincut, 2);
  }
}*/