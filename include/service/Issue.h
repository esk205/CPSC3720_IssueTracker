/**
 * Copyright 2020 Cole_Anderson, Christian_Walker, Micheal_Wynnychuk,
 * Radek_Lewandowski
 */

#ifndef ISSUE_H /* NOLINT */
#define ISSUE_H /* NOLINT */

#include <iostream>
#include <string>
#include <vector>

#include "Comment.h"

class Issue {
 public:
  Issue() {}
  /**
   * Constructor for Issue which takes in issue information as parameters
   * @param t Issue title
   * @param d Issue description
   * @param os Operating system of machine(s) affected by issue
   * @param type Issue type (Feature/Bug/Task)
   * @param u Author of issue (user who created issue)
   * @param a User assigned to issue
   */
  Issue(std::string t, std::string d, std::string os, std::string type,
        std::string u, std::string a);
  ~Issue() {}

  // Getters:

  /**
   * Gets the issue title
   * @return issue title
   */
  std::string getIssueTitle();
  /**
   * Gets the issue description
   * @return issue description
   */
  std::string getIssueDesc();
  /**
   * Gets the issue OS
   * @return issue OS
   */
  std::string getIssueOS();
  /**
   * Gets the issue type
   * @return issue type
   */
  std::string getIssueType();
  /**
   * Gets the issue author
   * @return issue author
   */
  std::string getIssueUser();
  /**
   * Gets the issue assignee
   * @return issue assignee
   */
  std::string getIssueAssignee();
  /**
   * Gets the vector of Comment pointers, comments
   * @return comment
   */
  std::vector<Comment*> getCommentVec();
  /**
   * Gets the size of comments vector
   * @return comments vector size
   */
  int getCommentNum();

  // Setters:

  /**
   * Sets the issue title
   * @param t issue title
   */
  void setTitle(std::string t);
  /**
   * Sets the issue description
   * @param d issue description
   */
  void setDesc(std::string d);
  /**
   * Sets the issue OS
   * @param o issue OS
   */
  void setOS(std::string o);
  /**
   * Sets the issue type
   * @param ty issue type
   */
  void setType(std::string ty);
  /**
   * Sets the issue author
   * @param u issue author
   */
  void setUser(std::string u);
  /**
   * Sets the issue assignee
   * @param sa issue assignee
   */
  void setAssignee(std::string sa);
  /**
   * Sets user at specified comment to "user_Removed" when user is deleted
   * @param index index of comments vector
   */
  void setCommentUser(int index);

  /**
   * Adds new comment to comments vector
   * @param c Comment pointer to be added to vector
   */
  void addToComments(Comment* c);
  
  /**
   * Handles deletion of object pointers when client is exited
   */
  void memoryCleanComments();

 private:
  /**
   * Issue title
   */
  std::string title;
  /**
   * Issue description
   */
  std::string desc;
  /**
   * Issue OS
   */
  std::string opSys;
  /**
   * Issue type
   */
  std::string issueType;
  /**
   * Issue author
   */
  std::string user;
  /**
   * Issue assignee
   */
  std::string assign;
  /**
   * Vector of Comment pointers
   */
  std::vector<Comment*> comments;
};
#endif /* NOLINT */
