#include "Scanner.h"
#include <string>
#include <fstream>

Scanner::Scanner(std::ifstream &ifs, SymbolTable &symboltable) : fin(ifs),
                      symtable(symboltable), line(""), inChar(' '), pos(0) {}

Token Scanner::getToken() {
  while (line[pos] == ' ') {
    pos++;
  }
  char s = line[pos];
  if(s != '$') {
    if(isSpecial(s)) {
        return recognizeSpecial();
    } else if(std::isdigit(s)) {
        return recognizeNumeral();
    } else {
        return recognizeName();
    }
  } else {
    std::string tmpline;
    if (!std::getline(fin, tmpline)) {
      return Token(Symbol::ENDFILE);
    } else {
        pos = 0;
        line = tmpline;
        return Token(Symbol::NEWLINE);
    }
  }

}


bool Scanner::isWhitespace(char inchar) {
  if (inchar == ' ') {
    return true;
  }
  return false;
}

bool Scanner::isSpecial(char inchar) {
  std::string specials = ".,;[]&|~<=>+-*/\():$";
  std::size_t found = specials.find(inchar);
  if(found != std::string::npos) {
    return true;
  }
  return false;
}

Token Scanner::recognizeName() {
  bool error = false;
  std::string lexeme = "";
  while(!isWhitespace(line[pos])) {
    if(isalpha(line[pos]) || line[pos] == '_')  {
        lexeme+=(line[pos]);
        pos++;
    } else {
      error = true;
    }
  }
  if(error == true) {
    return Token(Symbol::ERROR, lexeme);
  }
  int tokenIndex = symtable.search(lexeme);
  if (tokenIndex == -1) {
    tokenIndex = symtable.insert(lexeme);
    return Token(Symbol::ID, lexeme, tokenIndex);
  } else {
    return Token(Symbol::ID, lexeme, tokenIndex);
  }
}


Token Scanner::recognizeSpecial() {
  return Token();
}

Token Scanner::recognizeNumeral() {
  return Token();
}
