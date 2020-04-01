#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <set>
#include "Symbol.h"
#include "Token.h"
#include "TableEntry.h"
#include "Administration.h"
#include "BlockTable.h"


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
   int label;
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
   * Block table structure that keeps track of variables and their scope.
   */
  BlockTable blocks;

  /**
   * Returns a numerical label that is incremented on each use.
   */
  int NewLabel();

  /**
   * Match a Token and move to the next one.
   * @param symbol The symbol being matched
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
   * @param stop The stopsets used to recover from an error.
   */
  void program(std::set<Symbol> stop);

  /**
   * Parses a block from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @param entries The entries being added to the block
   * @param startLabel The current numerical label that this block started at
   * @param varLabel Used to determine the total size of variables in the block
   * (we think).
   */
  void block(std::set<Symbol> stop,
             std::vector<TableEntry> entries,
             int startlabel, int varlabel);

  /**
   * Parses a definition part from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @return the size of the variables defined.
   */
  int defPart(std::set<Symbol> stop);

  /**
   * Parses a definition from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @param start The starting label for variable being defined.
   * @return the size of the variables defined.
   */
  int def(std::set<Symbol> stop, int& start);

  /**
   * Parses a constant definitions from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   */
  void constDef(std::set<Symbol> stop);

  /**
   * Parses a procedure definition from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   */
  void procDef(std::set<Symbol> stop);

  /**
   * Parses the statement part of the program.
   * @param stop The stopsets used to recover from an error.
   */
  void stmtPart(std::set<Symbol> stop);

  /**
   * Parses a statement.
   * @param stop The stopsets used to recover from an error.
   */
  void stmt(std::set<Symbol> stop);

  /**
   * Parses an empty statement.
   * @param stop The stopsets used to recover from an error.
   */
  void emptyStmt(std::set<Symbol> stop);

  /**
   * Parses a read statement.
   * @param stop The stopsets used to recover from an error.
   */
  void readStmt(std::set<Symbol> stop);

  /**
   * Parses a write stamtement.
   * @param stop The stopsets used to recover from an error.
   */
  void writeStmt(std::set<Symbol> stop);

  /**
   * Parses an assignment statement.
   * @param stop The stopsets used to recover from an error.
   */
  void assignStmt(std::set<Symbol> stop);

  /**
   * Parses a procedure call.
   * @param stop The stopsets used to recover from an error.
   */
  void procStmt(std::set<Symbol> stop);

  /**
   * Parses an if statement.
   * @param stop The stopsets used to recover from an error.
   */
  void ifStmt(std::set<Symbol> stop);

  /**
   * Parses a do statement.
   * @param stop The stopsets used to recover from an error.
   */
  void doStmt(std::set<Symbol> stop);

  /**
   * Parses a variable access list.
   * @param stop The stopsets used to recover from an error.
   * @return A vector of the types of the variables in the list in order.
   */
  std::vector<Type> vacsList(std::set<Symbol> stop);

  /**
   * Parses variable access.
   * @param stop The stopsets used to recover from an error.
   * @param isConst Wether or not the variable being accessed is a const variable.
   * @param return The type of the variable accessed.
   */
  Type varAccess(std::set<Symbol> stop, bool& isConst);

  /**
   * Parses a varaible definition from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @param start The starting label for variable being defined.
   * @return the size of the variable defined.
   */
  int varDef(std::set<Symbol> stop, int& start);

  /**
   * Parses a varaible vs array from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @param type The type of the table entry that is being created
   * @param start The starting label for variable being defined.
   * @return the size of the variable defined.
   */
  int vPrime(std::set<Symbol> stop, Type type, int& start);

  /**
   * Parses a varaible list from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @return A vector of the symbol table index for each variable in the list.
   */
  std::vector<int> varList(std::set<Symbol> stop);

  /**
   * Parses an index selector. ie) A[i].
   * @param stop The stopsets used to recover from an error.
   * @param entry The Table entry being created
   * @return The type of the accessed variable.
   */
  Type idxSelect(std::set<Symbol> stop, TableEntry entry);

  /**
   * Parses a expression list from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @return The type of each expression result in the list in order.
   */
  std::vector<Type> exprList(std::set<Symbol> stop);

  /**
   * Parses a expression from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @return The type of the expression result.
   */
  Type expr(std::set<Symbol> stop);

  /**
   * Parses a primary expression from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @return The type of the expression result.
   */
  Type primeExpr(std::set<Symbol> stop);

  /**
   * Parses a simple expression from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @return The type of the expression result.
   */
  Type simpleExpr(std::set<Symbol> stop);

  /**
   * Parses a list of guarded commands.
   * @param stop The stopsets used to recover from an error.
   * @param start The current label to be set.
   * @param next A lable used to decide where to jump to after execution.
   */
  void guardedList(std::set<Symbol> stop, int& start, int next);

  /**
   * Parses a guarded command.
   * @param stop The stopsets used to recover from an error.
   * @param start The current label to be set.
   * @param next A lable used to decide where to jump to after execution.
   */
  void guardedComm(std::set<Symbol> stop, int& start, int next);

  /**
   * Parses a term from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @return the resulting type of the term.
   */
  Type term(std::set<Symbol> stop);

  /**
   * Parses a factor from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @param the resulting type of the factor.
   */
  Type factor(std::set<Symbol> stop);

  /**
   * Parses a primary operator from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @return the psuedo code string for the operator parsed, or ERROR.
   */
  std::string primeOp(std::set<Symbol> stop);

   /**
    * Parses a realtional operator from the stream of tokens.
    * @param stop The stopsets used to recover from an error.
   * @return the psuedo code string for the operator parsed, or ERROR.
    */
  std::string relOp(std::set<Symbol> stop);

  /**
   * Parses a plus or minus operator from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @return the psuedo code string for the operator parsed, or ERROR.
   */
  std::string addOp(std::set<Symbol> stop);

  /**
   * Parses a multiplication or division or modulus operator from the
   * stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @return the psuedo code string for the operator parsed, or ERROR.
   */
  std::string multOp(std::set<Symbol> stop);

  /**
   * Parses a const non-terminal.
   * @param stop The stopsets used to recover from an error.
   * @return a pair with the type and value of the constant parsed.
   */
  std::pair<Type,int> constant(std::set<Symbol> stop);

  /**
   * Parses a const num non-terminal.
   * @param stop The stopsets used to recover from an error.
   * @return the type of the non-terminal parsed.
   */
  Type cPrime(std::set<Symbol> stop);

  /**
   * Parses a definition type from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @return the type of the type symbol parsed.
   */
  Type typeSym(std::set<Symbol> stop);

  /**
   * Parses a true or false from the stream of tokens.
   * @param stop The stopsets used to recover from an error.
   * @return a number to represent the value parsed 0 = false, 1 = true.
   */
  int boolSym(std::set<Symbol> stop);

  /**
   * Parses the a list of all the fields and their corresponding types declared.
   * in a record.
   * @param stop The stopsets used to recover from an error.
   * @param fields The field of the record being declared.
   */
  void fieldList(std::set<Symbol> stop, std::vector<TableEntry>& fields);

  /**
    * Parses a list of idetifiers of the same type declared in a record.
    * @param stop The stopsets used to recover from an error.
    * @param fields The field of the record being declared.
    */
  void recordSection(std::set<Symbol> stop, std::vector<TableEntry>& fields);

  /**
    * Parses the block for a procedure declaration.
    * @param stop The stopsets used to recover from an error.
    * @param id The id of the procedure.
    * @param start The start label when the block was parsed.
    * @param var The size of the variables in the block.
    * @param proc The label of the procedure.
    */
  void procBlock(std::set<Symbol> stop, int id, int start, int var, int proc);

  /**
    * Parses the parameter list when a procdure is being declared.
    * @param stop The stopsets used to recover from an error.
    * @param params The parameters of the procedure being defined.
    */
  void formParamList(std::set<Symbol> stop, std::vector<TableEntry>& params);

  /**
    * Parses a list of idetifiers being passed into the procedure, can be
    * tagged with "var" meaning it is pass by reference, pass by value otherwise.
    * @param stop The stopsets used to recover from an error.
    * @param params The parameters of the procedure being defined.
    */
  void paramDef(std::set<Symbol> stop, std::vector<TableEntry>& params);

  /**
    * Parases the list of parameters when a procedure is being called.
    * @param stop The stopsets used to recover from an error.
    * @return The types of the parameters in order.
    */
  std::vector<Type> actParamList(std::set<Symbol> stop);

  /**
    * Parses the individual paramters inside the paramater list when a
    * procedure is called.
    * @param stop The stopsets used to recover from an error.
    * @return The type of the parameter.
    */
  Type actParam(std::set<Symbol> stop);

  /**
    * Parses whether the varaible being accessed is in a record or expression.
    * @param stop The stopsets used to recover from an error.
    * @param entry The table entry of the record being accesssed.
    * @return The type of the variable being accessed.
    */
  Type selec(std::set<Symbol> stop, TableEntry entry);

  /**
    * Parses field/variable being selected from a record
    * @param stop The stopsets used to recover from an error.
    * @param entry The table entry of the record being accessed.
    * @return The type of the variable being accessed.
    */
  Type fieldSelec(std::set<Symbol> stop, TableEntry entry);
};

#endif
