#include <gtest/gtest.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

#include <chrono>
#include <iostream>

#include "Human.h"

// Test that the signal and slot functions work correctly
TEST(Human, say_hello_and_goodbye) {
  PLOGD << "--------------------------------------------";
  PLOGD << "say hello and goodbye";
  std::shared_ptr<Human> human = std::make_shared<Human>("John");

  human->SendSayHelloSignal();
  EXPECT_EQ(human->GetSentence(), "Hello, John!");

  human->SendSayGoodByeSignal();
  EXPECT_EQ(human->GetSentence(), "GoodBye, John!");
}

// Test whether parameters can be passed correctly
TEST(Human, will_do) {
  PLOGD << "--------------------------------------------";
  PLOGD << "will do";
  std::shared_ptr<Human> human = std::make_shared<Human>("Alice");

  human->SendWillDoSignal("read a book");
  EXPECT_EQ(human->GetSentence(), "I'm going to go to read a book");
}

// Test whether multiple parameters can be passed correctly
TEST(Human, plan_to_do) {
  PLOGD << "--------------------------------------------";
  PLOGD << "plan to do";
  std::shared_ptr<Human> human = std::make_shared<Human>("Bob");

  Plan plan{"08:00", "10:00", "Study"};
  human->SendPlanToDoSignal(plan);
  EXPECT_EQ(human->GetSentence(), "I plan to Study from 08:00 to 10:00.");
}

// Test whether two thread objects can interact with each other
TEST(Human, chat) {
  PLOGD << "--------------------------------------------";
  PLOGD << "chat";
  std::shared_ptr<Human> xiaoMing = std::make_shared<Human>("xiaoMing");
  std::shared_ptr<Human> xiaoHong = std::make_shared<Human>("xiaoHong");

  xiaoMing->SendAskAQuestionSignal(xiaoHong, "Do you like cats?");
  EXPECT_EQ(xiaoMing->GetSentence(), "xiaoHong, Do you like cats?");
  EXPECT_EQ(xiaoHong->GetQuestionFromOtherPeople(), "Do you like cats?");

  xiaoHong->SendAnswerAQuestionSignal(xiaoMing, "Yes! I like!");
  EXPECT_EQ(xiaoHong->GetSentence(), "xiaoMing, Yes! I like!");
  EXPECT_EQ(xiaoMing->GetAnswerFromOtherPeople(), "Yes! I like!");
}

// Test that the timer works correctly
TEST(Human, want_to_sleep) {
  PLOGD << "--------------------------------------------";
  PLOGD << "want to sleep";
  std::shared_ptr<Human> human = std::make_shared<Human>("k");
  PLOGD << "start timer";
  human->CreateTimer();
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  PLOGD << "stop timer";
  human->DestroyTimer();
  EXPECT_GT(human->GetCountSayWantToSleep(), 0);
  EXPECT_EQ(human->GetSentence(), "I want to sleep.");
}

// Test asynchronous operations
TEST(Human, async_operations) {
  PLOGD << "--------------------------------------------";
  PLOGD << "async operations";
  std::shared_ptr<Human> person1 = std::make_shared<Human>("Person1");
  std::shared_ptr<Human> person2 = std::make_shared<Human>("Person2");

  // Send asynchronous signals
  person1->SendWillDoSignal("async activity");
  person2->SendWillDoSignal("async task");

  // Allow time for asynchronous signals to be processed
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // Check if the sentences have been updated
  EXPECT_EQ(person1->GetSentence(), "I'm going to go to async activity");
  EXPECT_EQ(person2->GetSentence(), "I'm going to go to async task");
}

int main(int argc, char **argv) {
  // Set up the log file
  auto now = std::chrono::system_clock::now();
  std::time_t t = std::chrono::system_clock::to_time_t(now);
  std::string timeStr = std::ctime(&t);
  timeStr.erase(std::remove(timeStr.begin(), timeStr.end(), '\n'),
                timeStr.end());

  // Set the log file name to include the current time
  static plog::RollingFileAppender<plog::TxtFormatter> fileAppender(
      ("cxx_thread_" + timeStr + ".log").c_str(), 1000000, 3);
  plog::init(plog::verbose, &fileAppender);

  // Run the tests
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
