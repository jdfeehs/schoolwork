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
SymbolTable<Symbol, Class__class> clazz;



#define program_EXTRAS                          \
void semant();     				\
void dump_with_types(ostream&, int);            

#define Class__EXTRAS                   \
virtual Symbol get_filename() = 0;      \
virtual void dump_with_types(ostream&,int) = 0; \
virtual Symbol get_parent() = 0;  \
virtual Symbol get_name() = 0; \
SymbolTable<Symbol, Class__class> clazz;  \
SymbolTable<Symbol, Symbol> objs; \
SymbolTable<Symbol, method_class> functs;

#define class__EXTRAS                                 \
Symbol get_filename() { return filename; }             \
void dump_with_types(ostream&,int);   \
Symbol get_parent() { return parent; } \
Symbol get_name() { return name; } \
void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable) { \
    for(int i = features->first(); features->more(i); i = features->next(i)) { \
      Feature_class* a_feature = (Feature_class*)features->nth(i); \
      if (a_feature->is_method()) \
        ftable->addid(a_feature->get_name(), (method_class *)a_feature); \
      else \
        otable->addid(a_feature->get_name(), a_feature->get_type_addr()); \
    } \
    for(int i = features->first(); features->more(i); i = features->next(i)) \
      features->nth(i)->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }

#define Feature_EXTRAS                                        \
virtual void dump_with_types(ostream&,int) = 0; 		\
virtual void scan(SymbolTable<Symbol, Symbol>*, 			\
                    SymbolTable<Symbol, method_class>*,		\
                    SymbolTable<Symbol, Class__class>*) = 0; \
SymbolTable<Symbol, Symbol> objs; \
SymbolTable<Symbol, method_class> functs; \
SymbolTable<Symbol, Class__class> clazz;

#define Feature_SHARED_EXTRAS                                       \
void dump_with_types(ostream&,int);    





#define Formal_EXTRAS                              \
virtual void dump_with_types(ostream&,int) = 0; \
 virtual void scan(SymbolTable<Symbol, Symbol>*, \
                    SymbolTable<Symbol, method_class>*, \
                    SymbolTable<Symbol, Class__class>*) = 0; \
SymbolTable<Symbol, Symbol> objs; \
SymbolTable<Symbol, method_class> functs; \
SymbolTable<Symbol, Class__class> clazz;


#define formal_EXTRAS                           \
void dump_with_types(ostream&,int); \
void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable) { \
    otable->addid(name, &type_decl); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }


#define Case_EXTRAS                             \
virtual void dump_with_types(ostream& ,int) = 0;


#define branch_EXTRAS                                   \
void dump_with_types(ostream& ,int);


#define Expression_EXTRAS                    \
Symbol type;                                 \
Symbol get_type() { return type; }           \
Expression set_type(Symbol s) { type = s; return this; } \
virtual void dump_with_types(ostream&,int) = 0;  \
void dump_type(ostream&, int);               \
Expression_class() { type = (Symbol) NULL; } \
virtual void scan(SymbolTable<Symbol, Symbol>*, \
                    SymbolTable<Symbol, method_class>*, \
                    SymbolTable<Symbol, Class__class>*) = 0; \
 SymbolTable<Symbol, Symbol> objs; \
 SymbolTable<Symbol, method_class> functs; \
 SymbolTable<Symbol, Class__class> clazz;


