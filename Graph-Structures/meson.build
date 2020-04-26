project('fastnet', 'cpp',
  version : '1.0.0',
  license : 'MIT',
  meson_version : '>= 0.47.0',
  default_options: [
    'cpp_std=c++17',
    'warning_level=3',
    'buildtype=release'
  ]
)

project_sources = []
project_headers = []
project_test_sources = []
project_benchmark_sources = []

compiler = meson.get_compiler('cpp')
warning_level = get_option('warning_level').to_int()
if compiler.get_id() == 'gcc'
    message('Compiler: GCC')
    libs_compiler = ['-lstdc++fs', '-fopenmp']
    libs_linker   = ['-lstdc++fs', '-fopenmp']
elif compiler.get_id() == 'clang'
    message('Compiler: LLVM/clang')
    libs_compiler = ['-stdlib=libc++']
    libs_linker   = ['-stdlib=libc++', '-lstdc++fs']
endif

add_global_arguments('-Wall', '-Wextra', '-Wunused',
                     '-O3', '-Ofast',
                     #'-g',
                     # UNDONE: upgrade to packages or create source files
                     '-Wno-missing-declarations',
                     libs_compiler,
                     language: 'cpp')

# for tests:
add_global_arguments('-Wno-gnu-zero-variadic-macro-arguments', language: 'cpp')

add_global_link_arguments(libs_linker, language: 'cpp')

# Actual instantiation of external dependencies:
subdir('third_party')

# Include creation:
inc = include_directories('./')

# Subdirs:
subdir('structures')
subdir('algorithms')

# Executables:
main_sources = files('main.cpp')
project_sources += main_sources

main_exec = executable('main', main_sources,
                    include_directories : inc,
                    install : true,
                    link_with: graph_utils_lib
                    )

# test('main test', main_exec, timeout: 1)
subdir('tests')
subdir('benchmarks')

# This adds the clang format file to the build directory
configure_file(input : '.clang-format',
               output : '.clang-format',
      	       copy: true)
run_target('format',
  command : ['clang-format','-i','-style=file',project_sources,project_test_sources,project_headers])

# This regex excludes any sources from the third_party, tests, benchmarks 
# and gtest related files.
regex = '^((?!(third_party|tests|benchmarks|gtest|subprojects)).)*$'
# This adds clang tidy support
configure_file(input : '.clang-tidy',
               output : '.clang-tidy',
               copy : true)
run_target('tidy',
  command : ['run-clang-tidy','-fix', '-j' , '8', 'files', regex ,'-format', '-p='+ meson.build_root()])