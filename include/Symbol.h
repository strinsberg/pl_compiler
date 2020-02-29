#ifndef SYMBOL_H
#define SYMBOL_H

#include <map>

/**
 * Enum containing all possible Symbols.
 */
enum Symbol {
  DOT = 256, COMMA, SEMI, LHSQR, RHSQR, AMP, BAR, TILD, LESS, EQUAL, GREAT,
  PLUS, MINUS, TIMES, FSLASH, BSLASH, LHRND, RHRND, INIT, GUARD, ARROW, DOLLAR,

  INT, BOOL, FALSE, TRUE, BEGIN, END, CONST, ARRAY, PROC, SKIP, READ, WRITE,
  CALL, IF, FI, DO, OD,

  ID, KEY, ENDFILE, EMPTY, EPSILON, NEWLINE, NUM, RECORD, FLOAT, VAR,

  NAME_ERR, NUM_ERR, CHAR_ERR, FULL_TAB, UNDEF
};


/**
 * Map from all symbols to string versions of themselves for printing.
 */
const std::map<Symbol, std::string> SymbolToString {
  {DOT, "'.'"},
  {COMMA, "','"},
  {SEMI, "';'"},
  {LHSQR, "'['"},
  {RHSQR, "']'"},
  {AMP, "'&'"},
  {BAR, "'|'"},
  {TILD, "'~'"},
  {LESS, "'<'"},
  {EQUAL, "'='"},
  {GREAT, "'>'"},
  {PLUS, "'+'"},
  {MINUS, "'-'"},
  {TIMES, "'*'"},
  {FSLASH, "'/'"},
  {BSLASH, "MOD"},
  {LHRND, "'('"},
  {RHRND, "')'"},
  {INIT, "':='"},
  {GUARD, "'[]'"},
  {ARROW, "'->'"},
  {DOLLAR, "'$'"},
  {INT, "integer"},
  {BOOL, "Boolean"},
  {TRUE, "true"},
  {FALSE, "false"},
  {ID, "ID"},
  {KEY, "keyword"},
  {ENDFILE, "EOF"},
  {EMPTY, "EMPTY"},
  {NEWLINE, "NEWLINE"},
  {NUM, "NUMBER"},
  {NAME_ERR, "NameErr"},
  {NUM_ERR, "NumErr"},
  {CHAR_ERR, "CharErr"},
  {FULL_TAB, "FullTableErr"},
  {BEGIN, "begin"},
  {END, "end"},
  {CONST, "const"},
  {ARRAY, "array"},
  {PROC, "proc"},
  {SKIP, "skip"},
  {READ, "read"},
  {WRITE, "write"},
  {CALL, "call"},
  {IF, "if"},
  {FI, "fi"},
  {DO, "do"},
  {OD, "od"},
  {VAR, "var"},
  {RECORD, "record"},
  {FLOAT, "float"},
  {UNDEF, "Undefined"}
};

#endif
