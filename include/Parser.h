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

  void defPart();
  bool defFirst();
  void def();
  void constDef();
  void varDef();
  void typeSym();
  void vPrime();
  void varList();
  void procDef();

  /**
   * Parses the statement part of the program.
   */
  void stmtPart();

  /**
   * Checks to see if the next token is in the first set of stmt.
   */
  bool stmtFirst();

  /**
   * Parses a statement.
   */
  void stmt();

  /**
   * Parses an empty statement.
   */
  void emptyStmt();

  /**
   * Parses a read statement.
   */
  void readStmt();

  /**
   * Parses a variable access list.
   */
  void vacsList();

  /**
   * Parses a write stamtement.
   */
  void writeStmt();

  /**
   * Parses a expression list from the stream of tokens.
   */
  void exprList();

  /**
   * Parses an assignment statement.
   */
  void assignStmt();

  /**
   * Parses a procedure call.
   */
  void procStmt();

  /**
   * Parses an if statement.
   */
  void ifStmt();

  /**
   * Parses a do statement.
   */
  void doStmt();

  /**
   * Parses a list of guarded commands.
   */
  void guardedList();

  /**
   * Parses a guarded command.
   */
  void guardedComm();

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
