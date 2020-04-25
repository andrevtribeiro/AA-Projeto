l_benchmark_sources = files(
    'ufds.cpp'
)

l_benchmark_deps = [
    
]

l_benchmark_lib_deps = [
    
]

l_benchmark = executable('ufds_benchmark', 
    [l_benchmark_sources, benchmark_main], 
    include_directories : inc,
    dependencies : [l_benchmark_deps,benchmark_deps], 
    link_with : [l_benchmark_lib_deps, benchmark_dep_libs])

benchmark('ufds benchmark', l_benchmark, timeout: 240)

all_benchmark_deps += l_benchmark_deps
all_benchmark_dep_libs += l_benchmark_lib_deps
project_benchmark_sources += l_benchmark_sources