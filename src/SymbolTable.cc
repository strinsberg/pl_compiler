#include "SymbolTable.h"
#include <string>


SymbolTable::SymbolTable() : table(MOD), load(0) {
  loadKeywords();
}

Token SymbolTable::search(const std::string& str) {
  int pos = hash(str);
  return probe(pos, str).second;
}


Token SymbolTable::insert(const std::string& str) {
  if (full())
    return Token(Symbol::FULL_TAB, str);

  // Hash the string and find the right position in the table
  int pos = hash(str);
  std::pair<int, Token> item = probe(pos, str);

  // If the string is not already in the table add it
  if (item.second.getSymbol() == Symbol::EMPTY)
    table[item.first] = Token(Symbol::ID, str, item.first);

  // Increase load and return token
  load++;
  return table[item.first];
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


std::pair<int, Token> SymbolTable::probe(int pos, std::string lexeme) {
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
  loadKey(Symbol::BEGIN, "begin");
  loadKey(Symbol::END, "end");
  loadKey(Symbol::CONST, "const");
  loadKey(Symbol::ARRAY, "array");
  loadKey(Symbol::PROC, "proc");
  loadKey(Symbol::SKIP, "skip");
  loadKey(Symbol::READ, "read");
  loadKey(Symbol::WRITE, "write");
  loadKey(Symbol::CALL, "call");
  loadKey(Symbol::IF, "if");
  loadKey(Symbol::FI, "fi");
  loadKey(Symbol::DO, "do");
  loadKey(Symbol::OD, "od");
  loadKey(Symbol::INT, "integer");
  loadKey(Symbol::BOOL, "Boolean");
  loadKey(Symbol::TRUE, "true");
  loadKey(Symbol::FALSE, "false");
  loadKey(Symbol::RECORD, "record");
  loadKey(Symbol::VAR, "var");
  loadKey(Symbol::FLOAT, "float");
}
