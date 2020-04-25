#include "structures/baseutils/assertions.hpp"
#include "structures/graph/adjacency_list.hpp"
#include "structures/graph/adjacency_matrix.hpp"
#include "structures/graph/compressed_sparse_rows.hpp"
#include "structures/graph/edge_list.hpp"
#include "structures/ufds/disjoint_set.hpp"
#include "structures/ufds/disjoint_set_simple.hpp"
#include "structures/ufds/undoable_disjoint_set.hpp"

#include "tests/test_utils/test_utils.hpp"

#include <gtest/gtest.h>

#ifndef GTEST_HAS_TYPED_TEST
ERROR("NO typed tests")
#endif

using testing::Types;
#define TEST_HEADER() \
  SEED_RAND();        \
  using DSType = typename TestFixture::DSType;

template <class T>
struct UFDS_Base_Test : public testing::Test {
  typedef T DSType;

 protected:
  UFDS_Base_Test() {}
};
using UFDSImplementations =
    ::testing::Types<SimpleDisjointSet, DisjointSet, UndoableDisjointSet>;

TYPED_TEST_SUITE(UFDS_Base_Test, UFDSImplementations);

TYPED_TEST(UFDS_Base_Test, EmptyConstructor) {
  TEST_HEADER();
  DSType ds;
  ASSERT_EQ(ds.N, 0);
}

TYPED_TEST(UFDS_Base_Test, Constructor) {
  TEST_HEADER();

  DSType ds = DSType(5);
  for (int i = 0; i < 5; i++) {
    ASSERT_EQ(ds.Find(i + 1), i + 1);
  }
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      ASSERT_EQ(ds.AreConnected(i + 1, j + 1), (i == j));
    }
  }
}

TYPED_TEST(UFDS_Base_Test, Union1) {
  TEST_HEADER();

  DSType ds = DSType(5);
  ds.Union(0 + 1, 1 + 1);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      bool shouldBeEqual = i == j;
      shouldBeEqual = shouldBeEqual || (i == 0 && j == 1);
      shouldBeEqual = shouldBeEqual || (i == 1 && j == 0);
      ASSERT_EQ(ds.AreConnected(i + 1, j + 1), shouldBeEqual);
    }
  }

  ds.Union(2 + 1, 3 + 1);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      bool shouldBeEqual = i == j;
      shouldBeEqual = shouldBeEqual || (i == 0 && j == 1);
      shouldBeEqual = shouldBeEqual || (i == 1 && j == 0);
      shouldBeEqual = shouldBeEqual || (i == 2 && j == 3);
      shouldBeEqual = shouldBeEqual || (i == 3 && j == 2);
      ASSERT_EQ(ds.AreConnected(i + 1, j + 1), shouldBeEqual);
    }
  }

  ds.Union(2 + 1, 4 + 1);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      bool shouldBeEqual = i == j;
      shouldBeEqual = shouldBeEqual || (i == 0 && j == 1);
      shouldBeEqual = shouldBeEqual || (i == 1 && j == 0);
      shouldBeEqual = shouldBeEqual || (i == 2 && (j == 3 || j == 4));
      shouldBeEqual = shouldBeEqual || (i == 3 && (j == 2 || j == 4));
      shouldBeEqual = shouldBeEqual || (i == 4 && (j == 2 || j == 3));
      ASSERT_EQ(ds.AreConnected(i + 1, j + 1), shouldBeEqual);
    }
  }

  ds.Union(2 + 1, 3 + 1);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      bool shouldBeEqual = i == j;
      shouldBeEqual = shouldBeEqual || (i == 0 && j == 1);
      shouldBeEqual = shouldBeEqual || (i == 1 && j == 0);
      shouldBeEqual = shouldBeEqual || (i == 2 && (j == 3 || j == 4));
      shouldBeEqual = shouldBeEqual || (i == 3 && (j == 2 || j == 4));
      shouldBeEqual = shouldBeEqual || (i == 4 && (j == 2 || j == 3));
      ASSERT_EQ(ds.AreConnected(i + 1, j + 1), shouldBeEqual);
    }
  }

  ds.Union(3 + 1, 1 + 1);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      ASSERT(ds.AreConnected(i + 1, j + 1));
    }
  }
}

TYPED_TEST(UFDS_Base_Test, Random1) {
  TEST_HEADER();

  const int SET_SIZE = 1000;
  auto REF_TRANSFORM = [](int x) { return SET_SIZE - x + 1; };
  DSType ds = DSType(SET_SIZE);
  DisjointSet reference = DisjointSet(SET_SIZE * 2);

  for (int i = 1; i <= SET_SIZE; i++) {
    reference.Union(i, SET_SIZE + i);
  }

  for (int i = 0; i < SET_SIZE * 100; i++) {
    int a, b, c, d;
    a = (random() % SET_SIZE) + 1;
    b = (random() % SET_SIZE) + 1;
    c = random() % 2;
    switch (c) {
      case 0:
        ASSERT_EQ(ds.AreConnected(a, b),
                  reference.AreConnected(REF_TRANSFORM(a), REF_TRANSFORM(b)));
        break;
      case 1:
        d = random() % 2;

        ds.Union(a, b);
        if (d == 0) {
          reference.Union(REF_TRANSFORM(a), REF_TRANSFORM(b));
        } else if (d == 1) {
          reference.Union(REF_TRANSFORM(a) + SET_SIZE,
                          REF_TRANSFORM(b) + SET_SIZE);
        }
        break;
      default:
        assert(false);
    }
  }
}

TEST(UFDS_Undo_Test, Random1) {
  const int SET_SIZE = 400;
  SEED_RAND();
  UndoableDisjointSet ds = UndoableDisjointSet(SET_SIZE);
  DisjointSet reference = DisjointSet(SET_SIZE);
  vector<DisjointSet> crossCheck;
  int checkpoints = 0;

  for (int i = 0; i < SET_SIZE * 100; i++) {
    int a, b, c;
    a = (random() % SET_SIZE) + 1;
    b = (random() % SET_SIZE) + 1;
    c = random() % 4;

    switch (c) {
      case 0:
        ASSERT_EQ(ds.AreConnected(a, b), reference.AreConnected(a, b));
        break;
      case 1:
        ds.Union(a, b);
        reference.Union(a, b);
        break;
      case 2:
        checkpoints++;
        crossCheck.push_back(reference);
        ds.SaveState();
        break;
      case 3:
        if (checkpoints > 0) {
          checkpoints--;
          ds.RestoreState();
          reference = crossCheck.back();
          crossCheck.pop_back();
        }
        break;
      default:
        assert(false);
    }
  }
}

TEST(UFDS_Undo_Test, Bug) {
  const int SET_SIZE = 4;
  UndoableDisjointSet ds = UndoableDisjointSet(SET_SIZE);
  ds.Union(1, 3);
  ds.Union(2, 3);
  ds.SaveState();
  ds.Union(1, 4);
  ds.AreConnected(1, 1);
  ds.RestoreState();
}