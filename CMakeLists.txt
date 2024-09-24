cmake_minimum_required(VERSION 3.20)
project(
    tenno
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23 REQUIRED)
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(TENNO_SOURCES
        src/tenno.cpp
)
set(TENNO_HEADERS include)
set(TENNO_COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Werror -Wconversion -Wshadow)
set(TENNO_TEST_SOURCES
        tests/array_test.cpp
        tests/ranges_test.cpp
        tests/expected_test.cpp
        tests/algorithm_test.cpp
        tests/atomic_test.cpp
        tests/mutex_test.cpp
        fuzz/array_fuzz.cpp
        benchmarks/array_benchmark.cpp
)
set(TENNO_TEST_HEADERS)
set(TENNO_LINK_LIBRARIES)

option(TENNO_USE_CLANG "Use clang" OFF)
option(TENNO_BUILD_TESTS "Build tests" ON)
option(TENNO_BUILD_SHARED "Build shared library" OFF)
option(TENNO_BUILD_STATIC "Build static library" OFF)
option(TENNO_OPTIMIZE "O3 optimization" OFF)
option(TENNO_OPTIMIE_AGGRESSIVE "Highest possible optimizations" OFF)

include(cmake/CPM.cmake) # <3
CPMUsePackageLock(package-lock.cmake)
CPMAddPackage(
    NAME valfuzz
    GITHUB_REPOSITORY San7o/valFuzz
    GIT_TAG v1.0.2
    DOWNLOAD_ONLY True
)
if (valfuzz_ADDED)
    list(APPEND TENNO_TEST_SOURCES ${valfuzz_SOURCE_DIR}/src/valfuzz.cpp)
    list(APPEND TENNO_TEST_HEADERS ${valfuzz_SOURCE_DIR}/include)
endif()

if(TENNO_USE_CLANG)
    set(CMAKE_CXX_COMPILER clang++)
    list(APPEND TENNO_COMPILE_OPTIONS -std=c++23 -fexperimental-library)
    list(APPEND TENNO_LINK_LIBRARIES -fexperimental-library)
endif()
if(TENNO_OPTIMIZE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()
if(TENNO_OPTIMIZE_AGGRESSIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math -march=native")
endif()

if(TENNO_BUILD_SHARED)
    add_library(tenno_shared SHARED ${TENNO_SOURCES})
    target_include_directories(tenno_shared PRIVATE ${TENNO_HEADERS})
    target_compile_options(tenno_shared PRIVATE ${TENNO_COMPILE_OPTIONS})
    target_link_libraries(tenno_shared PRIVATE ${TENNO_LINK_LIBRARIES})
endif()

if(TENNO_BUILD_STATIC)
    add_library(tenno_static STATIC ${TENNO_SOURCES})
    target_include_directories(tenno_static PRIVATE ${TENNO_HEADERS})
    target_compile_options(tenno_static PRIVATE ${TENNO_COMPILE_OPTIONS})
    target_link_libraries(tenno_static PRIVATE ${TENNO_LINK_LIBRARIES})
endif()

if(TENNO_BUILD_TESTS AND valfuzz_ADDED)
    add_executable(tests ${TENNO_SOURCES} ${TENNO_TEST_SOURCES})
    target_include_directories(tests PRIVATE ${TENNO_HEADERS} ${TENNO_TEST_HEADERS})
    target_compile_options(tests PRIVATE ${TENNO_COMPILE_OPTIONS})
    target_link_libraries(tests PRIVATE ${TENNO_LINK_LIBRARIES})
endif()
