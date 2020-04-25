#pragma once
#include "structures/baseutils/assertions.hpp"
#include "structures/ufds/disjoint_set.hpp"

// This disjoint set assumes that 1 <= i <= N
//

struct UndoableDisjointSet : DisjointSet {
  struct HistoryOp {
    uint64_t id;
    DisjointSet::Node oldstate;
    uint64_t componentsCh;
  };

  stack<HistoryOp> history;

  UndoableDisjointSet() : DisjointSet() { ; }
  UndoableDisjointSet(uint64_t _N) : DisjointSet(_N) { ; }

  uint64_t Find(uint64_t i) {
    assert(i > 0);
    assert(i <= N);
    if (nodes[i].id != i) {
      history.push(HistoryOp{i, nodes[i], 0});
      uint64_t newId = Find(nodes[i].id);
      if (newId == nodes[i].id) {
        history.pop();
      }
      nodes[i].id = newId;
    }
    return nodes[i].id;
  }

  void Union(uint64_t i, uint64_t j) {
    assert(i > 0);
    assert(j > 0);
    i = Find(i);
    j = Find(j);
    if (i == j) return;
    if (nodes[i].rank < nodes[j].rank) swap(i, j);

    history.push(HistoryOp{i, nodes[i], 1});
    history.push(HistoryOp{j, nodes[j], 0});
    history.push(HistoryOp{nodes[j].prevroot, nodes[nodes[j].prevroot], 0});
    history.push(HistoryOp{nodes[j].tailnode, nodes[nodes[j].tailnode], 0});
    if (nodes[j].nextroot != static_cast<uint64_t>(-1)) {
      history.push(HistoryOp{nodes[j].nextroot, nodes[nodes[j].nextroot], 0});
    }

    DisjointSet::Union(i, j);
  }

  void SaveState() {
    history.push({static_cast<uint64_t>(-1), DisjointSet::Node(), 0});
  }

  void RestoreState() {
    assert(history.size() > 0);
    while (true) {
      HistoryOp t = history.top();
      if (t.id == static_cast<uint64_t>(-1)) {
        history.pop();
        break;
      }
      nodes[t.id] = t.oldstate;
      components += t.componentsCh;
      history.pop();
    }
  }

  // Gets all the component id's
  //
  vector<uint64_t> GetRoots() {
    // UNDONE(test): test this function
    vector<uint64_t> ret;
    uint64_t currRoot = nodes[0].nextroot;
    while (currRoot != static_cast<uint64_t>(-1)) {
      currRoot = Find(currRoot);
      ret.push_back(currRoot);
      currRoot = nodes[currRoot].nextroot;
    }
    return ret;
  }

  // Gets all the nodes on a given component
  //
  vector<uint64_t> GetNodesOfComponent(uint64_t root) {
    // UNDONE(test): test this function
    vector<uint64_t> ret;
    root = Find(root);
    while (root != static_cast<uint64_t>(-1)) {
      ret.push_back(root);
      root = nodes[root].nextnode;
    }
    return ret;
  }

  bool AreConnected(uint64_t i, uint64_t j) {
    assert(i > 0);
    assert(j > 0);
    return Find(i) == Find(j);
  }
};