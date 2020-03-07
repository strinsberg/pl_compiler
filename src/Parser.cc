#include <set>
#include "Parser.h"
#include "Administration.h"
#include "BlockTable.h"
#include "Symbol.h"
#include "Grammar.h"


Parser::Parser(Administration& a) : admin(a) {}


void Parser::parse() {
  look = admin.getToken();
  program( std::set<Symbol>{Symbol::ENDFILE} );
}


// Matching and errors ///////////////////////////////////////////////////////

void Parser::match(Symbol sym, std::set<Symbol> stop) {

  // If the symbol matched look ahead token then move to the next one
  // otherwise call a syntax error.
  if (sym == look.getSymbol()) {
    admin.debugInfo("Matched: " + look.toString() +"\n");
    look = admin.getToken();
  } else {
    syntaxError(stop);
  }
  syntaxCheck(stop);
}


void Parser::syntaxError(std::set<Symbol> stop) {
  admin.error("Syntax error near: '" + look.getLexeme() + "'");  // need more info using look.getSymbol()
  while (! stop.count(look.getSymbol()))
    look = admin.getToken();
}

void Parser::syntaxCheck(std::set<Symbol> stop) {
  if (! stop.count(look.getSymbol())){
    admin.debugInfo("Fail Syntax Check: '" + look.getLexeme() + "'");
    syntaxError(stop);
  }
}


// Recursive rule functions //////////////////////////////////////////////////

void Parser::program(std::set<Symbol> stop) {
  admin.debugInfo("program");

  blocks.pushBlock();
  block(munion({stop, {Symbol::DOT}}));
  blocks.popBlock();
  match(Symbol::DOT, stop);
}


void Parser::block(std::set<Symbol> stop) {
  admin.debugInfo("block");

  blocks.pushBlock();

  match(Symbol::BEGIN, munion({stop, First.at(NT::DEF_PART), First.at(NT::STMT_PART), {Symbol::END}}));
  defPart(munion({stop, First.at(NT::STMT_PART), {Symbol::END}}));
  stmtPart(munion({stop, {Symbol::END}}));
  blocks.popBlock();
  match(Symbol::END, stop);
}


// Definition Rules //////////////////////////////////////////////////////////

void Parser::defPart(std::set<Symbol> stop) {
  admin.debugInfo("defPart");

  while (First.at(NT::DEF).count(look.getSymbol())) {
    def(munion({stop, {Symbol::SEMI}}));
    match(Symbol::SEMI, munion({stop, First.at(NT::DEF)}));
  }
}


void Parser::def(std::set<Symbol> stop) {
  admin.debugInfo("def");

  bool err = false;
  Symbol next = look.getSymbol();
  if (next == Symbol::CONST) {
    constDef(stop);
  } else if (First.at(NT::VAR_DEF).count(next)) {
    varDef(stop);
  } else if (First.at(NT::PROC_DEF).count(next)) {
    procDef(stop);
  }
  else
    err = true;

  std::set<Symbol> allFirst = munion({First.at(NT::CONST_DEF), First.at(NT::VAR_DEF), First.at(NT::PROC_DEF)});

  if(!allFirst.count(Symbol::EPSILON) && err)
    syntaxError(stop);
  else
    syntaxCheck(stop);

}


void Parser::constDef(std::set<Symbol> stop) {
   admin.debugInfo("constDef");

   match(Symbol::CONST, munion({stop, {Symbol::ID}, {Symbol::EQUAL}, First.at(NT::CONST_NT)}));
   int idx = look.getVal();
   match(Symbol::ID, munion({stop, {Symbol::EQUAL}, First.at(NT::CONST_NT)}));
   match(Symbol::EQUAL, munion({stop, First.at(NT::CONST_NT)}));
   Type type = constant(stop);

   // May need to deal with the idx if there is a syntax error and the
   // look token is not an ID token at all
   blocks.define(idx, Kind::CONSTANT, type, 0, 0);  // Set val properly later
}


