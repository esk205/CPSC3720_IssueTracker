/**
 * Copyright 2020 Cole_Anderson, Christian_Walker, Micheal_Wynnychuk,
 * Radek_Lewandowski
 */

#include "IssueTracker.h"

#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Issue.h"
#include "User.h"
IssueTracker::IssueTracker() {}
IssueTracker::~IssueTracker() {}

/**
 * Handles deletion of object pointers when client is exited
 */
void IssueTracker::memoryCleanIssues() {
  // Clears Vector
  for (int i = 0; i < issues.size(); i++) {
    delete (issues[i]);
  }
  for (int i = 0; i < users.size(); i++) {
    delete (users[i]);
  }
  users.clear();
}

/**
 * Handles deletion of object pointers (for comments) when client is exited
 */
void IssueTracker::memoryCleanCom() {
  for (int i = 0; i < issues.size(); i++) {
    issues.at(i)->memoryCleanComments();
  }
}

/**
 * Gets the vector of Issue pointers, issues
 * @return issues vector
 */
std::vector<Issue*> IssueTracker::getIssueVec() { return issues; }

/**
 * Gets the vector of User pointers, users
 * @return users vector
 */
std::vector<User*> IssueTracker::getUserVec() { return users; }

/**
 * Gets the size of issues vector
 * @return size of issues vector
 */
int IssueTracker::retSize() { return issues.size(); }

/**
 * Adds Issue pointer to vector issues
 * @param i Issue pointer
 */
void IssueTracker::addToIssueVec(Issue* i) { issues.push_back(i); }

/**
 * Adds User pointer to vector users
 * @param u User pointer
 */
void IssueTracker::addToUserVec(User* u) { users.push_back(u); }

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
void IssueTracker::addAnIssue(std::string title, std::string desc,
                              std::string os, std::string type,
                              std::string user, std::string assign,
                              std::string& result) {
  // Creates new Issue object pointer with given attributes
  Issue* newIssue = new Issue(title, desc, os, type, user, assign);
  addToIssueVec(newIssue);     // Adds issue to issues vector
  writeFile();                 // Writes issue to file
  result = "New Issue Added";  // Sends result back to client
}

/**
 * Retrieves all existing issues from server and returns their titles
 * @return the title of each existing issue
 */
std::string IssueTracker::getAllIssues() {
  std::string result;
  if (!issues.empty()) {  // If any issues exist, concatenate titles to result
    for (int i = 0; i < issues.size(); i++) {
      result += issues[i]->getIssueTitle() + "[^";
    }
  } else {  // If no issues are found
    result = "(BLANK)[^";
  }
  return result;
}

/**
 * Retrieves an existing issue from the vector of issues based on it's title
 * @param issueTitle The issue title
 * @return returns the issue data if issue is found and "(BLANK)" if not
 */
std::string IssueTracker::getAnIssue(std::string issueTitle) {
  std::string user;
  std::string result = "(BLANK)[^";
  for (int i = 0; i < issues.size(); i++) {
    if (issues[i]->getIssueTitle() == issueTitle) {  // Issue title found
      for (int j = 0; j < users.size(); j++) {
        // If username from issue still matches existing user
        if (users.at(j)->getName() == issues[i]->getIssueUser()) {
          user = issues[i]->getIssueUser();
          break;
        } else {  // User was deleted
          user = "user_Removed";
        }
      }
      // Concatenate all issue attributes into result with delimiter
      result = issues[i]->getIssueTitle() + "^]" + issues[i]->getIssueDesc() +
               "^]" + issues[i]->getIssueOS() + "^]" +
               issues[i]->getIssueType() + "^]" + user + "^]" +
               issues[i]->getIssueAssignee() + "^]";

      // Concatenate comments if any exist
      if (!issues[i]->getCommentVec().empty()) {
        result += getComments(issues[i]);
      }
    }
  }
  return result;
}

/**
 * Deletes an existing issue from the issues vector based on it's title
 * and re-writes the text file
 * @param issueTitle The issue title
 * @return returns the status of issue deletion
 */
