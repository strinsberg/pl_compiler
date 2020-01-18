#ifndef SYMBOL_H
#define SYMBOL_H

enum Symbol {
  DOT, COMMA, SEMI, LHSQR, RHSQR, AMP, BAR, TILD, LESS, EQUAL, GREAT, PLUS,
  MINUS, TIMES, FSLASH, BSLASH, LHRND, RHRND, INIT, GUARD, ARROW, DOLLAR,

  BEGIN, END, CONST, ARRAY, INT, BOOL, PROC, SKIP, READ, WRITE, CALL, IF, DO,
  FI, OD, FALSE, TRUE,

  ID, ENDFILE, EMPTY, NEWLINE, ERROR
};


#endif
