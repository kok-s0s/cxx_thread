#include <gtest/gtest.h>

#include <iostream>

#include "Human.h"

TEST(Human, say_hello) {
  std::unique_ptr<Human> human(new Human("k"));
  human->SendSayHelloSignal();
  EXPECT_EQ(human->GetSentence(), "Hello, k!");
}

TEST(Human, plan_to_do) {
  std::unique_ptr<Human> human(new Human("k"));
  human->SendPlanToDoSignal("9:00", "13:00", "play the game");
  EXPECT_EQ(human->GetSentence(),
            "I plan to play the game from 9:00 to 13:00.");
}

TEST(Human, do_what) {
  std::unique_ptr<Human> human(new Human("k"));
  human->SendWillDoSignal("sleep");
  EXPECT_EQ(human->GetSentence(), "I'm going to go to sleep");
}

TEST(Human, ask_a_question) {
  std::unique_ptr<Human> human(new Human("k"));
  human->SendAskAQuestionSignal("q", "Where are you going?");
  EXPECT_EQ(human->GetSentence(), "q, Where are you going?");
}

TEST(Human, answer_a_question) {
  std::unique_ptr<Human> human(new Human("k"));
  human->SendAnswerAQuestionSignal("k", "I plan to go to the lake.");
  EXPECT_EQ(human->GetSentence(), "k, I plan to go to the lake.");
}

TEST(Human, say_goodbye) {
  std::unique_ptr<Human> human(new Human("k"));
  human->SendSayGoodByeSignal();
  EXPECT_EQ(human->GetSentence(), "GoodBye, k!");
}

TEST(Human, want_to_sleep) {
  std::unique_ptr<Human> human(new Human("k"));
  human->WakeUp();
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  human->FellAsleep();
  EXPECT_EQ(human->countSayWantToSleep, 5);
  EXPECT_EQ(human->GetSentence(), "I want to sleep.");
}

TEST(Human, interact) {
  std::unique_ptr<Human> xiaoMing(new Human("xiaoMing"));
  std::unique_ptr<Human> xiaoHong(new Human("xiaoHong"));
  xiaoMing->SendAskAQuestionSignal(xiaoHong->GetName(), "Do you like cats?");
}