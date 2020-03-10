#ifndef BLOCKTABLE_H
#define BLOCKTABLE_H
#include <vector>
#include <map>
#include "Types.h"

#define MAXBLOCK 10

struct TableEntry;

class BlockTable {
public:
  BlockTable();
  bool search(int lookId);
  bool define(int nid, Kind nkind, Type ntype, int nsize, int nval);
  bool define(TableEntry& entry);
  TableEntry find(int lookId, bool& error);
  bool addFields(int idx, std::vector<TableEntry> fields);
  bool pushBlock();
  void popBlock();
private:
  std::vector<std::map<int, TableEntry>> table;
  int blockLevel;
};

struct TableEntry {
  TableEntry() : id(-1), tkind(Kind::UNDEFINED), ttype(Type::UNIVERSAL),
                  size(0), val(0) {}
  TableEntry(int nid, Kind nkind, Type ntype, int nsize, int nval) :
              id(nid), tkind(nkind), ttype(ntype), size(nsize), val(nval) {}
  int id;
  Kind tkind;
  Type ttype;
  int size;
  int val;
  BlockTable fields;
};

#endif
