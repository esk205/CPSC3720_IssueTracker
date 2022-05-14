// Copyright 2020 Cole_Anderson,Christian_Walker, Micheal_Wynnychuck,
// Radek_Lewandowski

#include "Issue.h"
#include "User.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::Expectation;
using ::testing::Return;

TEST(IssueUser, Test_Constructor) {
  User* user = new User("Cole");
  ASSERT_EQ("Cole", user->getName());
  delete user;
}
