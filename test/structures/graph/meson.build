
# Header files:

project_headers += files('graph.hpp')
project_headers += files('adjacency_list.hpp')
project_headers += files('adjacency_matrix.hpp')
project_headers += files('compressed_sparse_rows.hpp')

# Source files:

graph_utils_sources = files('utils.cpp')
project_sources += graph_utils_sources
graph_utils_lib = library('graph_utils',
                        graph_utils_sources,
                        include_directories : inc,
                        install : true)