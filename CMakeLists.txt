cmake_minimum_required(VERSION 3.0.0)
project(cxx_thread VERSION 0.1.0)

include(CTest)
enable_testing()

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

add_executable(cxx_thread main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)