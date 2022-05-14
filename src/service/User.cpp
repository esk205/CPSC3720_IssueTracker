/**
 * Copyright 2020 Cole_Anderson, Christian_Walker, Micheal_Wynnychuk,
 * Radek_Lewandowski
 */

#include "User.h"

#include <iostream>
#include <string>
/**
 * Constructor for User
 * @param n Username of new user
 */
User::User(std::string n) {
  if (!n.empty()) {
    name = n;
  }
}

/**
 * Gets the username of specified user
 * @return username of user as string name
 */
std::string User::getName() { return name; }
