cmake_minimum_required(VERSION 3.16)

project(qthread VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

add_executable(qthread
    main.cpp
    worker.cpp
    boss.cpp
    consumer.cpp
    producer.h
)

target_link_libraries(qthread PRIVATE Qt6::Core)
