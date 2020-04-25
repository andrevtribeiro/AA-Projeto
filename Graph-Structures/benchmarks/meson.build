benchmark_deps = [
    gbenchmark_dep, 
    thread_dep
]

benchmark_dep_libs = [

]

benchmark_dep_libs = []
benchmark_sources = []


benchmark_main = files('benchmark_main.cpp')

all_benchmark_sources = []
all_benchmark_deps = []
all_benchmark_dep_libs = []

subdir('structures')

# This executable contains all the benchmarks
all_benchmark_sources += benchmark_main
all_benchmark_deps += benchmark_deps
all_benchmark_dep_libs += benchmark_dep_libs

project_benchmark_sources += all_benchmark_sources

all_benchmarkes = executable('all_benchmarks', 
    all_benchmark_sources, 
    include_directories : inc, 
    dependencies : all_benchmark_deps, 
    link_with : all_benchmark_dep_libs)