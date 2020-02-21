#include <set>
#include "Parser.h"
#include "Administration.h"
#include "Symbol.h"
#include "Grammar.h"


Parser::Parser(Administration& a) : admin(a) {}


void Parser::parse() {
  look = admin.getToken();
  program( std::set<Symbol>{Symbol::ENDFILE} );
}


void Parser::match(Symbol sym, std::set<Symbol> stop) {

  // If the symbol matched look ahead token then move to the next one
  // otherwise call a syntax error.
  if (sym == look.getSymbol()) {
    std::cout << "Matched: " << look.toString() << std::endl << std::endl;
    look = admin.getToken();
  } else {
    syntaxError(stop);
  }
  syntaxCheck(stop);
}


void Parser::syntaxError(std::set<Symbol> stop) {
  admin.error("Syntax Error");  // need more info using look.getSymbol()
  while (! stop.count(look.getSymbol()))
    look = admin.getToken();
}

void Parser::syntaxCheck(std::set<Symbol> stop) {
  if (! stop.count(look.getSymbol()))
    syntaxError(stop);
}


// Recursive rule functions ///////////////////////////////////////////

void Parser::program(std::set<Symbol> stop) {
  std::cout << "program" << std::endl;
  block(munion({stop, {Symbol::DOT}}));
  match(Symbol::DOT, stop);
}


void Parser::block(std::set<Symbol> stop) {
  std::cout << "block" << std::endl;

  match(Symbol::BEGIN, munion({stop, First.at(NT::DEF_PART), First.at(NT::STMT_PART), {Symbol::END}}));
  defPart(munion({stop, First.at(NT::STMT_PART), {Symbol::END}}));
  stmtPart(munion({stop, {Symbol::END}}));
  match(Symbol::END, stop);
}

void Parser::stmtPart(std::set<Symbol> stop) {
  std::cout << "stmtPart" << std::endl;

  while (First.at(NT::STMT).count(look.getSymbol())) {
    stmt(munion({stop, {Symbol::SEMI}}));
    match(Symbol::SEMI, stop);
  }
}


void Parser::stmt(std::set<Symbol> stop) {
  std::cout << "stmt" << std::endl;

  Symbol next = look.getSymbol();
  if (next == Symbol::SKIP)
    emptyStmt(stop);
  else if (next == Symbol::READ)
    readStmt(stop);
  else if (next == Symbol::WRITE)
    writeStmt(stop);
  else if (next == Symbol::ID)
    assignStmt(stop);
  else if (next == Symbol::CALL)
    procStmt(stop);
  else if (next == Symbol::IF)
    ifStmt(stop);
  else
    doStmt(stop);
  
  // Probably having an error here if nothing is matched would be good
  // otherwise the error when none are correct will expect a do stmt
}

void Parser::emptyStmt(std::set<Symbol> stop) {
  std::cout << "emptyStmt" << std::endl;

  match(Symbol::SKIP, stop);
}

void Parser::readStmt(std::set<Symbol> stop) {
  std::cout << "readStmt" << std::endl;

  match(Symbol::READ, stop);
  vacsList(stop);
}

void Parser::vacsList(std::set<Symbol> stop) {
  std::cout << "vacsList" << std::endl;

  varAccess(stop);
  while (look.getSymbol() == Symbol::COMMA) {
    match(Symbol::COMMA, stop);
    varAccess(stop);
  }
}

void Parser::writeStmt(std::set<Symbol> stop) {
  std::cout << "writeStmt" << std::endl;

  match(Symbol::WRITE, stop);
  exprList(stop);
}


void Parser::defPart(std::set<Symbol> stop) {
  std::cout << "defPart" << std::endl;

  while (First.at(NT::DEF).count(look.getSymbol())) {
    def(munion({stop, {Symbol::SEMI}}));
    match(Symbol::SEMI, stop);
  }
}

bool Parser::defFirst() {
  Symbol next =  look.getSymbol();

  return (next == Symbol::CONST
        or next == Symbol::INT
          or next == BOOL
            or next == Symbol::PROC);
}


void Parser::def(std::set<Symbol> stop) {
  std::cout << "def" << std::endl;

  Symbol next = look.getSymbol();
  if (next == Symbol::CONST) {
    constDef(stop);
  } else if (First.at(NT::VAR_DEF).count(next)) {
    varDef(stop);
  } else if (First.at(NT::PROC_DEF).count(next)) {
    procDef(stop);
  } // else error checks
}


void Parser::constDef(std::set<Symbol> stop) {
   std::cout << "constDef" << std::endl;

   match(Symbol::CONST, stop);
   match(Symbol::ID, stop);
   match(Symbol::EQUAL, stop);
   constant(stop);
}


void Parser::varDef(std::set<Symbol> stop) {
  std::cout << "varDef" << std::endl;

  typeSym(munion({stop, First.at(NT::VPRIME)}));
  vPrime(stop);
}


