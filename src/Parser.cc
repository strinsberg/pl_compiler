#include <set>
#include "Parser.h"
#include "Administration.h"
#include "BlockTable.h"
#include "Symbol.h"
#include "Grammar.h"


Parser::Parser(Administration& a) : admin(a) {
  label = 0;
}


void Parser::parse() {
  look = admin.getToken();
  program( std::set<Symbol>{Symbol::ENDFILE} );
}

int Parser::NewLabel() {
  label++;
  return label;
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

  int start = NewLabel(), var = NewLabel();
  admin.emit("PROG", var, start);

  block(munion({stop, {Symbol::DOT}}), std::vector<TableEntry>(), start, var);
  match(Symbol::DOT, stop);
  admin.emit("ENDPROG");
}


void Parser::block(std::set<Symbol> stop, std::vector<TableEntry> entries,
                    int startlabel, int varlabel) {
  admin.debugInfo("block");

  blocks.pushBlock();
  for (auto & e : entries) {
    if (!blocks.define(e))
      admin.error("Multiple definitions of a variable");
  }

  match(Symbol::BEGIN, munion({stop, First.at(NT::DEF_PART), First.at(NT::STMT_PART), {Symbol::END}}));

  int length = defPart(munion({stop, First.at(NT::STMT_PART), {Symbol::END}}));
  admin.emit("DEFARG", varlabel, length);
  admin.emit("DEFADDR", startlabel);

  stmtPart(munion({stop, {Symbol::END}}));
  blocks.popBlock();
  match(Symbol::END, stop);
}


// Definition Rules //////////////////////////////////////////////////////////

int Parser::defPart(std::set<Symbol> stop) {
  admin.debugInfo("defPart");

  int length = 0, nextStart = 3;
  while (First.at(NT::DEF).count(look.getSymbol())) {
    length += def(munion({stop, {Symbol::SEMI}}), nextStart);
    match(Symbol::SEMI, munion({stop, First.at(NT::DEF)}));
  }
  return length;
}


int Parser::def(std::set<Symbol> stop, int& start) {
  admin.debugInfo("def");

  int size = 0;
  bool err = false;
  Symbol next = look.getSymbol();
  if (next == Symbol::CONST) {
    constDef(stop);
  } else if (First.at(NT::VAR_DEF).count(next)) {
    size = varDef(stop, start);
  } else if (First.at(NT::PROC_DEF).count(next)) {
    procDef(stop);
  } else {
    err = true;
  }

  std::set<Symbol> allFirst = munion({First.at(NT::CONST_DEF), First.at(NT::VAR_DEF), First.at(NT::PROC_DEF)});

  if(!allFirst.count(Symbol::EPSILON) && err)
    syntaxError(stop);
  else
    syntaxCheck(stop);
  return size;
}


void Parser::constDef(std::set<Symbol> stop) {
   admin.debugInfo("constDef");

   match(Symbol::CONST, munion({stop, {Symbol::ID}, {Symbol::EQUAL}, First.at(NT::CONST_NT)}));
   int idx = look.getVal();

   match(Symbol::ID, munion({stop, {Symbol::EQUAL}, First.at(NT::CONST_NT)}));
   match(Symbol::EQUAL, munion({stop, First.at(NT::CONST_NT)}));

   auto temp = constant(stop);
   blocks.define(idx, Kind::CONSTANT, temp.first, 0, temp.second, 0);
}


int Parser::varDef(std::set<Symbol> stop, int& start) {
  admin.debugInfo("varDef");

  if (look.getSymbol() == Symbol::RECORD) {  // Ignore for code gen for now
    match(Symbol::RECORD,munion({stop, {Symbol::END},
          First.at(NT::FIELD_LIST), First.at(NT::VAR_LIST)}));

    std::vector<int> idxs = varList(munion({stop, {Symbol::END}, First.at(NT::FIELD_LIST)}));
    std::vector<TableEntry> fields;
    fieldList(munion({stop, {Symbol::END}}), fields);

    for (auto i : idxs) {
      TableEntry record = TableEntry(i, Kind::K_RECORD, Type::UNIVERSAL, fields.size(), 0, 0);
      record.level = blocks.level();
      for (auto & f : fields) {
        record.entries.push_back(f);
      }
      blocks.define(record);
    }

    match(Symbol::END, stop);
    return 0;  // Would need to change for code gen
  } else {
    Type type = typeSym(munion({stop, First.at(NT::VPRIME)}));
    return vPrime(stop, type, start);
  }
}


