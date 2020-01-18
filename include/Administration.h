#ifndef ADMIN_H
#define ADMIN_H

#include <fstream>

class Scanner;

const int MAX_ERRORS = 10;


class Administration {
 public:
  Administration(std::ofstream& fout, Scanner& sc);

  /**
    * Adds line number and resets correctLine.
    */ 
  void newLine();

  /**
    * Display text for an error.
    */ 
  void error(std::string text);

  /**
    * Scan the whole file and output all tokens to fout.
    * Returns the number of tokens.??
    */ 
  int scan();

 private:
  std::ofstream& fout;    // File to print all tokens to
  Scanner& scanner;  // The scanner to use on the input
  int lineNo;        // The current line number
  bool correctLine;  // True if the line has no errors so far
  int errorCount;    // The total number of errors so far
};

#endif
