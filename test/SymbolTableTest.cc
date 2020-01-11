#include "gtest/gtest.h"
#include "SymbolTable.h"
#include <string>

#define MOD 307
#define MODEQ 1e9

TEST(SymbolTableTests, test_hash_MOD) {
  SymbolTable symtab;
  std::string R = "Ricky";
  std::string r = "ricky";
  int Ricky = symtab.hash(R, MOD);
  int ricky = symtab.hash(r, MOD);
  //EXPECT_EQ(145, Ricky);
  //EXPECT_EQ(10 , ricky);
}