void Parser::procDef(std::set<Symbol> stop){
  admin.debugInfo("procDef");

  int proclabel = NewLabel();
  int varlabel = NewLabel();
  int startlabel = NewLabel();
  admin.emit("DEFADDR", proclabel);
  admin.emit("PROC", varlabel, startlabel);

  match(Symbol::PROC, munion({stop, {Symbol::ID}, First.at(NT::PROC_BLOCK)}));
  int id  = look.getVal();
  match(Symbol::ID, munion({stop, First.at(NT::PROC_BLOCK)}));

  procBlock(stop, id, startlabel, varlabel, proclabel);
  admin.emit("ENDPROC");
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
  std::vector<Type> types = vacsList(stop);
  admin.emit("READ", types.size());
}


void Parser::writeStmt(std::set<Symbol> stop) {
  admin.debugInfo("writeStmt");

  match(Symbol::WRITE, munion({stop, First.at(NT::EXP_LIST)}));
  std::vector<Type> types = exprList(stop);
  admin.emit("WRITE", types.size());
}


void Parser::assignStmt(std::set<Symbol> stop) {
  admin.debugInfo("assignStmt");

  std::vector<Type> vars = vacsList(munion({stop, First.at(NT::EXP_LIST), {Symbol::INIT}}));
  match(Symbol::INIT, munion({stop, First.at(NT::EXP_LIST)}));
  std::vector<Type> exprs = exprList(stop);

  if (vars.size() != exprs.size()) {
    admin.error("Number of variables does not match number of expressions");
  } else {
    for (size_t i = 0; i < vars.size(); i++) {
      if (vars[i] != exprs[i])
        admin.error("Type mismatch in assignment position " + std::to_string(i+1));
    }
  }
  admin.emit("ASSIGN", vars.size());
}


void Parser::procStmt(std::set<Symbol> stop) {
  admin.debugInfo("procStmt");

  match(Symbol::CALL, munion({stop, {Symbol::ID, Symbol::LHRND, Symbol::RHRND},
        First.at(NT::ACT_PLIST)}));

  int id = look.getVal();
  match(Symbol::ID, munion({stop, {Symbol::LHRND, Symbol::RHRND},
        First.at(NT::ACT_PLIST)}));

  std::vector<Type> types;

  if(look.getSymbol() == Symbol::LHRND) {
    match(Symbol::LHRND, munion({stop, {Symbol::RHRND}, First.at(NT::ACT_PLIST)}));
    types = actParamList(munion({stop, {Symbol::RHRND}}));
    match(Symbol::RHRND, stop);
  }

  bool err;
  TableEntry proc = blocks.find(id, err);
  if (err) {
    admin.error("Procedure undeclared");
  } else if (proc.entries.size() != types.size()) {
    admin.error("Incorrect number of parameters");
  } else {
    for (size_t i = 0; i < types.size(); i++) {
      if (proc.entries[i].ttype != types[i])
        admin.error("Param types do not match. pos: " + std::to_string(i));
    }
  }
  
  admin.debugInfo(std::to_string(blocks.level()) + " " + std::to_string(proc.level));
  admin.emit("CALL", blocks.level() - proc.level, proc.startLabel);
}


void Parser::ifStmt(std::set<Symbol> stop) {
  admin.debugInfo("ifStmt");

  match(Symbol::IF, munion({stop, First.at(NT::GRCOM_LIST), {Symbol::FI}}));
  int startlabel = NewLabel();
  int donelabel = NewLabel();
  guardedList(munion({stop, {Symbol::FI}}), startlabel, donelabel);
  admin.emit("DEFADDR", startlabel);
  admin.emit("FI", admin.currentLine());
  admin.emit("DEFADDR", donelabel);
  match(Symbol::FI, stop);
}


