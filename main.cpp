#include <gtest/gtest.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

#include <chrono>
#include <iostream>

#include "Human.h"

// Test that the signal and slot functions work correctly
TEST(Human, say_hello) {
  PLOGD << "say hello";
  std::shared_ptr<Human> human = std::make_shared<Human>("k");
  human->SendSayHelloSignal();
  EXPECT_EQ(human->GetSentence(), "Hello, k!");
  human->SendSayGoodByeSignal();
  EXPECT_EQ(human->GetSentence(), "GoodBye, k!");
  PLOGD << "say hello end";
}

// Test whether parameter can be passed correctly
TEST(Human, will_do) {
  PLOGD << "will do";
  std::shared_ptr<Human> human = std::make_shared<Human>("k");
  human->SendWillDoSignal("sleep");
  EXPECT_EQ(human->GetSentence(), "I'm going to go to sleep");
  PLOGD << "will do end";
}

// Test whether multiple parameters can be passed correctly
TEST(Human, plan_to_do) {
  PLOGD << "plan to do";
  std::shared_ptr<Human> human = std::make_shared<Human>("k");
  human->SendPlanToDoSignal("9:00", "13:00", "play the game");
  EXPECT_EQ(human->GetSentence(),
            "I plan to play the game from 9:00 to 13:00.");
  PLOGD << "plan to do end";
}

// Test whether two thread objects can interact with each other
TEST(Human, chat) {
  PLOGD << "chat";
  std::shared_ptr<Human> xiaoMing = std::make_shared<Human>("xiaoMing");
  std::shared_ptr<Human> xiaoHong = std::make_shared<Human>("xiaoHong");
  xiaoMing->SendAskAQuestionSignal(xiaoHong, "Do you like cats?");
  EXPECT_EQ(xiaoMing->GetSentence(), "xiaoHong, Do you like cats?");
  EXPECT_EQ(xiaoHong->GetQuestionFromOtherPeople(), "Do you like cats?");
  xiaoHong->SendAnswerAQuestionSignal(xiaoMing, "Yes! I like!");
  EXPECT_EQ(xiaoHong->GetSentence(), "xiaoMing, Yes! I like!");
  EXPECT_EQ(xiaoMing->GetAnswerFromOtherPeople(), "Yes! I like!");
  PLOGD << "chat end";
}

// Test that the timer works correctly
TEST(Human, want_to_sleep) {
  PLOGD << "want to sleep";
  std::shared_ptr<Human> human = std::make_shared<Human>("k");
  human->WakeUp();
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  human->FellAsleep();
  EXPECT_EQ(human->countSayWantToSleep, 5);
  EXPECT_EQ(human->GetSentence(), "I want to sleep.");
  PLOGD << "want to sleep end";
}

int main(int argc, char **argv) {
  // Get the current time as a string
  auto now = std::chrono::system_clock::now();
  std::time_t t = std::chrono::system_clock::to_time_t(now);
  std::string timeStr = std::ctime(&t);
  timeStr.erase(std::remove(timeStr.begin(), timeStr.end(), '\n'),
                timeStr.end());

  // Set the log file name to include the current time
  static plog::RollingFileAppender<plog::TxtFormatter> fileAppender(
      ("cxx_thread_" + timeStr + ".log").c_str(), 1000000, 3);
  plog::init(plog::verbose, &fileAppender);

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}