void Parser::varDef(std::set<Symbol> stop) {
  admin.debugInfo("varDef");

  if (look.getSymbol() == Symbol::RECORD) {
    match(Symbol::RECORD,munion({stop, {Symbol::END},
          First.at(NT::FIELD_LIST), First.at(NT::VAR_LIST)}));
    varList(munion({stop, {Symbol::END}, First.at(NT::FIELD_LIST)}));
    fieldList(munion({stop, {Symbol::END}}));
    match(Symbol::END, stop);
  } else {
    typeSym(munion({stop, First.at(NT::VPRIME)}));
    vPrime(stop);
  }
  // Add type information to IDs in the symbol table
  // Type sym will have to return this info
}


void Parser::procDef(std::set<Symbol> stop){
  admin.debugInfo("procDef");

  match(Symbol::PROC, munion({stop, {Symbol::ID}, First.at(NT::PROC_BLOCK)}));
  match(Symbol::ID, munion({stop, First.at(NT::PROC_BLOCK)}));
  procBlock(stop);
  // Also add type info to the ID in symbol table
}


// Statement Rules ///////////////////////////////////////////////////////////

void Parser::stmtPart(std::set<Symbol> stop) {
  admin.debugInfo("stmtPart");

  while (First.at(NT::STMT).count(look.getSymbol())) {
    stmt(munion({stop, {Symbol::SEMI}}));
    match(Symbol::SEMI, munion({stop, First.at(NT::STMT)}));
  }
}


void Parser::stmt(std::set<Symbol> stop) {
  admin.debugInfo("stmt");
  bool err = false;
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
  else if (next == Symbol::DO)
    doStmt(stop);
  else
    err = true;

  std::set<Symbol> allFirst = munion({First.at(NT::EMPTY_STMT), First.at(NT::READ_STMT), First.at(NT::WRITE_STMT),
      First.at(NT::ASC_STMT), First.at(NT::PROC_STMT), First.at(NT::IF_STMT), First.at(NT::DO_STMT)});

  if(!allFirst.count(Symbol::EPSILON) && err)
    syntaxError(stop);
  else
    syntaxCheck(stop);
}


void Parser::emptyStmt(std::set<Symbol> stop) {
  admin.debugInfo("emptyStmt");

  match(Symbol::SKIP, stop);
}


void Parser::readStmt(std::set<Symbol> stop) {
  admin.debugInfo("readStmt");

  match(Symbol::READ, munion({stop, First.at(NT::VACS_LIST)}));
  vacsList(stop);
}


void Parser::writeStmt(std::set<Symbol> stop) {
  admin.debugInfo("writeStmt");

  match(Symbol::WRITE, munion({stop, First.at(NT::EXP_LIST)}));
  exprList(stop);
}


void Parser::assignStmt(std::set<Symbol> stop) {
  admin.debugInfo("assignStmt");

  std::vector<TableEntry> vars = vacsList(munion({stop, First.at(NT::EXP_LIST), {Symbol::INIT}}));
  match(Symbol::INIT, munion({stop, First.at(NT::EXP_LIST)}));
  std::vector<TableEntry> exprs = exprList(stop);

  if (vars.size() != exprs.size()) {
    admin.error("Number of variables does not match number of expressions");
  } else {
    for (size_t i = 0; i < vars.size(); i++) {
      if (vars[i].ttype != exprs[i].ttype)
        admin.error("Type mismatch in assignment position " + std::to_string(i+1));
    }
  }
}


void Parser::procStmt(std::set<Symbol> stop) {
  admin.debugInfo("procStmt");

  match(Symbol::CALL, munion({stop, {Symbol::ID, Symbol::LHRND, Symbol::RHRND},
        First.at(NT::ACT_PLIST)}));
  match(Symbol::ID, munion({stop, {Symbol::LHRND, Symbol::RHRND},
        First.at(NT::ACT_PLIST)}));

  if(look.getSymbol() == Symbol::LHRND) {
    match(Symbol::LHRND, munion({stop, {Symbol::RHRND}, First.at(NT::ACT_PLIST)}));
    actParamList(munion({stop, {Symbol::RHRND}}));
    match(Symbol::RHRND, stop);
  }
  // May need a new block in here for dealing with optional function params
  // Or some way of adding them to the procedure token so that they can be
  // accessed later?
}


void Parser::ifStmt(std::set<Symbol> stop) {
  admin.debugInfo("ifStmt");

  match(Symbol::IF, munion({stop, First.at(NT::GRCOM_LIST), {Symbol::FI}}));
  guardedList(munion({stop, {Symbol::FI}}));
  match(Symbol::FI, stop);
}


