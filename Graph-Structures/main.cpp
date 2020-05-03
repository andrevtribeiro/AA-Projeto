#include <algorithm>
#include "algorithms/hyperball/hyperball3.hpp"
#include "algorithms/bc/bc3.hpp"
#include "structures/graph/compressed_sparse_rows.hpp"
#include <random>
#include <set>

using namespace std;

vector<pair<int,int> > randomGraph(int N, uint64_t M){
    vector<pair<int,int> > ret;
    set<pair<int,int> > used;
    int colisions = 0;
    srand(time(0));

    while(ret.size() != M){
      int a = random()%N;
      int b = random()%N;

      if(a != b && used.count({a,b}) == 0){
        ret.emplace_back(pair<int, int>{a, b});
        used.insert(pair<int, int>{a,b});
        used.insert(pair<int, int>{b,a});
      } else {
        colisions++;
      }
    }
    return ret;
}

vector<pair<int, int> > randomERGraph(int N, double p){
  std::default_random_engine generator;
  generator.seed(time(0));
  std::binomial_distribution<int> distribution(N*(N-1)/2, p);

  uint64_t M = distribution(generator);
  return randomGraph(N, M);

}



int main(int argc, char** argv) {

  // vector<pair<int, int> > graph = randomERGraph(atol(argv[1]), 0.2);
  // int counter = 0;
  // cout << atol(argv[1]) << " " << graph.size() << endl;
  // for(pair<int, int> & e : graph){
  //   cout << e.first << " " << e.second << endl;
  //   counter ++;
  // }

  // EdgeListW graphW = EdgeListW(4, {{0, 2}, {0, 3}, {1, 2}, {1, 3}, {1, 4},
  // {2, 3}, {2, 4}, {3, 4}});
  // Exemplo 1 wikipedia:
 
  // Exemplo 2 wikipedia:
  // EdgeListW graphW = EdgeListW(7, {{0,}, {0,}, {1,}, {1,3,9},
  // {1,4,10}, {2,4,5}, {3,4,15}, {3,5,6}, {4,5,12}, {4,6,8}, {5,6,13}});

  // EdgeListW mst = Boruvka(graphW);
  CSRGraph graph = ReadUndirectedGraphAsCSR();
  
  // 0.01, 0.1, 0.5
  bc(graph, stod(argv[1]), stod(argv[2]), stod(argv[3]));

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
