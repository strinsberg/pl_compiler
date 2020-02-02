#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include "Token.h"
#include "Scanner.h"


const int MAX_ERRORS = 10;


class Administration {
 public:
   /**
    * Creates a new Administration object.
    * @param fout The output file stream.
    * @param sc The scanner beign used by administration.
    */
  Administration(std::ostream& fout, Scanner& sc);

  Token getToken();

  /**
   * Adds line number and resets correctLine.
   */
  void newLine();

  /**
   * Deal with a syntax error.
   * Updates error states correctly and prints an error showing information
   * about the expected and actual tokens or symbols.
   * @parm expected The Symbol type we expected to see.
   * @param actual The lexeme we got.
   * @return false when the max number of errors is reached.
   */
  void syntaxError(Symbol expected, Token& actual);

  /**
   * Display text for an error.
   * @param text The error message.
   */
  void error(std::string text);

 private:
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
