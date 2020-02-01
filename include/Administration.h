#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include "Token.h"

class Scanner;

const int MAX_ERRORS = 10;


class Administration {
 public:
   /**
     * Creates a new Administration object.
     * @param fout The output file stream.
     * @param sc The scanner beign used by administration.
     */
  Administration(std::ostream& fout, Scanner& sc);

  /**
    * Adds line number and resets correctLine.
    */
  void newLine();

  /**
    * Display text for an error.
    * @param text The error message.
    */
  void error(std::string text);

  /**
    * Scan the whole file and output all tokens to fout.
    * Returns the number of tokens.
    */
  int scan();

  /**
   * File to print all tokens to.
   */
  std::ostream& fout;
  /**
    * The scanner to use on the input.
    */
  Scanner& scanner;
  /**
    * The current line number.
    */
  int lineNum;
  /**
    * True if the line has no errors so far.
    */
  bool correctLine;
  /**
    * The total number of errors so far.
    */
  int errorCount;
  /**
    * Checks if current token is an error token.
    * @param ntoken The current token.
    */
  void checkError(Token ntoken);
};

#endif
