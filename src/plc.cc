#include "SymbolTable.h"
#include "Scanner.h"
#include "Administration.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <getopt>

using namespace std;

int main(int argc, char** argc) {
  int opt;

  while((opt = getopt(argc, argv, "")) != 1) {
    switch (opt) {
      default:
        break;
    }
  }


  std::ifstream fs("test/scannerTest.pl");

  SymbolTable sym;
  Scanner scanner(fs, sym);
  Administration admin(std::cout, scanner);

  admin.scan();

  return 0;
}
