#include "Token.h"
#include "Symbol.h"

Token::Token() : sname(Symbol::EMPTY) {}

Token::Token(Symbol sym, std::string lex, int v)
    : sname(sym), lexeme(lex), val(v) {}
