// Copyright 2020 Cole_Anderson,Christian_Walker, Micheal_Wynnychuck,
// Radek_Lewandowski

#include "Issue.h"
#include "IssueTracker.h"
#include "User.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::AtLeast;
using ::testing::Expectation;
using ::testing::Return;
using namespace ::testing;

class MockIssueTracker : public IssueTracker {
 public:
  explicit MockIssueTracker() {}
  virtual ~MockIssueTracker() {}

  MOCK_METHOD7(addAnIssue,
               void(std::string title, std::string desc, std::string os,
                    std::string type, std::string user, std::string assign,
                    std::string& result));
  MOCK_METHOD1(deleteIssue, std::string(std::string title));
  MOCK_METHOD1(getAnIssue, std::string(std::string issueTitle));
  MOCK_METHOD1(getUser, std::string(std::string username));
  MOCK_METHOD0(getAllUsers, std::string());
};

TEST(MockIssueTracker, getters_setters) {
  MockIssueTracker* issuetracker = new MockIssueTracker();
  Issue* i = new Issue("hello", "desc", "os", "type", "user", "assignee");
  User* u = new User("user");
  ASSERT_EQ("(BLANK)[^", issuetracker->getAllIssues());
  issuetracker->addToIssueVec(i);
  issuetracker->addToUserVec(u);
  std::vector<Issue*> getI = issuetracker->getIssueVec();
  std::vector<User*> getU = issuetracker->getUserVec();
  ASSERT_EQ(1, issuetracker->retSize());
  ASSERT_EQ("hello", getI.at(0)->getIssueTitle());
  ASSERT_EQ("desc", getI.at(0)->getIssueDesc());
  ASSERT_EQ("os", getI.at(0)->getIssueOS());
  ASSERT_EQ("type", getI.at(0)->getIssueType());
  ASSERT_EQ("user", getI.at(0)->getIssueUser());
  ASSERT_EQ("assignee", getI.at(0)->getIssueAssignee());

  Issue* i1 = new Issue("hello there", "Republic", "Windows", "Feature",
                        "Obi-Wan", "Grievous");
  issuetracker->addToIssueVec(i1);

  Issue* i2 = new Issue("General Kenobi", "CIS", "MacOS", "Task", "Grievous",
                        "Obi-Wan");
  issuetracker->addToIssueVec(i2);
  ASSERT_EQ("hello[^hello there[^General Kenobi[^",
            issuetracker->getAllIssues());
  delete i;
  delete i1;
  delete i2;
  delete u;
  delete issuetracker;
}
TEST(MockIssueTracker, comments) {
  MockIssueTracker* issuetracker = new MockIssueTracker();
  Issue* i = new Issue("Execute Order 66", "Empire", "Linux", "Task",
                       "Palpatine", "Commander Cody");
  User* u = new User("Obi-Wan Kenobi");
  issuetracker->addToIssueVec(i);
  issuetracker->addToUserVec(u);

  User* u1 = new User("Anakin Skywalker");
  issuetracker->addToUserVec(u1);

  std::string result;

  std::vector<Issue*> getI = issuetracker->getIssueVec();
  std::vector<User*> getU = issuetracker->getUserVec();
  issuetracker->addToCommentVec("Execute Order 66",
                                "You were my brother Anakin, I loved you",
                                "Obi-Wan Kenobi", result);
  issuetracker->addToCommentVec("Execute Order 66", "I HATE YOU",
                                "Anakin Skywalker", result);
  ASSERT_EQ(
      "You were my brother Anakin, I loved you^]Obi-Wan Kenobi^]I HATE "
      "YOU^]Anakin Skywalker^]",
      issuetracker->getComments(i));

  issuetracker->memoryCleanCom();
  // issuetracker->memoryCleanIssues();
  delete i;
  delete u;
  delete u1;
  delete issuetracker;
}
TEST(MockIssueTracker, addAnIssue) {
  std::string RESULT = "hello^]desc^]os^]type^]user^]assignee^]";
  std::string res = "";
  MockIssueTracker* issuetracker = new MockIssueTracker();
  EXPECT_CALL(*issuetracker, addAnIssue("hello", "desc", "os", "type", "user",
                                        "assignee", res))
      .Times(1);
  issuetracker->addAnIssue("hello", "desc", "os", "type", "user", "assignee",
                           res);

  issuetracker->memoryCleanIssues();
  delete issuetracker;
}
TEST(MockIssueTracker, deleteIssue) {
  std::string TITLE = "hello";
  MockIssueTracker* issuetracker = new MockIssueTracker();
  Issue* i = new Issue("hello", "desc", "os", "type", "user", "assignee");
  issuetracker->addToIssueVec(i);
  EXPECT_CALL(*issuetracker, deleteIssue(TITLE))
      .Times(1)
      .WillOnce(Return("hello has been removed."));

  issuetracker->deleteIssue("hello");
  // issuetracker->memoryCleanIssues();
  delete i;
  delete issuetracker;
}
TEST(MockIssueTracker, getAnIssue) {
  std::string RESULT = "hello^]desc^]os^]type^]user^]assignee^]";
  std::string TITLE = "hello";
  MockIssueTracker* issuetracker = new MockIssueTracker();
  Issue* i = new Issue("hello", "desc", "os", "type", "user", "assignee");
  issuetracker->addToIssueVec(i);
  EXPECT_CALL(*issuetracker, getAnIssue(TITLE))
      .Times(1)
      .WillOnce(Return(RESULT));
  std::string in = issuetracker->getAnIssue("hello");
  ASSERT_EQ(in, RESULT);
  delete i;
  delete issuetracker;
}
TEST(MockIssueTracker, getUser) {
  std::string RESULT = "user";
  std::string USER = "user";
  MockIssueTracker* issuetracker = new MockIssueTracker();
  issuetracker->createUser("user");
  EXPECT_CALL(*issuetracker, getUser(USER)).Times(1).WillOnce(Return(RESULT));
  issuetracker->getUser("user");
  issuetracker->memoryCleanIssues();

  delete issuetracker;
}
///
TEST(MockIssueTracker, createUser) {
  MockIssueTracker* IssueTracker = new MockIssueTracker();
  std::string USER = "Obi-Wan Kenobi";
  std::string RESULT = "Obi-Wan Kenobi";
  User* u = new User("Obi-Wan Kenobi");
  IssueTracker->addToUserVec(u);

  std::vector<User*> vec = IssueTracker->getUserVec();

  EXPECT_CALL(*IssueTracker, getUser("Obi-Wan Kenobi"))
      .Times(1)
      .WillOnce(Return("Obi-Wan Kenobi"));
  RESULT = IssueTracker->getUser(USER);
  IssueTracker->memoryCleanIssues();

  delete IssueTracker;
  vec.empty();
}
TEST(MockIssueTracker, getAllUsers) {
  MockIssueTracker* IssueTracker = new MockIssueTracker();
  User* u = new User("Obi-Wan Kenobi");
  User* u2 = new User("Obi-Wan Kenobi2");
  IssueTracker->addToUserVec(u);
  IssueTracker->addToUserVec(u2);
  std::string RESULT = "";

  std::vector<User*> vec = IssueTracker->getUserVec();

  EXPECT_CALL(*IssueTracker, getAllUsers()).Times(1).WillOnce(Return(RESULT));
  IssueTracker->getAllUsers();
  IssueTracker->memoryCleanIssues();

  delete IssueTracker;
}
/**
 * @note: This causes coverage on CI server to fail but locally worked fine
 * -For reference in the makefile all the commented out code actually works
 * locally with g++7 but this chunk of code breaks on the ci server
 **/
