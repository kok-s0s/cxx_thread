cmake_minimum_required(VERSION 3.0.0)
project(thread VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 20)

add_executable(thread
  main.cpp
  CustomThread.cpp
)
