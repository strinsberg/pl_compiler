#include "gtest/gtest.h"
#include "SymbolTable.h"
#include "Scanner.h"
#include <fstream>
#include <string>

TEST(SymbolTableTests, test_hash_MOD) {
  std::ifstream input;
  SymbolTable symtab;
  Scanner tmp  = Scanner(input, symtab);
  EXPECT_TRUE(tmp.isWhitespace(' '));
  EXPECT_FALSE(tmp.isWhitespace('f'));

}
