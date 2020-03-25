#include "Administration.h"
#include "Scanner.h"
#include "Symbol.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>


Administration::Administration(std::ostream& ofs, Scanner& sc, bool deb)
    : fout(ofs), scanner(sc), lineNum(1), correctLine(true), errorCount(0),
      debug(deb) {}


Token Administration::getToken() {
  Token t;

  // Get the next token, but process newlines here
  while (1) {
    t = scanner.getToken();
    //fout << t.toString() << " ";

    if (t.getSymbol() != Symbol::NEWLINE)
      break;
    newLine();
  }

  // Deal with error tokens here
  checkError(t);
  return t;
}


void Administration::newLine() {
  lineNum++;
  correctLine = true;
  //fout << std::endl << std::endl << lineNum << ":  ";
}


void Administration::error(std::string text) {
  if (errorCount > MAX_ERRORS) {
    std::cerr << "*** Reached Maximum Allowed Errors ***" << std::endl;
    exit(-1);
  }

  // If we have not yet had an error on this line print out error text
  if (correctLine != false) {
    errorCount++;
    std::cerr << "Error: Line: " << lineNum << " -> " << text << std::endl;
    correctLine = false;
  }
}


void Administration::debugInfo(std::string text) {
  if (debug)
    std::cout << text << std::endl;
}


void Administration::checkError(Token ntoken){
  if(ntoken.getSymbol() == Symbol::CHAR_ERR)
    error("Invalid Char: '" + ntoken.getLexeme() + "'");
  else if(ntoken.getSymbol() == Symbol::NUM_ERR)
    error("Number not a valid integer: '" + ntoken.getLexeme() + "'");
}


void Administration::emit(std::string text) {
  fout << text;
}
