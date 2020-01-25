#ifndef SCANNER_H
#define SCANNER_h
#include <iostream>
#include "SymbolTable.h"
#include "Token.h"
#include <map>


class Scanner {
 public:
  /**
  * Constructor for the scanner, initializes the private varaibles to
  * appropriate values
  */
  Scanner(std::istream &ifs, SymbolTable &symboltable);
  /**
  * Destructor of rthe scanner
  */
  ~Scanner() {};
  /**
  * Get the next Token in the line
  */
  Token getToken();

 private:
   /**
   * The file stream
   */
  std::istream &fin;
  /**
  * The Symbol Table being checked and filled with tokens
  */
  SymbolTable &symtable;
  /**
  * The current line the scanner is reading
  */
  std::string line;
  /**
  * The postion of the char the scanner is reading
  */
  std::size_t pos;
  /**
  * The map containing the symbols
  */
  std::map<std::string, Symbol> symmap;
  /**
  * Check input symbol against Whitespace whether tab or space
  */
  bool isWhitespace(char inchar);
  /**
  * Checks the inputed char against all possible symbols
  */
  bool isSpecial(char inchar);
  /**
  * Read and generate tokens for keywords and ID's
  * As well checks for invalid characters and returns a CHAR_ERR token
  * If the symbol table is filled then return a FULL_TAB error token
  */
  Token recognizeName();
  /**
  * Read and generate a token for any of the special symbols
  */
  Token recognizeSpecial();
  /**
  * Read and generate a token for any number/digit
  */
  Token recognizeNumeral();
};

#endif
