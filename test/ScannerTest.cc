#include "gtest/gtest.h"
#include "SymbolTable.h"
#include "Scanner.h"
#include <fstream>
#include <string>

TEST(ScannerTests, testIsWhitespace) {
  std::ifstream input;
  SymbolTable symtab;
  Scanner tmp  = Scanner(input, symtab);
  EXPECT_TRUE(tmp.isWhitespace(' '));
  EXPECT_FALSE(tmp.isWhitespace('f'));
}

TEST(ScannerTests, testIsSpecial) {
  std::ifstream input;
  SymbolTable symtab;
  Scanner tmp  = Scanner(input, symtab);
  EXPECT_TRUE(tmp.isSpecial(':'));
  EXPECT_FALSE(tmp.isSpecial('_'));
}
