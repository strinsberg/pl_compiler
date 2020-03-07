#ifndef BLOCKTABLE_H
#define BLOCKTABLE_H
#include <vector>
#include <map>
#include "Types.h"

#define MAXBLOCK 10

struct TableEntry {
  TableEntry() : id(0), tkind(Kind::UNDEFINED), ttype(Type::UNIVERSAL),
                  size(0), val(0) {}
  TableEntry(int nid, Kind nkind, Type ntype, int nsize, int nval) :
              id(nid), tkind(nkind), ttype(ntype), size(nsize), val(nval) {}
  int id;
  Kind tkind;
  Type ttype;
  int size;
  int val;
};

class BlockTable {
public:
  BlockTable();
  ~BlockTable();
  bool search(int lookId);
  bool define(int nid, Kind nkind, Type ntype, int nsize, int nval);
  TableEntry find(int lookId, bool& error);
  bool pushBlock();
  void popBlock();
private:
  std::vector<std::map<int, TableEntry>> table;
  int blockLevel;
};

#endif
