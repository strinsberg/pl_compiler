#include "Token.h"
#include "Symbol.h"

Token::Token() : sname(Symbol::EMPTY) {}

Token::Token(Symbol sym, std::string lex, int v)
    : sname(sym), lexeme(lex), val(v) {}

Symbol Token::getSymbol() {
  return sname;
}

std::string Token::getLexeme() {
  return lexeme;
}

int Token::getVal() {
  return val;
}
