#ifndef TOKEN_H
#define TOKEN_H

#include "Symbol.h"
#include <iostream>
#include <string>

class Token {
 public:
  Token();
  Token(Symbol sym, std::string lexeme = "", int val = -1);

  /**
    * Returns the symbol.
    */ 
  Symbol getSymbol();
  
  /**
    * Returns the lexeme.
    */
  std::string getLexeme();
  
  /**
    * Returns the value.
    */
  int getVal();

  /**
    * Sets the symbol.
    */
  void setSymbol(Symbol sym);

  /**
    * Sets the lexeme.
    */
  void setLexeme(std::string lexeme);

  /**
    * Sets the value.
    */
  void setVal(int val);

  /**
    * returns a string representation of the Token.
    */
  void toString(std::ostream& out) const;

 private:
  Symbol sname;
  std::string lexeme;
  int val;

};

#endif
