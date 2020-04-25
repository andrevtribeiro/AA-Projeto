
#include "tests/structures/test_utils.hpp"
#include "structures/graph/graph.hpp"
using namespace std;

bool contains(const vector<Edge>& vec, const Edge& val, int start) {
  int ign = 0;
  for (const Edge& edge : vec) {
    if (ign++ < start) continue;
    if (edge.u == val.u && edge.v == val.v) return true;
  }
  return false;
}
bool contains(const vector<NodeId>& vec, const NodeId& val, int start) {
  int ign = 0;
  for (const NodeId& node : vec) {
    if (ign++ < start) continue;
    if (node == val) return true;
  }
  return false;
}
bool contains(const vector<EdgeW>& vec, const EdgeW& val, int start) {
  int ign = 0;
  for (const EdgeW& edge : vec) {
    if (ign++ < start) continue;
    if (edge.u == val.u && edge.v == val.v && edge.w == val.w) return true;
  }
  return false;
}
bool contains(const vector<NeighW>& vec, const NeighW& val, int start) {
  int ign = 0;
  for (const NeighW& node : vec) {
    if (ign++ < start) continue;
    if (node.v == val.v && node.w == val.w) return true;
  }
  return false;
}
bool contains(const vector<EdgeW>& vec, const Edge& val, int start) {
  int ign = 0;
  for (const EdgeW& edge : vec) {
    if (ign++ < start) continue;
    if (edge.u == val.u && edge.v == val.v) return true;
  }
  return false;
}
bool contains(const vector<NeighW>& vec, const NodeId& val, int start) {
  int ign = 0;
  for (const NeighW& node : vec) {
    if (ign++ < start) continue;
    if (node.v == val) return true;
  }
  return false;
}
