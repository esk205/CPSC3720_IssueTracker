/**
 * Copyright 2020 Cole_Anderson, Christian_Walker, Micheal_Wynnychuk,
 * Radek_Lewandowski
 */

#include "Issue.h"

#include <iostream>
#include <string>
#include <vector>

/**
 * Constructor for Issue which takes in issue information as parameters
 * @param t Issue title
 * @param d Issue description
 * @param os Operating system of machine(s) affected by issue
 * @param type Issue type (Feature/Bug/Task)
 * @param u Author of issue (user who created issue)
 * @param a User assigned to issue
 */
Issue::Issue(std::string t, std::string d, std::string os, std::string type,
             std::string u, std::string a) {
  title = t;         // Title is set
  desc = d;          // Description is set
  opSys = os;        // OS is set
  issueType = type;  // Type is set
  user = u;          // Author is set
  assign = a;        // Assignee is set
}

/**
 * Handles deletion of object pointers when client is exited
 */
void Issue::memoryCleanComments() {
  for (int i = 0; i < comments.size(); i++) {
    delete (comments[i]);
  }
  comments.clear();
}

/**
 * Sets the issue title
 * @param t issue title
 */
void Issue::setTitle(std::string t) { title = t; }

/**
 * Sets the issue description
 * @param d issue description
 */
void Issue::setDesc(std::string d) { desc = d; }

/**
 * Sets the issue OS
 * @param o issue OS
 */
void Issue::setOS(std::string o) { opSys = o; }

/**
 * Sets the issue type
 * @param ty issue type
 */
void Issue::setType(std::string ty) { issueType = ty; }

/**
 * Sets the issue author
 * @param u issue author
 */
void Issue::setUser(std::string u) { user = u; }

/**
 * Sets the issue assignee
 * @param sa issue assignee
 */
void Issue::setAssignee(std::string sa) {
  if (sa.empty()) {
    assign = "user_Removed";
  } else {
    assign = sa;
  }
}

/**
 * Sets user at specified comment to "user_Removed" when user is deleted
 * @param index index of comments vector
 */
void Issue::setCommentUser(int index) {
  comments.at(index)->setUser("user_Removed");
}

// Getters

/**
 * Gets the vector of Comment pointers, comments
 * @return comment
 */
std::vector<Comment*> Issue::getCommentVec() { return comments; }

/**
 * Gets the size of comments vector
 * @return comments vector size
 */
int Issue::getCommentNum() { return comments.size(); }

/**
 * Gets the issue title
 * @return issue title
 */
std::string Issue::getIssueTitle() { return title; }

/**
 * Gets the issue description
 * @return issue description
 */
std::string Issue::getIssueDesc() { return desc; }

/**
 * Gets the issue OS
 * @return issue OS
 */
std::string Issue::getIssueOS() { return opSys; }

/**
 * Gets the issue type
 * @return issue type
 */
std::string Issue::getIssueType() { return issueType; }

/**
 * Gets the issue assignee
 * @return issue assignee
 */
std::string Issue::getIssueAssignee() {
  if (assign.empty()) {
    return "user_Removed";
  } else {
    return assign;
  }
}

/**
 * Gets the issue author
 * @return issue author
 */
std::string Issue::getIssueUser() {
  if (user.empty()) {
    return "user_Removed";
  } else {
    return user;
  }
}

/**
 * Adds new comment to comments vector
 * @param c Comment pointer to be added to vector
 */
void Issue::addToComments(Comment* c) { comments.push_back(c); }
