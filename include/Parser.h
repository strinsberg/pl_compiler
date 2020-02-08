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
   * Parses a expression from the stream of tokens.
   */
  void expr();
  /**
   * Parses a primary operator from the stream of tokens.
   */
  void primeOp();

  /**
   * Parses a primary expression from the stream of tokens.
   */
  void primeExpr();

   /**
    * Parses a realtional operator from the stream of tokens.
    */
  void relOp();

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
   * Parses variable access.
   */
  void varAccess();

  /**
   * Parses an index selector. ie) A[i].
   */
  void idxSelect();

  /**
   * Parses a const non-terminal
   */
  void constant();

  /**
   * Parses a true or false from the stream of tokens.
   */
  void boolSym();
};

#endif
