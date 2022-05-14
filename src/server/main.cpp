/**
 * Copyright 2020 Cole_Anderson, Christian_Walker, Micheal_Wynnychuk,
 * Radek_Lewandowski
 */

#include <cstdlib>
#include <cstring>
#include <memory>
#include <nlohmann/json.hpp>  //NOLINT
#include <restbed>            //NOLINT
#include <string>             //NOLINT
#include <vector>             //NOLINT

#include "Issue.h"
#include "IssueTracker.h"

/**
 * List of request operations
 */
enum OPERATION {
  ADD_ISSUE,
  GET_ISSUE,
  GET_ALL_ISSUES,
  DELETE_ISSUE,
  ADD_COMMENT,
  DELETE_COMMENT,
  CREATE_USER,
  GET_USER,
  LIST_ALL_USERS,
  REMOVE_USER,
  ISSUE,
  USER,
  COMMENT,
  UNKNOWN
};

/**
 * Operation types and message data
 */
struct expression {
  OPERATION type;
  OPERATION op;
  std::string title;
  std::string description;
  std::string os;
  std::string issueType;
  std::string username;
  std::string assign;
  std::string comment;
};

IssueTracker* issueTracker;

#define ALLOW_ALL \
  { "Access-Control-Allow-Origin", "*" }

// Response header to close connection
#define CLOSE_CONNECTION \
  { "Connection", "close" }

/**
 * Sets the request type (ISSUE/USER/COMMENT)
 * @param expr pointer to the expression with the assigned OPERATION type
 * @param type type of operation expressed as a const char*
 */
void set_type(expression* expr, const char* type) {
  if (strcmp("issueType", type) == 0)
    expr->type = ISSUE;
  else if (strcmp("userType", type) == 0)
    expr->type = USER;
  else if (strcmp("commentType", type) == 0)
    expr->type = COMMENT;
  else
    expr->type = UNKNOWN;
}

/**
 * Sets the operation of the request in order for server to handle
 * GET/POST/DELETE requests separately
 * @param expr pointer to the expression with the assigned OPERATION op
 * @param operation operation expressed as a const char*
 */
void set_operation(expression* expr, const char* operation) {
  if (strcmp("addIssue", operation) == 0)
    expr->op = ADD_ISSUE;
  else if (strcmp("getIssue", operation) == 0)
    expr->op = GET_ISSUE;
  else if (strcmp("getAllIssues", operation) == 0)
    expr->op = GET_ALL_ISSUES;
  else if (strcmp("deleteIssue", operation) == 0)
    expr->op = DELETE_ISSUE;
  else if (strcmp("addComment", operation) == 0)
    expr->op = ADD_COMMENT;
  else if (strcmp("deleteComment", operation) == 0)
    expr->op = DELETE_COMMENT;
  else if (strcmp("createUser", operation) == 0)
    expr->op = CREATE_USER;
  else if (strcmp("getUser", operation) == 0)
    expr->op = GET_USER;
  else if (strcmp("listAllUsers", operation) == 0)
    expr->op = LIST_ALL_USERS;
  else if (strcmp("removeUser", operation) == 0)
    expr->op = REMOVE_USER;
  else
    expr->op = UNKNOWN;
}

/**
 * Parses the message sent from client based on request type (expr->type)
 * @param data message being sent from client
 * @param expr expression pointer which will have attributes assigned
 */
void parse(const char* data, expression* expr) {
  char* data_mutable = const_cast<char*>(data);  // data set to char*
  std::vector<std::string> result;
  std::stringstream ss(data_mutable);  // data_mutable set to stringstream
  while (ss.good()) {                  // Continues if no errors are found in ss
    std::string substr;
    getline(ss, substr, '~');  // Checks for '~' in client message to parse data
    result.push_back(substr);  // Pushes parsed data back into string vector
  }

  // Converts first element of result into char* in order to set request type
  std::string convert1 = result[0];
  const char* convert2 = convert1.c_str();
  char* convert3 = const_cast<char*>(convert2);
  set_type(expr, convert3);

  // Converts second element of result into char* to set request operation
  convert1 = result[1];
  convert2 = convert1.c_str();
  convert3 = const_cast<char*>(convert2);
  set_operation(expr, convert3);

  // Checks for request type
  switch (expr->type) {
    case ISSUE: {  // If ISSUE, sets issue data to appropriate expr attributes
      expr->title = result[2];
      if (result.size() > 3) {
        expr->description = result[3];
        expr->os = result[4];
        expr->issueType = result[5];
        expr->username = result[6];
        expr->assign = result[7];
      }
      break;
    }
    case USER: {  // If USER, sets expr username to username data
      expr->username = result[2];
      break;
    }
    case COMMENT: {  // If COMMENT, sets comment data to expr attributes
      expr->title = result[2];
      expr->comment = result[3];
      expr->username = result[4];
      break;
    }
    default:
      break;
  }
}

/**
 * Handles all POST issue operations
 * @param exp expression used to hold issue fields and request operations
 * @param session closes the restbed session and sends the response back
 * to the client
 */
