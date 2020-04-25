#include <gtest/gtest.h>
#include <structures/baseutils/assertions.hpp>
#include "algorithms/mincut/karger.hpp"
#include "algorithms/mincut/kargerStein.hpp"
#include "structures/graph/edge_list.hpp"

TEST(Karger, simple) {
  vector<Edge> _edges = {{0, 1}, {0, 5}, {1, 2}, {1, 5}, {2, 3},
                         {2, 4}, {2, 5}, {3, 4}, {4, 5}};
  EdgeList G = EdgeList(6, _edges);

  for (int i = 0; i < 1000; i++) {
    int mincut = Karger(G);
    ASSERT_LE(mincut, 3);
    ASSERT_GE(mincut, 2);
  }
}

TEST(KargerStein, simple) {
  vector<Edge> _edges = {{0, 1}, {0, 5}, {1, 2}, {1, 5}, {2, 3},
                         {2, 4}, {2, 5}, {3, 4}, {4, 5}};
  EdgeList G = EdgeList(6, _edges);

  for (int i = 0; i < 1000; i++) {
    int mincut = KargerStein(G);
    ASSERT_LE(mincut, 3);
    ASSERT_GE(mincut, 2);
  }
}