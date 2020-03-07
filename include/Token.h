#ifndef TOKEN_H
#define TOKEN_H

#include "Symbol.h"
#include <iostream>
#include <string>


class Token {
 public:
  /**
   * Creates a new default token.
   * Sets Symbol to EMPTY, lexeme to "", and value to -1.
   */
  Token();

  /**
   * Creates a new token.
   * @param sym The symbol for the token.
   * @param lexeme The lexeme for the token. Default "".
   * @param val The numerical value to give to the token. Default -1.
   */
  Token(Symbol sym, std::string lexeme = "", int val = -1);

  Token(const Token& tok);

  /**
   * Returns the symbol.
   */ 
  Symbol getSymbol() const;
  
  /**
   * Returns the lexeme.
   */
  std::string getLexeme() const;
  
  /**
   * Returns the value.
   */
  int getVal() const;

  /**
   * Sets the symbol.
   * @param sym The symbol to give the token.
   */
  void setSymbol(Symbol sym);

  /**
   * Sets the lexeme.
   * @param lexeme The lexeme to give the token.
   */
  void setLexeme(std::string lexeme);

  /**
   * Sets the value.
   * @param val The value to give the token.
   */
  void setVal(int val);

  /**
   * Returns a string representation of the Token.
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
   * The numeric value of the token.
   */
  int val;
};

#endif
