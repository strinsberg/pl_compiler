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
  Parser(Administration& admin);

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

  /**
   * Parses a program from the stream of tokens.
   */
  void program();

  /**
   * Parses a block from the stream of tokens.
   */
  void block();

  /**
   * Parses a expression list from the stream of tokens.
   */
  void exprList();

  /**
   * Parses a simple expression from the stream of tokens.
   */
  void simpleExpr();

  /**
   * Parses a term from the stream of tokens.
   */
  void term();

  /**
   * Parses a factor from the stream of tokens.
   */
  void factor();

  /**
   * Parses a plus or minus operator from the stream of tokens.
   */
  void addOp();

  /**
   * Parses a multiplication or division or modulus operator from the
   * stream of tokens.
   */
  void multOp();

  /**
   * Parses a true or false from the stream of tokens.
   */
  void boolSym();
};

#endif
