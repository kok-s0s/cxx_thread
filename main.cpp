#include <gtest/gtest.h>

#include <iostream>

#include "Human.h"

// Test that the signal and slot functions work correctly
TEST(Human, say_hello) {
  std::unique_ptr<Human> human(new Human("k"));
  human->SendSayHelloSignal();
  EXPECT_EQ(human->GetSentence(), "Hello, k!");
  human->SendSayGoodByeSignal();
  EXPECT_EQ(human->GetSentence(), "GoodBye, k!");
}

// Test whether parameter can be passed correctly
TEST(Human, will_do) {
  std::unique_ptr<Human> human(new Human("k"));
  human->SendWillDoSignal("sleep");
  EXPECT_EQ(human->GetSentence(), "I'm going to go to sleep");
}

// Test whether multiple parameters can be passed correctly
TEST(Human, plan_to_do) {
  std::unique_ptr<Human> human(new Human("k"));
  human->SendPlanToDoSignal("9:00", "13:00", "play the game");
  EXPECT_EQ(human->GetSentence(),
            "I plan to play the game from 9:00 to 13:00.");
}

// Test whether two thread objects can interact with each other
TEST(Human, chat) {
  std::shared_ptr<Human> xiaoMing(new Human("xiaoMing"));
  std::shared_ptr<Human> xiaoHong(new Human("xiaoHong"));
  xiaoMing->SendAskAQuestionSignal(xiaoMing, xiaoHong, "Do you like cats?");
  xiaoHong->SendAnswerAQuestionSignal("Yes! I like!");
  EXPECT_EQ(xiaoHong->GetQuestionFromOtherPeople(), "Do you like cats?");
  EXPECT_EQ(xiaoMing->GetAnswerFromOtherPeople(), "Yes! I like!");
}

// Test whether it runs synchronously
TEST(Human, want_to_sleep) {
  std::unique_ptr<Human> human(new Human("k"));
  human->WakeUp();
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  human->FellAsleep();
  EXPECT_EQ(human->countSayWantToSleep, 5);
  EXPECT_EQ(human->GetSentence(), "I want to sleep.");
}
