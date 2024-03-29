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
    * @param debug Set debug mode. Default false.
    */
  Administration(std::ostream& fout, Scanner& sc, bool debug = false);

  /**
   * Returns the current line number.
   */
  int currentLine() {return lineNum;}

  /**
   * Get a new token from the scanner.
   * @return The next scanned token.
   */
  Token getToken();

  /**
   * Adds line number and resets correctLine.
   */
  void newLine();

  /**
   * Print debugging info to the console if in debug mode.
   * @param text The info to print.
   */
  void debugInfo(std::string text);

  /**
   * Display text for an error.
   * @param text The error message.
   */
  void error(std::string text);

  /**
   * Emit assembly code to the output file
   */
  void emit(std::string text, int var = -1, int start = -1);

  /**
   * Return the number of errors.
   */
  int error_count() {return errorCount;}

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
   * Wether or not to print debugging info.
   */
  bool debug;

  /**
   * Checks if current token is an error token.
   * @param ntoken The current token.
   */
  void checkError(Token ntoken);
};

#endif
