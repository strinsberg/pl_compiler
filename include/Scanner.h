#ifndef SCANNER_H
#define SCANNER_h

#include "Token.h"
#include "SymbolTable.h"

class Scanner {
 public:
  Scanner(ifstream &instream, SymbolTable &symboltable);
  ~Scanner();
  Token getToken();

 private:
  std::string line;
  ifstream &inputfile;
  SymbolTable &symtable;
  bool isWhitespace(char inchar);
  bool isSpecial(char inchar);
  Token recognizeName();
  Token recognizeSpecial();
  Token recognizeNumeral();
  void recognizeComment();
};

#endif