void issue_operations(expression exp,
                      const std::shared_ptr<restbed::Session>& session) {
  std::string result = "";  // Result of request operation to be sent to client

  // Issue fields set from expression attributes
  std::string title = exp.title;
  std::string desc = exp.description;
  std::string os = exp.os;
  std::string iType = exp.issueType;
  std::string user = exp.username;
  std::string assign = exp.assign;

  try {
    switch (exp.op) {
      case ADD_ISSUE: {  // Create new issue
        issueTracker->addAnIssue(title, desc, os, iType, user, assign, result);
        break;
      }
      case DELETE_ISSUE: {  // Delete an existing issue
        issueTracker->deleteIssue(title);
        break;
      }
      default: {  // Error message, exp.op not set properly
        std::string errorMsg = "Issue Operation Error";
        session->close(restbed::BAD_REQUEST, "Unknown exp.op value",
                       {ALLOW_ALL,
                        {"Content-Length", std::to_string(errorMsg.length())},
                        CLOSE_CONNECTION});
        return;
      }
    }
  } catch (int e) {  // Any other errors caught and message thrown
    std::string errorMsg = "Unexpected Error Found";
    session->close(restbed::BAD_REQUEST, "Unable to perform Issue Operation",
                   {ALLOW_ALL,
                    {"Content-Length", std::to_string(errorMsg.length())},
                    CLOSE_CONNECTION});
    return;
  }

  // Result converted to JSON
  std::string resultStr = result;
  nlohmann::json resultJSON;
  resultJSON["result"] = resultStr;
  std::string response = resultJSON.dump();

  // Response sent back to client and session closed
  session->close(restbed::OK, response,
                 {ALLOW_ALL,
                  {"Content-Length", std::to_string(response.length())},
                  CLOSE_CONNECTION});
}

/**
 * Handles all POST user operations
 * @param exp expression used to hold username and request operations
 * @param session closes the restbed session and sends the response back
 * to the client
 */
void user_operations(expression exp,
                     const std::shared_ptr<restbed::Session>& session) {
  std::string result = "";  // Result of request operation to be sent to client

  // Username set from expression username attribute
  std::string username = exp.username;

  try {
    switch (exp.op) {
      case CREATE_USER: {  // Create new user
        result = issueTracker->createUser(username);
        break;
      }
      case REMOVE_USER: {  // Delete existing user
        result = issueTracker->deleteUser(username);
        break;
      }
      default: {  // Error message, exp.op not set properly
        std::string errorMsg = "User Operation Error";
        session->close(restbed::BAD_REQUEST, "Unknown exp.op value",
                       {ALLOW_ALL,
                        {"Content-Length", std::to_string(errorMsg.length())},
                        CLOSE_CONNECTION});
        return;
      }
    }
  } catch (int e) {  // Any other errors caught and message thrown
    std::string errorMsg = "Unexpected Error Found";
    session->close(restbed::BAD_REQUEST, "Unable to perform User Operation",
                   {ALLOW_ALL,
                    {"Content-Length", std::to_string(errorMsg.length())},
                    CLOSE_CONNECTION});
    return;
  }

  // Result converted to JSON
  std::string resultStr = result;
  nlohmann::json resultJSON;
  resultJSON["result"] = resultStr;
  std::string response = resultJSON.dump();

  // Response sent back to client and session closed
  session->close(restbed::OK, response,
                 {ALLOW_ALL,
                  {"Content-Length", std::to_string(response.length())},
                  CLOSE_CONNECTION});
}

/**
 * Handles all POST comment operations
 * @param exp expression used to hold comment fields and request operations
 * @param session closes the restbed session and sends the response back
 * to the client
 */
void comment_operations(expression exp,
                        const std::shared_ptr<restbed::Session>& session) {
  // Result of request operation to be sent to client
  std::string result = "Comment not added";

  // Comment fields set from expression attributes
  std::string title = exp.title;
  std::string username = exp.username;
  std::string comment = exp.comment;

  try {
    switch (exp.op) {
      case ADD_COMMENT: {  // Create new comment
        issueTracker->addToCommentVec(title, comment, username, result);
        break;
      }
      default: {  // Error message, exp.op not set properly
        std::string errorMsg = "Comment Operation Error";
        session->close(restbed::BAD_REQUEST, "Unknown exp.op value",
                       {ALLOW_ALL,
                        {"Content-Length", std::to_string(errorMsg.length())},
                        CLOSE_CONNECTION});
        return;
      }
    }
  } catch (int e) {  // Any other errors caught and message thrown
    std::string errorMsg = "Unexpected Error Found";
    session->close(restbed::BAD_REQUEST, "Unable to perform Comment Operation",
                   {ALLOW_ALL,
                    {"Content-Length", std::to_string(errorMsg.length())},
                    CLOSE_CONNECTION});
    return;
  }

  // Result converted to JSON
  std::string resultStr = result;
  nlohmann::json resultJSON;
  resultJSON["result"] = resultStr;
  std::string response = resultJSON.dump();

  // Response sent back to client and session closed
  session->close(restbed::OK, response,
                 {ALLOW_ALL,
                  {"Content-Length", std::to_string(response.length())},
                  CLOSE_CONNECTION});
}

