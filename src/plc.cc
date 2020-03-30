#include "SymbolTable.h"
#include "Scanner.h"
#include "Administration.h"
#include "Parser.h"
#include "Assembler.h"
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
        return EXIT_FAILURE;
    }
  }

  // Check if there was an argument given for the file to compile
  if (optind >= argc) {
    cerr << "No filename given" << endl;
    return EXIT_FAILURE;
  }

  // Open up the file to read from
  std::string filename(argv[optind]);
  std::ifstream fs(filename);

  if (!fs.is_open()) {
    cerr << "Invalid input file: " << filename << endl;
    return EXIT_FAILURE;
  }

  // Create an intermediate representation file
  std::ofstream asmfile("pl.asm");

  // Create necessary components
  SymbolTable symTab;
  Scanner scanner(fs, symTab);
  Administration admin(asmfile, scanner, verbose);
  Parser parser(admin);

  // Run the compiler
  parser.parse();
  asmfile.close();

  // If there are errors then quit. If MAX_ERRORS are reached the admin will
  // exit(EXIT_FAILURE) during parsing.
  if (admin.error_count() > 0) {
    std::cout << "***Compilation failure***" << std::endl;
    return EXIT_FAILURE;
  }

  // Open up the file to write to
  if (outfile == "")
    outfile = "pl.out";
  std::ofstream ofs(outfile);

  // Open the input file to read from
  std::ifstream ifs("pl.asm");

  // Run the assembler 
  Assembler assembler(ifs, ofs);
  assembler.firstPass();
  ifs.seekg(0);
  assembler.secondPass();

  ifs.close();
  ofs.close();

  return EXIT_SUCCESS;
}
