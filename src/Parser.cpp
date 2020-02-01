#include "Parser.h"
#include "Administration.h"
#include "Symbol.h"

Parser::Parser(Administration& a) : admin(a) {}

void Parser::parse() {

}

void Parser::match(Symbol sym) {
  if (sym == look.getSymbol()) {
    std::cout << look.toString() << std::endl;  // For debug
    look = scanner.getToken();

  } else {
    admin.syntaxError(sym, look.getSymbol());
  }
}

void Parser::program() {

}

void Parser::block() {

}

void Parser::exprList() {
  std::cout << "Expression List" << endl;
  simpleExpr();
  while(look.getSymbol() == Symbol::COMMA) {
    match(Symbol::COMMA);
    simpleExpr();
  }
}

void Parser::simpleExpr() {

}

void Parser::term() {
  std::cout << "Term" << endl;
  factor();
  while(look.getSymbol() == Symbol::TIMES or Symbol::FSLASH or Symbol::BSLASH) {
    multOp();
    factor();
  }
}

void Parser::factor() {
  std::cout << "Factor" << endl;
  if(look.getSymbol() == Symbol::NUM) {
    match(Symbol::NUM);
  } else if (look.getSymbol() == true or false){
    boolSym();
  } else {
    match(Symbol::LHRND);
    simpleExpr();
    match(Symbol::RHRND);
  }
}

void Parser::addOp() {

}

void Parser::multOp() {

}

void Parser::boolSym() {

}
