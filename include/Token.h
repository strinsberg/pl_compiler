#ifndef TOKEN_H
#define TOKEN_H

#include "Symbol.h"
#include <string>

class Token {
 public:
  Token();
  Token(Symbol sym, std::string lexeme = "", int val = -1);

  Symbol getSymbol();
  int getSVal();  // type attvalue

  void setSymbol(Symbol sym);
  void setSVal(int val);

 private:
  Symbol sname;
  std::string lexeme;
  int val;  // type attvalue

};

#endif
