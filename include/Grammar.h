#ifndef Grammar_H
#define Grammar_H

#include <Symbol.h>
#include <map>
#include <set>

/**
 * Enum to represent all non terminals that are possible in our language.
 */
enum NT {
  NAME = 512, BOOL_SYM, NUM_NT, CONST_NT, IDX_SEL, VACS, FACTOR, MULT_OP, TERM,
  ADD_OP, SIMP_EXP, REL_OP, PRIM_EXP, PRIM_OP, EXP, GRCOM, GRCOM_LIST,
  DO_STMT, IF_STMT, PROC_STMT, VACS_LIST, ASC_STMT, EXP_LIST, WRITE_STMT,
  READ_STMT, EMPTY_STMT, STMT, STMT_PART, PROC_DEF, VAR_LIST, TYPE_SYM, CONST_DEF,
  DEF, VAR_DEF, DEF_PART, BLOCK, PROGRAM, VPRIME
};

/**
 * Map from non terminals to thier first sets of symbols.
 */
const std::map<NT, std::set<Symbol>> First{
  {NAME, {Symbol::ID}},
  {BOOL_SYM, {Symbol::FALSE, Symbol::TRUE}},
  {NUM_NT, {Symbol::NUM}},
  {CONST_NT, {Symbol::NUM, Symbol::FALSE, Symbol::TRUE, Symbol::ID}},
  {IDX_SEL, {Symbol::LHSQR}},
  {VACS, {Symbol::ID}},
  {FACTOR, {Symbol::ID, Symbol::FALSE, Symbol::TRUE, Symbol::NUM, Symbol::LHRND, Symbol::TILD}},
  {MULT_OP, {Symbol::TIMES, Symbol::FSLASH, Symbol::BSLASH}},
  {TERM, {Symbol::NUM, Symbol::FALSE, Symbol::TRUE, Symbol::ID, Symbol::LHRND, Symbol::TILD}},
  {ADD_OP, {Symbol::PLUS, Symbol::MINUS}},
  {SIMP_EXP, {Symbol::NUM, Symbol::ID, Symbol::FALSE, Symbol::TRUE, Symbol::MINUS, Symbol::LHRND, Symbol::TILD}},
  {REL_OP, {Symbol::LESS, Symbol::EQUAL, Symbol::GREAT}},
  {PRIM_EXP, {Symbol::NUM, Symbol::ID, Symbol::FALSE, Symbol::TRUE, Symbol::MINUS, Symbol::LHRND, Symbol::TILD}},
  {PRIM_OP, {Symbol::AMP, Symbol::BAR}},
  {EXP, {Symbol::NUM, Symbol::ID, Symbol::FALSE, Symbol::TRUE, Symbol::MINUS, Symbol::LHRND, Symbol::TILD}},
  {GRCOM, {Symbol::NUM, Symbol::ID, Symbol::FALSE, Symbol::TRUE, Symbol::MINUS, Symbol::LHRND, Symbol::TILD}},
  {GRCOM_LIST, {Symbol::NUM, Symbol::ID, Symbol::FALSE, Symbol::TRUE, Symbol::MINUS, Symbol::LHRND, Symbol::TILD}},
  {DO_STMT, {Symbol::DO}},
  {IF_STMT, {Symbol::IF}},	
  {PROC_STMT, {Symbol::CALL}},
  {VACS_LIST, {Symbol::ID}},
  {ASC_STMT, {Symbol::ID}},
  {EXP_LIST, {Symbol::NUM, Symbol::ID, Symbol::TRUE, Symbol::FALSE, Symbol::EQUAL, Symbol::LHRND, Symbol::TILD}},
  {WRITE_STMT, {Symbol::WRITE}},
  {READ_STMT, {Symbol::READ}},
  {EMPTY_STMT, {Symbol::SKIP}},
  {STMT, {Symbol::SKIP, Symbol::READ, Symbol::WRITE, Symbol::CALL, Symbol::IF, Symbol::DO, Symbol::ID}},
  {STMT_PART, {Symbol::EPSILON, Symbol::SKIP, Symbol::READ, Symbol::WRITE, Symbol::CALL, Symbol::IF, Symbol::DO, Symbol::ID}},
  {PROC_DEF, {Symbol::PROC}},
  {VAR_LIST, {Symbol::ID}},
  {TYPE_SYM, {Symbol::INT, Symbol::BOOL}},
  {CONST_DEF, {Symbol::CONST}},
  {DEF, {Symbol::CONST, Symbol::PROC, Symbol::INT, Symbol::BOOL}},
  {VAR_DEF, {Symbol::INT, Symbol::BOOL}},
  {DEF_PART, {Symbol::EPSILON, Symbol::CONST, Symbol::PROC, Symbol::INT, Symbol::BOOL}},
  {BLOCK, {Symbol::BEGIN}},
  {PROGRAM, {Symbol::BEGIN}},
  {VPRIME, {Symbol::ID, Symbol::ARRAY}},
}; 

std::set<Symbol> munion(std::vector<std::set<Symbol>> stopSets) {
  std::set<Symbol> newSet;
  for (auto& S : stopSets) {
    for (auto sym : S)
      newSet.insert(sym);
  }
  return newSet;
}

#endif

