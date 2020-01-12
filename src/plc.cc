#include "SymbolTable.h"
#include <iostream>

using namespace std;

int main() {
  SymbolTable st;
  cout << st.hash("Ricky") << endl;
  cout << st.hash("ricky") << endl;

}
