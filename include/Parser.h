#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <set>
#include "Symbol.h"
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
   * @param stop The stopsets used to recover from the error.
   */
  void match(Symbol symbol, std::set<Symbol> stop);

  /**
   * Process a syntax error and perform error recovery.
   * @param stop The stopsets used to recover from the error.
   */
  void syntaxError(std::set<Symbol> stop);

  /**
   * Checks the next token to see if it will be valid.
   * @param stop The stopsets used to recover from an error.
   */
  void syntaxCheck(std::set<Symbol> stop);

  /**
   * Parses a program from the stream of tokens.
   */
  void program(std::set<Symbol> stop);

  /**
   * Parses a block from the stream of tokens.
   */
  void block(std::set<Symbol> stop);

  /**
   * Parses a definition part from the stream of tokens.
   */
  void defPart(std::set<Symbol> stop);

  /**
   * Parses a definition from the stream of tokens.
   */
  void def(std::set<Symbol> stop);

  /**
   * Parses a constant definitions from the stream of tokens.
   */
  void constDef(std::set<Symbol> stop);

  /**
   * Parses a procedure definition from the stream of tokens.
   */
  void procDef(std::set<Symbol> stop);

  /**
   * Parses the statement part of the program.
   */
  void stmtPart(std::set<Symbol> stop);

  /**
   * Parses a statement.
   */
  void stmt(std::set<Symbol> stop);

  /**
   * Parses an empty statement.
   */
  void emptyStmt(std::set<Symbol> stop);

  /**
   * Parses a read statement.
   */
  void readStmt(std::set<Symbol> stop);

  /**
   * Parses a write stamtement.
   */
  void writeStmt(std::set<Symbol> stop);

  /**
   * Parses an assignment statement.
   */
  void assignStmt(std::set<Symbol> stop);

  /**
   * Parses a procedure call.
   */
  void procStmt(std::set<Symbol> stop);

  /**
   * Parses an if statement.
   */
  void ifStmt(std::set<Symbol> stop);

  /**
   * Parses a do statement.
   */
  void doStmt(std::set<Symbol> stop);

  /**
   * Parses a variable access list.
   */
  void vacsList(std::set<Symbol> stop);

  /**
   * Parses variable access.
   */
  void varAccess(std::set<Symbol> stop);

  /**
   * Parses a varaible definition from the stream of tokens.
   */
  void varDef(std::set<Symbol> stop);

  /**
   * Parses a varaible vs array from the stream of tokens.
   */
  void vPrime(std::set<Symbol> stop);

  /**
   * Parses a varaible list from the stream of tokens.
   */
  void varList(std::set<Symbol> stop);

  /**
   * Parses an index selector. ie) A[i].
   */
  void idxSelect(std::set<Symbol> stop);

  /**
   * Parses a expression list from the stream of tokens.
   */
  void exprList(std::set<Symbol> stop);

  /**
   * Parses a expression from the stream of tokens.
   */
  void expr(std::set<Symbol> stop);

  /**
   * Parses a primary expression from the stream of tokens.
   */
  void primeExpr(std::set<Symbol> stop);

  /**
   * Parses a simple expression from the stream of tokens.
   */
  void simpleExpr(std::set<Symbol> stop);

  /**
   * Parses a list of guarded commands.
   */
  void guardedList(std::set<Symbol> stop);

  /**
   * Parses a guarded command.
   */
  void guardedComm(std::set<Symbol> stop);

  /**
   * Parses a term from the stream of tokens.
   */
  void term(std::set<Symbol> stop);

  /**
   * Parses a factor from the stream of tokens.
   */
  void factor(std::set<Symbol> stop);

  /**
   * Parses a primary operator from the stream of tokens.
   */
  void primeOp(std::set<Symbol> stop);

   /**
    * Parses a realtional operator from the stream of tokens.
    */
  void relOp(std::set<Symbol> stop);

  /**
   * Parses a plus or minus operator from the stream of tokens.
   */
  void addOp(std::set<Symbol> stop);

  /**
   * Parses a multiplication or division or modulus operator from the
   * stream of tokens.
   */
  void multOp(std::set<Symbol> stop);

  /**
   * Parses a const non-terminal
   */
  void constant(std::set<Symbol> stop);

  /**
   * Parses a definition type from the stream of tokens.
   */
  void typeSym(std::set<Symbol> stop);

  /**
   * Parses a true or false from the stream of tokens.
   */
  void boolSym(std::set<Symbol> stop);

  void fieldList(std::set<Symbol> stop);

  void recordSection(std::set<Symbol> stop);

  void procBlock(std::set<Symbol> stop);

  void formParamList(std::set<Symbol> stop);

  void paramDef(std::set<Symbol> stop);

  void actParamList(std::set<Symbol> stop);

  void actParam(std::set<Symbol> stop);

  void selec(std::set<Symbol> stop);

  void fieldSelec(std::set<Symbol> stop);
};

#endif
