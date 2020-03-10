#include "BlockTable.h"
#include "Types.h"
#include <map>
#include <vector>


BlockTable::BlockTable() : blockLevel(0) {
}


bool BlockTable::search(int lookId) {
  return table.back().find(lookId) != table.back().end();
}


bool BlockTable::define(int nid, Kind nkind, Type ntype, int nsize, int nval) {
  if(!search(nid)){
    TableEntry newTE = TableEntry(nid, nkind, ntype, nsize, nval);
    table.back()[nid] = newTE;
    return true;
  }
  return false;
}

bool BlockTable::define(TableEntry& entry) {
  if (!search(entry.id)) {
    table.back()[entry.id] = {entry.id, entry.tkind, entry.ttype, entry.size, entry.val};
    table.back()[entry.id].fields = entry.fields;
    return true;
  } else {
    return false;
  }
}


TableEntry BlockTable::find(int lookId, bool& error) {
  for(auto it = table.rbegin(); it != table.rend(); it++) {
    if(it->find(lookId) != it->end()) {
      error = false;
      return (*it)[lookId];
    }
  }
  error =  true;
  return TableEntry();
}


bool BlockTable::pushBlock() {
  if(table.size() < MAXBLOCK) {
    blockLevel++;
    table.emplace_back();
    return true;
  }
  return false;

}


void BlockTable::popBlock() {
  table.pop_back();
}

bool BlockTable::addFields(int idx, std::vector<TableEntry> field) {
  bool err = false;
  for(auto s : field) {
    auto it = table.back().find(idx);
    if(it != table.back().end())
      it->second.fields.define(s.id, s.tkind, s.ttype, s.size, s.val);
    else {
      err = true;
    }
  }
  return err;
}
