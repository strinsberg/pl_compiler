#ifndef BLOCKTABLE_H
#define BLOCKTABLE_H
#include <vector>
#include <map>
#include "TableEntry.h"
#include "Types.h"


#define MAXBLOCK 10


class BlockTable {
public:

  /**
   * Default Constructor for a BlockTable
   */
  BlockTable();

  /**
   * Searches the current level of the blocktable for a table entry
   * @param lookID The id of the table entry being searched for
   */
  bool search(int lookId);

    /**
     * Creates a new table entry and puts it into the current block if it
     * doesnt already exist
     * @param nid The id of the table entry
     * @param nkind The kind of the table entry
     * @param ntype The type of the table entry
     * @param nsize The memory size required by the table entry
     * @param nval The value of the table entry
     */
  bool define(int nid, Kind nkind, Type ntype, int nsize, int nval);

    /**
     * Overloaded define function that takes in a table entry to define
     * @param entry The table entry that will be define
     */
  bool define(TableEntry& entry);

    /**
     * Searches the entire blocktable for the table entry
     * @param lookId The id of the table entry being searched for
     * @param error The error check for when the table entry does not exist
     */
  TableEntry find(int lookId, bool& error);

    /**
     * Creates and pushes a new blocktable onto the currect block
     */
  bool pushBlock();

    /**
     * Removes the highest level (most recent) block of the blocktable
     */
  void popBlock();
private:

    /**
     * Vector of maps storing the table entries for a block (the block table)
     */
  std::vector<std::map<int, TableEntry>> table;

    /**
     * The current blocklevel
     */
  int blockLevel;
};



#endif
