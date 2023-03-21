cmake_minimum_required(VERSION 3.16.0)
project(thread VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME}
  main.cpp
  Human.cpp
)

target_link_libraries(${PROJECT_NAME}
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

enable_testing()