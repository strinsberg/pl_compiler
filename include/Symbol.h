#ifndef SYMBOL_H
#define SYMBOL_H

#include <map>

  /**
    * Enum containing all possible Symbols.
    */
enum Symbol {
  DOT = 256, COMMA, SEMI, LHSQR, RHSQR, AMP, BAR, TILD, LESS, EQUAL, GREAT, PLUS,
  MINUS, TIMES, FSLASH, BSLASH, LHRND, RHRND, INIT, GUARD, ARROW, DOLLAR,

  INT, BOOL, FALSE, TRUE,

  ID, KEY, ENDFILE, EMPTY, NEWLINE, NUM,

  NAME_ERR, NUM_ERR, CHAR_ERR, FULL_TAB
};

  /**
    * Map mapping all the symbols to string versions of themselves for printing.
    */
const std::map<Symbol, std::string> SymbolToString {
  {DOT, "Dot"},
  {COMMA, "Commma"},
  {SEMI, "Semi"},
  {LHSQR, "SquareL"},
  {RHSQR, "SquareR"},
  {AMP, "Amp"},
  {BAR, "Bar"},
  {TILD, "Tild"},
  {LESS, "Less"},
  {EQUAL, "Equal"},
  {GREAT, "Greater"},
  {PLUS, "Plus"},
  {MINUS, "Minus"},
  {TIMES, "Times"},
  {FSLASH, "SlashF"},
  {BSLASH, "SlashB"},
  {LHRND, "RoundL"},
  {RHRND, "RoundR"},
  {INIT, "Init"},
  {GUARD, "[]"},
  {ARROW, "Arrow"},
  {DOLLAR, "Dollar"},
  {INT, "Integer"},
  {BOOL, "Boolean"},
  {TRUE, "True"},
  {FALSE, "False"},
  {ID, "ID"},
  {KEY, "Keyword"},
  {ENDFILE, "EOF"},
  {EMPTY, "Empty"},
  {NEWLINE, "Newline"},
  {NUM, "Number"},
  {NAME_ERR, "NameErr"},
  {NUM_ERR, "NumErr"},
  {CHAR_ERR, "CharErr"},
  {FULL_TAB, "SymTabFull"}
};

#endif
