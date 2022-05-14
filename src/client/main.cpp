/**
 * Copyright 2020 Cole_Anderson, Christian_Walker, Micheal_Wynnychuk,
 * Radek_Lewandowski
 */

#include <stdio.h>

#include <cstdio>
#include <cstdlib>
#include <future>  // NOLINT
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>  // NOLINT
#include <restbed>            // NOLINT
#include <string>             // NOLINT

#include "IssueTrackerUI.h"

/* Service information */
const char* HOST = "localhost";
const int PORT = 1234;

/* Server operations */
const char* ADD_ISSUE = "addIssue";
const char* GET_ISSUE = "getIssue";
const char* GET_ALL_ISSUES = "getAllIssues";
const char* DELETE_ISSUE = "deleteIssue";

const char* ADD_COMMENT = "addComment";

const char* CREATE_USER = "createUser";
const char* GET_USER = "getUser";
const char* LIST_ALL_USERS = "listAllUsers";
const char* REMOVE_USER = "removeUser";

const char* ISSUE = "issueType";
const char* USER = "userType";
const char* COMMENT = "commentType";

IssueTrackerUI ui;

/**
 * Handle the response from the service.
 * @param response The response object from the server.
 * @param result Result of response object being converted from JSON to string
 */
void handle_response(std::shared_ptr<restbed::Response> response,
                     std::string result) {
  int status_code = response->get_status_code();
  auto length = response->get_header("Content-Length", 0);

  switch (status_code) {
    case 200: {
      restbed::Http::fetch(length, response);
      std::string responseStr(
          reinterpret_cast<char*>(response->get_body().data()), length);

      nlohmann::json resultJSON = nlohmann::json::parse(responseStr);
      result = resultJSON["result"];
      ui.parseMessage(result);  // Parses result and splits data into vectors
      break;
    }
    case 400: {
      restbed::Http::fetch(length, response);
      fprintf(stderr, "Something went wrong on our end: %.*s\n", length,
              response->get_body().data());
      break;
    }
    default:  // Request message was not handled properly in server
      fprintf(
          stderr,
          "An error occurred with the service. (Is the service running?)\n");
      break;
  }
}

/**
 * Creates GET request with single parameter stating the request operation
 * @param operation The type of operation being sent to the server
 * @return The request with the URI string and query parameter attached
 */
std::shared_ptr<restbed::Request> create_get_request(
    const std::string& operation) {
  // Create the URI string
  std::string uri_str;
  uri_str.append("http://");
  uri_str.append(HOST);
  uri_str.append(":");
  uri_str.append(std::to_string(PORT));
  uri_str.append("/issueServer");

  // Configure request headers
  auto request = std::make_shared<restbed::Request>(restbed::Uri(uri_str));
  request->set_method("GET");

  // Set the parameters
  request->set_query_parameter("op", operation);

  return request;
}

/**
 * Creates GET request with three parameters stating the request operation
 * @param operation The type of operation being sent to the server
 * @param text The data being sent to the server to be parsed (title/username)
 * @param param The parameter which states what kind of query parameter to set,
 * ("title", "user")
 * @return The request with the URI string and query parameters attached
 */
std::shared_ptr<restbed::Request> create_get_request(
    const std::string& operation, const std::string& text,
    const std::string& param) {
  // Create the URI string
  std::string uri_str;
  uri_str.append("http://");
  uri_str.append(HOST);
  uri_str.append(":");
  uri_str.append(std::to_string(PORT));
  uri_str.append("/issueServer");

  // Configure request headers
  auto request = std::make_shared<restbed::Request>(restbed::Uri(uri_str));
  request->set_method("GET");

  // Set the parameters
  request->set_query_parameter("op", operation);
  request->set_query_parameter(param, text);

  return request;
}

/**
 * Creates POST request with request type and operation
 * @param type The request type (ISSUE/USER/COMMENT) which tells the server how
 * to handle the response
 * @param operation The type of operation being sent to the server
 * @return The request with the URI string and message attached
 */
