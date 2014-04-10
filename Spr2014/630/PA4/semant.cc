

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"
#include <map>
#include <string>
#include <set>
#include <iostream>
extern int semant_debug;
extern char *curr_filename;

extern int node_lineno;
/*
 * The following declarations are the global map and set.
 * The map stores mappings from child -> parent
 * In order to get the parent class of a child, use:
 * map[child Symbol]    --> this will return the Symbol of its parent
 */
std::map<Symbol,Symbol> map;
std::set<Symbol> children;
SymbolTable<Symbol,method_class> *method_table=new SymbolTable<Symbol, method_class>();
SymbolTable<Symbol,Symbol> *object_table=new SymbolTable<Symbol, Symbol>();
SymbolTable<Symbol,class__class> *class_table=new SymbolTable<Symbol, class__class>();
ClassTable *classtable;

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
static Symbol 
    arg,
    arg2,
    Bool,
    concat,
    cool_abort,
    copy,
    Int,
    in_int,
    in_string,
    IO,
    length,
    Main,
    main_meth,
    No_class,
    No_type,
    Object,
    out_int,
    out_string,
    prim_slot,
    self,
    SELF_TYPE,
    Str,
    str_field,
    substr,
    type_name,
    val;



//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
    arg         = idtable.add_string("arg");
    arg2        = idtable.add_string("arg2");
    Bool        = idtable.add_string("Bool");
    concat      = idtable.add_string("concat");
    cool_abort  = idtable.add_string("abort");
    copy        = idtable.add_string("copy");
    Int         = idtable.add_string("Int");
    in_int      = idtable.add_string("in_int");
    in_string   = idtable.add_string("in_string");
    IO          = idtable.add_string("IO");
    length      = idtable.add_string("length");
    Main        = idtable.add_string("Main");
    main_meth   = idtable.add_string("main");
    //   _no_class is a symbol that can't be the name of any 
    //   user-defined class.
    No_class    = idtable.add_string("_no_class");
    No_type     = idtable.add_string("_no_type");
    Object      = idtable.add_string("Object");
    out_int     = idtable.add_string("out_int");
    out_string  = idtable.add_string("out_string");
    prim_slot   = idtable.add_string("_prim_slot");
    self        = idtable.add_string("self");
    SELF_TYPE   = idtable.add_string("SELF_TYPE");
    Str         = idtable.add_string("String");
    str_field   = idtable.add_string("_str_field");
    substr      = idtable.add_string("substr");
    type_name   = idtable.add_string("type_name");
    val         = idtable.add_string("_val");
}
/*
 * The method tests to see if class_a is a subclass of class_B
 * returns true if class_a <= class_b, false otherwise
 */
bool test_subclass(Symbol class_a, Symbol class_b)
{
  int counter;
  int child_count = children.size();
  //take care of easy cases
  if(class_b == Object){
    return true;
  }
  else if (class_a == Object){
    return false;
  }
  Symbol child = class_a;
  Symbol parent = map[class_a];
  counter = 0;
  while(parent != Object && counter <= child_count)
  {
    if(parent == class_b){
      return true;
    }
    child = parent;
    parent = map[child];
    counter ++;
     
 }
 return false;

}

ClassTable::ClassTable(Classes classes) : semant_errors(0) , error_stream(cerr) {
	class_table->enterscope();
        build_inheritance_graph(classes);
        //Symbol first = classes->nth(3)->get_name();
        //Symbol second = classes->nth(4)->get_name();
        //cout << "LCP of " << first << " and " << second << " is: " << least_common_parent(first,second) << endl;
        //child = classes->nth(i)->get_name();
        //parent = classes->nth(i)->get_parent();
	//class_table=new SymbolTable<Symbol, class__class>();
	install_basic_classes();
          
	//Put the stuff in to check for duplicates and unpermitted redeclarations... do later
	for(int i = classes->first(); classes->more(i); i = classes->next(i)) 
	{
		class__class *INSTANTIATEDclass=(class__class *)(classes->nth(i));
		//insert into class map as well
	
		//This should test for any duplicates
		if (class_table->probe(INSTANTIATEDclass->get_name())!=NULL)
			semant_error(INSTANTIATEDclass, DUPLICATE);

		class_table->addid(INSTANTIATEDclass->get_name(), INSTANTIATEDclass);
	
		//Now we look for any illegal redeclarations
		if(INSTANTIATEDclass->get_name()==No_class)
			semant_error(INSTANTIATEDclass, REDEFINITION);
		else if(INSTANTIATEDclass->get_name()==Object)
			semant_error(INSTANTIATEDclass, REDEFINITION);
		else if(INSTANTIATEDclass->get_name()==IO)
			semant_error(INSTANTIATEDclass, REDEFINITION);
		else if(INSTANTIATEDclass->get_name()==Int)
			semant_error(INSTANTIATEDclass, REDEFINITION);
		else if(INSTANTIATEDclass->get_name()==Str)
			semant_error(INSTANTIATEDclass, REDEFINITION);

	}

        traverse_tree_for_checking(classes);

}

