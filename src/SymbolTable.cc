#include "SymbolTable.h"
#include <string>
#include <stdexcept>


SymbolTable::SymbolTable() : table(MOD), load(0) {
  loadKeywords();
}


int SymbolTable::search(const std::string& str) {
  int pos = hash(str);
  auto item = probe(pos, str);

  if (item.second.getSymbol() == Symbol::EMPTY)
    return -1;
  else
    return item.first;
}


int SymbolTable::insert(const std::string& str) {
  if (full())
    throw std::length_error("Symbol table is full");

  // Hash the string and find the right position in the table
  int pos = hash(str);
  std::pair<int, Token&> item = probe(pos, str);

  // If the string is not already in the table add it
  if (item.second.getSymbol() == Symbol::EMPTY)
    table[item.first] = Token(Symbol::ID, str, item.first);
  else
    return -1;

  // Increase load and return token
  load++;
  return item.first;
}


Token& SymbolTable::getToken(int idx, bool& found) {
  Token& item = table.at(idx);
  if (item.getSymbol() == Symbol::EMPTY)
    found = false;
  return item;
}


int SymbolTable::hash(const std::string& str) {
  int hash_val = 0;
  int pow = 1;

  // Calculate the rolling has for the first 10 chars of the string
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


std::pair<int, Token&> SymbolTable::probe(int pos, std::string lexeme) {
  Token current = table[pos];

  // linear probe for the lexeme or an empty token return a pair with
  // the position and the token found
  while (current.getSymbol() != Symbol::EMPTY) {
    if (current.getLexeme() == lexeme)
      return {pos, current};

    pos = (pos + 1) % MOD;
    current = table[pos];
  }

  return {pos, current};
}


void SymbolTable::loadKey(Symbol sym, const std::string& lexeme) {
  int hs = hash(lexeme);
  auto place = probe(hs, lexeme);

  table[place.first] = Token(sym, lexeme);
  load++;
}


void SymbolTable::loadKeywords() {
  for (auto& it : WordSym) {
    loadKey(it.second, it.first);
  }
}