void Parser::doStmt(std::set<Symbol> stop) {
  admin.debugInfo("doStmt");

  match(Symbol::DO, munion({stop, First.at(NT::GRCOM_LIST), {Symbol::OD}}));
  guardedList(munion({stop, {Symbol::OD}}));
  match(Symbol::OD, stop);
}


// Variable Rules ////////////////////////////////////////////////////////////

std::vector<TableEntry> Parser::vacsList(std::set<Symbol> stop) {
  admin.debugInfo("vacsList");
  std::vector<TableEntry> types;

  varAccess(munion({stop, {Symbol::COMMA}}));
  while (look.getSymbol() == Symbol::COMMA) {
    match(Symbol::COMMA, munion({stop, First.at(NT::VACS_LIST)}));
    varAccess(munion({stop, {Symbol::COMMA}}));
  }
  // May need to return a list of token references so semantic checks and
  // assignments or access can be done.
  return types;
}


void Parser::vPrime(std::set<Symbol> stop) {
  admin.debugInfo("vPrime");

  if (look.getSymbol() == Symbol::ID) {
    varList(stop);
  } else {
      match(Symbol::ARRAY, munion({stop, First.at(NT::VAR_LIST), {Symbol::LHSQR}, First.at(NT::CONST_NT), {Symbol::RHSQR}}));
      varList(munion({stop, {Symbol::LHSQR}, First.at(NT::CONST_NT), {Symbol::RHSQR}}));
      match(Symbol::LHSQR, munion({stop, First.at(NT::CONST_NT), {Symbol::RHSQR}}));
      constant(munion({stop, {Symbol::RHSQR}}));
      match(Symbol::RHSQR, stop);
  }
  // May need to return some info
}


void Parser::varList(std::set<Symbol> stop) {
  admin.debugInfo("varList");

  match(Symbol::ID, munion({stop, {Symbol::COMMA}}));
  while(look.getSymbol() == Symbol::COMMA) {
      match(Symbol::COMMA, munion({stop, {Symbol::ID}}));
      match(Symbol::ID, munion({stop, {Symbol::COMMA}}));
  }
  // May need to return a list of token references for the variables
}


Type Parser::varAccess(std::set<Symbol> stop) {
  admin.debugInfo("varAccess");

  std::string name = look.getLexeme();
  match(Symbol::ID, munion({stop, First.at(NT::SELECT)}));

  bool err;
  TableEntry entry = blocks.find(look.getVal(), err);  
  if (err)
    admin.error(name + " is undeclared");

  if (First.at(NT::SELECT).count(look.getSymbol())) {
    return selec(stop, entry);
  } else {
    return entry.ttype;
  }
}


Type Parser::idxSelect(std::set<Symbol> stop, TableEntry entry) {
  admin.debugInfo("idxSelect");

  match(Symbol::LHSQR, munion({stop, First.at(NT::EXP), {Symbol::RHSQR}}));
  Type type = expr(munion({stop, {Symbol::RHSQR}}));
  if (type != Type::INTEGER)
    admin.error("Index must be an integer");

  // need to bounds check once expressions return proper values
  match(Symbol::RHSQR, stop);
  return entry.ttype;
}


// Expression Rules //////////////////////////////////////////////////////////

std::vector<TableEntry> Parser::exprList(std::set<Symbol> stop) {
  admin.debugInfo("Expression List");
  std::vector<TableEntry> types;

  expr(munion({stop, First.at(NT::EXP), {Symbol::COMMA}}));

  while(look.getSymbol() == Symbol::COMMA) {
    match(Symbol::COMMA, munion({stop, First.at(NT::EXP)}));
    expr(munion({stop, {Symbol::COMMA}}));
  }

  return types;
}


Type Parser::expr(std::set<Symbol> stop) {
  admin.debugInfo("expr");

  Type type = primeExpr(munion({stop, First.at(NT::PRIM_OP), First.at(NT::PRIM_EXP)}));

  while (look.getSymbol() == Symbol::AMP or
      look.getSymbol() == Symbol::BAR) {
    primeOp(munion({stop, First.at(NT::PRIM_EXP)}));
    Type type2 = primeExpr(munion({stop, First.at(NT::PRIM_OP)}));
    if(type2 != Type::BOOLEAN) {
      type = Type::UNIVERSAL;
      admin.error("Cannot use & or | with non Boolean types");
    }
  }

  return type;

  // May need to return token info so the results of the expression can be used
}