void Parser::doStmt(std::set<Symbol> stop) {
  admin.debugInfo("doStmt");

  int start = NewLabel();
  int loop = NewLabel(); 
  admin.emit("DEFADDR", loop);

  match(Symbol::DO, munion({stop, First.at(NT::GRCOM_LIST), {Symbol::OD}}));
  guardedList(munion({stop, {Symbol::OD}}), start, loop);

  admin.emit("DEFADDR", start);
  match(Symbol::OD, stop);
}


// Variable Rules ////////////////////////////////////////////////////////////

std::vector<Type> Parser::vacsList(std::set<Symbol> stop) {
  admin.debugInfo("vacsList");
  std::vector<Type> types;

  bool temp;
  types.push_back(varAccess(munion({stop, {Symbol::COMMA}}), temp));
  while (look.getSymbol() == Symbol::COMMA) {
    match(Symbol::COMMA, munion({stop, First.at(NT::VACS_LIST)}));
    types.push_back(varAccess(munion({stop, {Symbol::COMMA}}), temp));
  }
  return types;
}


int Parser::vPrime(std::set<Symbol> stop, Type type, int& start) {
  admin.debugInfo("vPrime");
  std::vector<int> idxs;

  if (look.getSymbol() == Symbol::ID) {
    idxs = varList(stop);
    for (auto i : idxs) {
      if (!blocks.define(i, Kind::VARIABLE, type, 0, 0, start))
        admin.error("Multiple definitions of the same variable.");
      start++;
    }
    return idxs.size();
  } else {
      match(Symbol::ARRAY, munion({stop, First.at(NT::VAR_LIST), {Symbol::LHSQR}, First.at(NT::CONST_NT), {Symbol::RHSQR}}));
      idxs = varList(munion({stop, {Symbol::LHSQR}, First.at(NT::CONST_NT), {Symbol::RHSQR}}));
      match(Symbol::LHSQR, munion({stop, First.at(NT::CONST_NT), {Symbol::RHSQR}}));

      auto temp = constant(munion({stop, {Symbol::RHSQR}}));
      Type idx_type = temp.first;
      int size = temp.second;
      
      if (idx_type != Type::INTEGER) {
        admin.error("Array size must be an integer");
        size = 0;
      }

      for (auto i : idxs) {
        if(!blocks.define(i, Kind::K_ARRAY, type, size, 0, start))
          admin.error("Multiple definitions of the same variable.");
        start += size;
      }

      match(Symbol::RHSQR, stop);
      return idxs.size() * size;
  }
}


std::vector<int> Parser::varList(std::set<Symbol> stop) {
  admin.debugInfo("varList");

  std::vector<int> indices;
  int idx = look.getVal();
  indices.push_back(idx);
  match(Symbol::ID, munion({stop, {Symbol::COMMA}}));
  while(look.getSymbol() == Symbol::COMMA) {
      match(Symbol::COMMA, munion({stop, {Symbol::ID}}));
      idx = look.getVal();
      indices.push_back(idx);
      match(Symbol::ID, munion({stop, {Symbol::COMMA}}));
  }
  return indices;
}


Type Parser::varAccess(std::set<Symbol> stop, bool& isConst) {
  admin.debugInfo("varAccess");

  bool err;
  TableEntry entry = blocks.find(look.getVal(), err);
  std::string name = look.getLexeme();
  match(Symbol::ID, munion({stop, First.at(NT::SELECT)}));

  if (err)
    admin.error(name + " is undeclared");

  Type type; 
  if (First.at(NT::SELECT).count(look.getSymbol())) {
    type = selec(stop, entry);
  } else {
    type = entry.ttype;
  }

  if (entry.tkind == Kind::CONSTANT) {
    admin.emit("CONSTANT", entry.val);
    isConst = true;
  } else {
    admin.emit("VARIABLE", blocks.level() - entry.level, entry.displace);
    isConst = false;
  }
  return type;
}


