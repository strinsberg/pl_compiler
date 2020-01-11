#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <vector>
#include <string>


class SymbolTable {
 public:
  SymbolTable();

  bool search(std::string str);
  int find(std::string str);
  void insert(std::string str);
  int hash(std::string& str, int p);

 private:
  std::vector<int> table;  // Hash table backing array
  int MOD = 307;      // Table size and hash function modulus = 307
  int MODEQ = 1e9 + 9;    // Second hash function modulus for linear probing equality
  const int PRIME = 67;  // Rolling hash prime

};

#endif
