#ifndef SCANNER_H
#define SCANNER_h
#include <iostream>
#include "SymbolTable.h"
#include "Token.h"
#include <map>


class Scanner {
 public:
  Scanner(std::istream &ifs, SymbolTable &symboltable);
  ~Scanner() {};
  Token getToken();

 private:
  std::istream &fin;
  SymbolTable &symtable;
  std::string line;
  char inChar;
  std::size_t pos;
  std::map<std::string, Symbol> symmap;
  bool isWhitespace(char inchar);
  bool isSpecial(char inchar);
  bool isSeparator(char inchar);
  Token recognizeName();
  Token recognizeSpecial();
  Token recognizeNumeral();
};

#endif
