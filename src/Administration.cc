#include "Administration.h"
#include "Scanner.h"
#include "Symbol.h"
#include <fstream>
#include <iostream>


Administration::Administration(std::ostream& ofs, Scanner& sc)
    : fout(ofs), scanner(sc), lineNum(1), correctLine(true), errorCount(0)  {}


void Administration::newLine() {
  lineNum++;
  correctLine = true;
}


void Administration::error(std::string text) {
  std::cerr << "Error: Line: " << lineNum << " -> " << text << std::endl;
}

void Administration::checkError(Token ntoken){
  if(ntoken.getSymbol() == Symbol::CHAR_ERR && correctLine == true) {
    error("Invalid Char ( " + ntoken.getLexeme() + " )");
    correctLine = false;
    errorCount++;
  }
}

int Administration::scan() {
  Token current;
  int numTokens = 0;

  while (current.getSymbol() != Symbol::ENDFILE) {
    current = scanner.getToken();
    checkError(current);
    if(current.getSymbol() == Symbol::NEWLINE) {
      newLine();
    }
    if (errorCount >= MAX_ERRORS) {
      error("Max Error limit reached! Quitting compiler!");
      break;
    } else if (current.getSymbol() == Symbol::FULL_TAB) {
      error("Symbol Table is Full! Quitting compiler!");
      break;
    }
    current.toString(fout);
    fout << std::endl;
  }

  return numTokens;
}
