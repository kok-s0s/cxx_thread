#include <gtest/gtest.h>

#include <iostream>

#include "Human.h"

TEST(Human, sayHello) {
  Human* human = new Human("k");
  human->sendSayHelloSignal();
  EXPECT_EQ(human->getSentence(), "Hello, k!");
}

TEST(Human, sayGoodBye) {
  Human* human = new Human("k");
  human->sendSayGoodByeSignal();
  EXPECT_EQ(human->getSentence(), "GoodBye, k!");
}