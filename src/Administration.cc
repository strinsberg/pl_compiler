#include "Administration.h"
#include "Scanner.h"
#include <fstream>


Administration::Administration(
    std::ifstream& ifs, std::ofstream& ofs, Scanner& sc)
    : fin(ifs), fout(ofs), scanner(sc) {}


void Administration::newLine() {
  
}


void Administration::error(std::string text) {
  std::cerr << text << std::endl;
}


int Administration::scan() {
  return -1;
}
