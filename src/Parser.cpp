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
  cout << "program" << endl;
  block();
  match(Symbol::DOT);
}

void Parser::block() {
  cout << "block" << endl;
  match(Symbol::BEGIN);
  exprList();
  match(Symbol::END);
}

void Parser::exprList() {

}

void Parser::simpleExpr() {
  cout << "simpleExpr" << endl;
  if (look.getSymbol() == Symbol::MINUS)
    match(Symbol::MINUS);
  term();
  while (look.getSymbol() == Symbol::PLUS
      or look.getSymbol() == Symbol::MINUS) {
    addOp();
    term();
  }
}

void Parser::term() {

}

void Parser::factor() {

}

void Parser::addOp() {
  cout << "addOp" << endl;
  if (look.getSymbol() == Symbol::PLUS)
    match(Symbol::PLUS);
  else
    match(Symbol::MINUS);
}

void Parser::multOp() {
  cout << "multOp" << endl;
  if (look.getSymbol() == Symbol::TIMES)
    match(Symbol::TIMES);
  else if (look.getSymbol() == Symbol::FSLASH
    match(Symbol::FSLASH);
  else
    match(Symbol::BSLASH);
}

void Parser::boolSym() {
  cout << "boolSym" << endl;
  if (look.getSymbol() == Symbol::TRUE)
    match(Symbol::TRUE);
  else
    match(Symbol::FALSE);
}

