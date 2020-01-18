#include "Administration.h"
#include "Scanner.h"
#include <fstream>
#include <iostream>


Administration::Administration(std::ostream& ofs, Scanner& sc)
    : fout(ofs), scanner(sc), lineNum(0), correctLine(true), errorCount(0)  {}


void Administration::newLine() {
  lineNum++;
  correctLine = true;
}


void Administration::error(std::string text) {
  std::cerr << "Error: Line: " << lineNum << " -> " << text << std::endl;
}


int Administration::scan() {
  Token current;
  int numTokens = 0;

  while (current.getSymbol() != Symbol::ENDFILE) {
    current = scanner.getToken();
    current.toString(fout);
    fout << std::endl;
  }

  return numTokens;
}
