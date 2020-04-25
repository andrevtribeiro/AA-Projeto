#include <algorithm>
#include "algorithms/mst/boruvka.hpp"
#include "structures/graph/edge_list.hpp"
#include "structures/ufds/disjoint_set.hpp"
using namespace std;

int main() {
  // EdgeListW graphW = EdgeListW(4, {{0, 2}, {0, 3}, {1, 2}, {1, 3}, {1, 4},
  // {2, 3}, {2, 4}, {3, 4}});
  // Exemplo 1 wikipedia:
  EdgeListW graphW = EdgeListW(
      12, {{0, 1, 13}, {0, 2, 6},   {1, 2, 7},  {1, 3, 1},   {2, 3, 14},
           {2, 4, 8},  {2, 7, 20},  {3, 4, 9},  {3, 5, 3},   {4, 5, 2},
           {4, 9, 18}, {6, 7, 15},  {6, 8, 5},  {6, 9, 19},  {6, 10, 10},
           {7, 9, 17}, {8, 10, 11}, {9, 11, 4}, {9, 10, 16}, {10, 11, 12}});
  // Exemplo 2 wikipedia:
  // EdgeListW graphW = EdgeListW(7, {{0,1,7}, {0,3,4}, {1,2,11}, {1,3,9},
  // {1,4,10}, {2,4,5}, {3,4,15}, {3,5,6}, {4,5,12}, {4,6,8}, {5,6,13}});

  EdgeListW mst = Boruvka(graphW);
  int total = 0;
  for (EdgeW& e : mst.edges) {
    cout << e.u << " " << e.v << " " << e.w << endl;
    total += e.w;
  }
  cout << total << endl;
}