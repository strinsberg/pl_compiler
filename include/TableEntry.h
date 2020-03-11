#ifndef TABLEENTRY_H
#define TABLEENTRY_H
#include <vector>
#include "Types.h"

class TableEntry {
public:

  /**
   * Default Constructor that creates a empty table entry set to default values
   */
  TableEntry() : id(-1), tkind(Kind::UNDEFINED), ttype(Type::UNIVERSAL),
                  size(0), val(0) {}

  /**
   * Overloaded constructor that creates the table entry with the input values
   * @param nid The id of the table entry
   * @param nkind The Kind of the table entry
   * @param ntype The Type of the table entry
   * @param nsize The memory size required by the table entry
   * @param nval The value of the table entry
   */
  TableEntry(int nid, Kind nkind, Type ntype, int nsize, int nval) :
              id(nid), tkind(nkind), ttype(ntype), size(nsize), val(nval) {}

  /**
   * Check if the table entry input is a param or field of a procedure or record
   * @param entry The table entry being searched for
   */
  int findEntry(TableEntry& entry) {return findEntry(entry.id);}

  /**
   * Overloaded function to check if a table entry is a param or field using its
   * id of a procedure or record
   * @param The id of the table entry being searched for
   */
  int findEntry(int id) {
    for (int i = 0; i < (int)entries.size(); i++) {
      if (id == entries[i].id)
        return i;
    }
    return -1;
  }

  /**
   * The table entry id
   */
  int id;

    /**
     * The kind of table entry
     */
  Kind tkind;

    /**
     * The type of the table entry
     */
  Type ttype;

    /**
     * The size of the required memory for the table entry
     */
  int size;

    /**
     * The value of the table entry
     */
  int val;

    /**
     * The field/params of a record/procedure respectively
     */
  std::vector<TableEntry> entries;
};

#endif
