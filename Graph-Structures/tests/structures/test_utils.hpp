#include "tests/test_utils/test_utils.hpp"
#include "structures/graph/graph.hpp"
#include <vector>

bool contains(const std::vector<EdgeW>& vec, const EdgeW& val, int start = 0);
bool contains(const std::vector<Edge>& vec, const Edge& val, int start = 0);
bool contains(const std::vector<NeighW>& vec, const NeighW& val, int start = 0);
bool contains(const std::vector<NodeId>& vec, const NodeId& val, int start = 0);
bool contains(const std::vector<EdgeW>& vec, const Edge& val, int start = 0);
bool contains(const std::vector<NeighW>& vec, const NodeId& val, int start = 0);