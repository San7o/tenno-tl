# ====================================================================
# Project:   Brenta Engine
# Author:    Giovanni Santini
# Mail:      giovanni.santini@proton.me
# Github:    @San7o
# License:   MIT
# ====================================================================

cmake_minimum_required(VERSION 3.16)

project(tenno
  VERSION 0.2.0
  DESCRIPTION "C++ data structures and containers"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # no gnu extensions
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_program(CCACHE_PROGRAM ccache) # use ccache if available
if(CCACHE_PROGRAM)
  message(STATUS "Using ccache: ${CCACHE_PROGRAM}")
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

set(TENNO_HEADERS include)
set(TENNO_COMPILE_OPTIONS
  -Wall -Wextra -Wpedantic -Werror -Wconversion -Wshadow)

file(GLOB TENNO_TEST_SOURCES
  tests/*.cpp
  fuzz/*.cpp
  benchmarks/*.cpp)

set(TENNO_TEST_HEADERS)
set(TENNO_LINK_LIBRARIES)

#
# Options
#

option(TENNO_BUILD_TESTS "Build tests" ON)
option(TENNO_USE_CLANG "Use clang" OFF)
option(TENNO_OPTIMIZE "O3 optimization" OFF)
option(TENNO_OPTIMIE_AGGRESSIVE "Highest possible optimizations" OFF)

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
if (TENNO_BUILD_TESTS)
  list(APPEND TENNO_COMPILE_OPTIONS -DTENNO_DEBUG -g)
endif()

#
# Dependencies
#

# Use CPM for dependency management
include(cmake/CPM.cmake)
CPMUsePackageLock(package-lock.cmake)

CPMAddPackage(
  NAME valfuzz
  GITHUB_REPOSITORY San7o/valFuzz
  GIT_TAG origin/main)
list(APPEND TENNO_LINK_LIBRARIES valfuzz)

#
# Targets
#

if(TENNO_BUILD_TESTS AND valfuzz_ADDED)
  add_executable(${PROJECT_NAME}_tests
    ${TENNO_TEST_SOURCES})
  target_include_directories(${PROJECT_NAME}_tests PRIVATE
    ${TENNO_HEADERS}
    ${valfuzz_SOURCE_DIR}/include
    ${TENNO_TEST_HEADERS})
  target_compile_options(${PROJECT_NAME}_tests PRIVATE
    ${TENNO_COMPILE_OPTIONS})
  target_link_libraries(${PROJECT_NAME}_tests PRIVATE
    valfuzz
    ${TENNO_LINK_LIBRARIES})
endif()

#
# End of CMakeLists.txt
# ====================================================================