std::shared_ptr<restbed::Request> create_post_request(
    const std::string& type, const std::string& operation) {
  std::string title;
  std::string desc;
  std::string os;
  std::string issueType;
  std::string username;
  std::string assign;
  // Create the URI string
  std::string uri_str;
  uri_str.append("http://");
  uri_str.append(HOST);
  uri_str.append(":");
  uri_str.append(std::to_string(PORT));
  uri_str.append("/issueServer");
  // Configure request headers
  auto request = std::make_shared<restbed::Request>(restbed::Uri(uri_str));
  request->set_header("Accept", "*/*");
  request->set_method("POST");
  request->set_header("Content-Type", "text/plain");
  // Create the message
  std::string message;
  if (operation == "addIssue") {  // Creates new issue
    /**
     * Sends GET request to retrieve all existing users in order to assign
     * them to issue
     */
    std::string result;
    std::shared_ptr<restbed::Request> req = create_get_request(LIST_ALL_USERS);
    auto response = restbed::Http::sync(req);
    handle_response(response, result);

    /**
     * Sends GET request to retrieve all existing issues by title
     * for title match checking (titles must be unique)
     */
    req = create_get_request(GET_ALL_ISSUES);
    response = restbed::Http::sync(req);
    handle_response(response, result);
    ui.enterIssueFields(title, desc, os, issueType, username, assign);

    message.append(type);  // Request type (ISSUE/USER/COMMENT)
    message.append("~");   // Delimiter to parse message when sent to server
    message.append(operation);  // Request operation
    message.append("~");
    message.append(title);  // Issue title
    message.append("~");
    message.append(desc);  // Issue description
    message.append("~");
    message.append(os);  // Operating system of machine(s) affected by issue
    message.append("~");
    message.append(issueType);  // Issue type (Feature/Bug/Task)
    message.append("~");
    message.append(username);  // Author of issue (logged in user)
    message.append("~");
    message.append(assign);  // User assigned to issue
    message.append("/");     // Ends message so server can parse it properly
  } else if (operation == "createUser") {  // Creates new user
    username = ui.createUser();  // Prompts user to create a new account
    message.append(type);
    message.append("~");
    message.append(operation);
    message.append("~");
    message.append(username);  // New username
    message.append("/");
  } else if (operation == "removeUser") {  // Deletes existing user
    message.append(type);
    message.append("~");
    message.append(operation);
    message.append("~");
    message.append(ui.getActiveUser());  // Gets logged in user
    message.append("/");
  } else if (operation == "deleteIssue") {  // Deletes existing issue
    message.append(type);
    message.append("~");
    message.append(operation);
    message.append("~");
    std::string test =
        ui.pickIssueToDelete();  // Prompts user to delete an issue
    message.append(test);
    message.append("/");
  }
  // Sets request header and message as the body
  request->set_header("Content-Length", std::to_string(message.length()));
  request->set_body(message);
  return request;
}

/**
 * Creates POST request specifically for comments being sent to the server
 * @param type The request type (COMMENT) which tells the server how
 * to handle the response
 * @param operation The type of operation being sent to the server
 * @param comment The comment being appended to the request message
 * @return The request with the URI string and message attached
 */
std::shared_ptr<restbed::Request> comment_post_request(
    const std::string& type, const std::string& operation,
    const std::string& comment) {
  // Create the URI string
  std::string uri_str;
  uri_str.append("http://");
  uri_str.append(HOST);
  uri_str.append(":");
  uri_str.append(std::to_string(PORT));
  uri_str.append("/issueServer");
  // Configure request headers
  auto request = std::make_shared<restbed::Request>(restbed::Uri(uri_str));
  request->set_header("Accept", "*/*");
  request->set_method("POST");
  request->set_header("Content-Type", "text/plain");
  // Create the message
  std::string message;
  message.append(type);
  message.append("~");
  message.append(operation);
  message.append("~");
  message.append(ui.getIssueTitle());
  message.append("~");
  message.append(comment);
  message.append("~");
  message.append(ui.getActiveUser());
  message.append("/");
  request->set_header("Content-Length", std::to_string(message.length()));
  request->set_body(message);
  return request;
}

/**
 * User login method which calls IssueTrackerUI methods to display login
 * and sends user GET/POST requests depending on logging in or creating a
 * new account
 */
void login() {
  ui.displayLoginScreen();  // Displays UI message for login screen
  int menuChoice;
  std::string result;

  // Prompts user for choice between logging in or creating a new account
  menuChoice = ui.loginChoice();

  switch (menuChoice) {
    case 0: {  // Login with Existing Username
      // Prompts user to enter their existing username
      std::string username = ui.loginUser();
      // Sends GET request to check if username exists in server
      std::shared_ptr<restbed::Request> request =
          create_get_request(GET_USER, username, "user");
      auto response = restbed::Http::sync(request);
      handle_response(response, result);
      if (ui.issueData[0] != "(BLANK)") {   // Username found in system
        printf("\e[1;1H\e[2J");             // "Clear" the screen
        ui.setActiveUser(ui.issueData[0]);  // Sets the logged in username
      } else {                              // Username does not exist in system
        std::cout
            << "No matches were found for that username. Please try again."
            << std::endl;
        login();  // Displays login screen again
      }
      break;
    }
    case 1: {  // Create New Username
      // Creates POST request to add new username to server
      std::shared_ptr<restbed::Request> request =
          create_post_request(USER, CREATE_USER);
      auto response = restbed::Http::sync(request);
      handle_response(response, result);
      printf("\e[1;1H\e[2J");              // "Clear" the screen
      if (ui.issueData[0] == "(TAKEN)") {  // Username is already taken
        std::cout << "\nThat username is in use, please choose another."
                  << std::endl;
        login();
      } else {
        ui.setActiveUser(ui.issueData[0]);  // Sets the logged in username
      }
      break;
    }
    default:
      std::cout << "Invalid input" << std::endl;  // User input error checking
      break;
  }
}

