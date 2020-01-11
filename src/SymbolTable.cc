#include "SymbolTable.h"


SymbolTable::SymbolTable() : table(std::vector<int>(MOD)){}


bool SymbolTable::search(std::string str) {
  // check if a value is in the table
  // hash once to find initial index
  // hash second time to ensure we get the one that equals the given value
}


int SymbolTable::find(std::string str) {
  // check if a value is in the table
  // same as search but returns value
}


void SymbolTable::insert(std::string str) {
  // Add a new value. Might want to check to make sure we don't add twice.
}


/*
 * Computes a rolling hash for a string given a modulus
 */ 
int SymbolTable::hash(std::string& str, int mod) {
  int hash_val = 0;
  int pow = 1;
  for (auto c : str) {
    hash_val = (hash_val + c * pow) % mod;
    pow = (pow * PRIME) % mod;
  }
  return hash_val;
}
