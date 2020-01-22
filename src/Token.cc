#include "Token.h"
#include "Symbol.h"
#include <string>
#include <iostream>

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


void Token::setSymbol(Symbol sym) {
  sname = sym;
}


void Token::setLexeme(std::string str) {
  lexeme = str;
}


void Token::setVal(int v) {
  val = v;
}

void Token::toString(std::ostream& out) {
  out << "<" << sname << ", " << lexeme << ", " << val << ">";
}
