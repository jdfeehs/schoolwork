//
// The following include files must come first.

#ifndef COOL_TREE_HANDCODE_H
#define COOL_TREE_HANDCODE_H

#include <iostream>
#include "tree.h"
#include "cool.h"
#include "stringtab.h"
#include "symtab.h"
#define yylineno curr_lineno;
extern int yylineno;

class program_class;
class class__class;
class method_class;
class formal_class;

inline Boolean copy_Boolean(Boolean b) {return b; }
inline void assert_Boolean(Boolean) {}
inline void dump_Boolean(ostream& stream, int padding, Boolean b)
	{ stream << pad(padding) << (int) b << "\n"; }

void dump_Symbol(ostream& stream, int padding, Symbol b);
void assert_Symbol(Symbol b);
Symbol copy_Symbol(Symbol b);

class Program_class;
typedef Program_class *Program;
class Class__class;
typedef Class__class *Class_;
class Feature_class;
typedef Feature_class *Feature;
class Formal_class;
typedef Formal_class *Formal;
class Expression_class;
typedef Expression_class *Expression;
class Case_class;
typedef Case_class *Case;

typedef list_node<Class_> Classes_class;
typedef Classes_class *Classes;
typedef list_node<Feature> Features_class;
typedef Features_class *Features;
typedef list_node<Formal> Formals_class;
typedef Formals_class *Formals;
typedef list_node<Expression> Expressions_class;
typedef Expressions_class *Expressions;
typedef list_node<Case> Cases_class;
typedef Cases_class *Cases;


#define Program_EXTRAS                          \
virtual void semant() = 0;			\
virtual void dump_with_types(ostream&, int) = 0;  \
SymbolTable<Symbol, class__class> clazz; \

//We want semant for errors
#define program_EXTRAS                          \
void semant();     				\
void dump_with_types(ostream&, int);            

//We define a type_check for each of the nodes
#define branch_EXTRAS \
Symbol type_check();\
void dump_with_types(std::ostream&,int);

//A class Phylum has no filename,parent,name,or features, however we define
//the 3 symbol tables that will be used for each node: clazz for classes,
//objs for objects, and functs for methods.
#define Class__EXTRAS                   \
virtual Symbol get_filename() = 0;      \
virtual void dump_with_types(ostream&,int) = 0; \
virtual Symbol get_parent() = 0;  \
virtual Symbol get_name() = 0; \
virtual Features get_features() = 0;\
SymbolTable<Symbol, class__class> clazz;  \
SymbolTable<Symbol, Symbol> objs; \
SymbolTable<Symbol, method_class> functs; \
 virtual void scan(SymbolTable<Symbol, Symbol>*, \
                    SymbolTable<Symbol, method_class>*, \
                    SymbolTable<Symbol, class__class>*) = 0; \
virtual Symbol type_check() = 0;


//For an actual constructor, we return the proper information.
//To populate the tables, we go through the features adding them
//to the method table (if they are methods) or object table
//(otherwise). Then we scan each of the features themselves 
//before saving the tables.
#define class__EXTRAS                                 \
Symbol get_name() { return name; }\
  Symbol get_filename() { return filename; }\
  Symbol get_parent() { return parent; }\
  Features get_features() { return features; }\
Symbol type_check();\
void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable) { \
for(int i = features->first(); features->more(i); i = features->next(i)) {\
      Feature_class* afeature = (Feature_class*)features->nth(i); \
      if (afeature->is_method())\
        ftable->addid(afeature->get_name(), (method_class *)afeature);\
      else \
        otable->addid(afeature->get_name(), afeature->get_type_addr());\
    }\
    for(int i = features->first(); features->more(i); i = features->next(i)) \
      features->nth(i)->scan(otable, ftable, ctable);\
    objs = *otable;\
    functs = *ftable;\
    clazz = *ctable;\
}\
void dump_with_types(std::ostream&, int);

//Feature phylum. Like we classes define the tables and methods but set them
//all to 0.
#define Feature_EXTRAS                                        \
virtual void dump_with_types(ostream&,int) = 0; 		\
virtual void scan(SymbolTable<Symbol, Symbol>*, 			\
                    SymbolTable<Symbol, method_class>*,		\
                    SymbolTable<Symbol, class__class>*) = 0; \
