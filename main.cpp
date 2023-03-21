#include <gtest/gtest.h>

#include <iostream>

#include "Human.h"

TEST(Human, sayHello) {
  Human* human = new Human("k");
  human->SendSayHelloSignal();
  EXPECT_EQ(human->GetSentence(), "Hello, k!");
  delete human;
}

TEST(Human, doWhat) {
  Human* human = new Human("k");
  human->SendTodoSignal("sleep");
  EXPECT_EQ(human->GetSentence(), "I'm going to go to sleep");
  delete human;
}

TEST(Human, sayGoodBye) {
  Human* human = new Human("k");
  human->SendSayGoodByeSignal();
  EXPECT_EQ(human->GetSentence(), "GoodBye, k!");
  delete human;
}