std::string IssueTracker::deleteIssue(std::string title) {
  std::string result = "(BLANK)";
  int index;
  for (int i = 0; i < issues.size(); i++) {
    //  If issue found by title, delete the issue
    if (issues.at(i)->getIssueTitle() == title) {
      index = i;
      result = title + " has been removed.";
      issues.erase(issues.begin() + index);
    }
  }
  writeFile();  // Re-write files to remove issue from them
  return result;
}

/**
 * Creates a new user object and adds them to a vector of users and
 * writes them to a text file
 * @param username The username of the new user being created
 * @return returns the username if available or "(TAKEN)" if unavailable
 */
std::string IssueTracker::createUser(std::string username) {
  std::string name;
  std::string result = "";
  bool nameTaken = false;
  std::ifstream readFile("users.txt");  // Creates/opens users.txt
  if (readFile) {
    // Searches through users.txt for matching username
    while (getline(readFile, name)) {
      if (name == username) {  // If matching name found, username already taken
        nameTaken = true;
      }
    }
  }
  readFile.close();  // Close users.txt

  /**
   *  If name available, create User object pointer, push it to users vector,
   *  and write to file to add username
   */
  if (!nameTaken) {
    std::ofstream writeFile;
    writeFile.open("users.txt", std::ofstream::app);
    writeFile << username << '\n';
    writeFile.close();
    result = username;
    User* newUser = new User(username);
    users.push_back(newUser);
  } else {  // If taken, return "(TAKEN)" as result to client
    result = "(TAKEN)";
  }
  return result;
}

/**
 * Retrieves a user from the users vector based on their username
 * @param username The username of the user being retrieved
 * @return returns the username if found or "(BLANK)" if not
 */
std::string IssueTracker::getUser(std::string username) {
  bool nameFound = false;
  std::string result;

  // Searches through users vector to find existing user
  for (int i = 0; i < users.size(); i++) {
    if (users[i]->getName() == username) nameFound = true;
  }

  // If user exists return username to client, else return "(BLANK)"
  if (nameFound) {
    result = username;
  } else {
    result = "(BLANK)";
  }
  return result;
}

/**
 * Retrieves all existing users from the users vector
 * @return returns all existing users
 */
std::string IssueTracker::getAllUsers() {
  std::string result;

  // Retrieve all existing users and parse their usernames by '-'
  for (int i = 0; i < users.size(); i++) {
    result += users[i]->getName() + '-';
  }
  return result;
}

/**
 * Deletes an existing user from the users vector as well as from
 * comments and issues and re-writes the text files
 * @param username The username of the user being deleted
 * @return returns the result of the deletion operation
 */
std::string IssueTracker::deleteUser(std::string username) {
  std::string name;
  std::string vectorRemove;
  std::string result = "(BLANK)";
  std::ifstream userFile("users.txt");
  std::ofstream tempFile("tempUsers.txt");

  // If users.txt exists
  if (userFile) {
    // Searches for existing username in users.txt
    while (getline(userFile, name)) {
      // Adds all non-matching usernames to temp file
      if (name != username) {
        tempFile << name << '\n';
      } else {  // Adds matching username to temp vector for removal
        result = username + " has been removed.";
        vectorRemove = username;
      }
    }
  }

  // Close both text files
  userFile.close();
  tempFile.close();

  // Delete users.txt
  remove("users.txt");

  // Rename tempUsers.txt to users.txt (all non-matched usernames are copied)
  rename("tempUsers.txt", "users.txt");

  // Delete user in userVector
  int index;
  for (int i = 0; i < users.size(); i++) {
    if (users.at(i)->getName() == vectorRemove) {
      index = i;
    }
  }
  if (users.size() == 1) {
    users.clear();
  } else {
    users.erase(users.begin() + index);
  }

  // Delete user from assignee
  for (int i = 0; i < issues.size(); i++) {
    if (vectorRemove == issues.at(i)->getIssueAssignee()) {
      issues.at(i)->setAssignee("");
    }
  }

  // Delete user from comments
  for (int i = 0; i < issues.size(); i++) {
    std::vector<Comment*> comment = issues.at(i)->getCommentVec();
    for (int j = 0; j < issues.at(i)->getCommentNum(); j++) {
      if (vectorRemove == comment.at(j)->getCommentUser()) {
        issues.at(i)->setCommentUser(j);
      }
    }
  }
  writeFile();  // Re-write text files to accept changes
  return result;
}

