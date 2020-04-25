test_main = files('tests_main.cpp')

test_deps = [
    gtest_dep, 
    thread_dep
]

subdir('algorithms')
subdir('structures')
subdir('test_utils')


all_testes = executable('all_tests',
    [project_test_sources, files('tests_main.cpp')], 
    include_directories : inc, 
    dependencies : test_deps)
