#include "SymbolTable.h"
#include <string>


SymbolTable::SymbolTable() : table(MOD), load(0) {}

Token SymbolTable::search(const std::string& str) {
  int pos = hash(str);
  return probe(pos, str).second;
}

// currently saves the actual position of the token in the table
// may want to make it save the hashed index
Token SymbolTable::insert(const std::string& str) {
  if (full())
    return Token(Symbol::ERROR, str);

  int pos = hash(str);
  std::pair<int, Token> item = probe(pos, str);

  if (item.second.getSymbol() == Symbol::EMPTY)
    table[item.first] = Token(Symbol::ID, str, item.first);

  load++;
  return table[item.first];
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
