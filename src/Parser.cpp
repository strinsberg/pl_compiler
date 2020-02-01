#include "Parser.h"

Parser::Parser(Administration& a) : admin(a) {}

void Parser::parse() {

}

void Parser::match(Symbol sym) {
  if (sym == look.getSymbol()) {
    std::cout << look.toString() << std::endl;  // For debug
    look = scanner.getToken();

  } else {
    admin.syntaxError(sym, look.getSymbol());
  }
}
