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


/**
 * Map for all special lexemes to their symbol.
 */
const std::map<std::string, Symbol> SpecialSym {
  {".", Symbol::DOT},
  {",", Symbol::COMMA},
  {";", Symbol::SEMI},
  {"[", Symbol::LHSQR},
  {"]", = Symbol::RHSQR},
  {"&", Symbol::AMP},
  {"|", Symbol::BAR},
  {"~", Symbol::TILD},
  {"<", Symbol::LESS},
  {"=", Symbol::EQUAL},
  {">", Symbol::GREAT},
  {"+", Symbol::PLUS},
  {"-", Symbol::MINUS},
  {"*", Symbol::TIMES},
  {"/", Symbol::FSLASH},
  {"\\", Symbol::BSLASH},
  {"(", Symbol::LHRND},
  {")", Symbol::RHRND},
  {":=", Symbol::INIT},
  {"[]", = Symbol::GUARD},
  {"->", Symbol::ARROW}
};


/**
 * Map for all keywords (word symbols) to their symbol.
 */
const std::map<std::string, Symbol> WordSym {
  {"begin", Symbol::BEGIN},
  {"end", Symbol::END},
  {"const", Symbol::CONST},
  {"array", Symbol::ARRAY},
  {"proc", Symbol::PROC},
  {"skip", Symbol::SKIP},
  {"read", Symbol::READ},
  {"write", Symbol::WRITE},
  {"call", Symbol::CALL},
  {"if", Symbol::IF},
  {"fi", Symbol::FI},
  {"do", Symbol::DO},
  {"od", Symbol::OD},
  {"integer", Symbol::INT},
  {"Boolean", Symbol::BOOL},
  {"true", Symbol::TRUE},
  {"false", Symbol::FALSE},
  {"record", Symbol::RECORD},
  {"var", Symbol::VAR},
  {"float", Symbol::FLOAT}
};
#endif
