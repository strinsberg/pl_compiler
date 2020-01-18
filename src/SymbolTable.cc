#include "SymbolTable.h"
#include <string>


SymbolTable::SymbolTable() : table(MOD), load(0) {}


// currently returns the actual table index
// should it return the hash index???
int SymbolTable::search(const std::string& str) {
  int pos = hash(str);
  std::pair<int, Token> item = probe(pos, str);

  if (item.second.getSymbol() == Symbol::EMPTY)
    return -1;
  else
    return item.first;
}


// currently saves the actual position of the token in the table
// may want to make it save the hashed index
int SymbolTable::insert(const std::string& str) {
  int pos = hash(str);
  std::pair<int, Token> item = probe(pos, str);

  if (full())
    return -1;

  if (item.second.getSymbol() == Symbol::EMPTY)
    table[item.first] = Token(Symbol::ID, str, item.first);

  load++;
  return item.first;
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
  return load >= MOD - 1;
}


int SymbolTable::getLoad() {
  return load;
}


std::string SymbolTable::toString() {
  return "Symbol Table -> Not Implemented Yet!";
}


std::pair<int, Token> SymbolTable::probe(int pos, std::string lexeme) {
  Token current = table[pos];
  while (current.getSymbol() != Symbol::EMPTY) {
    if (current.getLexeme() == lexeme)
      return {pos, current};
    pos = (pos + 1) % MOD;
    current = table[pos];
  }
  return {pos, current};
}
