/**
 * Copyright 2020 Cole_Anderson, Christian_Walker, Micheal_Wynnychuk,
 * Radek_Lewandowski
 */

#include "IssueTrackerUI.h"

#include <stdio.h>

#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "Issue.h"
IssueTrackerUI::IssueTrackerUI() {}
IssueTrackerUI::~IssueTrackerUI() {}

/**
 *  Lists existing issues by title for users to select and open
 *  @return the issue title which the user chose to open
 */
std::string IssueTrackerUI::pickIssueToDisplay() {
  printf("\e[1;1H\e[2J");  // "Clear" the screen
  std::cout << "\nWhite Water Reporting - Retrieve an Issue" << std::endl;

  // Displays issue titles if any issues exist
  if (!titleData.empty() && titleData[0] != "(BLANK)") {
    std::cout << "\nIssue Titles:" << std::endl;
    for (int i = 0; i < titleData.size(); i++) {
      std::cout << "\t(" << i << ") " << titleData[i] << std::endl;
    }
    std::cout << "\nSelect Issue To Display Via Index\n";

    // User input error checking
    int issueIndex;
    while (!(std::cin >> issueIndex) || issueIndex < 0 ||
           issueIndex > titleData.size() - 1) {
      std::cout << "INVALID INPUT\n ";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return titleData[issueIndex];
  } else {
    // No existing issues and prompts user to return to main menu
    std::cout << "\nNo Issues To Display\n";
    std::string returnHome;
    std::cout << "\nPress any key to return home." << std::endl;
    std::cin >> returnHome;
    return "(BLANK)";
  }
}

/**
 *  Lists existing issues by title for users to select and delete
 *  @return the issue title which the user chose to delete
 */
std::string IssueTrackerUI::pickIssueToDelete() {
  printf("\e[1;1H\e[2J");  // "Clear" the screen
  std::cout << "\nWhite Water Reporting - Delete an Issue" << std::endl;

  // Displays issue titles if any issues exist
  if (!titleData.empty() && titleData[0] != "(BLANK)") {
    std::cout << "\nIssue Titles:" << std::endl;
    for (int i = 0; i < titleData.size(); i++) {
      std::cout << "\t(" << i << ") " << titleData[i] << std::endl;
    }
    std::cout << "Select Issue To Delete Via Index\n";

    // User input error checking
    int issueIndex;
    while (!(std::cin >> issueIndex) || issueIndex < 0 ||
           issueIndex > titleData.size() - 1) {
      std::cout << "INVALID INPUT\n ";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return titleData[issueIndex];
  } else {
    // No existing issues and prompts user to return to main menu
    std::cout << "\nNo Issues To Delete\n";
    std::string returnHome;
    std::cout << "\nPress any key to return home." << std::endl;
    std::cin >> returnHome;
    return "(BLANK)";
  }
}

/**
 *  Lists existing issues by titles
 */
void IssueTrackerUI::displayIssues() {
  printf("\e[1;1H\e[2J");  // "Clear" the screen
  std::cout << "\nWhite Water Reporting - All Active Issues" << std::endl;

  // Displays issue titles if any issues exist
  if (!titleData.empty() && titleData[0] != "(BLANK)" &&
      titleData[0] != "No issues") {
    std::cout << "\nIssue Titles:" << std::endl;
    for (int i = 0; i < titleData.size(); i++) {
      std::cout << "\t(" << i << ") " << titleData[i] << std::endl;
    }
  } else {
    std::cout << "\nNo issues found" << std::endl;
  }

  // Prompts user to return to main menu
  std::string returnHome;
  std::cout << "\nPress any key to return home." << std::endl;
  std::cin >> returnHome;
}

/**
 *  Displays main menu and all available options
 */
void IssueTrackerUI::displayMainMenu() {
  printf("\e[1;1H\e[2J");  // "Clear" the screen
  std::cout << "\nHi " << getActiveUser() << "!" << std::endl;
  std::cout << "\nWhite Water Reporting - Home" << std::endl;
  std::cout << "What would you like to do?" << std::endl;
  std::cout << "\nIssue Options:" << std::endl;
  std::cout << "\t(0) Create a New Issue" << std::endl;
  std::cout << "\t(1) Retrieve an Issue" << std::endl;
  std::cout << "\t(2) List All Issues" << std::endl;
  std::cout << "\t(3) Delete an Issue" << std::endl;
  std::cout << "\nUser Options:" << std::endl;
  std::cout << "\t(4) List All Users" << std::endl;
  std::cout << "\t(5) Delete Your Account" << std::endl;
  std::cout << "\nOther Options:" << std::endl;
  std::cout << "\t(6) Logout" << std::endl;
  std::cout << "\t(7) Exit Program\n" << std::endl;
}

/**
 *  Prompts the user for menu option choices
 *  @return the users choice by index
 */
int IssueTrackerUI::mainMenuChoice() {
  // User input error checking for available menu options
  int menuChoice;
  while (!(std::cin >> menuChoice) || menuChoice < 0 || menuChoice > 7) {
    std::cout << "INVALID INPUT\n ";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return menuChoice;
}

/**
 *  Displays the UI login screen
 */
void IssueTrackerUI::displayLoginScreen() {
  std::cout << "\nWhite Water Reporting - Login" << std::endl;
  std::cout << "Please login or create a new username to continue."
            << std::endl;
}

/**
 *  Gives user option to login with existing account or create new account
 *  @return 0 to login with existing account or 1 to create new account
 */
int IssueTrackerUI::loginChoice() {
  int loginChoice;
  // User input error checking
  while ((std::cout << "\n(0) Login with Existing Username\n"
                    << "(1) Create new username\n"
                    << std::endl) &&
             !(std::cin >> loginChoice) ||
         loginChoice < 0 || loginChoice > 1) {
    std::cout << "INVALID INPUT\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return loginChoice;
}

/**
 *  Displays user creation page
 *  @return username for the new user
 */
std::string IssueTrackerUI::createUser() {
  printf("\e[1;1H\e[2J");  // "Clear" the screen
  std::cout << "\nWhite Water Reporting - Create New User" << std::endl;
  std::cout << "\nPlease enter a new username: ";
  // Creates and returns new username as string
  std::string username;
  std::cin.ignore();
  std::getline(std::cin, username);
  return username;
}

/**
 *  Displays login page
 *  @return username of the existing user
 */
std::string IssueTrackerUI::loginUser() {
  printf("\e[1;1H\e[2J");  // "Clear" the screen
  std::cout << "\nWhite Water Reporting - Login" << std::endl;
  std::cout << "\nPlease enter your username: ";
  // User enters existing username as string
  std::string username;
  std::cin.ignore();
  std::getline(std::cin, username);
  return username;
}

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
void IssueTrackerUI::enterIssueFields(std::string& title, std::string& desc,
                                      std::string& os, std::string& type,
                                      std::string& user, std::string& assign) {
  int osSelect = -1;
  int typeSelect = -1;
  int userSelect = -1;
  bool titleMatch = false;
  printf("\e[1;1H\e[2J");  // "Clear" the screen
  std::cout << "\nWhite Water Reporting - Create Issue" << std::endl;
  std::cout << "\nIssue Title: " << std::endl;
  std::cin.ignore();
  std::getline(std::cin, title);

  // Checks for existing titles with same name
  for (int i = 0; i < titleData.size(); i++) {
    if (title == titleData[i]) {
      titleMatch = true;
    }
  }
  /**
   * If titles match, user must enter a different title as all titles must
   * be unique
   */
  while (titleMatch) {
    titleMatch = false;
    std::cout << "Issue titles must be unique and this one is already in use. "
                 "Please select another.\n";
    std::cout << "\nIssue Title: " << std::endl;
    std::getline(std::cin, title);
    for (int i = 0; i < titleData.size(); i++) {
      if (title == titleData[i]) {
        titleMatch = true;
      }
    }
  }
  // User enters issue description
  std::cout << "\nIssue Description: " << std::endl;
  std::getline(std::cin, desc);
  // User selects their operating system
  osSelect = selectOS();
  switch (osSelect) {
    case 0: {
      os = "Linux";
      break;
    }
    case 1: {
      os = "MacOS";
      break;
    }
    case 2: {
      os = "Windows";
      break;
    }
    default:
      break;
  }
  // User selects the type of issue
  typeSelect = selectType();
  switch (typeSelect) {
    case 0: {
      type = "Feature";
      break;
    }
    case 1: {
      type = "Bug";
      break;
    }
    case 2: {
      type = "Task";
      break;
    }
    default:
      break;
  }
  // User creating the issue is automatically set as author
  user = getActiveUser();
  // User assigns issue to an active user (can be themself)
  userSelect = selectAssignee();
  assign = issueData[userSelect];
}

/**
 *  Checks for existing titles when creating a new issue
 *  @param title title of issue
 *  @return true if existing title found and false if title is unique
 */
bool IssueTrackerUI::titlesMatch(std::string title) {
  for (int i = 0; i < titleData.size(); i++) {
    if (title == titleData[i]) {
      std::cout << "Title must be unique. This title is already in use"
                << std::endl;
      return true;
    }
  }
  return false;
}

/**
 *  Parses message received from server based on special character delimiters
 *  @param message the message being received from the server
 */
void IssueTrackerUI::parseMessage(std::string message) {
  std::stringstream ss(message);
  std::string contents = ss.str();
  std::string delim = "^]";  // Special delimiter characters for parsing issues
  std::string titleDelim =
      "[^";  // Special delimiter characters for parsing titles
  int found = contents.find("^]");       // Searches for issue delimiter
  int foundTitle = contents.find("[^");  // Searches for title delimiter
  if (found != std::string::npos) {      // If delim found
    issueData.clear();
    size_t pos = 0;
    std::string tok;
    // Parses message based on delim and adds to issueData vector
    while ((pos = contents.find(delim)) != std::string::npos) {
      tok = contents.substr(0, pos);
      issueData.push_back(tok);
      contents.erase(0, pos + delim.length());
    }
  } else if (foundTitle != std::string::npos) {  // If titleDelim found
    titleData.clear();
    size_t pos = 0;
    std::string tok;
    // Parses message based on titleDelim and adds to titleData vector
    while ((pos = contents.find(titleDelim)) != std::string::npos) {
      tok = contents.substr(0, pos);
      titleData.push_back(tok);
      contents.erase(0, pos + titleDelim.length());
    }
  } else {  // If neither delimiter found, check for "-" delimiter (Users)
    titleData.clear();
    issueData.clear();
    while (ss.good()) {
      std::string substr;
      getline(ss, substr, '-');
      if (substr != "") {
        issueData.push_back(substr);
      }
    }
  }
}

/**
 *  Lists all active users
 */
void IssueTrackerUI::listAllUsers() {
  printf("\e[1;1H\e[2J");  // "Clear" the screen
  std::cout << "\nWhite Water Reporting - All Active Users" << std::endl;

  // Checks for any users created then displays them
  if (!issueData.empty() && issueData[0] != "(BLANK)") {
    std::cout << "\nActive Users:" << std::endl;
    for (int i = 0; i < issueData.size(); i++) {
      std::cout << "\t" << issueData[i] << std::endl;
    }
  } else {
    std::cout << "\nNo active users found." << std::endl;
  }

  // Prompts user to return to main menu
  std::string returnHome;
  std::cout << "\nPress any key to return home." << std::endl;
  std::cin >> returnHome;
}

/**
 *  Prompts user to delete their account
 *  @return true if user chooses to delete their account and false otherwise
 */
bool IssueTrackerUI::deleteUser() {
  // User input error checking
  int resp = -1;
  while ((std::cout << "\nAre you sure you would like to delete your account?\n"
                    << "Enter 0 for YES!\n"
                    << "Enter 1 for NO!\n"
                    << std::endl) &&
             !(std::cin >> resp) ||
         resp < 0 || resp > 1) {
    std::cout << "INVALID INPUT\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  if (resp == 0) {
    return true;
  }
  return false;
}

/**
 *  Prompts user to logout
 *  @return true if user chooses to logout and false otherwise
 */
bool IssueTrackerUI::logoutPrompt() {
  // User input error checking
  int resp = -1;
  while ((std::cout << "\nAre you sure you would like to logout?\n"
                    << "Enter 0 for YES!\n"
                    << "Enter 1 for NO!\n"
                    << std::endl) &&
             !(std::cin >> resp) ||
         resp < 0 || resp > 1) {
    std::cout << "INVALID INPUT\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  if (resp == 0) {
    return true;
  }
  return false;
}

/**
 *  Prompts user to exit program
 *  @return trus if user chooses to exit program and false otherwise
 */
bool IssueTrackerUI::exitPrompt() {
  // User input error checking
  int resp = -1;
  while ((std::cout << "\nAre you sure you would like to exit the program?\n"
                    << "Enter 0 for YES!\n"
                    << "Enter 1 for NO!\n"
                    << std::endl) &&
             !(std::cin >> resp) ||
         resp < 0 || resp > 1) {
    std::cout << "INVALID INPUT\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  if (resp == 0) {
    return true;
  }
  return false;
}

/**
 *  Gets the username of user who is currently logged in
 *  @return username of user logged in
 */
std::string IssueTrackerUI::getActiveUser() { return activeUser; }

/**
 *  Sets the logged in user as the active user by their username
 *  @param user the username of the logged in user
 */
void IssueTrackerUI::setActiveUser(std::string user) { activeUser = user; }

/**
 *  Gets specified issue title from issueTitle
 *  @return the issue's title
 */
std::string IssueTrackerUI::getIssueTitle() { return issueTitle; }

/**
 *  Sets issueTitle based on specified issue title
 *  @param title the specified issue title
 */
void IssueTrackerUI::setIssueTitle(std::string title) { issueTitle = title; }

/**
 *  Prompts user to select their operating system
 *  @return 1 for Linux, 2 for MacOS, or 3 for Windows
 */
int IssueTrackerUI::selectOS() {
  // User input error checking
  int selection;
  while ((std::cout << "\nYour Operating System: "
                    << "\n\t(0) Linux"
                    << "\n\t(1) MacOS"
                    << "\n\t(2) Windows" << std::endl) &&
             !(std::cin >> selection) ||
         selection < 0 || selection > 2) {
    std::cout << "INVALID INPUT\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return selection;
}

/**
 *  Prompts user to select the issue type
 *  @return 1 for feature, 2 for bug, or 3 for task
 */
int IssueTrackerUI::selectType() {
  // User input error checking
  int selection;
  while ((std::cout << "\nIssue Type: "
                    << "\n\t(0) Feature"
                    << "\n\t(1) Bug"
                    << "\n\t(2) Task" << std::endl) &&
             !(std::cin >> selection) ||
         selection < 0 || selection > 2) {
    std::cout << "INVALID INPUT\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return selection;
}

/**
 *  Prompts user issue to assign an active user
 *  @return username based on corresponding index
 */
int IssueTrackerUI::selectAssignee() {
  int selection;
  std::cout << "\nAssign User to Issue:" << std::endl;

  // Displays all active users
  for (int i = 0; i < issueData.size(); i++) {
    std::cout << "\t(" << i << ") " << issueData[i] << std::endl;
  }

  // User input error checking
  while (!(std::cin >> selection) || selection < 0 ||
         selection > issueData.size() - 1) {
    std::cout << "INVALID INPUT\n";
    std::cout << "\nAssign User to Issue:" << std::endl;
    for (int i = 0; i < issueData.size(); i++) {
      std::cout << "\t(" << i << ") " << issueData[i] << std::endl;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return selection;
}

/**
 *  Displays a single issue and all its information as well as prompting
 *  user to either add a comment or return to main menu
 *  @return new comment if added or (NoComment) if user chooses to return home
 */
std::string IssueTrackerUI::displaySingleIssue() {
  printf("\e[1;1H\e[2J");  // "Clear" the screen
  std::cout << "\nWhite Water Reporting - Display an Issue\n" << std::endl;
  std::string comment;

  // Displays issue fields with respective infomation
  std::vector<std::string> fields;
  fields.push_back("\tTitle: ");
  fields.push_back("\tDescription: ");
  fields.push_back("\tOS: ");
  fields.push_back("\tIssue Type: ");
  fields.push_back("\tAuthor: ");
  fields.push_back("\tAssigned User: ");
  fields.push_back("\n\nComments:\n\n");
  fields.push_back("\tComment: ");
  fields.push_back("\tUser: ");

  // If issue exists, display all of their information and comments (if created)
  if (!issueData.empty() && issueData[0] != "(BLANK)") {
    setIssueTitle(issueData[0]);
    int iter = 0;
    for (int i = 0; i < issueData.size(); i++) {
      if (iter < 6) {  // Display first 6 issue fields
        std::cout << fields[iter];
        iter++;
      } else {
        if (iter == 6) {  // Display "Comments:" field
          std::cout << fields[iter];
          iter++;
        }
        std::cout << fields[iter];
        iter++;
        if (iter > 8) {  // For each comment added, repeat last two fields
          iter = 7;
        }
      }
      std::cout << issueData[i] << std::endl;
      if (iter == 7) {  // Add extra space for readability
        std::cout << "\n";
      }
    }

    /**
     * Prompt user to add a comment or return home.
     * Performs input error checking as well
     */
    int selection;
    while ((std::cout << "\nWhat would you like to do next?"
                      << "\n\t(0) Add a comment"
                      << "\n\t(1) Return home" << std::endl) &&
               !(std::cin >> selection) ||
           selection < 0 || selection > 1) {
      std::cout << "INVALID INPUT\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    if (selection == 0) {
      comment = addComment();
    } else {
      comment =
          "(NoComment)";  // Returns (NoComment) in order to skip commenting
    }
  } else {
    // No issues exist and user is prompted to return to main menu
    std::cout << "No issues to display" << std::endl;
    comment = "(NoComment)";
    std::string returnHome;
    std::cout << "\nPress any key to return home." << std::endl;
    std::cin >> returnHome;
  }
  return comment;
}

/**
 *  Prompts user to enter their comment
 *  @return the comment
 */
std::string IssueTrackerUI::addComment() {
  std::string comment;
  std::cout << "\nEnter your comment:\n";
  std::cin.ignore();
  std::getline(std::cin, comment);
  return comment;
}
