#include <algorithm>
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
  return 0;
}
