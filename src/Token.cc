#include "Token.h"
#include "Symbol.h"
#include <string>
#include <sstream>


Token::Token() : sname(Symbol::EMPTY) {}


Token::Token(Symbol sym, std::string lex, int v)
    : sname(sym), lexeme(lex), iVal(v) {}


Symbol Token::getSymbol() {
  return sname;
}


std::string Token::getLexeme() {
  return lexeme;
}


int Token::getVal() {
  return iVal;
}


double Token::getFVal() {
  return fVal;
}


int Token::getSize() {
  return size;
}


Symbol Token::getDType() {
  return dType;
}


Symbol Token::getIdType() {
  return idType;
}


std::map<std::string, Token>& Token::getFields() {
  return fields;
}

void Token::setSymbol(Symbol sym) {
  sname = sym;
}


void Token::setLexeme(std::string str) {
  lexeme = str;
}


void Token::setIVal(int v) {
  iVal = v;
}


void Token::setFVal(double val) {
  fVal = val;
}


void Token::setSize(int s) {
  size = s;
}


void Token::setDType(Symbol type) {
  dType = type;
}


void Token::setIdType(Symbol type) {
  idType = type;
}


std::string Token::toString() {
  std::stringstream ss;
  ss << "<" << SymbolToString.at(sname);

  if (sname == Symbol::ID)  
    ss << "  " << lexeme;

  if (sname == Symbol::NUM)
    ss << " "<< iVal;

  ss << ">";
  return ss.str();
}