void Parser::typeSym(std::set<Symbol> stop) {
  std::cout << "typeSym" << std::endl;

  Symbol next = look.getSymbol();
  if(next == Symbol::INT) {
    match(Symbol::INT, stop);
  } else {
    match(Symbol::BOOL, stop);
  }
}


void Parser::vPrime(std::set<Symbol> stop) {
  std::cout << "vPrime" << std::endl;

  if (look.getSymbol() == Symbol::ID) {
    varList(stop);
  } else {
      match(Symbol::ARRAY, munion({stop, First.at(NT::VAR_LIST), {Symbol::LHSQR}, First.at(NT::CONST_NT), {Symbol::RHSQR}}));
      varList(munion({stop, {Symbol::LHSQR}, First.at(NT::CONST_NT), {Symbol::RHSQR}}));
      match(Symbol::LHSQR, munion({stop, First.at(NT::CONST_NT), {Symbol::RHSQR}}));
      constant(munion({stop, {Symbol::RHSQR}}));
      match(Symbol::RHSQR, stop);
  }
}

void Parser::varList(std::set<Symbol> stop) {
  std::cout << "varList" << std::endl;

  match(Symbol::ID, stop);
  while(look.getSymbol() == Symbol::COMMA) {
      match(Symbol::COMMA, stop);
      match(Symbol::ID, stop);
  }
}


void Parser::procDef(std::set<Symbol> stop){
  std::cout << "procDef" << std::endl;

  match(Symbol::PROC, stop);
  match(Symbol::ID, stop);
  block(stop);
}


// Will need to be adjusted when we add proper grammar
void Parser::exprList(std::set<Symbol> stop) {
  std::cout << "Expression List" << std::endl;

  expr(stop);

  while(look.getSymbol() == Symbol::COMMA) {
    match(Symbol::COMMA, stop);
    expr(stop);
  }
}

void Parser::assignStmt(std::set<Symbol> stop) {
  std::cout << "assignStmt" << std::endl;

  vacsList(stop);
  match(Symbol::INIT, stop);
  exprList(stop);
}

void Parser::procStmt(std::set<Symbol> stop) {
  std::cout << "procStmt" << std::endl;

  match(Symbol::CALL, stop);
  match(Symbol::ID, stop);
}

void Parser::ifStmt(std::set<Symbol> stop) {
  std::cout << "ifStmt" << std::endl;

  match(Symbol::IF, stop);
  guardedList(stop);
  match(Symbol::FI, stop);
}

void Parser::doStmt(std::set<Symbol> stop) {
  std::cout << "doStmt" << std::endl;

  match(Symbol::DO, stop);
  guardedList(stop);
  match(Symbol::OD, stop);
}

void Parser::guardedList(std::set<Symbol> stop) {
  std::cout << "guardedList" << std::endl;

  guardedComm(munion({stop, {Symbol::GUARD}, First.at(NT::GRCOM)}));
  while (look.getSymbol() == Symbol::GUARD) {
    match(Symbol::GUARD, munion({stop, {Symbol::GUARD}, First.at(NT::GRCOM)}));
    guardedComm(munion({stop, {Symbol::GUARD}, First.at(NT::GRCOM)}));
  }
}

void Parser::guardedComm(std::set<Symbol> stop) {
  std::cout << "guardedComm" << std::endl;

  expr(munion({stop, {Symbol::ARROW}, First.at(NT::STMT_PART)}));
  match(Symbol::ARROW, munion({stop, First.at(NT::STMT_PART)}));
  stmtPart(stop);
}


void Parser::expr(std::set<Symbol> stop) {
  std::cout << "expr" << std::endl;

  primeExpr(stop);

  while (look.getSymbol() == Symbol::AMP or
      look.getSymbol() == Symbol::BAR) {
    primeOp(stop);
    primeExpr(stop);
  }
}


void Parser::primeOp(std::set<Symbol> stop) {
  std::cout << "prime-op" << std::endl;

  if(look.getSymbol() == Symbol::AMP) {
    match(Symbol::AMP, stop);
  } else if (look.getSymbol() == Symbol::BAR){
    match(Symbol::BAR, stop);
  } else {
    syntaxError(stop);
  }
  syntaxCheck(stop);
}


void Parser::primeExpr(std::set<Symbol> stop) {
  std::cout << "prime-Expr" << std::endl;

  simpleExpr(stop);

  if(look.getSymbol() == Symbol::LESS or
      look.getSymbol() == Symbol::EQUAL or
      look.getSymbol() == Symbol::GREAT) {
        relOp(stop);
        simpleExpr(stop);
      }
}


void Parser::relOp(std::set<Symbol> stop) {
  std::cout << "rel-Op" << std::endl;

  if(look.getSymbol() == Symbol::LESS) {
    match(Symbol::LESS, stop);
  } else if (look.getSymbol() == Symbol::EQUAL) {
    match(Symbol::EQUAL, stop);
  } else if (look.getSymbol() == Symbol::GREAT) {
    match(Symbol::GREAT, stop);
  } else {
    syntaxError(stop);
  }
  syntaxCheck(stop);
}


