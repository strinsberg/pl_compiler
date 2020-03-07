#include "Scanner.h"
#include <stdexcept>
#include <string>
#include <fstream>

Scanner::Scanner(std::istream &ifs) : fin(ifs), line(""), pos(0) {}


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

  // If the lexeme is not a keyword return an ID token
  // Otherwise return the token for the keyword.
  auto found = WordSym.find(lexeme);
  if (found == WordSym.end())
    return Token(Symbol::ID, lexeme);
  else
    return Token(found->second, lexeme);
}


Token Scanner::recognizeSpecial() {
  std::string lexeme = "";

  // If we can make a lexeme of size 2 do it 
  lexeme+=(line[pos++]);
  if (pos < line.size()) {
    std::string checkmap = lexeme + line[pos];
   
    if(SpecialSym.find(checkmap) != SpecialSym.end()) {
      pos++;
      return Token(SpecialSym.at(checkmap), checkmap);
    }
  }

  // If character is not a symbol return a char_err token
  // otherwise return the proper symbol token
  if(SpecialSym.find(lexeme) == SpecialSym.end()) {
    return Token(Symbol::CHAR_ERR, lexeme);
  } else {
    return Token(SpecialSym.at(lexeme), lexeme);
  }
}


Token Scanner::recognizeNumeral() {
  std::string lexeme = "";

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

