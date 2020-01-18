#include "Scanner.h"
#include <string>
#include <fstream>

Scanner::Scanner(std::ifstream &instream, SymbolTable &symboltable) {
  inputfile = instream;
  symtable = symboltable;
}
/*
Token Scanner::getToken() {
  std::istringstream iss = "";
  std::string charCluster;
  if(iss >> charCluster)

  }

}
*/

bool Scanner::isWhitespace(char inchar) {
  if (inchar == ' ') {
    return true;
  }
  return false;
}

bool Scanner::isSpecial(char inchar) {
  std::string specials = ".,;[]&|~<=>+-*/\():$"
  std::size_t found = specials.find(inchar);
  if(found != std::string npos) {
    return true;
  }
  return false;
}

/*
Token Scanner::recognizeName() {
}

Token Scanner::recognizeSpecial() {
}

Token Scanner::recognizeNumeral() {
}
*/