#define Expression_SHARED_EXTRAS           \
void dump_with_types(ostream&,int);  \
void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable) { \
    expr->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }

  #define method_EXTRAS \
  Boolean is_method() {return true;} \
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable) { \
    otable->enterscope(); \
    for (int i = formals->first(); formals->more(i); \
         i = formals->next(i)) \
      formals->nth(i)->scan(otable, ftable, ctable);\
    expr->scan(otable, ftable, ctable); \
    objs = *otable;	\
    functs = *ftable; \
    clazz = *ctable; \
    otable->exitscope(); \
  }
  
  #define attr_EXTRAS \
  Boolean is_method() { return false; }\
  Symbol get_name() { return name; } \
   void scan(SymbolTable<Symbol, Symbol>* otable,\
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable) { \
    if (init) \
      init->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  #define branch_EXTRAS \
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable) { \
    otable->enterscope(); \
    otable->addid(name, &type_decl); \
    expr->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
    otable->exitscope(); \
  }
  
  #define assign_EXTRAS \
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable) { \
    expr->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  #define static_dispatch_EXTRAS\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable) { \
    for(int i = actual->first(); actual->more(i); i = actual->next(i)) \
      actual->nth(i)->scan(otable, ftable, ctable); \
    expr->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  #define cond_EXTRAS \
  void scan(SymbolTable<Symbol, Symbol>* otable, \
       SymbolTable<Symbol, method_class>* ftable, \
       SymbolTable<Symbol, Class__class>* ctable ) { \
    pred->scan(otable, ftable, ctable); \
    then_exp->scan(otable, ftable, ctable); \
    else_exp->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
	clazz = *ctable; \
  }
  
  #define loop_EXTRAS \
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable) { \
    pred->scan(otable, ftable, ctable); \
    body->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  #define typcase_EXTRAS \
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable) { \
    expr->scan(otable, ftable, ctable); \
    for(int i = cases->first(); cases->more(i); i = cases->next(i)) \
      cases->nth(i)->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  #define block_EXTRAS \
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable) { \
    for(int i = body->first(); body->more(i); i = body->next(i)) \
      body->nth(i)->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  #define let_EXTRAS \
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable) { \
    otable->enterscope(); \
    otable->addid(identifier, &type_decl); \
    init->scan(otable, ftable, ctable); \
    body->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
    otable->exitscope(); \
  }
  
  #define plus_EXTRAS \
   void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable) { \
    e1->scan(otable, ftable, ctable); \
    e2->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  #define sub_EXTRAS \
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable) { \
    e1->scan(otable, ftable, ctable); \
    e2->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  #define mul_EXTRAS \
   void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable ) { \
    e1->scan(otable, ftable, ctable); \
    e2->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  #define divide_EXTRAS \
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, class__class>* ctable ) { \
    e1->scan(otable, ftable, ctable); \
    e2->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  #define neg_EXTRAS \
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable ) { \
    e1->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  #define lt_EXTRAS\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable ) { \
    e1->scan(otable, ftable, ctable); \
    e2->scan(otable, ftable, ctable); \
    objs = *otable;\
    functs = *ftable;\
    clazz = *ctable;\
  }
  
  #define eq_EXTRAS\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable ) { \
    e1->scan(otable, ftable, ctable); \
    e2->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  #define leq_EXTRAS\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable ) { \
    e1->scan(otable, ftable, ctable); \
    e2->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable;\
  }
  
  #define comp_EXTRAS\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable ) { \
    e1->scan(otable, ftable, ctable); \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  #define int_const_EXTRAS\
  void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable ) { \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  #define bool_const_EXTRAS\
   void scan(SymbolTable<Symbol, Symbol>* otable, \
            SymbolTable<Symbol, method_class>* ftable, \
            SymbolTable<Symbol, Class__class>* ctable ) { \
    objs = *otable; \
    functs = *ftable; \
    clazz = *ctable; \
  }
  
  #define string_const_EXTRAS\
   void scan(SymbolTable<Symbol, Symbol>* otable,\
            SymbolTable<Symbol, method_class>* ftable,\
            SymbolTable<Symbol, Class__class>* ctable ) {\
    objs = *otable;\
    functs = *ftable;\
    clazz = *ctable;\
  }
  
  #define new__EXTRAS\
  void scan(SymbolTable<Symbol, Symbol>* otable,\
            SymbolTable<Symbol, method_class>* ftable,\
            SymbolTable<Symbol, Class__class>* ctable ) {\
    objs = *otable;\
    functs = *ftable;\
    clazz = *ctable;\
  }
  
  #define isvoid_EXTRAS\
  void scan(SymbolTable<Symbol, Symbol>* otable,\
            SymbolTable<Symbol, method_class>* ftable,\
            SymbolTable<Symbol, Class__class>* ctable ) {\
    e1->scan(otable, ftable, ctable);\
    objs = *otable;\
    functs = *ftable;\
    clazz = *ctable;\
  }
  
  #define no_expr_EXTRAS\
  void scan(SymbolTable<Symbol, Symbol>* otable,\
            SymbolTable<Symbol, method_class>* ftable,\
            SymbolTable<Symbol, Class__class>* ctable ) {\
    e1->scan(otable, ftable, ctable);\
    objs = *otable;\
    functs = *ftable;\
    clazz = *ctable;\
  }
  
  #define object_EXTRAS\
  void scan(SymbolTable<Symbol, Symbol>* otable,\
            SymbolTable<Symbol, method_class>* ftable,\
            SymbolTable<Symbol, Class__class>* ctable) {\
    objs = *otable;\
    functs = *ftable; \
    clazz = *ctable;\
  }
  
#endif
