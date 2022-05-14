/**
 * Copyright 2020 Cole_Anderson, Christian_Walker, Micheal_Wynnychuk,
 * Radek_Lewandowski
 */

#ifndef COMMENT_H /* NOLINT */
#define COMMENT_H /* NOLINT */

#include <string>
class Comment {
 public:
  Comment();
  ~Comment();

  /**
   * Gets the text from a comment
   * @return commentText
   */
  std::string getCommentText();
  /**
   * Gets the author of a comment
   * @return commentUser
   */
  std::string getCommentUser();

  /**
   * Sets the text of a comment
   * @param t the text of a comment
   */
  void setText(std::string t);
  /**
   * Sets the author of a comment
   * @param u the author of a comment
   */
  void setUser(std::string u);

 private:
  /**
   * Author of a comment
   */
  std::string commentUser;
  /**
   * Text of a comment
   */
  std::string commentText;
};
#endif /* NOLINT */
