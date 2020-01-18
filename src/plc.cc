#include "SymbolTable.h"
#include <iostream>

using namespace std;

int main() {
  cout << "Enter a line of lexemes" << std::endl;

  std::stringstream ss;
  while(std::cin >> ss) {}

  SymbolTable sym;
  Scanner scanner(ss, sym);
  Administration(std::cout, scanner);

  return 0;
}