Type Parser::primeExpr(std::set<Symbol> stop) {
  admin.debugInfo("prime-Expr");

  Type type = simpleExpr(munion({stop, First.at(NT::REL_OP), First.at(NT::SIMP_EXP)}));

  if(First.at(NT::REL_OP).count(look.getSymbol())) {
    relOp(munion({stop, First.at(NT::SIMP_EXP)}));
    Type type2 = simpleExpr(munion({stop, First.at(NT::REL_OP)}));
    if(type != type2) {
      type = Type::UNIVERSAL;
      admin.error("Type Mismatch");
    }
  }

  return type;
  // May need to return token info so the results of the expression can be used
}


Type Parser::simpleExpr(std::set<Symbol> stop) {
  admin.debugInfo("simpleExpr");

  syntaxCheck(munion({stop, {Symbol::MINUS}}));
  if (look.getSymbol() == Symbol::MINUS)
    match(Symbol::MINUS, munion({stop, First.at(NT::TERM)}));

  Type type = term(munion({stop, First.at(NT::ADD_OP)}));

  while (look.getSymbol() == Symbol::PLUS
      or look.getSymbol() == Symbol::MINUS) {
    addOp(munion({stop, First.at(NT::TERM)}));
    syntaxCheck(munion({stop, {Symbol::MINUS}}));

    if (look.getSymbol() == Symbol::MINUS)
      match(Symbol::MINUS, munion({stop, First.at(NT::TERM)}));

    Type type2 = term(munion({stop, First.at(NT::ADD_OP)}));
    if(type != type2) {
      type = Type::UNIVERSAL;
      admin.error("Type Mismatch");
    }
  }
  return type;
  // May need to return token info so the results of the expression can be used
}


// Guarded Command Rules /////////////////////////////////////////////////////

void Parser::guardedList(std::set<Symbol> stop) {
  admin.debugInfo("guardedList");

  guardedComm(munion({stop, {Symbol::GUARD}, First.at(NT::GRCOM)}));
  while (look.getSymbol() == Symbol::GUARD) {
    match(Symbol::GUARD, munion({stop, First.at(NT::GRCOM)}));
    guardedComm(munion({stop, {Symbol::GUARD}}));
  }
}

void Parser::guardedComm(std::set<Symbol> stop) {
  admin.debugInfo("guardedComm");

  Type type = expr(munion({stop, {Symbol::ARROW}, First.at(NT::STMT_PART)}));
  match(Symbol::ARROW, munion({stop, First.at(NT::STMT_PART)}));
  stmtPart(stop);

  if(type != Type::BOOLEAN) {
    type = Type::UNIVERSAL;
    admin.error("Expression before \"->\" must be of the type BOOLEAN");
  }
}


// Term and Factor Rules /////////////////////////////////////////////////////

Type Parser::term(std::set<Symbol> stop) {
  admin.debugInfo("Term");

  Type type = factor(munion({stop, First.at(NT::MULT_OP)}));

  while (First.at(NT::MULT_OP).count(look.getSymbol())) {
    multOp(munion({stop, First.at(NT::FACTOR)}));
    Type type2 = factor(munion({stop, First.at(NT::MULT_OP)}));
    if(type != type2) {
      type = Type::UNIVERSAL;
      admin.error("Type Mismatch");
    }
  }
  // May need to return token info so the results of the expression can be used
  return type;
}


