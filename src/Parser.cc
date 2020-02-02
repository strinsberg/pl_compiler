#include "Parser.h"
#include "Administration.h"
#include "Symbol.h"

using std::cout;
using std::endl;

Parser::Parser(Administration& a) : admin(a) {}

void Parser::parse() {
  look = admin.getToken();
  program();
}

void Parser::match(Symbol sym) {
  look.toString(std::cout);  // For debug
  cout << endl;
  if (sym == look.getSymbol()) {
    look = admin.getToken();

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
  std::cout << "Expression List" << endl;
  simpleExpr();
  while(look.getSymbol() == Symbol::COMMA) {
    match(Symbol::COMMA);
    simpleExpr();
  }
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
  std::cout << "Term" << endl;
  factor();
  while(look.getSymbol() == Symbol::TIMES
      or look.getSymbol() == Symbol::FSLASH
      or look.getSymbol() == Symbol::BSLASH) {
    multOp();
    factor();
  }
}

void Parser::factor() {
  std::cout << "Factor" << endl;
  if(look.getSymbol() == Symbol::NUM) {
    match(Symbol::NUM);
  } else if (look.getSymbol() == Symbol::TRUE
      or look.getSymbol() == Symbol::FALSE){
    boolSym();
  } else {
    match(Symbol::LHRND);
    simpleExpr();
    match(Symbol::RHRND);
  }
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
  else if (look.getSymbol() == Symbol::FSLASH)
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
