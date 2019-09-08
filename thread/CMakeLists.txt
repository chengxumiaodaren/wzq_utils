cmake_minimum_required(VERSION 3.10.0)
project(wzq_thread)

set (CMAKE_CXX_FLAGS "--std=c++17")

add_library(wzq_thread src/count_down_latch.cc)
target_link_libraries(wzq_thread pthread)

add_executable(test_thread test/test.cc)
target_link_libraries(test_thread wzq_thread)