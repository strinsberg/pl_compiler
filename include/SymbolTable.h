#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "Token.h"
#include <vector>
#include <string>

const int MOD = 307;          // Mod for hashing as well as the backing array size
const int PRIME = 67;         // Mod for p in our rolling hash
const int ID_MAX_CHARS = 10;  // The maximum number of chars to hash from a string


class SymbolTable {
 public:
  SymbolTable();

  /**
    * Searches for a lexeme in the symbol table and returns its position.
    * Return -1 if not found.
    */ 
  int search(const std::string& str);

  /**
    * Insert a new lexeme into the symbol table.
    * Creates a new ID token for the lexeme as once the reserve words are
    * loaded the only thing loaded should be IDs.
    */
  int insert(const std::string& str);

  /**
    * Computes a rolling hash for a given string using the MOD constant.
    * Only looks at a max of 10 characters from the string.
    * Returns the integer hash of the string.
    */ 
  int hash(const std::string& str);

  /**
    * Returns true if the table is full.
    */
  bool full();

  /**
    * Returns the number items in the table.
    */ 
  int getLoad();

  /**
    * Returns a string representation of the table.
    */
  std::string toString();

 private:
  std::vector<Token> table;  // Hash table backing array
  int load;  // The number of elements in the table

  /**
    * Given a position linear probe until the token with the given lexeme
    * is found or an empty token is found.
    * Returns a pair with the position of the token and the lexeme.
    */ 
  std::pair<int, Token> probe(int idx, std::string lexeme);
};

#endif