/*
 * This method takes in two Symbols (class names)
 * It returns their least common parent (as a Symbol)
 */
Symbol least_common_parent(Symbol a, Symbol b)
{
  Symbol temp_A = a;
  Symbol temp_B = b;
  //Take care of the easy cases
  while(temp_A != Object){
    if(test_subclass(temp_A,temp_B)){
        //cout << "Result 1" << endl;
	return temp_B;}
    if(test_subclass(temp_B,temp_A)){
        //cout << "Result 2" << endl;
	return temp_A;}
    while(temp_B != Object){
      temp_B = map[temp_B];
      if(test_subclass(temp_A,temp_B)){
        //cout << "Result 3" << endl;
	return temp_B;}
      if(test_subclass(temp_B,temp_A)){
        //cout << "Result 4" << endl;
	return temp_A;}
    }
    temp_A = map[temp_A];
  }
  return Object;
}


/*
 * This method actually builds the inheritance graph
 * It uses a similar algorithm to the inheritance checker
 * in order to check for a cycle not caused by one class
 * being a child twice
 * This function does not return anything, as it builds the global map
 * As a reminder, the map is map[child] = parent, where child and parent
 * are both of type Symbol.
 */
void ClassTable::build_inheritance_graph(Classes classes)
{
    Symbol child;
    Symbol parent;
    map.insert(std::pair<Symbol,Symbol>(IO,Object));
    map.insert(std::pair<Symbol,Symbol>(Int,Object));
    map.insert(std::pair<Symbol,Symbol>(Str,Object));
    map.insert(std::pair<Symbol,Symbol>(Bool,Object));
    children.insert(Object);
    children.insert(IO);
    children.insert(Str);
    children.insert(Bool);
    children.insert(Int);
    int child_count = 5;
    // This is a way to iterate through all of the classes in a Classes object
    for(int i = classes->first(); classes->more(i); i = classes->next(i))
    {
        child = classes->nth(i)->get_name();
        child_count ++;
        parent = classes->nth(i)->get_parent();
        //cout << child << " : " << parent << endl;
	//For now, print out the child/parent pairs
	//This is just so I can debug it
	//cout << child << endl;
	//cout << "----------" << endl;
        map[child] = parent;
        //map.insert(std::pair<Symbol,Symbol>(child,parent));
        //cout << map.at(child) << endl;
        if(children.count(child) == 0)
	{
	    children.insert(child);
	}
	else //it's a cycle!
	{
	    //i'm not sure how we're reporting errors yet.
	    //for now, I'll do what's below
	    //cout << "CYCLE!!!!!!!!!!!!" << endl;
	    //Close, try this ;)
	    this->semant_error(child,CYCLE);
	}
    }
    //This is my second way to check for cycles
    int counter = 0;
    for(std::map<Symbol,Symbol>::iterator it = map.begin(); it!=map.end(); ++it)
    {
	counter = 0;
        //child = (it)->first;
        //parent = (it)->second;
        //dump_Symbol(cout,10,child); 
        //cout << "child is: "<< child << endl;
	while(parent != Object && counter <= child_count)
	{
	    child = parent;
	    parent = map[child];
	    counter ++;
	}
	if(counter >= child_count)
	{
	  //cout << "CYCLE!!!!1!!11!!!!" << endl;
	  this->semant_error(child,CYCLE);
	}
    }
    //This code is supposed to make sure all parents are declared
    Symbol curChild;
    for(int j = classes->first(); classes->more(j); j=  classes->next(j))
    {
        curChild = classes->nth(j)->get_name();
        //cout << "Here!" << endl;
        //cout << curChild  << endl;
        //cout << "Heeeeeeere" << endl;
        //cout << map[curChild] << endl;
        if(children.count(map[curChild]) == 0 && map[curChild] != Object)
	{
          Symbol badParent = map[curChild];
	  //This means the parent wasn't defined
	  this->semant_error(badParent,UNDECLARED);
  	  break;
	}

    }
}

