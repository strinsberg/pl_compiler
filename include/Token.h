#ifndef TOKEN_H
#define TOKEN_H

#include "Symbol.h"
#include <string>

class Token {
 public:
  Token();
  Token(Symbol sym, std::string lexeme = "", int val = -1);

  Symbol getSymbol();
  std::string getLexeme();
  int getVal();  // type attvalue

  void setSymbol(Symbol sym);
  void setVal(int val);

 private:
  Symbol sname;
  std::string lexeme;
  int val;  // type attvalue

};

#endif
