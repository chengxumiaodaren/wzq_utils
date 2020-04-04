cmake_minimum_required(VERSION 3.0.0)
project(wzq_utils)

message( STATUS "CMake version ${CMAKE_VERSION}" )
message (STATUS "source dir: ${CMAKE_SOURCE_DIR}")
message (STATUS "build type: ${CMAKE_BUILD_TYPE}")
message (STATUS "compiler: ${CMAKE_CXX_COMPILER}")

set (CMAKE_CXX_FLAGS "--std=c++17")

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/thread/include/
                    ${CMAKE_SOURCE_DIR}/timer/include/
                    ${CMAKE_SOURCE_DIR}/singleton/include/
                    ${CMAKE_SOURCE_DIR}/common/include/)

add_subdirectory(thread)
add_subdirectory(singleton)

add_executable(test_wzq test.cc)
target_link_libraries(test_wzq pthread)

# target_link_libraries(test_thread wzq_thread)