/**
 * Handles POST request operations separately based on exp.type
 * @param exp Decides which method to handle operation based on exp.type
 * @param session Passes the session through to the appropriate operation
 */
void post_operations(expression exp,
                     const std::shared_ptr<restbed::Session>& session) {
  switch (exp.type) {
    case ISSUE: {
      issue_operations(exp, session);  // Handles all issue operations
      break;
    }
    case USER: {
      user_operations(exp, session);  // Handles all user operations
      break;
    }
    case COMMENT: {
      comment_operations(exp, session);  // Handles all comment operations
      break;
    }
    default: {
      break;
    }
  }
}

/**
 * Handles GET operations for each type (exp.type)
 * @param exp Holds issue title/description, username and handles operation
 * @param session closes the restbed session and sends the response back
 * to the client
 */
void get_operations(expression exp,
                    const std::shared_ptr<restbed::Session>& session) {
  std::string result = "";  // Result of request operation to be sent to client

  // Issue title/description and username set from expression attributes
  std::string title = exp.title;
  std::string desc = exp.description;
  std::string username = exp.username;

  try {
    switch (exp.op) {
      case GET_ISSUE: {  // Get a single issue by title
        result = issueTracker->getAnIssue(title);
        break;
      }
      case GET_ALL_ISSUES: {  // Get all existing issues
        result = issueTracker->getAllIssues();
        break;
      }
      case GET_USER: {  // Get a single user by username
        result = issueTracker->getUser(username);
        break;
      }
      case LIST_ALL_USERS: {  // Get all existing users
        result = issueTracker->getAllUsers();
        break;
      }
      default: {  // Error message, exp.op not set properly
        std::string errorMsg = "GET Operation Error";
        session->close(restbed::BAD_REQUEST, "Unknown exp.op value",
                       {ALLOW_ALL,
                        {"Content-Length", std::to_string(errorMsg.length())},
                        CLOSE_CONNECTION});
        return;
      }
    }
  } catch (int e) {  // Any other errors caught and message thrown
    std::string errorMsg = "Unexpected Error Found";
    session->close(restbed::BAD_REQUEST, "Unable to perform GET Operation",
                   {ALLOW_ALL,
                    {"Content-Length", std::to_string(errorMsg.length())},
                    CLOSE_CONNECTION});
    return;
  }

  // Result converted to JSON
  std::string resultStr = result;
  nlohmann::json resultJSON;
  resultJSON["result"] = resultStr;
  std::string response = resultJSON.dump();

  // Response sent back to client and session closed
  session->close(restbed::OK, response,
                 {ALLOW_ALL,
                  {"Content-Length", std::to_string(response.length())},
                  CLOSE_CONNECTION});
}

/**
 * POST request callback function.
 * @param session Passes the session through to post_operations
 * @param body The body of the message sent from the client
 */
void post_request(const std::shared_ptr<restbed::Session>& session,
                  const restbed::Bytes& body) {
  expression exp;

  // Converts data to string format
  const char* data = reinterpret_cast<const char*>(body.data());
  std::string str(data);

  // Searches for "/" to mark end of message
  std::string mySub = str.substr(0, str.find("/", 0));
  const char* nData = mySub.c_str();

  parse(nData, &exp);  // Parses the data into separate expression attributes
  post_operations(exp, session);  // Handles which post operation to execute
}

/**
 * Handle a POST request.
 * @param session The request session.
 */
void post_method_handler(const std::shared_ptr<restbed::Session>& session) {
  const auto request = session->get_request();
  size_t content_length = request->get_header("Content-Length", 0);
  session->fetch(content_length, &post_request);
}

/**
 * Handle a GET request.
 * @param session The request session.
 */
void get_method_handler(const std::shared_ptr<restbed::Session>& session) {
  const auto request = session->get_request();

  expression exp;
  exp.op = UNKNOWN;
  exp.type = UNKNOWN;
  exp.title = UNKNOWN;
  exp.username = UNKNOWN;

  if (request->has_query_parameter("op")) {
    // Sets exp.op value
    set_operation(&exp, request->get_query_parameter("op").c_str());
    if (request->has_query_parameter("title")) {
      // Sets exp.title as title sent from client
      exp.title = request->get_query_parameter("title");
    } else if (request->has_query_parameter("user")) {
      // Sets exp.username as username sent from client
      exp.username = request->get_query_parameter("user");
    }
  }
  get_operations(exp, session);  // Executes get operations
}

int main(const int, const char**) {
  // Setup service and request handlers
  auto resource = std::make_shared<restbed::Resource>();
  resource->set_path("/issueServer");

  // Initialize:
  issueTracker = new IssueTracker();
  issueTracker->readFile();

  resource->set_method_handler("POST", post_method_handler);
  resource->set_method_handler("GET", get_method_handler);

  auto settings = std::make_shared<restbed::Settings>();
  settings->set_port(1234);

  // Publish and start service
  restbed::Service service;
  service.publish(resource);
  service.start(settings);

  // Cleanup any memory leaks
  issueTracker->memoryCleanIssues();
  delete issueTracker;
  issueTracker = NULL;

  return EXIT_SUCCESS;
}
