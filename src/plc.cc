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
  bool verbose = false;

  // Deal with command line arguments
  std::string outfile;
  while((opt = getopt(argc, argv, "o:v")) != -1) {
    switch (opt) {
      case 'o':
        outfile = std::string(optarg);
        break;
      case 'v':
        verbose = true;
        break;
      default:
        cerr << "Invalid Argument: " << (char)opt << endl;
        return 1;
    }
  }

  // Check if there was an argument given for the file to compile
  if (optind >= argc) {
    cerr << "No filename given" << endl;
    return 2;
  }

  // Open up the file to read from
  std::string filename(argv[optind]);
  std::ifstream fs(filename);

  if (!fs.is_open()) {
    cerr << "Invalid input file: " << filename << endl;
    return 3;
  }

  // Open up the file to write to
  if (outfile == "")
    outfile = "pl.out";
  std::ofstream ofs(outfile);

  // Create necessary components
  Scanner scanner(fs);
  Administration admin(ofs, scanner, verbose);
  Parser parser(admin);

  // Run the compiler
  parser.parse();

  return 0;
}
