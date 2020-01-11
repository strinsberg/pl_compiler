#ifndef TOKEN_H
#define TOKEN_H

#include "Symbol.h"

class Token {
 public:
  Symbol(Symbol sym, int val);

  Symbol getSymbol();
  int getSVal();  // type attvalue

  void setSymbol(Symbol sym);
  void setSVal(int val);

 private:
  Symbol sname;
  int sval;  // type attvalue

};

#endif
