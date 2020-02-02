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
  std::string toString();

 private:
  /**
   * The token's symbol.
   */
  Symbol sname;

  /**
   * The tokens lexeme.
   */
  std::string lexeme;

  /**
   * The numeric value of the token or it's position in the symbol table.
   */
  int val;
};

#endif
