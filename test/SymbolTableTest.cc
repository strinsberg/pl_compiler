#include "gtest/gtest.h"
#include "SymbolTable.h"
#include <string>

#define MOD 307
#define MODEQ 1e9

TEST(SymbolTableTests, test_hash_MOD) {
  SymbolTable symtab;
  int Ricky = symtab.hash("Ricky");
  int ricky = symtab.hash("ricky");
  EXPECT_EQ(145, Ricky);
  EXPECT_EQ(177, ricky);
}
