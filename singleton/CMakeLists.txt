cmake_minimum_required(VERSION 3.10.0)
project(wzq_singleton)

set (CMAKE_CXX_FLAGS "--std=c++17")

add_executable(test_singleton test/test.cc)