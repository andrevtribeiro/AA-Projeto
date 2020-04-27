#include <algorithm>
#include "algorithms/hyperball/hyperball3.hpp"
#include "structures/graph/compressed_sparse_rows.hpp"
using namespace std;

int main() {
  // EdgeListW graphW = EdgeListW(4, {{0, 2}, {0, 3}, {1, 2}, {1, 3}, {1, 4},
  // {2, 3}, {2, 4}, {3, 4}});
  // Exemplo 1 wikipedia:
 
  // Exemplo 2 wikipedia:
  // EdgeListW graphW = EdgeListW(7, {{0,}, {0,}, {1,}, {1,3,9},
  // {1,4,10}, {2,4,5}, {3,4,15}, {3,5,6}, {4,5,12}, {4,6,8}, {5,6,13}});

  // EdgeListW mst = Boruvka(graphW);

  CSRGraph graphW = ReadGraphAsCSR();
  hyperball(graphW,4096);

  /*uint64_t *c = (uint64_t*) calloc(1, sizeof(uint64_t)*16);
  int i=0;
  for(;i<30;i++){
    cout<<i<<" ";
    size(c);
    add(c,i);
  }
  cout<<i<<" ";
  size(c);*/
  
  
}
