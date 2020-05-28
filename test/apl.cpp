#include <algorithm>
#include "algorithms/hyperball/hyperball.hpp"
#include "structures/graph/compressed_sparse_rows.hpp"
#include <random>
#include <set>

using namespace std;

int main(int argc, char** argv) {
  CSRGraph graph = ReadGraphAsCSR();

  hyperball(graph, stod(argv[1]));

}
