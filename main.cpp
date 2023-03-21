#include <gtest/gtest.h>

#include <iostream>

#include "Human.h"

TEST(Human, sayHello) {
  Human* human = new Human("k");
  human->SendSayHelloSignal();
  EXPECT_EQ(human->GetSentence(), "Hello, k!");
  delete human;
}

TEST(Human, PlanToDo) {
  Human* human = new Human("k");
  human->SendPlanToDoSignal("9:00", "13:00", "play the game");
  EXPECT_EQ(human->GetSentence(),
            "I plan to play the game from 9:00 to 13:00.");
  delete human;
}

TEST(Human, doWhat) {
  Human* human = new Human("k");
  human->SendWillDoSignal("sleep");
  EXPECT_EQ(human->GetSentence(), "I'm going to go to sleep");
  delete human;
}

TEST(Human, askAQuestion) {
  Human* human = new Human("k");
  human->SendAskAQuestionSignal("q", "Where are you going?");
  EXPECT_EQ(human->GetSentence(), "q, Where are you going?");
  delete human;
}

TEST(Human, answerAQuestion) {
  Human* human = new Human("q");
  human->SendAnswerAQuestionSignal("k", "I plan to go to the lake.");
  EXPECT_EQ(human->GetSentence(), "k, I plan to go to the lake.");
  delete human;
}

TEST(Human, sayGoodBye) {
  Human* human = new Human("k");
  human->SendSayGoodByeSignal();
  EXPECT_EQ(human->GetSentence(), "GoodBye, k!");
  delete human;
}