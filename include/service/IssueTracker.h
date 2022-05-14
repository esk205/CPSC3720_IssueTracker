/**
 * Copyright 2020 Cole_Anderson, Christian_Walker, Micheal_Wynnychuk,
 * Radek_Lewandowski
 */

#ifndef ISSUETRACKER_H /* NOLINT */
#define ISSUETRACKER_H /* NOLINT */
#include <iostream>
#include <string>
#include <vector>

#include "Issue.h"
#include "IssueTrackerUI.h"
#include "User.h"

class IssueTracker {
 public:
  IssueTracker();
  /**
   * Constructor for IssueTracker which takes a vector of Issue pointers
   * and a vector of User pointers as parameters
   * @param i Vector of Issue pointers
   * @param u Vector of User pointers
   */
  IssueTracker(std::vector<Issue*> i, std::vector<User*> u);
  virtual ~IssueTracker();

  /**
   * Handles deletion of object pointers when client is exited
   */
  virtual void memoryCleanIssues();
  /**
 * Handles deletion of object pointers(for comments) when client is exited
 */
  virtual void memoryCleanCom();

  // Adders:
  /**
   * Adds Issue pointer to vector issues
   * @param i Issue pointer
   */
  virtual void addToIssueVec(Issue* i);
  /**
   * Adds User pointer to vector users
   * @param u User pointer
   */
  virtual void addToUserVec(User* u);

  // Get Functions
  /**
   * Gets the vector of Issue pointers, issues
   * @return issues vector
   */
  virtual std::vector<Issue*> getIssueVec();
  /**
   * Gets the vector of User pointers, users
   * @return users vector
   */
  virtual std::vector<User*> getUserVec();
  /**
   * Gets the size of issues vector
   * @return size of issues vector
   */
  int retSize();

  // Issue Methods
  /**
   * Creates a new issue object then adds it to the issues vector
   * as well as writing it to the text file
   * @param title The issue title
   * @param desc The issue description
   * @param os The issue operating system
   * @param type The issue type
   * @param user The issue author
   * @param assign The issue assignee
   * @param result The result of the issue adding operation
   */
  virtual void addAnIssue(std::string title, std::string desc, std::string os,
                          std::string type, std::string user,
                          std::string assign, std::string& result);
  /**
   * Retrieves all existing issues from server and returns their titles
   * @return the title of each existing issue
   */
  virtual std::string getAllIssues();
  /**
   * Retrieves an existing issue from the vector of issues based on it's title
   * @param issueTitle The issue title
   * @return returns the issue data if issue is found and "(BLANK)" if not
   */
  virtual std::string getAnIssue(std::string issueTitle);
  /**
   * Deletes an existing issue from the issues vector based on it's title
   * and re-writes the text file
   * @param issueTitle The issue title
   * @return returns the status of issue deletion
   */
  virtual std::string deleteIssue(std::string title);

  // User Methods
  /**
   * Creates a new user object and adds them to a vector of users and
   * writes them to a text file
   * @param username The username of the new user being created
   * @return returns the username if available or "(TAKEN)" if unavailable
   */
  virtual std::string createUser(std::string username);
  /**
   * Retrieves a user from the users vector based on their username
   * @param username The username of the user being retrieved
   * @return returns the username if found or "(BLANK)" if not
   */
  virtual std::string getUser(std::string username);
  /**
   * Retrieves all existing users from the users vector
   * @return returns all existing users
   */
  virtual std::string getAllUsers();
  /**
   * Deletes an existing user from the users vector as well as from
   * comments and issues and re-writes the text files
   * @param username The username of the user being deleted
   * @return returns the result of the deletion operation
   */
  virtual std::string deleteUser(std::string username);

  // Comment Methods
  /**
   * Creates a new comment object and adds it to a vector of comments
   * as well as writing it to a text file
   * @param issueTitle The title of the issue which the comment is being added
   * @param comment The comment text
   * @param user The author of the comment
   * @param result The result of the operation
   */
  virtual void addToCommentVec(std::string issueTitle, std::string comment,
                               std::string user, std::string result);
  /**
   * Gets comments from a given issue and parses them by text and user
   * @param issue The issue which the comment was added to
   * @return The comment(s)
   */
  virtual std::string getComments(Issue* issue);

  // Parse Methods
  /**
   * Reads from comments.txt and context.txt when server is first started and
   * extracts data from them in order to parse data and create appropriate
   * objects for issues and comments. If text files don't exist, then they will
   * be created and nothing will be extracted.
   */
  virtual void readFile();

  /**
   * Retrieves issue/user/comment data from appropriate object pointer
   * vectors and writes them to context.txt and comment.txt. Re-writes both
   * files if any changes are made.
   */
  virtual void writeFile();

 private:
  /**
   * Vector of Issue pointers
   */
  std::vector<Issue*> issues;
  /**
   * Vector of User pointers
   */
  std::vector<User*> users;
};
#endif /* NOLINT */
