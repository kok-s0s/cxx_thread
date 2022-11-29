#include <chrono>
#include <iostream>
#include <vector>

#include "ThreadPool.h"

int main() {

  // create thread pool with 4 worker threads
  ThreadPool pool(4);
  std::vector<std::future<int>> results;

  for (int i = 0; i < 8; ++i) {
    // enqueue and store future
    results.emplace_back(pool.enqueue([i] {
      std::cout << "hello " << i << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
      std::cout << "world " << i << std::endl;
      return i * i;
    }));
  }

  for (auto &&result : results)
    // get result from future
    std::cout << result.get() << ' ';
  std::cout << std::endl;

  return 0;
}
