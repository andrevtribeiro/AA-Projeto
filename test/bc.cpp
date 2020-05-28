#include "structures/graph/compressed_sparse_rows.hpp"
#include "algorithms/bc/bc3.hpp"
#include <algorithm>
#include <random>
#include <set>

using namespace std;

int main(int argc, char** argv) {
	if(argc != 5)
		return 0;

	int directed = atoi(argv[4]);
	CSRGraph graph;
	if(directed)
		graph = ReadGraphAsCSR();
	else
		graph = ReadUndirectedGraphAsCSR();

	bc(graph, stod(argv[1]), stod(argv[2]), stod(argv[3]), directed);
}
