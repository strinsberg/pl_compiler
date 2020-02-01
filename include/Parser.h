#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include "Token.h"
#include "Administration.h"


class Parser {
 public:
   /**
     * Creates a new Parser object.
     * @param admin An administration object for handling errors and holding
     * our scanner etc. for now.
     */
  Parser(Administration admin);

  /**
    * Parses a PL program.
    */
  void parse();

 private:
  /**
   * The administration object for errors and holding the scanner and symbol
   * table.
   */
  Administration& admin;

  /**
   * The look ahead token.
   */
  Token look;

  /**
   * Match a Token and move to the next one.
   */
  void match(Symbol symbol);

  void program();
  void block();
  void exprList();
  void simpleExpr();
  void term();
  void factor();
  void addOp();
  void multOp();
  void boolSym();
};

#endif