// TEST(MockIssueTracker, write_read) {
//   std::string res = "";
//   std::string RESULT = "hello^]desc^]os^]type^]user^]assignee^]";
//   MockIssueTracker* issuetracker = new MockIssueTracker();
//   // EXPECT_CALL(*issuetracker, addAnIssue("hello", "desc", "os", "type",
//   // "user",
//   //                                       "assignee", res))
//   //     .Times(1);
//   Issue* i1 = new Issue("hello", "desc", "os", "type", "user", "user");
//   User* u1 = new User("user");
//   issuetracker->addToIssueVec(i1);
//   issuetracker->addToUserVec(u1);
//   issuetracker->writeFile();
//   MockIssueTracker* issuetrackerRead = new MockIssueTracker();
//   issuetrackerRead->readFile();

//   EXPECT_CALL(*issuetracker, getAnIssue("hello"))
//       .Times(1)
//       .WillOnce(Return(RESULT));
//   EXPECT_CALL(*issuetrackerRead, getAnIssue("hello"))
//       .Times(1)
//       .WillOnce(Return(RESULT));

//   ASSERT_EQ(issuetracker->getAnIssue("hello"),
//             issuetrackerRead->getAnIssue("hello"));
//   issuetracker->memoryCleanIssues();
//   issuetrackerRead->memoryCleanIssues();
//   delete u1;
//   delete i1;
//   delete issuetracker;
//   delete issuetrackerRead;
// }
