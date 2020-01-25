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
  } else if(ntoken.getSymbol() == Symbol::FULL_TAB && correctLine == true) {
    error("Symbol Table is Full");
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
    current.toString(fout);
    fout << std::endl;
  }

  return numTokens;
}
