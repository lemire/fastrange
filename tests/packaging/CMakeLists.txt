# Standalone project used to check that an *installed* fastrange can be picked
# up with find_package(). It is deliberately not part of the main build:
# configure it separately, pointing CMAKE_PREFIX_PATH at the install prefix.
#
#   cmake -S tests/packaging -B build-packaging -DCMAKE_PREFIX_PATH=/your/prefix
#   cmake --build build-packaging
#   ctest --test-dir build-packaging
cmake_minimum_required(VERSION 3.14...3.31)

project(fastrange_packaging_test LANGUAGES C CXX)

find_package(fastrange 1.0 REQUIRED)

enable_testing()

add_executable(consumer_c consumer.c)
target_link_libraries(consumer_c PRIVATE fastrange::fastrange)
add_test(NAME consumer_c COMMAND consumer_c)

add_executable(consumer_cpp consumer.cpp)
target_link_libraries(consumer_cpp PRIVATE fastrange::fastrange)
add_test(NAME consumer_cpp COMMAND consumer_cpp)
