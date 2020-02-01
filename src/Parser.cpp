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

}

void Parser::simpleExpr() {

}

void Parser::term() {

}

void Parser::factor() {

}

void Parser::addOp() {

}

void Parser::multOp() {

}

void Parser::boolSym() {

}

