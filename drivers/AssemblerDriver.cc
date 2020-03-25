#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>

#include "parser.h"
#include "assembler.h"

using namespace std;

int main(int argc, char *argv[])
{
   if (argc != 3 && argc != 4) {
      cout << "Usage: " << argv[0] << " [-d] source-program machine-program"
	   << endl;
      return -1;
   }
   if (argc == 3) {
      // Open the input file.
      ifstream inputfile(argv[1]);
      if (!inputfile) {
	 cerr << "Input file " << argv[1] << " could not be opened" << endl;
	 return 1;
      }
      // Generate the assembly filename.
      char intermediate[1024];
      strcat(intermediate, argv[2]);
      strcat(intermediate, ".asm");
      // Open the intermediate assembly file for output.
      ofstream assemblyOutfile;
      assemblyOutfile.open(intermediate, ios::out);
      if (!assemblyOutfile) {
	 cerr << "Intermediate file " << intermediate<< " could not be written"
	      << endl;
	 return 1;
      }
      // Start the compiler running.
      Parser compiler(inputfile, assemblyOutfile);
      int errors = compiler.parse();
      inputfile.close();
      assemblyOutfile.close();
      // Only continue if there are no errors.
      if (errors == 0) {
	 // Open the assembly file for input.
	 ifstream assemblyInfile;
	 assemblyInfile.open(intermediate, ios::in);
	 if (!assemblyInfile) {
	    cerr << "Intermediate file " << intermediate<< " could not be read"
		 << endl;
	    return 1;
	 }
	 // Open the output file.
	 ofstream outputfile(argv[2]);
	 if (!outputfile) {
	    cerr << "Output file " << argv[2] << " could not be opened"<< endl;
	    return 1;
	 }
	 Assembler assembler(assemblyInfile, outputfile);
	 assembler.firstPass();
	 // Rewind to the start of the file.
	 assemblyInfile.seekg(0);
	 assembler.secondPass();
	 assemblyInfile.close();
	 outputfile.close();
      }
      // Throw out the assembly file.
      // remove(intermediate);
   }
   // Basically a copy of the above.
   else {
      if ((strcmp(argv[1], "-d") != 0)) {
	 cout << "Usage: " << argv[0] << " [-d] source-program machine-program"
	      << endl;
	 return -1;
      }
      ifstream inputfile(argv[2]);
      if (!inputfile) {
	 cerr << "Input file " << argv[2] << " could not be opened" << endl;
	 return 1;
      }
      char intermediate[1024];
      strcpy(intermediate,"PLFile_");
      strcat(intermediate, argv[3]);
      strcat(intermediate, ".asm");
      ofstream assemblyOutfile;
      assemblyOutfile.open(intermediate, ios::out);
      if (!assemblyOutfile) {
	 cerr << "Intermediate file " << intermediate<< " could not be written"
	      << endl;
	 return 1;
      }
      Parser compiler(inputfile, assemblyOutfile);
      int errors = compiler.parse();
      inputfile.close();
      assemblyOutfile.close();
      if (errors == 0) {
	 ifstream assemblyInfile;
	 assemblyInfile.open(intermediate, ios::in);
	 if (!assemblyInfile) {
	    cerr << "Intermediate file " << intermediate<< " could not be read"
		 << endl;
	    return 1;
	 }
	 ofstream outputfile(argv[3]);
	 if (!outputfile) {
	    cerr << "Output file " << argv[3] << " could not be opened"<< endl;
	    return 1;
	 }
	 Assembler assembler(assemblyInfile, outputfile);
	 assembler.firstPass();
	 assemblyInfile.seekg(0);
	 assembler.secondPass();
	 assemblyInfile.close();
	 outputfile.close();
      }
      else
	 // Only throw out the assembly file if the parser had errors.
        ;
//	 remove(intermediate);
   }
   return 0;
}
