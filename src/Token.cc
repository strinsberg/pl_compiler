#include "Token.h"
#include "Symbol.h"
#include <string>
#include <sstream>


Token::Token() : sname(Symbol::EMPTY) {}


Token::Token(Symbol sym, std::string lex, int v)
    : sname(sym), lexeme(lex), val(v) {}


Token::Token(const Token& tok) : sname(tok.getSymbol()), lexeme(tok.getLexeme()),
    val(tok.getVal()) {}


Symbol Token::getSymbol() const {
  return sname;
}


std::string Token::getLexeme() const {
  return lexeme;
}


int Token::getVal() const {
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


std::string Token::toString() {
  std::stringstream ss;
  ss << "<" << SymbolToString.at(sname);

  if (sname == Symbol::ID)  
    ss << "  " << lexeme;

  if (sname == Symbol::NUM)
    ss << " "<< val;

  ss << ">";
  return ss.str();
}
