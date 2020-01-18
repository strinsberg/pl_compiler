#ifndef SCANNER_H
#define SCANNER_h
#include <fstream>
#include "SymbolTable.h"
#include "Token.h"
#include <map>


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
  std::map<std::string, Symbol> symmap;
  Token recognizeName();
  Token recognizeSpecial();
  Token recognizeNumeral();
};

#endif
