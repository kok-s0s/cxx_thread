cmake_minimum_required(VERSION 3.0.0)
project(signal_test VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(sigslot)

add_executable(signal_test main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
