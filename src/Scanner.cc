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
    line.push_back('\t');
  }

  while (isWhitespace(line[pos])) {
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
        line = tmpline + ' ';
        return Token(Symbol::NEWLINE);
    }
  }
}

bool Scanner::isWhitespace(char inchar) {
  if (inchar == ' ' || inchar == '\t') {
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

bool Scanner::isSeparator(char inchar) {
  return isWhitespace(inchar) || inchar == ';' || inchar == ',';
}

Token Scanner::recognizeName() {
  Symbol error = Symbol::EMPTY;
  std::string lexeme = "";

  while(!isSeparator(line[pos]) && pos < line.length()) {
    if(!std::isalpha(line[pos]) && !std::isdigit(line[pos]) && line[pos] != '_')  {
      if(!isSpecial(line[pos])) {
        error = Symbol::CHAR_ERR;
      } else {
        error = Symbol::NAME_ERR;
      }
    }
    lexeme+=(line[pos]);
    pos++;
  }

  if(error != Symbol::EMPTY) {
    return Token(error, lexeme);
  }

  Token token = symtable.search(lexeme);
  if (token.getSymbol() == Symbol::EMPTY)
    return symtable.insert(lexeme);
  else
    return token;
}


Token Scanner::recognizeSpecial() {
  Symbol error = Symbol::EMPTY;
  std::string lexeme = "";

  char chr = line[pos];
  if ((chr == '+' || chr == '-') && std::isdigit(line.at(pos+1)))
    return recognizeNumeral();

  while(!isWhitespace(line[pos]) && pos < line.length()) {
    if(!isSpecial(line[pos])) {
      error = Symbol::CHAR_ERR;
    }
    lexeme+=(line[pos]);
    if (isSeparator(line[pos++]))
      break;
  }

  if(symmap.find(lexeme) == symmap.end()) {
    error = Symbol::SYM_ERR;
  }

  if (error != Symbol::EMPTY) {
    return Token(error, lexeme);
  } else {
    return Token(symmap[lexeme], lexeme);
  }
}

Token Scanner::recognizeNumeral() {
  //overflow error check
  Symbol error = Symbol::EMPTY;
  std::string lexeme = "";

  while(!isSeparator(line[pos]) && pos < line.length()) {
    if(line[pos] != '-' && line[pos] != '+' && !std::isdigit(line[pos])) {
      error = Symbol::NUM_ERR;
    }
    lexeme+=(line[pos]);
    pos++;
  }

  if (error != Symbol::EMPTY) {
    return Token(error, lexeme);
  } else {
    int num  = std::stoi(lexeme);
    return Token(Symbol::NUM, lexeme, num);
  }
}