void ClassTable::install_basic_classes() {

    // The tree package uses these globals to annotate the classes built below.
    node_lineno  = 0;
    Symbol filename = stringtable.add_string("<basic class>");
    
    // The following demonstrates how to create dummy parse trees to
    // refer to basic Cool classes.  There's no need for method
    // bodies -- these are already built into the runtime system.
    
    // IMPORTANT: The results of the following expressions are
    // stored in local variables.  You will want to do something
    // with those variables at the end of this method to make this
    // code meaningful.

    // 
    // The Object class has no parent class. Its methods are
    //        abort() : Object    aborts the program
    //        type_name() : Str   returns a string representation of class name
    //        copy() : SELF_TYPE  returns a copy of the object
    //
    // There is no need for method bodies in the basic classes---these
    // are already built in to the runtime system.

    Class_ Object_class =
	class_(Object, 
	       No_class,
	       append_Features(
			       append_Features(
					       single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
					       single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
			       single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
	       filename);

	class_table->addid(Object, (class__class *)Object_class);

    // 
    // The IO class inherits from Object. Its methods are
    //        out_string(Str) : SELF_TYPE       writes a string to the output
    //        out_int(Int) : SELF_TYPE            "    an int    "  "     "
    //        in_string() : Str                 reads a string from the input
    //        in_int() : Int                      "   an int     "  "     "
    //
    Class_ IO_class = 
	class_(IO, 
	       Object,
	       append_Features(
			       append_Features(
					       append_Features(
							       single_Features(method(out_string, single_Formals(formal(arg, Str)),
										      SELF_TYPE, no_expr())),
							       single_Features(method(out_int, single_Formals(formal(arg, Int)),
										      SELF_TYPE, no_expr()))),
					       single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
			       single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
	       filename);  

	class_table->addid(IO, (class__class *)IO_class);

    //
    // The Int class has no methods and only a single attribute, the
    // "val" for the integer. 
    //
    Class_ Int_class =
	class_(Int, 
	       Object,
	       single_Features(attr(val, prim_slot, no_expr())),
	       filename);

	class_table->addid(Int, (class__class *)Int_class);

    //
    // Bool also has only the "val" slot.
    //
    Class_ Bool_class =
	class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename);

	class_table->addid(Bool, (class__class *)Bool_class);

    //
    // The class Str has a number of slots and operations:
    //       val                                  the length of the string
    //       str_field                            the string itself
    //       length() : Int                       returns length of the string
    //       concat(arg: Str) : Str               performs string concatenation
    //       substr(arg: Int, arg2: Int): Str     substring selection
    //       
    Class_ Str_class =
	class_(Str, 
	       Object,
	       append_Features(
			       append_Features(
					       append_Features(
							       append_Features(
									       single_Features(attr(val, Int, no_expr())),
									       single_Features(attr(str_field, prim_slot, no_expr()))),
							       single_Features(method(length, nil_Formals(), Int, no_expr()))),
					       single_Features(method(concat, 
								      single_Formals(formal(arg, Str)),
								      Str, 
								      no_expr()))),
			       single_Features(method(substr, 
						      append_Formals(single_Formals(formal(arg, Int)), 
								     single_Formals(formal(arg2, Int))),
						      Str, 
						      no_expr()))),
	       filename);

	class_table->addid(Str, (class__class *)Str_class);
}

//Collect declarations for method and objects: we create symbol tables and then
//traverse the tree using traverse.
void ClassTable::collect_declarations()
{
	//method_table=new SymbolTable<Symbol, method_class>();
	//object_table=new SymbolTable<Symbol, Symbol>();
	traverse(Object);
}

//This will travel the tree and make sure everything is correct & well formed magically! Ah, dreams. :)
void ClassTable::traverse(Symbol symbol)
{
	//method_table->enterscope();
	object_table->enterscope();
	class__class *INSTANTIATEDclass = class_table->lookup(symbol);
	
	INSTANTIATEDclass->scan(object_table,method_table,class_table);
	
	//Now we can use a multimap to recurse through the tree. This
	//is very similar to travelling for inheritance.
	//std::multimap<Symbol, Symbol>::iterator it = range.first; 
      // it != range.second; ++it)
	
	
	object_table->exitscope();
	//method_table->exitscope();
}

////////////////////////////////////////////////////////////////////
//
// semant_error is an overloaded function for reporting errors
// during semantic analysis.  There are three versions:
//
//    ostream& ClassTable::semant_error()                
//
//    ostream& ClassTable::semant_error(Class_ c)
//       print line number and filename for `c'
//
//    ostream& ClassTable::semant_error(Symbol filename, tree_node *t)  
//       print a line nuber and filename
//
///////////////////////////////////////////////////////////////////

//Added a code flag so that we can address specific issues, see semant.h
ostream& ClassTable::semant_error(Class_ c, code error_type)
{   
    switch(error_type)
	{
		case REDEFINITION:
			//Someone use sprintf to make these errors nicer
			cout << "Redefinition of basic class." << endl;
			break;
		case CYCLE:
			cout << "There is an ineritance cycle." << endl;
			break;
		case DUPLICATE:
			cout << "There is a duplicate class." << endl;
			break;
		case INHERIT:
			//With sprintf, we could say which classes! Do later. 
			cout << "A class cannot inherit another class." << endl;
			break;
		case UNDECLARED:
			cout << "Undeclared class or inheriting from undeclared class." << endl;
			break;
        case TYPE:
			cout << "Typechecking error in file " << c->get_filename() << endl;
			break;

	}                                                          
    return semant_error(c->get_filename(),c);
}    
ostream& ClassTable::semant_error(Symbol s,code error_type)
{   
    switch(error_type)
	{
		case REDEFINITION:
			//Someone use sprintf to make these errors nicer
			cout << "Redefinition of basic class." << endl;
			break;
		case CYCLE:
			cout << "There is an ineritance cycle." << endl;
			break;
		case DUPLICATE:
			cout << "There is a duplicate class." << endl;
			break;
		case INHERIT:
			//With sprintf, we could say which classes! Do later. 
			cout << "A class cannot inherit another class." << endl;
			break;
		case UNDECLARED:
		        cout << "Undeclared class or inheriting from undeclared class-." << endl;
			break;
        case TYPE:
			cout << "Typechecking error for Symbol " << s << endl;
			break;
	}                                                          
    return semant_error(); //not sure how to get the class to send the line number
}    

ostream& ClassTable::semant_error(Symbol filename, tree_node *t)
{
    error_stream << filename << ":" << t->get_line_number() << ": ";
    return semant_error();
}

ostream& ClassTable::semant_error()                  
{                                                 
    semant_errors++;                            
    return error_stream;
} 



/*   This is the entry point to the semantic checker.

     Your checker should do the following two things:

     1) Check that the program is semantically correct
     2) Decorate the abstract syntax tree with type information
        by setting the `type' field in each Expression node.
        (see `tree.h')

     You are free to first do 1), make sure you catch all semantic
     errors. Part 2) can be done in a second stage, when you want
     to build mycoolc.
 */




void program_class::semant()
{
    initialize_constants();
    /* ClassTable constructor may do some semantic analysis */
    //bool test = test_subclass(classes->nth(0)->get_name(),Object);
    //cout << "My two classes were: " << classes->nth(0)->get_name() << " and " << classes->nth(1)->get_name() << endl;
    //cout << "I sure hope that this is: " << test << endl;
    //test = test_subclass(classes->nth(0)->get_name(),classes->nth(1)->get_name());
    //cout << "But this should be :" << test << endl;
    //ClassTable *classtable = new ClassTable(classes);
    classtable = new ClassTable(classes);
    /* some semantic analysis code may go here */
    /******************************************/
    /*             Manal :)                   */
    /*      Let the tree building begin       */
    /******************************************/

    if (classtable->errors()) {
	cerr << "Compilation halted due to static semantic errors." << endl;
	exit(1);
    }

    //We begin by collecting the declarations, see it above. It works, but it calls a method that does
    //not exist yet. In short, DON'T uncomment the line below until we're ready to begin the chaos.
    //classtable->collect_declaration();
	//Add type checking here. We can do something crazy like this->typecheck and attach it to each node (XD)
	//or make a helper method or something:
	//if(typecheckingthing and node decoration has errors)
		//cout << "Compilation halted due to static semantic errors." << endl;
		//exit(1)
	//Nothing else, there's no errors and the nodes have been decorated.
}

/*
 *  The following methods wil deal with type checking
 *  These make the assumptions that there are valid global method and class tables
 *  those are currently called method_table and class_table
 */

void traverse_tree_for_checking(Classes classes)
{
  for(int i = classes->first(); classes->more(i); i = classes->next(i))
  {
        cout << "here1" << endl;
    //do something with the class?
    classes->nth(i)->type_check();
    
  }
}

Symbol class__class::type_check()
{
        cout << "here2" << endl;

   for(int i = features->first(); features->more(i); i = features->next(i))
     features->nth(i)->type_check();
}




Symbol method_class::type_check()
{
   //Go through all of the lower parts of the tree
   //for(int i = formals->first(); formals->more(i); i = formals->next(i))
   //{
   //  formals->nth(i)->type_check();
   //}
        cout << "here3" << endl;

   expr->type_check(); //make sure to type-check its expression
   //This is nowhere near done, but needed to be defined
   return return_type;
}

Symbol attr_class::type_check()
{
        cout << "hereinattr" << endl;
 
  Symbol e_type = init->type_check();
  Symbol decl_type =  *(object_table->lookup(name));
  if(!test_subclass(e_type,decl_type))
  {
    //This is an error
    classtable->semant_error(decl_type,TYPE);
  }
        cout << "hereinattr" << endl;
  //We will get this far if there were no errors, so return the declared type
  return decl_type;
}

//The next few are definitely wrong. They need to return something so it compiles
Symbol let_class::type_check()
{
  set_type(Object);
  return Object;
}

Symbol dispatch_class::type_check()
{
  set_type(Object);
  return Object;
}

Symbol static_dispatch_class::type_check()
{
  set_type(Object);
  return Object;
}

//We don't need to implement this, but it has to be defined for my method to compile
Symbol typcase_class::type_check()
{
  set_type(Object);
  return Object;
}

/*
 * This code implements the rules from the COOL manual
 * The names/types of objects were found in cool-tree.h
 */
Symbol eq_class::type_check()
{
  Symbol type1 = e1->type_check();
  Symbol type2 = e2->type_check();
  //Deal with tricky cases: are the types nonequal?
  if(!(type1 == type2))
  {
    //if either one is in {Bool,Int,Str}, we have an error
    if(type1 == Bool || type1 == Int || type1 == Str ||
       type2 == Bool || type2 == Int || type2 == Str)
    {
      classtable->semant_error(type1,TYPE);
    }
  }
  //if it makes this far, everything is ok, so return a bool
  set_type(Bool);
  return Bool;
}

Symbol plus_class::type_check()
{
  if(e1->type_check() != Int)
  {
    classtable->semant_error(e1->type_check(),TYPE);  
  }
  if(e2->type_check() != Int)
  {
    classtable->semant_error(e2->type_check(),TYPE);  
  }
  set_type(Int);
  return Int;
}

Symbol sub_class::type_check()
{
  if(e1->type_check() != Int)
  {
    classtable->semant_error(e1->type_check(),TYPE);
  }
  if(e2->type_check() != Int)
  {
    classtable->semant_error(e2->type_check(),TYPE);  
  }
  set_type(Int);
  return Int;
}

Symbol mul_class::type_check()
{
  if(e1->type_check() != Int)
  {
    classtable->semant_error(e1->type_check(),TYPE);  
  }
  if(e2->type_check() != Int)
  {
    classtable->semant_error(e2->type_check(),TYPE);  
  }
  set_type(Int);
  return Int;
}

Symbol divide_class::type_check()
{
  if(e1->type_check() != Int)
  {
    classtable->semant_error(e1->type_check(),TYPE);  
  }
  if(e2->type_check() != Int)
  {
    classtable->semant_error(e2->type_check(),TYPE);  
  }
  set_type(Int);
  return Int;
}

Symbol lt_class::type_check()
{
  if(e1->type_check() != Int)
  {
    classtable->semant_error(e1->type_check(),TYPE);  
  }
  if(e2->type_check() != Int)
  {
    classtable->semant_error(e2->type_check(),TYPE);  
  }
  set_type(Bool);
  return Bool;
}

Symbol leq_class::type_check()
{
  if(e1->type_check() != Int)
  {
    classtable->semant_error(e1->type_check(),TYPE);  
  }
  if(e2->type_check() != Int)
  {
    classtable->semant_error(e2->type_check(),TYPE);  
  }
  set_type(Bool);
  return Bool;
}


//This is the type check for NOT expressions
Symbol comp_class::type_check()
{
  if(e1->type_check() != Bool)
  {
    //This is an error
    classtable->semant_error(e1->type_check(),TYPE);   
  }
  set_type(Bool);
  return Bool;
}

//This is the type_check for the negative  expression
Symbol neg_class::type_check()
{
  if(e1->type_check() != Int)
  {
    //This is an error
  classtable->semant_error(e1->type_check(),TYPE);   
  }
  set_type(Int);
  return Int;
}

Symbol isvoid_class::type_check()
{
  set_type(Bool);
  return Bool; //Always a bool
}

Symbol loop_class::type_check()
{
  //Is the first expression a bool?
  if(pred->type_check() != Bool)
  {
    //If not, error
  classtable->semant_error(pred->type_check(),TYPE);   
  }
  set_type(Object);
  return Object;
}

Symbol block_class::type_check()
{
  Symbol temp;
	//for(int i = body->first(); body->more(i); i = body->next(i)) 
  for(int i = body->first(); body->more(i); i = body->next(i))
  {
    temp = body->nth(i)->type_check();
  }
  //The return type of a block is the return type of the last expr in the block
  set_type(temp);
  return temp;
}

Symbol assign_class::type_check()
{
        cout << "here4" << endl;

  //If it conforms properly
  Symbol T_prime = expr->type_check();
  cout << "before object table "<< endl;
  Symbol T = *(object_table->lookup(name));
  if(test_subclass(T_prime,T))
  {
    cout << "t_prime" << endl;
    set_type(T_prime);
    return T_prime;
  }
  else
  {
    //Find a way to pass errors
    classtable->semant_error(name,TYPE);
    set_type(Object);
        cout << "here5" << endl;

    return Object; //This will never get called, but I need a return for it to compile
  }
}

Symbol object_class::type_check()
{
  cout << "here in object class" << endl;
  Symbol temp = *(object_table->lookup(name));
  //I believe lookup will return a pointer to the Symbol. I want to return the value
  set_type(temp);
  return temp;
}

Symbol cond_class::type_check()
{
  if(pred->type_check() != Bool)
  {
    //This is an error
  classtable->semant_error(pred->type_check(),TYPE);   
  }
  //The type of an if is the least common parent of the types of its then and else expressions
  Symbol temp = least_common_parent(then_exp->type_check(),else_exp->type_check());
  set_type(temp);
  return temp;
}

Symbol new__class::type_check()
{ 
  //We don't have to deal with SELF_TYPE, so this is straightforward
  set_type(type_name);
  return type_name;
}


//Deal with all of the basic expressions (leaes)

Symbol int_const_class::type_check()
{
  set_type(Int);
  return Int;
}

Symbol bool_const_class::type_check()
{
  set_type(Bool);
  return Bool;
}

Symbol string_const_class::type_check()
{
  set_type(Str);
  return Str;
}

Symbol no_expr_class::type_check()
{
  set_type(No_type);
  return No_type;
}
