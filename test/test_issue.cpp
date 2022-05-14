// Copyright 2020 Cole_Anderson,Christian_Walker, Micheal_Wynnychuck,
// Radek_Lewandowski

#include "Issue.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::Expectation;
using ::testing::Return;

TEST(IssueTest, Test_Constructor) {
  Issue* test = new Issue("title", "desc", "os", "type", "user", "assignee");
  Issue* setters = new Issue();
  Issue* emptyA = new Issue();
  Comment* c = new Comment();
  c->setText("helloworld is easy");
  c->setUser("MastersStudent");
  setters->setTitle("hello");
  setters->setDesc("world");
  setters->setOS("Linux");
  setters->setType("Bug");
  setters->setUser("Cole");
  setters->setAssignee("Cole");

  ASSERT_EQ("title", test->getIssueTitle());
  ASSERT_EQ("desc", test->getIssueDesc());
  ASSERT_EQ("os", test->getIssueOS());
  ASSERT_EQ("type", test->getIssueType());
  ASSERT_EQ("user", test->getIssueUser());
  ASSERT_EQ("assignee", test->getIssueAssignee());

  ASSERT_EQ("hello", setters->getIssueTitle());
  ASSERT_EQ("world", setters->getIssueDesc());
  ASSERT_EQ("Linux", setters->getIssueOS());
  ASSERT_EQ("Bug", setters->getIssueType());
  ASSERT_EQ("Cole", setters->getIssueUser());
  ASSERT_EQ("Cole", setters->getIssueAssignee());

  std::string a;
  setters->setUser("");
  emptyA->setAssignee(a);
  ASSERT_EQ("user_Removed", emptyA->getIssueAssignee());
  ASSERT_EQ("user_Removed", setters->getIssueUser());
  ASSERT_EQ(0, test->getCommentNum());
  test->addToComments(c);
  ASSERT_EQ(1, test->getCommentNum());

  delete c;
  delete test;
  delete setters;
  delete emptyA;
}
TEST(IssueTest, Test_Comments) {
  Issue* test = new Issue("title", "desc", "os", "type", "user", "assignee");
  Comment* c = new Comment();
  c->setText("this is a comment");
  c->setUser("username");
  test->addToComments(c);

  std::vector<Comment*> cVector = test->getCommentVec();

  ASSERT_EQ("this is a comment", cVector.at(0)->getCommentText());
  ASSERT_EQ("username", cVector.at(0)->getCommentUser());
  test->setCommentUser(0);
  cVector = test->getCommentVec();
  ASSERT_EQ("user_Removed", cVector.at(0)->getCommentUser());

  delete test;
  delete c;
}
