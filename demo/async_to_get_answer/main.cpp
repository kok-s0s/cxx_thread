#include <future>
#include <iostream>

int find_the_answer_to_ltuae() { return 22; }

int main() {
  std::future<int> the_answer = std::async(find_the_answer_to_ltuae);

  // do_other_stuff()
  int i = 10;
  while (i) {
    std::cout << "test" << std::endl;
    i--;
    if (i == 5) {
      std::cout << "The answer is " << the_answer.get() << std::endl;
    }
  }

  return 0;
}