void Parser::simpleExpr(std::set<Symbol> stop) {
  std::cout << "simpleExpr" << std::endl;
  if (look.getSymbol() == Symbol::MINUS)
    match(Symbol::MINUS, stop);

  term(stop);

  while (look.getSymbol() == Symbol::PLUS
      or look.getSymbol() == Symbol::MINUS) {
    addOp(stop);
    term(stop);
  }
}


void Parser::term(std::set<Symbol> stop) {
  std::cout << "Term" << std::endl;

  factor(munion({stop, First.at(NT::MULT_OP), First.at(NT::FACTOR)}));

  while (First.at(NT::MULT_OP).count(look.getSymbol())) {
    multOp(munion({stop, First.at(NT::MULT_OP), First.at(NT::FACTOR)}));
    factor(munion({stop, First.at(NT::MULT_OP), First.at(NT::FACTOR)}));
  }
}


void Parser::factor(std::set<Symbol> stop) {
  std::cout << "Factor" << std::endl;

  bool err = false;
  if(look.getSymbol() == Symbol::NUM) {
    match(Symbol::NUM, stop);
  } else if (look.getSymbol() == Symbol::TRUE
      or look.getSymbol() == Symbol::FALSE) {
    boolSym(stop);
  } else if (look.getSymbol() == Symbol::LHRND) {
    match(Symbol::LHRND, stop);
    expr(stop);
    match(Symbol::RHRND, stop);
  } else if (look.getSymbol() == Symbol::TILD) {
    match(Symbol::TILD, stop);
    factor(stop);
  } else if (First.at(NT::VACS).count(look.getSymbol())) {
    varAccess(stop);
  } else {
    err = true;
  }

  std::set<Symbol> allFirst = munion({{Symbol::NUM}, First.at(NT::BOOL_SYM),
      {Symbol::LHRND}, {Symbol::TILD}, First.at(NT::VACS)});

  // Check if it is possible to have epsilon
  if (!allFirst.count(Symbol::EPSILON) and err)
    syntaxError(stop);
  else
    syntaxCheck(stop);
}


void Parser::addOp(std::set<Symbol> stop) {
  std::cout << "addOp" << std::endl;

  if (look.getSymbol() == Symbol::PLUS)
    match(Symbol::PLUS, stop);
  else if (look.getSymbol() == Symbol::MINUS)
    match(Symbol::MINUS, stop);
  else
    syntaxError(stop);  // epsilon is not possible
  syntaxCheck(stop);
}


void Parser::multOp(std::set<Symbol> stop) {
  std::cout << "multOp" << std::endl;

  if (look.getSymbol() == Symbol::TIMES)
    match(Symbol::TIMES, stop);
  else if (look.getSymbol() == Symbol::FSLASH)
    match(Symbol::FSLASH, stop);
  else if (look.getSymbol() == Symbol::BSLASH)
    match(Symbol::BSLASH, stop);
  else
    syntaxError(stop);  // epsilon is not possible
  syntaxCheck(stop);
}


void Parser::varAccess(std::set<Symbol> stop) {
  std::cout << "varAccess" << std::endl;

  match(Symbol::ID, munion({stop, First.at(NT::IDX_SEL)}));
  // syntax check on next symbol is run in match
  if (look.getSymbol() == Symbol::LHSQR)
    idxSelect(stop);
}


void Parser::idxSelect(std::set<Symbol> stop) {
  std::cout << "idxSelect" << std::endl;

  match(Symbol::LHSQR, munion({stop, First.at(NT::EXP), {Symbol::RHSQR}}));
  expr(munion({stop, {Symbol::RHSQR}}));
  match(Symbol::RHSQR, stop);
}


void Parser::constant(std::set<Symbol> stop) {
  std::cout << "constant" << std::endl;

  if (look.getSymbol() == Symbol::NUM)
    match(Symbol::NUM, stop);
  else if (First.at(NT::BOOL_SYM).count(look.getSymbol()))
    boolSym(stop);
  else if (look.getSymbol() == Symbol::ID)
    match(Symbol::ID, stop);
  else {
    syntaxError(stop);  // epsilon is guaranteed not in any of these
  }
  syntaxCheck(stop);
}


void Parser::boolSym(std::set<Symbol> stop) {
  std::cout << "boolSym" << std::endl;

  if (look.getSymbol() == Symbol::TRUE) {
    match(Symbol::TRUE, stop);
  } else if (look.getSymbol() == Symbol::FALSE) {
    match(Symbol::FALSE, stop);
  } else {
    // epsilon is clearly not in the first of true and false
    syntaxError(stop); 
  }
  syntaxCheck(stop);
}
