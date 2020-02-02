#include "SymbolTable.h"
#include "Scanner.h"
#include "Administration.h"
#include "Parser.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <getopt.h>

using namespace std;

int main(int argc, char** argv) {
  int opt;

  std::string outfile;
  while((opt = getopt(argc, argv, "o:")) != -1) {
    switch (opt) {
      case 'o':
        outfile = std::string(optarg);
        break;
      default:
        cerr << "Invalid Argument: " << (char)opt << endl;
        return 1;
    }
  }

  if (optind >= argc) {
    cerr << "No filename given" << endl;
    return 2;
  }

  std::string filename(argv[optind]);
  std::ifstream fs(filename);

  if (!fs.is_open()) {
    cerr << "Invalid input file: " << filename << endl;
    return 3;
  }

  if (outfile == "")
    outfile = "pl.out";
  std::ofstream ofs(outfile);

  SymbolTable sym;
  Scanner scanner(fs, sym);
  Administration admin(ofs, scanner);
  Parser parser(admin);

  parser.parse();

  return 0;
}