/**
 * Creates a new comment object and adds it to a vector of comments
 * as well as writing it to a text file
 * @param issueTitle The title of the issue which the comment is being added
 * @param comment The comment text
 * @param user The author of the comment
 * @param result The result of the operation
 */
void IssueTracker::addToCommentVec(std::string issueTitle, std::string comment,
                                   std::string user, std::string result) {
  Comment* newComment = new Comment();  // Creates new Comment pointer
  newComment->setText(comment);         // Sets comment text to newComment
  newComment->setUser(user);            // Sets author of comment
  // Adds comment to existing issue by it's matching title
  for (int i = 0; i < issues.size(); i++) {
    if (issues[i]->getIssueTitle() == issueTitle) {
      issues[i]->addToComments(newComment);
      writeFile();
      result = "New comment added";  // Result sent back to client
    }
  }
}

/**
 * Gets comments from a given issue and parses them by text and user
 * @param issue The issue which the comment was added to
 * @return The comment(s)
 */
std::string IssueTracker::getComments(Issue* issue) {
  std::string comments;
  for (int i = 0; i < issue->getCommentVec().size(); i++) {
    comments += issue->getCommentVec()[i]->getCommentText() + "^]" +
                issue->getCommentVec()[i]->getCommentUser() + "^]";
  }
  return comments;
}

/**
 * Reads from comments.txt and context.txt when server is first started and
 * extracts data from them in order to parse data and create appropriate
 * objects for issues and comments. If text files don't exist, then they will
 * be created and nothing will be extracted.
 */
void IssueTracker::readFile() {
  std::ifstream saveFile;
  std::ifstream commentFile;
  saveFile.open("context.txt");
  commentFile.open("comments.txt");
  std::string sep = "**";
  std::string delim = "^]";
  std::string tempIssueTitle;
  std::string tempIssueDesc;
  std::string tempIssueOS;
  std::string tempIssueType;
  std::string tempUser;
  std::string tempAssign;
  std::string tempTextCom;
  std::string tempUserCom;

  /**
   * Reads entire file into a string for parsing
   **/

  // context.txt
  std::stringstream parseData;
  parseData << saveFile.rdbuf();
  std::string contents = parseData.str();

  // comments.txt
  std::stringstream commentData;
  commentData << commentFile.rdbuf();
  std::string commentContent = commentData.str();
  /**
   * Parses out: title, text, os, type, user, assignee
   * from string based on delimiter
   **/
  size_t pos = 0;
  std::string tok;
  int control = 0;
  while ((pos = contents.find(delim)) != std::string::npos) {
    tok = contents.substr(0, pos);
    // Tok is now equal to start to delim ^]
    if (control == 0) {
      tempIssueTitle = tok;
      control++;
    } else if (control == 1) {
      tempIssueDesc = tok;
      control++;
    } else if (control == 2) {
      tempIssueOS = tok;
      control++;
    } else if (control == 3) {
      tempIssueType = tok;
      control++;
    } else if (control == 4) {
      tempUser = tok;
      control++;
    } else if (control == 5) {
      tempAssign = tok;
      Issue* transferI =
          new Issue(tempIssueTitle, tempIssueDesc, tempIssueOS, tempIssueType,
                    tempUser, tempAssign);  // Deletes when client closes

      size_t cpos = 0;
      std::string ctok;
      int commentIncr = 0;

      size_t firstStar = commentContent.find_first_of(sep);
      std::string startDelim = commentContent.substr(0, firstStar);
      // only pass in comments for current issue
      while ((cpos = startDelim.find(delim)) != std::string::npos) {
        ctok = startDelim.substr(0, cpos);
        if (commentIncr == 0) {
          commentIncr++;
          // do nothing
        } else if (commentIncr == 1) {
          // COMMENT TEXT
          tempTextCom = ctok;
          commentIncr++;
        } else if (commentIncr == 2) {
          // COMMENT USER
          tempUserCom = ctok;
          Comment* com = new Comment();
          com->setText(tempTextCom);
          com->setUser(tempUserCom);
          transferI->addToComments(com);
          commentIncr = 1;
        }
        startDelim.erase(0, cpos + delim.length());  // erase ^] move on
      }
      commentContent.erase(0, firstStar + sep.length());  // erase ** move on

      // PUSHBACK ISSUES
      addToIssueVec(transferI);
      // erase for next Issue
      control = 0;
    }
    contents.erase(0, pos + delim.length());
  }
  // CLOSE FILE
  saveFile.close();

  std::ifstream userFile;
  std::string name = "";
  userFile.open("users.txt");
  if (userFile) {
    while (getline(userFile, name)) {
      User* newUser = new User(name);
      users.push_back(newUser);
    }
  }
  userFile.close();
}

