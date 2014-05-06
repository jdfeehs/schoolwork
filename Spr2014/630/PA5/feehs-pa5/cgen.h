#include <assert.h>
#include <stdio.h>
#include "emit.h"
#include "cool-tree.h"
#include "symtab.h"
#include <map>
#include <string>

enum Basicness     {Basic, NotBasic};
#define TRUE 1
#define FALSE 0

class CgenClassTable;
typedef CgenClassTable *CgenClassTableP;

class CgenNode;
typedef CgenNode *CgenNodeP;

struct class_info
{
  int classtag; /*The class tag number; assigned by the compiler */
  int strconst_num; /*The number of the strconst that refers to this class */ 
};

class CgenClassTable : public SymbolTable<Symbol,CgenNode> {
private:
   List<CgenNode> *nds;
   ostream& str;
   int stringclasstag;
   int intclasstag;
   int boolclasstag;
   std::map<Symbol,class_info * > classinfo_map;
   int curclasstag;
   
// The following methods collect data for later use
   void collect_dispatch_tables();
   void collect_proto_object();
// The following methods emit code for
// constants and global declarations.

   void code_global_data();
   void code_global_text();
   void code_bools(int);
   void code_select_gc();
   void code_constants();
// The following methods are added by Josh
// They also emit code for their specific parts
   void code_classtab();
   void code_dispatch_tables();
   void code_prototype_objects();
   void code_object_table();
   void code_init_methods();
   void code_methods();
// The following creates an inheritance graph from
// a list of classes.  The graph is implemented as
// a tree of `CgenNode', and class names are placed
// in the base class symbol table.

   void install_basic_classes();
   void install_class(CgenNodeP nd);
   void install_classes(Classes cs);
   void build_inheritance_tree();
   void set_relations(CgenNodeP nd);
public:
   CgenClassTable(Classes, ostream& str);
   void code(Classes classes);
   CgenNodeP root();
};


class CgenNode : public class__class {
private: 
   CgenNodeP parentnd;                        // Parent of class
   List<CgenNode> *children;                  // Children of class
   Basicness basic_status;                    // `Basic' if class is basic
                                              // `NotBasic' otherwise

public:
   CgenNode(Class_ c,
            Basicness bstatus,
            CgenClassTableP class_table);

   void add_child(CgenNodeP child);
   List<CgenNode> *get_children() { return children; }
   void set_parentnd(CgenNodeP p);
   CgenNodeP get_parentnd() { return parentnd; }
   int basic() { return (basic_status == Basic); }
   //Added by Josh
   std::map<std::string,int>  dispatch_info; //Stores info needed for the dispatch tables
   int populate_dispatch_table();
   int method_count;
   void populate_prototype_info();
   int classtag; //the number of the classtag
   int strconst_num;
   int obj_size; //The value needed for the prototype object; the object's size
   std::map<std::string,int> attr_index; //maps an attribute name to its index
   std::map<std::string,Symbol> attr_type; //maps an attribute name to its type
   std::map<std::string,Expression> attr_expression; //maps an attribute name to its init expression

};

class BoolConst 
{
 private: 
  int val;
 public:
  BoolConst(int);
  void code_def(ostream&, int boolclasstag);
  void code_ref(ostream&) const;
};

