cmake_minimum_required(VERSION 3.25.0)

project(Tracer VERSION 0.0.0 LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

enable_testing()

include(NoInSourceBuilds)
include(EnableIPOSupport)
include(Precompiling)
include(BuildInfo)
include(Coverage)
include(Cppcheck)
include(Doxygen)
include(Format)

add_subdirectory(src bin)
add_subdirectory(test)
add_subdirectory(benchmark)
