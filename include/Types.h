#ifndef TYPES_H
#define TYPES_H

/**
 * Enum containing all the kinds of table entries
 */
enum Kind{CONSTANT=500, VARIABLE, K_ARRAY, PROCEDURE, UNDEFINED, K_RECORD};

/**
 * Enum containing all the Types of table entries
 */
enum Type{INTEGER=600, BOOLEAN, UNIVERSAL, T_FLOAT};

/**
 * Mapping the Kinds to strings representing the kinds
 */
const std::map<Kind, std::string> KindToString {
  {CONSTANT, "'Constant'"},
  {VARIABLE, "'Variable'"},
  {K_ARRAY, "'Array'"},
  {PROCEDURE, "'Procedure'"},
  {UNDEFINED, "'Undefined'"},
  {K_RECORD, "'Record'"}
};

/**
 * Mapping the Type to strings representing the types
 */
const std::map<Type, std::string> TypeToString {
  {INTEGER, "'Integer'"},
  {BOOLEAN, "'Boolean'"},
  {UNIVERSAL, "'Universal'"},
  {T_FLOAT, "'Float'"}
};


#endif
