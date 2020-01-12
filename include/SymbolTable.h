#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include <string>

const int MOD = 307;          // Mod for hashing as well as the backing array size
const int PRIME = 67;         // Mod for p in our rolling hash
const int ID_MAX_CHARS = 10;  // The maximum number of chars to hash from a string


class SymbolTable {
 public:
  SymbolTable();

  bool search(const std::string& str);
  int find(const std::string& str);
  void insert(const std::string& str);

  /**
    * Computes a rolling hash for a given string using the MOD constant.
    * Only looks at a max of 10 characters from the string.
    *
    * @param str The string to hash.
    * @return the integer hash of the string.
    */ 
  int hash(const std::string& str);

 private:
  std::pair<std::string, int> table[MOD];  // Hash table backing array
  int load;  // The number of elements in the table
};

#endif
