# Test compilation rules:
l_test_sources = files(
  'mincut.cpp',
  'mst.cpp'
)

project_test_sources += l_test_sources

gtest_algorithms = executable(
  'gtest_algorithms',
  [l_test_sources, test_main],
  include_directories : inc,
  cpp_args: '-Wno-sign-compare',
  dependencies : [test_deps], 
)

test('gtest tests/algorithms', gtest_algorithms, timeout: 1)