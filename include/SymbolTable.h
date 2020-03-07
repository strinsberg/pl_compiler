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
   * @return The index of the token in the symbol table, or -1 for not found.
   */
  int search(const std::string& str);

  /**
   * Inserts a new lexeme into the symbol table if it is not already there.
   * @param str Insert a string into the hash table.
   * @return The index of the token in the symbol table, or -1 if it exists.
   * @throws length_error if the symbol table is full.
   */
  int insert(const std::string& str);

  /**
   * Get a reference to the token in the symbol table by its index.
   * @param idx The index of the token.
   * @param found 
   * @return a reference to the token or a dummy empty token.
   * @throws out_of_range error if the idx is out of bounds.
   */
  Token& getToken(int idx, bool& found);

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
  std::pair<int, Token&> probe(int idx, std::string lexeme);

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
