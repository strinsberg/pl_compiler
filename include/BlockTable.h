#ifndef BLOCKTABLE_H
#define BLOCKTABLE_H
#include <vector>
#include <map>
#include "Types.h"


#define MAXBLOCK 10

// This should probably be a class now in it's own file
struct TableEntry {
  TableEntry() : id(-1), tkind(Kind::UNDEFINED), ttype(Type::UNIVERSAL),
                  size(0), val(0) {}

  TableEntry(int nid, Kind nkind, Type ntype, int nsize, int nval) :
              id(nid), tkind(nkind), ttype(ntype), size(nsize), val(nval) {}

  int findEntry(TableEntry& entry) {return findEntry(entry.id);}

  int findEntry(int id) {
    for (int i = 0; i < (int)entries.size(); i++) {
      if (id == entries[i].id)
        return i;
    }
    return -1;
  }

  int id;
  Kind tkind;
  Type ttype;
  int size;
  int val;
  std::vector<TableEntry> entries;
};


class BlockTable {
public:
  BlockTable();
  bool search(int lookId);
  bool define(int nid, Kind nkind, Type ntype, int nsize, int nval);
  bool define(TableEntry& entry);
  TableEntry find(int lookId, bool& error);
  bool pushBlock();
  void popBlock();
private:
  std::vector<std::map<int, TableEntry>> table;
  int blockLevel;
};



#endif