SymbolTable<Symbol, Symbol> objs; \
SymbolTable<Symbol, method_class> functs; \
SymbolTable<Symbol, class__class> clazz;\
virtual Boolean is_method() = 0;\
  virtual Symbol get_name() = 0;\
  virtual Symbol* get_type_addr() = 0;\
virtual Symbol type_check() = 0;

#define Feature_SHARED_EXTRAS                                       \
void dump_with_types(ostream&,int);    

//Formal phylum. See comments for Classes & Features phylums.
#define Formal_EXTRAS                              \
virtual void dump_with_types(ostream&,int) = 0; \
 virtual void scan(SymbolTable<Symbol, Symbol>*, \
                    SymbolTable<Symbol, method_class>*, \
                    SymbolTable<Symbol, class__class>*) = 0; \
SymbolTable<Symbol, Symbol> objs; \
SymbolTable<Symbol, method_class> functs; \
SymbolTable<Symbol, class__class> clazz; \
Symbol type_check();\
virtual Symbol get_type() = 0;

//Actual formal constructor: define proper procedure.
#define formal_EXTRAS                           \
void dump_with_types(ostream&,int); 	\
Symbol type_check();\
void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable) { \
    otable->addid(name, &type_decl); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }\
Symbol get_type() {return type_decl;}

#define Case_EXTRAS                             \
virtual void dump_with_types(ostream& ,int) = 0;

// Expression phylum. See all other phylums.
#define Expression_EXTRAS                    \
Symbol type;                                 \
Symbol get_type() { return type; }           \
Expression set_type(Symbol s) { type = s; return this; } \
virtual void dump_with_types(ostream&,int) = 0;  \
void dump_type(ostream&, int);               \
Expression_class() { type = (Symbol) NULL; } \
virtual void scan(SymbolTable<Symbol, Symbol>*, \
                    SymbolTable<Symbol, method_class>*, \
                    SymbolTable<Symbol, class__class>*) = 0; \
 SymbolTable<Symbol, Symbol> objs; \
 SymbolTable<Symbol, method_class> functs; \
 SymbolTable<Symbol, class__class> clazz; \
virtual Symbol type_check() = 0;\


#define Expression_SHARED_EXTRAS           \
void dump_with_types(ostream&,int);  \

