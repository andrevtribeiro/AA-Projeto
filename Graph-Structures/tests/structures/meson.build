# Test compilation rules:
l_test_sources = files(
  'graph_base_tests.cpp',
  'graph_base_tests_w.cpp',
  'ufds_tests.cpp',
  
  'test_utils.cpp'
)

project_test_sources += l_test_sources
gtest_structures = executable(
  'gtest_structures',
  [l_test_sources, test_main],
  include_directories : inc,
  cpp_args: '-Wno-sign-compare',
  dependencies : [test_deps], 
)

test('gtest tests/structures', gtest_structures, timeout: 1)