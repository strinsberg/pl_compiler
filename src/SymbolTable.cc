#include "SymbolTable.h"
#include <string>

// Probably want to keep the key in a pair with its value so that we
// can find the right key when linear probing. Or at least the first
// 10 characters of the key as this is all we need for our hash.


SymbolTable::SymbolTable() : table(MOD), load(0) {}


int SymbolTable::search(const std::string& str) {
  return false;
}


int SymbolTable::insert(const std::string& str) {
  // Add a new value. Might want to check to make sure we don't add twice.
  // Perhaps throw an error if the table is full. But make sure that
  // full is at least 1 less than MOD so that there is always an empty
  // cell to ensure search works. say 1/4 MOD empty.
  // add to load when you add something. For now I don't know if we
  // have to delete things but this may be an issue
  load++;
  return -1;
}


int SymbolTable::hash(const std::string& str) {
  int hash_val = 0;
  int pow = 1;

  for (int i = 0; i < ID_MAX_CHARS && i < (int)str.length(); i++) {
    char c = str[i];
    hash_val = (hash_val + c * pow) % MOD;
    pow = (pow * PRIME) % MOD;
  }

  return hash_val;
}


bool SymbolTable::full() {
  return load >= MOD;
}


int SymbolTable::getLoad() {
  return load;
}


std::string SymbolTable::toString() {
  return "Symbol Table -> Not Implemented Yet!";
}
