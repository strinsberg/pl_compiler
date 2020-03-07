#ifndef TYPES_H
#define TYPES_H

enum Kind{CONSTANT=500, VARIABLE, K_ARRAY, PROCEDURE, UNDEFINED, K_RECORD};
enum Type{INTEGER=600, BOOLEAN, UNIVERSAL, T_FLOAT};

const std::map<Kind, std::string> KindToString {
  {CONSTANT, "'Constant'"},
  {VARIABLE, "'Variable'"},
  {K_ARRAY, "'Array'"},
  {PROCEDURE, "'Procedure'"},
  {UNDEFINED, "'Undefined'"},
  {K_RECORD, "'Record'"}
};

const std::map<Type, std::string> TypeToString {
  {INTEGER, "'Integer'"},
  {BOOLEAN, "'Boolean'"},
  {UNIVERSAL, "'Universal'"},
  {T_FLOAT, "'Float'"}
};


#endif
