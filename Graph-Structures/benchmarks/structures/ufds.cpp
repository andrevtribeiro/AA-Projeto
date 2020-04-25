#include "structures/ufds/disjoint_set_simple.hpp"
#include "structures/ufds/disjoint_set.hpp"
#include "structures/ufds/undoable_disjoint_set.hpp"
#include "benchmark/benchmark.h"
#include "benchmarks/util/benchmark_assertions.hpp"
#include <math.h>
using std::vector;
using std::tuple;

template<class DSTYPE>
int ufds_do_ops(int N, vector<tuple<bool,int,int> >& ops) {
  DSTYPE ds(N);
  int cnt = 0;
  for (auto&& [isJoin, i, j] : ops) {
    if (isJoin) {
      ds.Union(i,j);
    } else {
      if (ds.AreConnected(i,j)) {
        cnt++;
      }
    }
  }
  return cnt;
}

static void gen_graph(vector<tuple<bool,int,int> >& ops, benchmark::State &state) {
  int opsCnt = state.range(0);
  int N = int(opsCnt/100) + 1;
  ops.clear();
  for (int i=0; i<opsCnt; i++) {
    ops.push_back(make_tuple<>(random()%2==0, (random()%N)+1, (random()%N)+1));
  }
}

static void BM_UDFS(benchmark::State &state) {
  int opsCnt = state.range(0);
  int N = int(opsCnt/100) + 1;

  vector<tuple<bool,int,int> > ops;
  for (auto _ : state) {
    state.PauseTiming();
    gen_graph(ops, state);
    state.ResumeTiming();
    benchmark::DoNotOptimize(ufds_do_ops<DisjointSet>(N,ops));
  }
   
  state.SetComplexityN(state.range(0)); 
}

BENCHMARK(BM_UDFS)
    ->RangeMultiplier(4)->Range(1<<10, 1<<22)->Complexity();
BENCHMARK(BM_UDFS)
    ->RangeMultiplier(4)->Range(1<<10, 1<<22)->Complexity([](int64_t n) { return 13*((double)n); });
    
    
static void BM_SUDFS(benchmark::State &state) {
  int opsCnt = state.range(0);
  int N = int(opsCnt/100) + 1;

  vector<tuple<bool,int,int> > ops;
  for (auto _ : state) {
    state.PauseTiming();
    gen_graph(ops, state);
    state.ResumeTiming();
    benchmark::DoNotOptimize(ufds_do_ops<SimpleDisjointSet>(N,ops));
  }
   
  state.SetComplexityN(state.range(0)); 
}

BENCHMARK(BM_SUDFS)
    ->RangeMultiplier(4)->Range(1<<10, 1<<22)->Complexity();
BENCHMARK(BM_SUDFS)
    ->RangeMultiplier(4)->Range(1<<10, 1<<22)->Complexity([](int64_t n) { return 11*((double)n); });
    
    
static void BM_UUDFS(benchmark::State &state) {
  int opsCnt = state.range(0);
  int N = int(opsCnt/100) + 1;

  vector<tuple<bool,int,int> > ops;
  for (auto _ : state) {
    state.PauseTiming();
    gen_graph(ops, state);
    state.ResumeTiming();
    benchmark::DoNotOptimize(ufds_do_ops<UndoableDisjointSet>(N,ops));
  }
   
  state.SetComplexityN(state.range(0)); 
}

BENCHMARK(BM_UUDFS)
    ->RangeMultiplier(4)->Range(1<<10, 1<<22)->Complexity();
BENCHMARK(BM_UUDFS)
    ->RangeMultiplier(4)->Range(1<<10, 1<<22)->Complexity([](int64_t n) { return 32*((double)n); });