#include "Scanner.h"
#include <string>
#include <fstream>

Scanner::Scanner(std::istream &ifs, SymbolTable &symboltable) : fin(ifs),
                      symtable(symboltable), line(""), inChar(' '), pos(0) {
  symmap["."] = Symbol::DOT;
  symmap[","] = Symbol::COMMA;
  symmap[";"] = Symbol::SEMI;
  symmap["["] = Symbol::LHSQR;
  symmap["]"] = Symbol::RHSQR;
  symmap["&"] = Symbol::AMP;
  symmap["|"] = Symbol::BAR;
  symmap["~"] = Symbol::TILD;
  symmap["<"] = Symbol::LESS;
  symmap["="] = Symbol::EQUAL;
  symmap[">"] = Symbol::GREAT;
  symmap["+"] = Symbol::PLUS;
  symmap["-"] = Symbol::MINUS;
  symmap["*"] = Symbol::TIMES;
  symmap["/"] = Symbol::FSLASH;
  symmap["\\"] = Symbol::BSLASH;
  symmap["("] = Symbol::LHRND;
  symmap[")"] = Symbol::RHRND;
  symmap[":="] = Symbol::INIT;
  symmap["[]"] = Symbol::GUARD;
  symmap["->"] = Symbol::ARROW;
}

Token Scanner::getToken() {
  if (line == "") {
    getline(fin, line); 
  }

  while (line[pos] == ' ') {
    pos++;
  }

  char s = line[pos];

  if(pos < line.length() && s != '$') {
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

  while(!isWhitespace(line[pos]) && pos < line.length()) {
    if(!std::isalpha(line[pos]) && !std::isdigit(line[pos]) && line[pos] != '_')  {
      error = true;
    }
    lexeme+=(line[pos]);
    pos++;
  }


  if(error == true) {
    return Token(Symbol::ERROR, lexeme);
  }

  int tokenIndex = symtable.search(lexeme);
  if (tokenIndex == -1) {
    tokenIndex = symtable.insert(lexeme);
    return Token(Symbol::ID, lexeme, tokenIndex);
  }
    //incorrect could be keyword or id
    // this is needed when we find the lexeme and need to check the token
    // to tell if it is an id or a keyword.
    return Token();
}


Token Scanner::recognizeSpecial() {
  bool error = false;
  std::string lexeme = "";

  while(!isWhitespace(line[pos]) && pos < line.length()) {
    if(!isSpecial(line[pos])) {
      error = true;
    }
    lexeme+=(line[pos]);
    pos++;
  }

  if(symmap.find(lexeme) == symmap.end()) {
    error = true;
  }

  if (error == true) {
    return Token(Symbol::ERROR, lexeme);
  } else {
    return Token(symmap[lexeme], lexeme);
  }
}

Token Scanner::recognizeNumeral() {
  bool error = false;
  std::string lexeme = "";

  while(!isWhitespace(line[pos]) && pos < line.length()) {
    if(!std::isdigit(line[pos])) {
      error = true;
    }
    lexeme+=(line[pos]);
    pos++;
  }

  if (error == true) {
    return Token(Symbol::ERROR, lexeme);
  } else {
    int num  = std::stoi(lexeme);
    return Token(Symbol::NUM, lexeme, num);
  }
}
