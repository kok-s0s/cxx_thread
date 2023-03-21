#include <gtest/gtest.h>

#include <iostream>

#include "Human.h"

TEST(Human, say_hello) {
  Human* human = new Human("k");
  human->SendSayHelloSignal();
  EXPECT_EQ(human->GetSentence(), "Hello, k!");
  delete human;
}

TEST(Human, plan_to_do) {
  Human* human = new Human("k");
  human->SendPlanToDoSignal("9:00", "13:00", "play the game");
  EXPECT_EQ(human->GetSentence(),
            "I plan to play the game from 9:00 to 13:00.");
  delete human;
}

TEST(Human, do_what) {
  Human* human = new Human("k");
  human->SendWillDoSignal("sleep");
  EXPECT_EQ(human->GetSentence(), "I'm going to go to sleep");
  delete human;
}

TEST(Human, ask_a_question) {
  Human* human = new Human("k");
  human->SendAskAQuestionSignal("q", "Where are you going?");
  EXPECT_EQ(human->GetSentence(), "q, Where are you going?");
  delete human;
}

TEST(Human, answer_a_question) {
  Human* human = new Human("q");
  human->SendAnswerAQuestionSignal("k", "I plan to go to the lake.");
  EXPECT_EQ(human->GetSentence(), "k, I plan to go to the lake.");
  delete human;
}

TEST(Human, say_goodbye) {
  Human* human = new Human("k");
  human->SendSayGoodByeSignal();
  EXPECT_EQ(human->GetSentence(), "GoodBye, k!");
  delete human;
}