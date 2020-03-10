#ifndef Grammar_H
#define Grammar_H

#include <Symbol.h>
#include <map>
#include <set>

/**
 * Enum to represent all non terminals that are possible in our language.
 */
enum NT {
  NAME = 400, BOOL_SYM, NUM_NT, CONST_NT, IDX_SEL, VACS, FACTOR, MULT_OP, TERM,
  ADD_OP, SIMP_EXP, REL_OP, PRIM_EXP, PRIM_OP, EXP, GRCOM, GRCOM_LIST,
  DO_STMT, IF_STMT, PROC_STMT, VACS_LIST, ASC_STMT, EXP_LIST, WRITE_STMT,
  READ_STMT, EMPTY_STMT, STMT, STMT_PART, PROC_DEF, VAR_LIST, TYPE_SYM, CONST_DEF,
  DEF, VAR_DEF, DEF_PART, BLOCK, PROGRAM, VPRIME, FIELD_LIST, PROC_BLOCK,
  REC_SEC, FORM_PLIST, PARAM_DEF, ACT_PLIST, ACT_PARAM, SELECT, FIELD_SEL,
  CPRIME
};

/**
 * Map from non terminals to thier first sets of symbols.
 */
const std::map<NT, std::set<Symbol>> First{
  {NAME,       {Symbol::ID}},
  {BOOL_SYM,   {Symbol::FALSE, Symbol::TRUE}},
  {NUM_NT,     {Symbol::NUM}},
  {CPRIME,     {Symbol::DOT, Symbol::EPSILON}},
  {CONST_NT,   {Symbol::NUM, Symbol::FALSE, Symbol::TRUE, Symbol::ID}},
  {FIELD_SEL,   {Symbol::DOT}},
  {IDX_SEL,    {Symbol::LHSQR}},
  {SELECT,     {Symbol::LHSQR, Symbol::DOT}},
  {VACS,       {Symbol::ID}},
  {FACTOR,     {Symbol::ID, Symbol::FALSE, Symbol::TRUE, Symbol::NUM,
                Symbol::LHRND, Symbol::TILD}},
  {MULT_OP,    {Symbol::TIMES, Symbol::FSLASH, Symbol::BSLASH}},
  {TERM,       {Symbol::NUM, Symbol::FALSE, Symbol::TRUE, Symbol::ID,
                Symbol::LHRND, Symbol::TILD}},
  {ADD_OP,     {Symbol::PLUS, Symbol::MINUS}},
  {SIMP_EXP,   {Symbol::NUM, Symbol::ID, Symbol::FALSE, Symbol::TRUE,
                Symbol::MINUS, Symbol::LHRND, Symbol::TILD}},
  {REL_OP,     {Symbol::LESS, Symbol::EQUAL, Symbol::GREAT}},
  {PRIM_EXP,   {Symbol::NUM, Symbol::ID, Symbol::FALSE, Symbol::TRUE,
                Symbol::MINUS, Symbol::LHRND, Symbol::TILD}},
  {PRIM_OP,    {Symbol::AMP, Symbol::BAR}},
  {EXP,        {Symbol::NUM, Symbol::ID, Symbol::FALSE, Symbol::TRUE,
                Symbol::MINUS, Symbol::LHRND, Symbol::TILD}},
  {GRCOM,      {Symbol::NUM, Symbol::ID, Symbol::FALSE, Symbol::TRUE,
                Symbol::MINUS, Symbol::LHRND, Symbol::TILD}},
  {GRCOM_LIST, {Symbol::NUM, Symbol::ID, Symbol::FALSE, Symbol::TRUE,
                Symbol::MINUS, Symbol::LHRND, Symbol::TILD}},
  {DO_STMT,    {Symbol::DO}},
  {IF_STMT,    {Symbol::IF}},
  {ACT_PARAM,  {Symbol::NUM, Symbol::ID, Symbol::FALSE, Symbol::TRUE,
                Symbol::MINUS, Symbol::LHRND, Symbol::TILD}},
  {ACT_PLIST,  {Symbol::NUM, Symbol::ID, Symbol::FALSE, Symbol::TRUE,
                Symbol::MINUS, Symbol::LHRND, Symbol::TILD}},
  {PROC_STMT,  {Symbol::CALL}},
  {VACS_LIST,  {Symbol::ID}},
  {ASC_STMT,   {Symbol::ID}},
  {EXP_LIST,   {Symbol::MINUS, Symbol::NUM, Symbol::ID, Symbol::TRUE,
                Symbol::FALSE, Symbol::EQUAL, Symbol::LHRND, Symbol::TILD}},
  {WRITE_STMT, {Symbol::WRITE}},
  {READ_STMT,  {Symbol::READ}},
  {EMPTY_STMT, {Symbol::SKIP}},
  {STMT,       {Symbol::SKIP, Symbol::READ, Symbol::WRITE, Symbol::CALL,
                Symbol::IF, Symbol::DO, Symbol::ID}},
  {STMT_PART,  {Symbol::EPSILON, Symbol::SKIP, Symbol::READ, Symbol::WRITE,
                Symbol::CALL, Symbol::IF, Symbol::DO, Symbol::ID}},
  {PARAM_DEF,  {Symbol::VAR, Symbol::INT, Symbol::BOOL, Symbol::FLOAT}},
  {FORM_PLIST, {Symbol::VAR, Symbol::INT, Symbol::BOOL, Symbol::FLOAT}},
  {PROC_BLOCK, {Symbol::LHRND, Symbol::BEGIN}},
  {PROC_DEF,   {Symbol::PROC}},
  {VAR_LIST,   {Symbol::ID}},
  {TYPE_SYM,   {Symbol::INT, Symbol::BOOL, Symbol::FLOAT}},
  {REC_SEC,    {Symbol::INT, Symbol::BOOL, Symbol::FLOAT}},
  {FIELD_LIST, {Symbol::INT, Symbol::BOOL, Symbol::FLOAT}},
  {CONST_DEF,  {Symbol::CONST}},
  {DEF,        {Symbol::CONST, Symbol::PROC, Symbol::INT, Symbol::BOOL,
                Symbol::FLOAT, Symbol::RECORD}},
  {VAR_DEF,    {Symbol::INT, Symbol::BOOL, Symbol::FLOAT, Symbol::RECORD}},
  {DEF_PART,   {Symbol::EPSILON, Symbol::CONST, Symbol::PROC, Symbol::INT,
                Symbol::BOOL, Symbol::FLOAT, Symbol::RECORD}},
  {BLOCK,      {Symbol::BEGIN}},
  {PROGRAM,    {Symbol::BEGIN}},
  {VPRIME,     {Symbol::ID, Symbol::ARRAY}},
};

/**
 * Check if a symbol is in a set.
 * Helper for checking stop set membership.
 * @param S The symbol set to check.
 * @param sym The symbol to check.
 * @return true if sym is in S.
 */
bool in(std::set<Symbol> S, Symbol sym) {
  return S.count(sym);
}

/**
 * Union a vector of stopsets together.
 * @param stopSets A vector of Symbol sets to union.
 * @return a set of all of the given stopsets.
 */
std::set<Symbol> munion(std::vector<std::set<Symbol>> stopSets) {
  std::set<Symbol> newSet;
  for (auto& S : stopSets) {
    for (auto sym : S)
      newSet.insert(sym);
  }
  return newSet;
}

#endif