Type Parser::idxSelect(std::set<Symbol> stop, TableEntry entry) {
  admin.debugInfo("idxSelect");

  match(Symbol::LHSQR, munion({stop, First.at(NT::EXP), {Symbol::RHSQR}}));
  Type type = expr(munion({stop, {Symbol::RHSQR}}));
  if (type != Type::INTEGER)
    admin.error("Index must be an integer");

  // need to bounds check once expressions return proper values
  match(Symbol::RHSQR, stop);
  admin.emit("INDEX", entry.size, admin.currentLine());
  return entry.ttype;
}


// Expression Rules //////////////////////////////////////////////////////////

std::vector<Type> Parser::exprList(std::set<Symbol> stop) {
  admin.debugInfo("Expression List");
  std::vector<Type> types;

  types.push_back(expr(munion({stop, First.at(NT::EXP), {Symbol::COMMA}})));

  while(look.getSymbol() == Symbol::COMMA) {
    match(Symbol::COMMA, munion({stop, First.at(NT::EXP)}));
    types.push_back(expr(munion({stop, {Symbol::COMMA}})));
  }

  return types;
}


Type Parser::expr(std::set<Symbol> stop) {
  admin.debugInfo("expr");

  Type type = primeExpr(munion({stop, First.at(NT::PRIM_OP), First.at(NT::PRIM_EXP)}));

  while (look.getSymbol() == Symbol::AMP or
      look.getSymbol() == Symbol::BAR) {
    std::string instr = primeOp(munion({stop, First.at(NT::PRIM_EXP)}));
    Type type2 = primeExpr(munion({stop, First.at(NT::PRIM_OP)}));
    admin.emit(instr);
    if(type2 != Type::BOOLEAN) {
      type = Type::UNIVERSAL;
      admin.error("Cannot use & or | with non Boolean types");
    }
  }

  return type;
}


Type Parser::primeExpr(std::set<Symbol> stop) {
  admin.debugInfo("prime-Expr");

  Type type = simpleExpr(munion({stop, First.at(NT::REL_OP), First.at(NT::SIMP_EXP)}));

  if (First.at(NT::REL_OP).count(look.getSymbol())) {
    std::string instr = relOp(munion({stop, First.at(NT::SIMP_EXP)}));
    Type type2 = simpleExpr(munion({stop, First.at(NT::REL_OP)}));
    admin.emit(instr);
    if(type != type2) {
      type = Type::UNIVERSAL;
      admin.error("Type Mismatch");
    } else {
      type = Type::BOOLEAN;
    }
  }

  return type;
}


Type Parser::simpleExpr(std::set<Symbol> stop) {
  admin.debugInfo("simpleExpr");

  syntaxCheck(munion({stop, {Symbol::MINUS}}));
  if (look.getSymbol() == Symbol::MINUS) {
    match(Symbol::MINUS, munion({stop, First.at(NT::TERM)}));
    admin.emit("MINUS");
  }

  Type type = term(munion({stop, First.at(NT::ADD_OP)}));

  while (look.getSymbol() == Symbol::PLUS
      or look.getSymbol() == Symbol::MINUS) {
    std::string instr = addOp(munion({stop, First.at(NT::TERM)}));
    syntaxCheck(munion({stop, {Symbol::MINUS}}));

    if (look.getSymbol() == Symbol::MINUS) {
      match(Symbol::MINUS, munion({stop, First.at(NT::TERM)}));
      admin.emit("MINUS");
    }

    Type type2 = term(munion({stop, First.at(NT::ADD_OP)}));
    admin.emit(instr);
    if(type != type2) {
      type = Type::UNIVERSAL;
      admin.error("Type Mismatch");
    }
  }
  return type;
}


// Guarded Command Rules /////////////////////////////////////////////////////

