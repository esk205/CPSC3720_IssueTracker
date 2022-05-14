/*
 * CPSC 2720 Fall 2018
 * Copyright 2018 Dr. Anvik <john.anvik@uleth.ca>
 */

#ifndef EXCEPTIONS_H_INCLUDED
#define EXCEPTIONS_H_INCLUDED

#include <stdexcept>

/**
 * Exception for dividing by zero
 */
class div_by_zero_error : public std::runtime_error {
 public:
  /**
   * @param errMessage An error message.
   */
  explicit div_by_zero_error(const char* errMessage)
      : std::runtime_error(errMessage) {}
};

#endif  // EXCEPTIONS_H_INCLUDED
