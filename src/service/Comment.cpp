/**
 * Copyright 2020 Cole_Anderson, Christian_Walker, Micheal_Wynnychuk,
 * Radek_Lewandowski
 */

#include "Comment.h"

#include <string>
Comment::Comment() {}
Comment::~Comment() {}

/**
 * Gets the author of a comment
 * @return commentUser
 */
std::string Comment::getCommentUser() { return commentUser; }
/**
 * Gets the text from a comment
 * @return commentText
 */
std::string Comment::getCommentText() { return commentText; }

/**
 * Sets the text of a comment
 * @param t the text of a comment
 */
void Comment::setText(std::string t) { commentText = t; }
/**
 * Sets the author of a comment
 * @param u the author of a comment
 */
void Comment::setUser(std::string u) { commentUser = u; }
