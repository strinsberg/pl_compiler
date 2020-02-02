#include "Administration.h"
#include "Scanner.h"
#include "Symbol.h"
#include <cstdlib>
#include <fstream>
#include <iostream>


Administration::Administration(std::ostream& ofs, Scanner& sc)
    : fout(ofs), scanner(sc), lineNum(1), correctLine(true), errorCount(0)  {}


Token Administration::getToken() {
  Token t = scanner.getToken();
  while (t.getSymbol() == Symbol::NEWLINE) {
    t = scanner.getToken();
    newLine();
  }
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

