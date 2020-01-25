#include "SymbolTable.h"
#include "Scanner.h"
#include "Administration.h"
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

int main() {
  std::ifstream fs("test/scannerOfficialTest.pl");

  SymbolTable sym;
  Scanner scanner(fs, sym);
  Administration admin(std::cout, scanner);

  admin.scan();

  return 0;
}