//Actual method constructor. Define implementation.
//Like we classes, we call scan through all parts
//of the class (namely formals and the expression).
//After ALL the scanning is complete we save the
//tables.
 #define method_EXTRAS \
 Symbol get_name() { return name; }\
 Formals get_formals() { return formals; }\
 Symbol get_return() {return return_type; }\
 Symbol* get_type_addr() { return NULL; }\
 Boolean is_method() {return true;}\
 Symbol type_check();\
 void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable) { \
		for (int i = formals->first(); formals->more(i); \
         i = formals->next(i)) \
      formals->nth(i)->scan(otable, ftable, ctable);\
    expr->scan(otable, ftable, ctable); \
    objs = *otable;	\
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  //Implementation of attr constructor. If there is an initiation field then
  //scan it, otherwise just save the tables.
  #define attr_EXTRAS \
  Symbol type_check();\
  Boolean is_method() { return false; }\
  Symbol get_name() { return name; } \
  Symbol* get_type_addr() { return &type_decl; } \
   void scan(SymbolTable<Symbol, Symbol>* otable,\
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable) { \
    otable->addid(name, &type_decl); \
    if (init){ \
      init->scan(otable, ftable, ctable); \
    }\
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  //Default scan behovior for assignment constructor.
  #define assign_EXTRAS \
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable) { \
    expr->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  //Scan actuals (real parts of static dispatch), followed by the expression,
  //then save.
  #define static_dispatch_EXTRAS\
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable) { \
    for(int i = actual->first(); actual->more(i); i = actual->next(i)) \
      actual->nth(i)->scan(otable, ftable, ctable); \
    expr->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  // Dispatch constructor. Much like static dispatch... exactly the same.
  #define dispatch_EXTRAS\
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable) { \
    for(int i = actual->first(); actual->more(i); i = actual->next(i)) \
      actual->nth(i)->scan(otable, ftable, ctable); \
    expr->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }

  //Must have this for compilation (every node needs a scan).
  #define typcase_EXTRAS\
  Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable) { \
  int placeholder = 42;\
  }
  
  //Must have this for compilation (every node needs a scan).
  #define no_expr_EXTRAS\
  Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable) { \
  int placeholder = 42;\
  }
  
  /*Conditional constructor. Scan the predicate, followed by the then
  clause and finally the else expression. As always, save the table last.*/
  #define cond_EXTRAS \
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
       SymbolTable<Symbol, method_class>* ftable, \
       SymbolTable<Symbol, class__class>* ctable ) { \
    pred->scan(otable, ftable, ctable); \
    then_exp->scan(otable, ftable, ctable); \
    else_exp->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
	clazz = *ctable; \
  }
  
  //Scan is always the same: call any other components to scan and then save table.
  #define loop_EXTRAS \
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable) { \
    pred->scan(otable, ftable, ctable); \
    body->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  //Loop through elements of body (scanning them) and save tables.
  #define block_EXTRAS \
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable) { \
    for(int i = body->first(); body->more(i); i = body->next(i)) \
      body->nth(i)->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  //Let requires an entire different scope. Otherwise scan components & save tables.
  #define let_EXTRAS \
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable) { \
    otable->enterscope(); \
    otable->addid(identifier, &type_decl); \
    init->scan(otable, ftable, ctable); \
    body->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
    otable->exitscope(); \
  }
  
  //Scan components, save tables.
  #define plus_EXTRAS \
  	Symbol type_check();\
   void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable) { \
    e1->scan(otable, ftable, ctable); \
    e2->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
    //Scan components, save tables.
  #define sub_EXTRAS \
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable) { \
    e1->scan(otable, ftable, ctable); \
    e2->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
    //Scan components, save tables.
  #define mul_EXTRAS \
  	Symbol type_check();\
   void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable ) { \
    e1->scan(otable, ftable, ctable); \
    e2->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
    //Scan components, save tables.
  #define divide_EXTRAS \
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable ) { \
    e1->scan(otable, ftable, ctable); \
    e2->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
    //Scan components, save tables.
  #define neg_EXTRAS \
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable ) { \
    e1->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
    //Scan components, save tables.
  #define lt_EXTRAS\
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable ) { \
    e1->scan(otable, ftable, ctable); \
    e2->scan(otable, ftable, ctable); \
    objs = *otable;\
    functs = *ftable;\
    clazz = *ctable;\
  }
  
    //Scan components, save tables.
  #define eq_EXTRAS\
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable ) { \
    e1->scan(otable, ftable, ctable); \
    e2->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
    //Scan components, save tables.
  #define leq_EXTRAS\
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable ) { \
    e1->scan(otable, ftable, ctable); \
    e2->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable;\
  }
  
    //Scan components, save tables.
  #define comp_EXTRAS\
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable ) { \
    e1->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
    //Scan components, save tables.
  #define int_const_EXTRAS\
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable ) { \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
    //Scan components, save tables.
  #define bool_const_EXTRAS\
  	Symbol type_check();\
   void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable ) { \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
    //Scan components, save tables.
  #define string_const_EXTRAS\
  	Symbol type_check();\
   void scan(SymbolTable<Symbol, Symbol>* otable,\
            SymbolTable<Symbol, method_class>* ftable,\
            SymbolTable<Symbol, class__class>* ctable ) {\
    objs = *otable;\
    functs = *ftable;\
    clazz = *ctable;\
  }
  
    //Scan components, save tables.
  #define new__EXTRAS\
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable,\
            SymbolTable<Symbol, method_class>* ftable,\
            SymbolTable<Symbol, class__class>* ctable ) {\
    objs = *otable;\
    functs = *ftable;\
    clazz = *ctable;\
  }
  
    //Scan components, save tables.
  #define isvoid_EXTRAS\
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable,\
            SymbolTable<Symbol, method_class>* ftable,\
            SymbolTable<Symbol, class__class>* ctable ) {\
    e1->scan(otable, ftable, ctable);\
    objs = *otable;\
    functs = *ftable;\
    clazz = *ctable;\
  }
  
  
  
    //Save tables.
  #define object_EXTRAS\
  	Symbol type_check();\
  void scan(SymbolTable<Symbol, Symbol>* otable,\
            SymbolTable<Symbol, method_class>* ftable,\
            SymbolTable<Symbol, class__class>* ctable) {\
    objs = *otable;\
    functs = *ftable; \
    clazz = *ctable;\
  }
 
#endif
