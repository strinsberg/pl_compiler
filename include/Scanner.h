#ifndef SCANNER_H
#define SCANNER_h

#include "Token.h"

class Scanner {
 public:
  Scanner(std::string text);
  Token getToken();

 private:
  size_t pos;  // Position in the input
};

#endif
