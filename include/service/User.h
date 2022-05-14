/**
 * Copyright 2020 Cole_Anderson, Christian_Walker, Micheal_Wynnychuk,
 * Radek_Lewandowski
 */

#ifndef USER_H /* NOLINT */
#define USER_H /* NOLINT */

#include <string>

class User {
 public:
  User() {}
  /**
   * Constructor for User
   * @param n Username of new user
   */
  User(std::string n);
  ~User() {}

  /**
   * Gets the username of specified user
   * @return username of user as string name
   */
  std::string getName();

 private:
  /**
   * Username of user
   */
  std::string name;
};
#endif /* NOLINT */
