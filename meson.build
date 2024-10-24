project('tennoTL', 'cpp',
  version : '0.1.0',
  license : 'MIT',
  default_options: [
    'cpp_std=c++23',
    'build.cpp_std=c++23'
  ]
)
add_project_arguments(
  [
    '-Wall',
    '-Wextra',
    '-pedantic',
    '-Werror',
    '-Wconversion',
    '-Wshadow'
  ],
  language: 'cpp'
)

# dependencies

cmake = import('cmake')
cmake_opts = cmake.subproject_options()
cmake_opts.add_cmake_defines({
  'VALFUZZ_BUILD_SHARED': 'ON'
})
valfuzz_proj = cmake.subproject(
  'valfuzz',
  options: cmake_opts
)
valfuzz_dep = valfuzz_proj.dependency('valfuzz_shared')

# project
if get_option('TENNO_BUILD_OPTIMIZED_AGGRESSIVE')
  add_project_arguments('-O3', '-march=native', '-ffast-math', language: 'cpp')
elif get_option('TENNO_BUILD_OPTIMIZED')
  add_project_arguments('-O3', language: 'cpp')
else
  add_project_arguments('-O0', language: 'cpp')
endif
if get_option('TENNO_BUILD_TESTS')
  add_project_arguments('-D TENNO_DEBUG', language: 'cpp')
endif

if get_option('TENNO_BUILD_SHARED')
  shared_library('tennoTL',
    include_directories: [
      'include',
    ],
    dependencies: [
      valfuzz_dep
    ],
  )
endif
if get_option('TENNO_BUILD_STATIC')
  static_library('tennoTL',
    include_directories: [
      'include',
    ],
    dependencies: [
      valfuzz_dep
    ],
  )
endif
if get_option('TENNO_BUILD_TESTS')
  test_sources = files(
    'tests/array_test.cpp',
    'tests/expected_test.cpp',
    'tests/ranges_test.cpp',
    'tests/algorithm_test.cpp',
    'tests/atomic_test.cpp',
    'tests/mutex_test.cpp',
    'tests/optional_test.cpp',
    'tests/shared_ptr_test.cpp',
    'tests/utility_test.cpp',
    'tests/memory_test.cpp',
    'tests/unique_ptr_test.cpp',
    'tests/thread_test.cpp',
    'tests/weak_ptr_test.cpp',
    'tests/vector_test.cpp',
    'tests/functional_test.cpp',
    'benchmarks/array_benchmark.cpp',
    'fuzz/array_fuzz.cpp',
  )
  executable('tests',
    sources: test_sources,
    include_directories: [
      'include',
    ],
    dependencies: [
      valfuzz_dep
    ],
  )
endif
