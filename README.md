# CXX_Thread

## Development environment

**Available C++ Compilers**

- GCC 9.4.0 x86_64-linux-gnu
- GCC 11.2.0 aarch64-apple-darwin21

**VSCode extensions**

- C/C++
- CMake
- CMake Tools
- clang-format

## DOCS

1. [Chapter_1](./docs/chapter_1.md)

2. [Chapter_2](./docs/chapter_2.md)

3. [Chapter_3](./docs/chapter_3.md)

## DEMO

- [background_task](./demo/background_task.cpp)
- [Thread](./demo/Thread/)
- [threadsafe_stack](./demo/threadsafe_stack/)
- [async_to_get_answer](../demo/async_to_get_answer/)
- [ThreadPool](./demo/ThreadPool/)
- [thread_class](./demo/thread_class/)
- [QThread](./demo/QThread_demo/)

## Thread Status

[Thread States in Operating Systems](https://www.geeksforgeeks.org/thread-states-in-operating-systems/)

![](https://media.geeksforgeeks.org/wp-content/uploads/20191120195346/States-of-a-thread.png)

## What i know now

1. Thread History

2. How to use a simple std::thread
  - start
    - give a function or some params
  - detach
  - join
  - joinable
  - std::move
  - thread id (only)

3. Share Data
  - mutex
  - lock
  - interface

4. Synchronous operations

## Project

[Replace_Qt](./project/replace_qt/README.md)
