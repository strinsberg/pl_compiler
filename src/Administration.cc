#include "Administration.h"
#include "Scanner.h"
#include "Symbol.h"
#include <cstdlib>
#include <fstream>
#include <iostream>


Administration::Administration(std::ostream& ofs, Scanner& sc)
    : fout(ofs), scanner(sc), lineNum(1), correctLine(true), errorCount(0)  {
  fout << "1:  ";
}


Token Administration::getToken() {
  Token t;

  // Get the next token, but process newlines here
  while (1) {
    t = scanner.getToken();
    fout << t.toString() << " ";

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
  fout << std::endl << std::endl << lineNum << ":  ";
}


void Administration::error(std::string text) {
  errorCount++;

  if (errorCount > MAX_ERRORS) {
    std::cerr << "*** Reached Maximum Allowed Errors ***" << std::endl;
    exit(-1);
  }

  // If we have not yet had an error on this line print out error text
  if (correctLine != false) {
    std::cerr << "Error: Line: " << lineNum << " -> " << text << std::endl;
    correctLine = false;
  }
}


void Administration::syntaxError(Symbol expected, Token& actual) {
  error("Syntax Error near: '" + actual.getLexeme()
    + "' Expected Token Type: " + SymbolToString.at(expected));
}


void Administration::checkError(Token ntoken){
  if(ntoken.getSymbol() == Symbol::CHAR_ERR)
    error("Invalid Char ( " + ntoken.getLexeme() + " )");
  else if(ntoken.getSymbol() == Symbol::NUM_ERR)
    error("Number not a valid integer ( " + ntoken.getLexeme() + " )");
}

