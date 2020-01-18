#ifndef SCANNER_H
#define SCANNER_h
#include <fstream>
#include "Token.h"
#include "SymbolTable.h"

class Scanner {
 public:
  Scanner(std::ifstream &ifs, SymbolTable &symboltable);
  ~Scanner() {};
  Token getToken();
  bool isWhitespace(char inchar);
  bool isSpecial(char inchar);

 private:
  std::ifstream &fin;
  SymbolTable &symtable;
  std::string line;
  char inChar;
  std::size_t pos;
  Token recognizeName();
  Token recognizeSpecial();
  Token recognizeNumeral();
};

#endif