void Parser::guardedList(std::set<Symbol> stop, int& label, int next) {
  admin.debugInfo("guardedList");

  guardedComm(munion({stop, {Symbol::GUARD}, First.at(NT::GRCOM)}), label, next);
  while (look.getSymbol() == Symbol::GUARD) {
    match(Symbol::GUARD, munion({stop, First.at(NT::GRCOM)}));
    guardedComm(munion({stop, {Symbol::GUARD}}), label, next);
  }
}

void Parser::guardedComm(std::set<Symbol> stop, int& thislabel, int next) {
  admin.debugInfo("guardedComm");
  admin.emit("DEFADDR", thislabel);

  Type type = expr(munion({stop, {Symbol::ARROW}, First.at(NT::STMT_PART)}));
  thislabel = NewLabel();
  admin.emit("ARROW", thislabel);
  match(Symbol::ARROW, munion({stop, First.at(NT::STMT_PART)}));
  stmtPart(stop);
  admin.emit("BAR", next);

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
  return type;
}


Type Parser::factor(std::set<Symbol> stop) {
  admin.debugInfo("Factor");
  Type type = Type::UNIVERSAL;
  int value = -1;
  std::pair<Type,int> tmp;

  bool err = false;
  if(look.getSymbol() == Symbol::NUM) {
    tmp = constant(stop);
    value = tmp.second;
    type = tmp.first;
    admin.emit("CONSTANT", value);
  } else if (look.getSymbol() == Symbol::TRUE
      or look.getSymbol() == Symbol::FALSE) {
    value = boolSym(stop);
    type  = Type::BOOLEAN;
    admin.emit("CONSTANT", value);
  } else if (look.getSymbol() == Symbol::LHRND) {
    match(Symbol::LHRND, munion({stop, First.at(NT::EXP), {Symbol::RHRND}}));
    type = expr(munion({stop, {Symbol::RHRND}}));
    match(Symbol::RHRND, stop);
  } else if (look.getSymbol() == Symbol::TILD) {
    match(Symbol::TILD, munion({stop, First.at(NT::FACTOR)}));
    type = factor(stop);
    admin.emit("NOT");
  } else if (First.at(NT::VACS).count(look.getSymbol())) {
    bool isConst;
    type = varAccess(stop, isConst);
    if (!isConst)
      admin.emit("VALUE");
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

std::string Parser::primeOp(std::set<Symbol> stop) {
  admin.debugInfo("prime-op");
  std::string instr = "ERROR";

  if(look.getSymbol() == Symbol::AMP) {
    match(Symbol::AMP, stop);
    instr = "AND";
  } else if (look.getSymbol() == Symbol::BAR){
    match(Symbol::BAR, stop);
    instr = "OR";
  } else {
    syntaxError(stop);
  }
  syntaxCheck(stop);
  return instr;
}


std::string Parser::relOp(std::set<Symbol> stop) {
  admin.debugInfo("rel-Op");
  std::string instr = "ERROR";

  if(look.getSymbol() == Symbol::LESS) {
    match(Symbol::LESS, stop);
    instr = "LESS";
  } else if (look.getSymbol() == Symbol::EQUAL) {
    match(Symbol::EQUAL, stop);
    instr = "EQUAL";
  } else if (look.getSymbol() == Symbol::GREAT) {
    match(Symbol::GREAT, stop);
    instr = "GREATER";
  } else {
    syntaxError(stop);
  }
  syntaxCheck(stop);
  return instr;
}


std::string Parser::addOp(std::set<Symbol> stop) {
  admin.debugInfo("addOp");
  std::string instr = "ERROR";

  if (look.getSymbol() == Symbol::PLUS) {
    match(Symbol::PLUS, stop);
    instr = "ADD";
  } else if (look.getSymbol() == Symbol::MINUS) {
    match(Symbol::MINUS, stop);
    instr = "SUBTRACT";
  } else {
    syntaxError(stop);  // epsilon is not possible
  }
  syntaxCheck(stop);
  return instr;
}


std::string Parser::multOp(std::set<Symbol> stop) {
  admin.debugInfo("multOp");
  std::string instr = "ERROR";

  if (look.getSymbol() == Symbol::TIMES) {
    match(Symbol::TIMES, stop);
    instr = "MULTIPLY";
  } else if (look.getSymbol() == Symbol::FSLASH) {
    match(Symbol::FSLASH, stop);
    instr = "DIVIDE";
  } else if (look.getSymbol() == Symbol::BSLASH) {
    match(Symbol::BSLASH, stop);
    instr = "MODULO";
  } else {
    syntaxError(stop);  // epsilon is not possible
  }
  syntaxCheck(stop);
  return instr;
}


// Symbol Rules //////////////////////////////////////////////////////////////

std::pair<Type, int> Parser::constant(std::set<Symbol> stop) {
  admin.debugInfo("constant");
  Type type = Type::UNIVERSAL;
  int value = 0;

  if (look.getSymbol() == Symbol::NUM) {
    value = look.getVal();
    match(Symbol::NUM, munion({stop, First.at(NT::CPRIME)}));
    type = cPrime(stop);
  } else if (First.at(NT::BOOL_SYM).count(look.getSymbol())) {
    value = boolSym(stop);
    type = Type::BOOLEAN;
  } else if (look.getSymbol() == Symbol::ID) {
    bool err = false;
    TableEntry ent = blocks.find(look.getVal(), err);
    if (err) {
      admin.error(look.getLexeme() + " not declared");
    } else {
      if(ent.tkind == Kind::CONSTANT || ent.tkind == Kind::UNDEFINED){
        value  = ent.val;
        type = ent.ttype;
      } else {
        admin.error("Constant expected but not found");
        value = 0;
        type = Type::UNIVERSAL;
      }
    }
    match(Symbol::ID, stop);
  } else {
    syntaxError(stop);  // epsilon is guaranteed not in any of these
  }
  syntaxCheck(stop);
  std::pair<Type,int> tmp({type, value});
  return tmp;
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


int Parser::boolSym(std::set<Symbol> stop) {
  admin.debugInfo("boolSym");
  int value = -1;

  if (look.getSymbol() == Symbol::TRUE) {
    match(Symbol::TRUE, stop);
    value = 1;
  } else if (look.getSymbol() == Symbol::FALSE) {
    match(Symbol::FALSE, stop);
    value = 0;
  } else {
    // epsilon is clearly not in the first of true and false
    syntaxError(stop);
  }
  syntaxCheck(stop);
  return value;
  // May need to return token with type and value
}


// Records Rules ///////////////////////////////////////////////////////////////
void Parser::fieldList (std::set<Symbol> stop, std::vector<TableEntry>& fields) {
  admin.debugInfo("fieldList");

  recordSection(munion({stop, {Symbol::SEMI},  First.at(NT::REC_SEC)}), fields);

  while(look.getSymbol() == Symbol::SEMI){
    match(Symbol::SEMI, munion({stop, First.at(NT::REC_SEC)}));
    recordSection(munion({stop, {Symbol::SEMI}}), fields);
  }
}


void Parser::recordSection(std::set<Symbol> stop, std::vector<TableEntry>& fields) {
  admin.debugInfo("recordSection");

  Type type = typeSym(munion({stop, {Symbol::ID, Symbol::COMMA}}));
  int id = look.getVal();
  fields.emplace_back(id, Kind::VARIABLE, type, 0, 0, 0);
  match(Symbol::ID, munion({stop, {Symbol::ID, Symbol::COMMA}}));

  while(look.getSymbol() == Symbol::COMMA) {
    match(Symbol::COMMA, munion({stop, {Symbol::ID}}));
    id = look.getVal();
    fields.emplace_back(id, Kind::VARIABLE, type, 0, 0, 0);
    match(Symbol::ID, munion({stop, {Symbol::COMMA}}));
  }
}

// Parameter Rules /////////////////////////////////////////////////////////////

void Parser::procBlock(std::set<Symbol> stop, int id, int startlabel,
                        int varlabel, int procLabel) {
  admin.debugInfo("procBlock");

  std::vector<TableEntry> params;
  TableEntry procedure = TableEntry(id, Kind::PROCEDURE, Type::UNIVERSAL, 0, 0, 0);
  procedure.level = blocks.level();  // should be in the params list
  procedure.startLabel = procLabel;

  if(look.getSymbol() == Symbol::LHRND){
    match(Symbol::LHRND, munion({stop, First.at(NT::FORM_PLIST),
         {Symbol::RHRND}, First.at(NT::BLOCK)}));

    formParamList(munion({stop, {Symbol::RHRND}, First.at(NT::BLOCK)}), params);

    procedure.size = params.size();
    for (auto & p : params) {
      if (procedure.findEntry(p) == -1)
        procedure.entries.push_back(p);
      else
        admin.error("Multiple definitions of a parameter");
    }

    match(Symbol::RHRND, munion({stop, First.at(NT::BLOCK)}));
  }

  if (!blocks.define(procedure)) {
    admin.error("Redeclaration of Procedure");
  }

  block(stop, params, startlabel, varlabel);
}


void Parser::formParamList(std::set<Symbol> stop, std::vector<TableEntry>& params) {
  admin.debugInfo("formParamList");

  paramDef(munion({stop, {Symbol::SEMI},  First.at(NT::PARAM_DEF)}), params);
  while(look.getSymbol() == Symbol::SEMI){
    match(Symbol::SEMI, munion({stop, First.at(NT::PARAM_DEF)}));
    paramDef(munion({stop, {Symbol::SEMI}}), params);
  }
}


void Parser::paramDef(std::set<Symbol> stop, std::vector<TableEntry>& params) {
  admin.debugInfo("paramDef");
  Kind kind = Kind::CONSTANT;

  if(look.getSymbol() == Symbol::VAR){
    match(Symbol::VAR, munion({stop, First.at(NT::TYPE_SYM),
    First.at(NT::VAR_LIST)}));
    kind = Kind::VARIABLE;
  }

  Type type = typeSym(munion({stop, First.at(NT::VAR_LIST)}));
  std::vector<int> idxs = varList(stop);
  for (auto i : idxs) {
    params.emplace_back(i, kind, type, 0, 0, 0);
  }
}


std::vector<Type> Parser::actParamList(std::set<Symbol> stop) {
  admin.debugInfo("actParamList");

  std::vector<Type> types;
  Type t;

  t = actParam(munion({stop, {Symbol::COMMA}, First.at(ACT_PARAM)}));
  types.push_back(t);
  while(look.getSymbol() == Symbol::COMMA){
    match(Symbol::COMMA, munion({stop, First.at(NT::ACT_PARAM)}));
    t = actParam(munion({stop, {Symbol::COMMA}}));
    types.push_back(t);
  }

  return types;
}


Type Parser::actParam(std::set<Symbol> stop) {
  admin.debugInfo("actParam");

  Type type;

  if(First.at(NT::EXP_LIST).count(look.getSymbol())) {
    type = expr(stop);
  } else if (First.at(NT::VACS_LIST).count(look.getSymbol())) {
    bool temp;
    type = varAccess(stop, temp);
  } else {
    syntaxError(stop);
  }
  syntaxCheck(stop);

  return type;
}


// Selector Rules //////////////////////////////////////////////////////////////

Type Parser::selec(std::set<Symbol> stop, TableEntry entry) {
  admin.debugInfo("selec");
  Type type = Type::UNIVERSAL;

  if(look.getSymbol() == Symbol::LHSQR) {
    type = idxSelect(stop, entry);
  } else if (look.getSymbol() == Symbol::DOT) {
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
  int idx = look.getVal();
  match(Symbol::ID, stop);

  if (entry.tkind != Kind::K_RECORD) {
    admin.error("Not a record");
    return Type::UNIVERSAL;
  }

  int pos = entry.findEntry(idx);
  if (pos == -1) {
    admin.error("Not a valid field");
    return Type::UNIVERSAL;
  } else {
    return entry.entries[pos].ttype;
  }
}
