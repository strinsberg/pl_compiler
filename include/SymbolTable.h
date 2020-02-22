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
   * Searches for a lexeme in the symbol table and returns its token.
   * @param str The lexeme to search for.
   * @return the token matching the lexeme or the EMPTY token if the table
   * is full.
   */
  Token search(const std::string& str);

  /**
   * Inserts a new lexeme into the symbol table.
   * Creates a new ID token for the lexeme as once the reserve words are
   * loaded the only thing loaded should be IDs.
   * @param str Insert a string into the hash table.
   * @return an ID token for the string or a FullTableError token if the
   * table is full.
   */
  Token insert(const std::string& str);

  /**
   * Computes a rolling hash for a given string using the MOD constant.
   * Only looks at a max of 10 characters from the string.
   * @param str The string to hash.
   * @return the integer hash value of the string.
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
  /**
   * Backing array for the hash table
   */
  std::vector<Token> table;

  /**
   * The number of elements in the hash table
   */
  int load;

  /**
   * Given a position linear probe until the token with the given lexeme
   * is found or an empty token is found.
   * @param idx The initial position to start probing. Generally the lexemes
   * hash value.
   * @param lexeme The lexeme to probe for.
   * @return a pair with the position of the token and the lexeme.
   */
  std::pair<int, Token> probe(int idx, std::string lexeme);

  /**
   * Load a token for a reserved keyword into the table.
   * @param lexeme The tokens's lexeme.
   * @param sym The token's symbol.
   */
  void loadKey(Symbol sym, const std::string& lexeme);

  /**
   * Loads all reserved keywords into the symbol table.
   */
  void loadKeywords();
};

#endif