Type Parser::factor(std::set<Symbol> stop) {
  admin.debugInfo("Factor");
  Type type = Type::UNIVERSAL;

  bool err = false;
  if(look.getSymbol() == Symbol::NUM) {
    type = constant(stop);
  } else if (look.getSymbol() == Symbol::TRUE
      or look.getSymbol() == Symbol::FALSE) {
    boolSym(stop);
    type  = Type::BOOLEAN;
  } else if (look.getSymbol() == Symbol::LHRND) {
    match(Symbol::LHRND, munion({stop, First.at(NT::EXP), {Symbol::RHRND}}));
    type = expr(munion({stop, {Symbol::RHRND}}));
    match(Symbol::RHRND, stop);
  } else if (look.getSymbol() == Symbol::TILD) {
    match(Symbol::TILD, munion({stop, First.at(NT::FACTOR)}));
    type = factor(stop);
  } else if (First.at(NT::VACS).count(look.getSymbol())) {
    type = varAccess(stop);
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
  // May need to return token info so the results of the expression can be used
  return type;
}


// Operator Rules ////////////////////////////////////////////////////////////

void Parser::primeOp(std::set<Symbol> stop) {
  admin.debugInfo("prime-op");

  if(look.getSymbol() == Symbol::AMP) {
    match(Symbol::AMP, stop);
  } else if (look.getSymbol() == Symbol::BAR){
    match(Symbol::BAR, stop);
  } else {
    syntaxError(stop);
  }
  syntaxCheck(stop);
}


void Parser::relOp(std::set<Symbol> stop) {
  admin.debugInfo("rel-Op");

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


void Parser::addOp(std::set<Symbol> stop) {
  admin.debugInfo("addOp");

  if (look.getSymbol() == Symbol::PLUS)
    match(Symbol::PLUS, stop);
  else if (look.getSymbol() == Symbol::MINUS)
    match(Symbol::MINUS, stop);
  else
    syntaxError(stop);  // epsilon is not possible
  syntaxCheck(stop);
}


void Parser::multOp(std::set<Symbol> stop) {
  admin.debugInfo("multOp");

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


// Symbol Rules //////////////////////////////////////////////////////////////

Type Parser::constant(std::set<Symbol> stop) {
  admin.debugInfo("constant");
  Type type = Type::UNIVERSAL;

  if (look.getSymbol() == Symbol::NUM) {
    match(Symbol::NUM, munion({stop, First.at(NT::CPRIME)}));
    type = cPrime(stop);
  } else if (First.at(NT::BOOL_SYM).count(look.getSymbol())) {
    boolSym(stop);
    type = Type::BOOLEAN;
  } else if (look.getSymbol() == Symbol::ID) {
    bool err = false;
    TableEntry ent = blocks.find(look.getVal(), err);
    if (err) {
      admin.error(look.getLexeme() + " not declared");
    } else {
      type = ent.ttype;
    }
    match(Symbol::ID, stop);
  } else {
    syntaxError(stop);  // epsilon is guaranteed not in any of these
  }
  syntaxCheck(stop);
  return type;
}


Type Parser::cPrime(std::set<Symbol> stop) {
  admin.debugInfo("cPrime");
  Type type = Type::INTEGER;

  if(look.getSymbol() == Symbol::DOT){
    match(Symbol::DOT, munion({stop, {Symbol::NUM}}));
    match(Symbol::NUM, stop);
    type = Type::T_FLOAT;
  }
  syntaxCheck(stop);
  return type;
}


Type Parser::typeSym(std::set<Symbol> stop) {
  admin.debugInfo("typeSym");
  Type type = Type::UNIVERSAL;

  Symbol next = look.getSymbol();
  if(next == Symbol::INT) {
    match(Symbol::INT, stop);
    type = Type::INTEGER;
  } else if (next == Symbol::BOOL){
    match(Symbol::BOOL, stop);
    type = Type::BOOLEAN;
  } else if (next == Symbol::FLOAT) {
    match(Symbol::FLOAT, stop);
    type = Type::T_FLOAT;
  } else {
    syntaxError(stop);
  }
  syntaxCheck(stop);
  return type;
}


void Parser::boolSym(std::set<Symbol> stop) {
  admin.debugInfo("boolSym");

  if (look.getSymbol() == Symbol::TRUE) {
    match(Symbol::TRUE, stop);
  } else if (look.getSymbol() == Symbol::FALSE) {
    match(Symbol::FALSE, stop);
  } else {
    // epsilon is clearly not in the first of true and false
    syntaxError(stop);
  }
  syntaxCheck(stop);
  // May need to return token with type and value
}


// Records Rules ///////////////////////////////////////////////////////////////
void Parser::fieldList (std::set<Symbol> stop) {
  admin.debugInfo("fieldList");

  recordSection(munion({stop, {Symbol::SEMI},  First.at(NT::REC_SEC)}));
  while(look.getSymbol() == Symbol::SEMI){
    match(Symbol::SEMI, munion({stop, First.at(NT::REC_SEC)}));
    recordSection(munion({stop, {Symbol::SEMI}}));
  }
  // need to deal with fields here by adding them to the appropriate record
  // named token. This may mean returning a map with these in it so that they
  // can be added to the record token
}

void Parser::recordSection(std::set<Symbol> stop) {
  admin.debugInfo("recordSection");

  typeSym(munion({stop, {Symbol::ID, Symbol::COMMA}}));
  match(Symbol::ID, munion({stop, {Symbol::ID, Symbol::COMMA}}));
  while(look.getSymbol() == Symbol::COMMA) {
    match(Symbol::COMMA, munion({stop, {Symbol::ID}}));
    match(Symbol::ID, munion({stop, {Symbol::COMMA}}));
  }
  // May need to return a list of ids and types
}

// Parameter Rules /////////////////////////////////////////////////////////////

// Somewhere in these functions we will need to deal with the parameters and
// adding them to a symtab or to a proc token as fields

void Parser::procBlock(std::set<Symbol> stop) {
  admin.debugInfo("procBlock");

  if(look.getSymbol() == Symbol::LHRND){
    match(Symbol::LHRND, munion({stop, First.at(NT::FORM_PLIST),
      {Symbol::RHRND}, First.at(NT::BLOCK)}));
    formParamList(munion({stop, {Symbol::RHRND}, First.at(NT::BLOCK)}));
    match(Symbol::RHRND, munion({stop, First.at(NT::BLOCK)}));
  }
  block(stop);
}

void Parser::formParamList(std::set<Symbol> stop) {
  admin.debugInfo("formParamList");

  paramDef(munion({stop, {Symbol::SEMI},  First.at(NT::PARAM_DEF)}));
  while(look.getSymbol() == Symbol::SEMI){
    match(Symbol::SEMI, munion({stop, First.at(NT::PARAM_DEF)}));
    paramDef(munion({stop, {Symbol::SEMI}}));
  }
}

void Parser::paramDef(std::set<Symbol> stop) {
  admin.debugInfo("paramDef");

  if(look.getSymbol() == Symbol::VAR){
    match(Symbol::VAR, munion({stop, First.at(NT::TYPE_SYM),
    First.at(NT::VAR_LIST)}));
  }

  typeSym(munion({stop, First.at(NT::VAR_LIST)}));
  varList(stop);
}

void Parser::actParamList(std::set<Symbol> stop) {
  admin.debugInfo("actParamList");

  actParam(munion({stop, {Symbol::COMMA}, First.at(ACT_PARAM)}));
  while(look.getSymbol() == Symbol::COMMA){
    match(Symbol::COMMA, munion({stop, First.at(NT::ACT_PARAM)}));
    actParam(munion({stop, {Symbol::COMMA}}));
  }
}

void Parser::actParam(std::set<Symbol> stop) {
  admin.debugInfo("actParam");

  if(First.at(NT::EXP_LIST).count(look.getSymbol()))
    exprList(stop);
  else if (First.at(NT::VACS_LIST).count(look.getSymbol()))
    varAccess(stop);
  else
    syntaxError(stop);
  syntaxCheck(stop);
}

// Selector Rules //////////////////////////////////////////////////////////////

void Parser::selec(std::set<Symbol> stop) {
  admin.debugInfo("selec");
  Type type = Type::UNIVERSAL;

  if(look.getSymbol() == Symbol::LHSQR) {
    TableEntry entry = blocks.find(look.getVal());
    type = idxSelect(stop, entry);
  } else if (look.getSymbol() == Symbol::DOT) {
    TableEntry entry = blocks.find(look.getVal());
    type = fieldSelec(stop, entry);
  } else {
    syntaxError(stop);
  }
  syntaxCheck(stop);
  return type;
}

Type Parser::fieldSelec(std::set<Symbol> stop, TableEntry entry) {
  admin.debugInfo("fieldSelec");

  match(Symbol::DOT, munion({stop, {Symbol::ID}}));
  match(Symbol::ID, stop);

  if (entry.tkind != Kind::K_RECORD) {
    admin.error("Not a record");
    return Type::UNIVERSAL;
  }

  bool err;
  TableEntry field = entry.fields.find(idx, err);
  if (err) {
    admin.error("Not a valid field");
    return Type::UNIVERSAL;
  } else {
    return field.ttype;
  }
}
