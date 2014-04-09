#ifndef SEMANT_H_
#define SEMANT_H_

#include <assert.h>
#include <iostream>  
#include "cool-tree.h"
#include "stringtab.h"
#include "symtab.h"
#include "list.h"
#include <map>
#include <set>
#define TRUE 1
#define FALSE 0

//We use these codes when reporting errors.
enum code{REDEFINITION,CYCLE,DUPLICATE,INHERIT,UNDECLARED,TYPE};

class ClassTable;
typedef ClassTable *ClassTableP;

// This is a structure that may be used to contain the semantic
// information such as the inheritance graph.  You may use it or not as
// you like: it is only here to provide a container for the supplied
// methods.

class ClassTable {
private:
  int semant_errors;
  void install_basic_classes();
 
  ostream& error_stream;
  //Table for each class, such that the comments hold below.
  SymbolTable<Symbol,class__class> *class_table;  //class name -> class address
  SymbolTable<Symbol,method_class> *method_table; //method name -> method address
  SymbolTable<Symbol,Symbol>       *object_table; //object name -> static type

public:
  ClassTable(Classes);
  int errors() { return semant_errors; }
  ostream& semant_error();
  ostream& semant_error(Class_ c);
  ostream& semant_error(Class_ c,code error_type);
  ostream& semant_error(Symbol s,code error_type);
  ostream& semant_error(Symbol filename, tree_node *t);
  void build_inheritance_graph(Classes classes);
  void traverse(Symbol symbol);
  Symbol least_common_parent(Symbol a, Symbol b);
  void collect_declarations();
};


#endif

