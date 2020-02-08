#include "Parser.h"
#include "Administration.h"
#include "Symbol.h"


Parser::Parser(Administration& a) : admin(a) {}


void Parser::parse() {
  look = admin.getToken();
  program();
}


void Parser::match(Symbol sym) {
  // To Debug
  std::cout << "Matched: " << look.toString() << std::endl << std::endl;

  // If the symbol matche look ahead token then move to the next one
  // otherwise call a syntax error.
  if (sym == look.getSymbol()) {
    look = admin.getToken();
  } else {
    admin.syntaxError(sym, look);
  }
}


void Parser::program() {
  std::cout << "program" << std::endl;
  block();
  match(Symbol::DOT);
}


void Parser::block() {
  std::cout << "block" << std::endl;

  match(Symbol::BEGIN);
  exprList();
  match(Symbol::END);
}


void Parser::defPart() {
  std::cout << "defPart" << std::endl;

  while (defFirst()) {
    def();
    match(Symbol::SEMI);
  }
}

bool Parser::defFirst() {
  Symbol next =  look.getSymbol();

  return (next == Symbol::CONST
        or next == Symbol::INT
          or next == BOOL
            or next == Symbol::PROC);
}


void Parser::def() {
  std::cout << "def" << std::endl;

  Symbol next = look.getSymbol();
  if (next == Symbol::CONST) {
    constDef();
  } else if (next == Symbol::INT or next == Symbol::BOOL) {
    varDef();
  } else {
    procDef();
  }
}


void Parser::constDef() {
   std::cout << "constDef" << std::endl;

   match(Symbol::CONST);
   match(Symbol::ID);
   match(Symbol::EQUAL);
   constant();
}


void Parser::varDef() {
  std::cout << "varDef" << std::endl;

  typeSym();
  vPrime();
}


void Parser::typeSym() {
  std::cout << "typeSym" << std::endl;

  Symbol next = look.getSymbol();
  if(next == Symbol::INT) {
    match(Symbol::INT);
  } else {
    match(Symbol::BOOL);
  }
}


void Parser::vPrime() {
  std::cout << "vPrime" << std::endl;

  if (look.getSymbol() == Symbol::ID) {
    varList();
  } else {
      match(Symbol::ARRAY);
      varList();
      match(Symbol::LHSQR);
      constant();
      match(Symbol::RHSQR);
  }
}

void Parser::varList() {
  std::cout << "varList" << std::endl;

  match(Symbol::ID);
  while(look.getSymbol() == Symbol::COMMA) {
      match(Symbol::COMMA);
      match(Symbol::ID);
  }
}


void Parser::procDef(){
  std::cout << "procDef" << std::endl;

  match(Symbol::PROC);
  match(Symbol::ID);
  block();
}


// Will need to be adjusted when we add proper grammar
void Parser::exprList() {
  std::cout << "Expression List" << std::endl;

  expr();

  while(look.getSymbol() == Symbol::COMMA) {
    match(Symbol::COMMA);
    expr();
  }
}


void Parser::expr() {
  std::cout << "expr" << std::endl;

  primeExpr();

  while (look.getSymbol() == Symbol::AMP or
      look.getSymbol() == Symbol::BAR) {
    primeOp();
    primeExpr();
  }
}


void Parser::primeOp() {
  std::cout << "prime-op" << std::endl;

  if(look.getSymbol() == Symbol::AMP) {
    match(Symbol::AMP);
  } else {
    match(Symbol::BAR);
  }
}


void Parser::primeExpr() {
  std::cout << "prime-Expr" << std::endl;

  simpleExpr();

  if(look.getSymbol() == Symbol::LESS or
      look.getSymbol() == Symbol::EQUAL or
      look.getSymbol() == Symbol::GREAT) {
        relOp();
        simpleExpr();
      }
}


void Parser::relOp() {
  std::cout << "rel-Op" << std::endl;

  if(look.getSymbol() == Symbol::LESS) {
    match(Symbol::LESS);
  } else if (look.getSymbol() == Symbol::EQUAL) {
    match(Symbol::EQUAL);
  } else {
    match(Symbol::GREAT);
  }
}


void Parser::simpleExpr() {
  std::cout << "simpleExpr" << std::endl;
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
  std::cout << "Term" << std::endl;

  factor();

  while(look.getSymbol() == Symbol::TIMES
      or look.getSymbol() == Symbol::FSLASH
      or look.getSymbol() == Symbol::BSLASH) {
    multOp();
    factor();
  }
}


void Parser::factor() {
  std::cout << "Factor" << std::endl;

  if(look.getSymbol() == Symbol::NUM) {
    match(Symbol::NUM);
  } else if (look.getSymbol() == Symbol::TRUE
      or look.getSymbol() == Symbol::FALSE) {
    boolSym();
  } else if (look.getSymbol() == Symbol::LHRND) {
    match(Symbol::LHRND);
    expr();
    match(Symbol::RHRND);
  } else if (look.getSymbol() == Symbol::TILD) {
    match(Symbol::TILD);
    factor();
  } else {
    varAccess();
  }
}


void Parser::addOp() {
  std::cout << "addOp" << std::endl;

  if (look.getSymbol() == Symbol::PLUS)
    match(Symbol::PLUS);
  else
    match(Symbol::MINUS);
}


void Parser::multOp() {
  std::cout << "multOp" << std::endl;

  if (look.getSymbol() == Symbol::TIMES)
    match(Symbol::TIMES);
  else if (look.getSymbol() == Symbol::FSLASH)
    match(Symbol::FSLASH);
  else
    match(Symbol::BSLASH);
}


void Parser::varAccess() {
  std::cout << "varAccess" << std::endl;

  match(Symbol::ID);
  if (look.getSymbol() == Symbol::LHSQR)
    idxSelect();
}


void Parser::idxSelect() {
  std::cout << "idxSelect" << std::endl;

  match(Symbol::LHSQR);
  expr();
  match(Symbol::RHSQR);
}


void Parser::constant() {
  std::cout << "constant" << std::endl;

  if (look.getSymbol() == Symbol::NUM)
    match(Symbol::NUM);
  else if (look.getSymbol() == Symbol::TRUE
        or look.getSymbol() == Symbol::FALSE)
    boolSym();
  else
    match(Symbol::ID);
}


void Parser::boolSym() {
  std::cout << "boolSym" << std::endl;

  if (look.getSymbol() == Symbol::TRUE)
    match(Symbol::TRUE);
  else
    match(Symbol::FALSE);
}