/**
 * Base method for directing user input after selecting UI options.
 * Switch statements handles user input and calls appropriate methods to
 * send either POST or GET requests to server.
 * @return EXIT_SUCCESS when program is exited
 */
int main() {
  int menuChoice = -1;
  std::string result;
  login();  // Displays login screen

  while (menuChoice != 99) {  // Returns to main menu after operations are done
    ui.displayMainMenu();     // Displays main menu
    menuChoice = ui.mainMenuChoice();  // Prompts user to choose menu options
    switch (menuChoice) {
      case 0: {  // Create New Issue
        // Sends POST request to create new issue
        std::shared_ptr<restbed::Request> request =
            create_post_request(ISSUE, ADD_ISSUE);
        auto response = restbed::Http::sync(request);
        handle_response(response, result);
        break;
      }
      case 1: {  // Retrieve an Issue
        // Sends GET request to retrieve all issue titles
        std::shared_ptr<restbed::Request> request =
            create_get_request(GET_ALL_ISSUES);
        auto response = restbed::Http::sync(request);
        handle_response(response, result);  // Issue titles are parsed

        // Prompts user to pick an issue based off its title
        std::string issueChoice = ui.pickIssueToDisplay();

        if (issueChoice != "(BLANK)") {  // If issue exists in server
          std::string comment;

          // Allows users to continuously add comments without returning to menu
          while (comment != "(NoComment)") {
            // Sends GET request to display an entire issue from its title
            request = create_get_request(GET_ISSUE, issueChoice, "title");
            response = restbed::Http::sync(request);
            handle_response(response, result);  // All issue data parsed

            comment = ui.displaySingleIssue();  // Displays all issue data
            if (comment != "(NoComment)") {     // Add a comment
              // Sends POST request to add a new comment to displayed issue
              request = comment_post_request(COMMENT, ADD_COMMENT, comment);
              response = restbed::Http::sync(request);
              handle_response(response, result);
            }
          }
        }
        break;
      }
      case 2: {  // List All Issues
        // Sends GET request to retrieve all issue titles from server
        std::shared_ptr<restbed::Request> request =
            create_get_request(GET_ALL_ISSUES);
        auto response = restbed::Http::sync(request);
        handle_response(response, result);  // Issue titles are parsed
        ui.displayIssues();                 // Issue titles are displayed
        break;
      }
      case 3: {  // Delete an Issue
        // Sends GET request to retrieve all issue titles from server
        std::shared_ptr<restbed::Request> request =
            create_get_request(GET_ALL_ISSUES);
        auto response = restbed::Http::sync(request);
        handle_response(response, result);  // Issue titles are parsed

        // Sends POST request to delete selected issue
        request = create_post_request(ISSUE, DELETE_ISSUE);
        response = restbed::Http::sync(request);
        handle_response(response, result);
        break;
      }
      case 4: {  // List All Users
        // Sends GET request to retrieve all existing usernames from server
        std::shared_ptr<restbed::Request> request =
            create_get_request(LIST_ALL_USERS);
        auto response = restbed::Http::sync(request);
        handle_response(response, result);  // Usernames are parsed
        ui.listAllUsers();                  // Displays all active usernames
        break;
      }
      case 5: {  // Delete a User
        // If user has chosen to delete their account
        if (ui.deleteUser()) {
          // Sends POST request to delete selected username from server
          std::shared_ptr<restbed::Request> request =
              create_post_request(USER, REMOVE_USER);
          auto response = restbed::Http::sync(request);
          handle_response(response, result);
          printf("\e[1;1H\e[2J");  // "Clear" the screen
          login();                 // Return to login screen
        }
        break;
      }
      case 6: {  // Logout
        if (ui.logoutPrompt()) {
          printf("\e[1;1H\e[2J");  // "Clear" the screen
          login();                 // Return to login screen
        }
        break;
      }
      case 7: {  // Exit Program
        if (ui.exitPrompt()) {
          printf("\e[1;1H\e[2J");  // "Clear" the screen
          return EXIT_SUCCESS;
        }
        break;
      }
      default: {  // Default (input error will be thrown)
        break;
      }
    }
  }
}
