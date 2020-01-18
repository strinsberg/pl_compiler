#include "SymbolTable.h"
#include "Scanner.h"
#include "Administration.h"
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

int main() {
  cout << "Enter a line of lexemes" << std::endl;

  std::string temp;
  getline(std::cin, temp);
  std::ifstream fs(temp);

  SymbolTable sym;
  Scanner scanner(fs, sym);
  Administration admin(std::cout, scanner);

  admin.scan();

  return 0;
}
