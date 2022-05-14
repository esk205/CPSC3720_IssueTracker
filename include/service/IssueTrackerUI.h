/**
 * Copyright 2020 Cole_Anderson, Christian_Walker, Micheal_Wynnychuk,
 * Radek_Lewandowski
 */

#ifndef ISSUETRACKERUI_H /* NOLINT */
#define ISSUETRACKERUI_H /* NOLINT */

#include <iostream>
#include <string>
#include <vector>

#include "Issue.h"

class IssueTrackerUI {
 public:
  IssueTrackerUI();
  ~IssueTrackerUI();

  /**
   *  Lists existing issues by title for users to select and open
   *  @return the issue title which the user chose to open
   */
  std::string pickIssueToDisplay();
  /**
   *  Lists existing issues by title for users to select and delete
   *  @return the issue title which the user chose to delete
   */
  std::string pickIssueToDelete();
  /**
   *  Lists existing issues by titles
   */
  void displayIssues();

  /**
   *  Displays the UI login screen
   */
  void displayLoginScreen();
  /**
   *  Gives user option to login with existing account or create new account
   *  @return 0 to login with existing account or 1 to create new account
   */
  int loginChoice();

  /**
   *  Displays user creation page
   *  @return username for the new user
   */
  std::string createUser();
  /**
   *  Displays login page
   *  @return username of the existing user
   */
  std::string loginUser();

  /**
   *  Displays main menu and all available options
   */
  void displayMainMenu();
  /**
   *  Prompts the user for menu option choices
   *  @return the users choice by index
   */
  int mainMenuChoice();

  /**
   *  Displays issue creation page and takes in user input as fields for issues
   *  in order to create a new issue
   *  @param title issue title
   *  @param desc issue description
   *  @param os operating system of machine(s) affected by issue
   *  @param type type of issues (bug/feature/task)
   *  @param user author of issue
   *  @param assign user assigned to issue
   */
  void enterIssueFields(std::string& title, std::string& desc, std::string& os,
                        std::string& type, std::string& user,
                        std::string& assign);

  /**
   *  Checks for existing titles when creating a new issue
   *  @param title title of issue
   *  @return true if existing title found and false if title is unique
   */
  bool titlesMatch(std::string title);

  /**
   *  Parses message received from server based on special character delimiters
   *  @param message the message being received from the server
   */
  void parseMessage(std::string message);
  
  /**
   *  Lists all active users
   */
  void listAllUsers();
  /**
   *  Prompts user to delete their account
   *  @return true if user chooses to delete their account and false otherwise
   */
  bool deleteUser();

  /**
   *  Prompts user to logout
   *  @return true if user chooses to logout and false otherwise
   */
  bool logoutPrompt();
  /**
   *  Prompts user to exit program
   *  @return trus if user chooses to exit program and false otherwise
   */
  bool exitPrompt();
  
  /**
   *  Gets the username of user who is currently logged in
   *  @return username of user logged in
   */
  std::string getActiveUser();
  /**
   *  Sets the logged in user as the active user by their username
   *  @param user the username of the logged in user
   */
  void setActiveUser(std::string user);
  
  /**
   *  Gets specified issue title from issueTitle
   *  @return the issue's title
   */
  std::string getIssueTitle();
  /**
   *  Sets issueTitle based on specified issue title
   *  @param title the specified issue title
   */
  void setIssueTitle(std::string title);
  
  /**
   *  Prompts user to select their operating system
   *  @return 1 for Linux, 2 for MacOS, or 3 for Windows
   */
  int selectOS();
  /**
   *  Prompts user to select the issue type
   *  @return 1 for feature, 2 for bug, or 3 for task
   */
  int selectType();
  /**
   *  Prompts user issue to assign an active user
   *  @return username based on corresponding index
   */
  int selectAssignee();
  
  /**
   *  Displays a single issue and all its information as well as prompting 
   *  user to either add a comment or return to main menu
   *  @return new comment if added or (NoComment) if user chooses to return home
   */
  std::string displaySingleIssue();
  /**
   *  Prompts user to enter their comment
   *  @return the comment
   */
  std::string addComment();
  
  /**
   * Vector of issue data parsed and sent from server
   */
  std::vector<std::string> issueData;
  /**
   * Vector of comment data parsed and sent from server
   */
  std::vector<std::string> commentData;
  /**
   * Vector of title data parsed and sent from server
   */
  std::vector<std::string> titleData;

 private:
  /**
   * String that holds the logged in username
   */
  std::string activeUser;
  /**
   * String that holds a specified issue title
   */
  std::string issueTitle;
};
#endif /* NOLINT */
