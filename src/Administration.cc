#include "Administration.h"
#include "Scanner.h"
#include <fstream>
#include <iostream>


Administration::Administration(std::ofstream& ofs, Scanner& sc)
    : fout(ofs), scanner(sc) {}


void Administration::newLine() {
  
}


void Administration::error(std::string text) {
  std::cerr << text << std::endl;
}


int Administration::scan() {
  return -1;
}
