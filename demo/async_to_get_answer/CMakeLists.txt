cmake_minimum_required(VERSION 3.0.0)
project(async_to_get_answer VERSION 0.1.0)

include(CTest)
enable_testing()

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

add_executable(async_to_get_answer main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