/**
 * Retrieves issue/user/comment data from appropriate object pointer
 * vectors and writes them to context.txt and comment.txt. Re-writes both
 * files if any changes are made.
 */
void IssueTracker::writeFile() {
  /*
  ISSUE TITLE
  ISSUE USER
  "TEXT"
  ISSUE OSTYPE
  */
  // TEXT
  std::ofstream saveFile;
  std::ofstream commentFile;
  saveFile.open("context.txt");
  commentFile.open("comments.txt");

  /**
   * -Overwrites the textfile with new context information(avoid duplicates)
   * -Writes current object vectors (issue, user) to textfile for when
   *server starts up to retreive previously entered data -Uses delimeter to
   *seperate data for easy parsing
   *
   * ^] Group seperator(used to seperate different aspects)
   * ^^ Record seperator(used to seperate comments on an issue)
   *
   * @ORDER: title ^] user ^] text ^] os
   **/
  bool delUser = true;
  bool delAssign = true;
  bool delCommentUser = true;
  for (int i = 0; i < issues.size(); i++) {
    std::string title = issues.at(i)->getIssueTitle();
    std::string text = issues.at(i)->getIssueDesc();
    std::string os = issues.at(i)->getIssueOS();
    std::string type = issues.at(i)->getIssueType();
    std::string user = issues.at(i)->getIssueUser();
    std::string assign = issues.at(i)->getIssueAssignee();

    // checks if users has been deleted
    for (int j = 0; j < users.size(); j++) {
      if (users.at(j)->getName() == issues.at(i)->getIssueUser()) {
        delUser = false;
        break;
      }
    }
    // checks if assigned user has been deleted
    for (int j = 0; j < users.size(); j++) {
      if (users.at(j)->getName() == issues.at(i)->getIssueAssignee()) {
        delAssign = false;
        break;
      }
    }
    if (delUser == true) {
      user = "user_Removed";
    }
    if (delAssign == true) {
      assign = "user_Removed";
    }
    // CONTEXT.TXT---
    saveFile << title << "^]" << text << "^]" << os << "^]" << type << "^]"
             << user << "^]" << assign << "^]";
    // COMMENTS.TXT---
    std::vector<Comment*> cWrite = issues.at(i)->getCommentVec();

    /* checks if there exists comments in current issue where the user was
     deleted */
    for (int j = 0; j < users.size(); j++) {
      for (int k = 0; k < cWrite.size(); k++) {
        if (users.at(j)->getName() == cWrite.at(k)->getCommentUser()) {
          delCommentUser = false;
          break;
        }
      }
    }
    if (!cWrite.empty()) {
      commentFile << title << "^]";
      for (int j = 0; j < issues.at(i)->getCommentNum(); j++) {
        commentFile << cWrite.at(j)->getCommentText() << "^]";
        if (delCommentUser == true) {
          commentFile << "userRemoved"
                      << "^]";
        } else {
          commentFile << cWrite.at(j)->getCommentUser() << "^]";
        }
      }
      commentFile << "**";  // seperate comments per issue title
    }
  }
  // CLOSE FILE
  saveFile.close();
  commentFile.close();
}
