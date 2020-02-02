#include "Scanner.h"
#include <stdexcept>
#include <string>
#include <fstream>

Scanner::Scanner(std::istream &ifs, SymbolTable &symboltable) : fin(ifs),
                      symtable(symboltable), line(""), pos(0) {
  // Load all symbol types into the symbol map for easy identification from
  // their characters
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
  // If the current line of input being parsed is empty get a new one
  if (line == "") {
    getline(fin, line);
    line.push_back('\t');
  }

  // Skip any whitespace before the next token
  while (isWhitespace(line[pos])) {
    pos++;
  }

  char s = line[pos];

  // Decide what token type to scan
  if(pos < line.length() && s != '$') {
    if(isSpecial(s)) {
        return recognizeSpecial();
    } else if(std::isdigit(s)) {
        return recognizeNumeral();
    } else {
        return recognizeName();
    }

  // Get a new line from the input and create newline or eof tokens if needed
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
  std::string specials = ".,;[]&|~<=>+-*/\\():$";
  std::size_t found = specials.find(inchar);

  if(found != std::string::npos) {
    return true;
  }

  return false;
}


Token Scanner::recognizeName() {
  std::string lexeme = "";

  // If the char is not allowed in a name return a char_err token
  if(!std::isalpha(line[pos]) && !std::isdigit(line[pos]) && line[pos] != '_') {
      lexeme+=(line[pos++]);
      return Token(Symbol::CHAR_ERR, lexeme);
  }

  // Until the next char is not alphanumeric or an underscore add it to the
  // lexeme being scanned.
  while(!isWhitespace(line[pos]) &&!isSpecial(line[pos]) && pos < line.length()) {
    if(!std::isalpha(line[pos]) && !std::isdigit(line[pos]) && line[pos] != '_')
      break;
  
    lexeme+=(line[pos++]);
  }

  // If the lexeme is in the symbol table return the token
  // otherwise insert the lexeme into the table and return the resulting token
  Token token = symtable.search(lexeme);
  if (token.getSymbol() == Symbol::EMPTY)
    return symtable.insert(lexeme);
  else
    return token;
}


Token Scanner::recognizeSpecial() {
  std::string lexeme = "";

  // If the symbol we have means a number should follow scan for the number
  if ((line[pos] == '+' || line[pos] == '-') && std::isdigit(line.at(pos+1)))
    return recognizeNumeral();

  
  // If we can make a lexeme of size 2 do it 
  lexeme+=(line[pos++]);
  if (pos < line.size()) {
    std::string checkmap = lexeme + line[pos];
   
    if(symmap.find(checkmap) != symmap.end())
      return Token(symmap[checkmap], checkmap);
  }

  // If character is not a symbol return a char_err token
  // otherwise return the proper symbol token
  if(symmap.find(lexeme) == symmap.end()) {
    return Token(Symbol::CHAR_ERR, lexeme);
  } else {
    return Token(symmap[lexeme], lexeme);
  }
}


Token Scanner::recognizeNumeral() {
  std::string lexeme = "";

  // If the first char is a plus or minus advance to the next char
  if (line[pos] == '+' or line[pos] == '-')
    lexeme += line[pos++];  

  // While the next char is a digit add it to the lexeme
  while(!isWhitespace(line[pos]) && pos < line.length()) {
    if(!std::isdigit(line[pos])) {
      break;
    }

    lexeme+=(line[pos]);
    pos++;
  }

  // Try turning the lexeme into a valid number
  // Throw an error if it is too big to be an integer
  int num;
  try {
    num  = std::stoi(lexeme);
  } catch (std::out_of_range& e) {
    return Token(NUM_ERR, lexeme);
  }

  return Token(Symbol::NUM, lexeme, num);
}

