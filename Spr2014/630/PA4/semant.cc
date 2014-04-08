

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



ClassTable::ClassTable(Classes classes) : semant_errors(0) , error_stream(cerr) {

	class_table=new SymbolTable<Symbol, class__class>();
	class_table->enterscope();
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

	install_basic_classes();
	//Check for inheritence and undeclared classes using your thing here Josh.
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

	//We need to account for this in our map and table
	//Remember that the table is Object to object
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

	//We need to account for this in our map and table
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

	//We need to account for this in our map and table
	class_table->addid(Int, (class__class *)Int_class);

    //
    // Bool also has only the "val" slot.
    //
    Class_ Bool_class =
	class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename);

	//We need to account for this in our map and table
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
	//We need to account for this in our map and table
	class_table->addid(Str, (class__class *)Str_class);
}

//Collect declarations for method and objects: we create symbol tables and then
//traverse the tree using traverse.
void ClassTable::collect_declarations()
{
	method_table=new SymbolTable<Symbol, method_class>();
	object_table=new SymbolTable<Symbol, Symbol>();
	traverse(Object);
}

//This will travel the tree and make sure everything is correct & well formed magically! Ah, dreams. :)
void ClassTable::traverse(Symbol symbol)
{
	method_table->enterscope();
	object_table->enterscope();
	class__class *INSTANTIATEDclass = class_table->lookup(symbol);
	
	//INSTANTIATEDclass->scan(object_table,methoud_table,class_table);
	
	//Now we can use a multimap to recurse through the tree. This
	//is very similar to travelling for inheritance.

	object_table->exitscope();
	method_table->exitscope();
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
ostream& ClassTable::semant_error(Class_ c, code)
{   
    switch(code)
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
	}                                                          
    return semant_error(c->get_filename(),c);
}    
ostream& ClassTable::semant_error(Symbol s, code)
{   
    switch(code)
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
/*
 * The following declarations are the global map and set.
 * The map stores mappings from child -> parent
 * In order to get the parent class of a child, use:
 * map[child Symbol]    --> this will return the Symbol of its parent
 */
std::map<Symbol,Symbol> map;
std::set<Symbol> children;

/*
 * The method tests to see if class_a is a subclass of class_B
 * returns true if class_a <= class_b, false otherwise
 */
bool test_subclass(Symbol class_a, Symbol class_b)
{
  int counter;
  int child_count = children.size();
  Symbol child = class_a;
  Symbol parent = map[class_a];
  counter = 0;
  while(parent != Object && counter <= child_count)
  {
    child = parent;
    parent = map[child];
    counter ++;
  }

  if(counter >= child_count -2 && class_b != Object)
  {
    return false;
  }
  else
  {
    return true;	
  }


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
void build_inheritance_graph(Classes classes)
{
    Symbol child;
    Symbol parent;
    int child_count = 0;
    // This is a way to iterate through all of the classes in a Classes object
    for(int i = classes->first(); classes->more(i); i = classes->next(i))
    {
        child = classes->nth(i)->get_name();
        child_count ++;
        parent = classes->nth(i)->get_parent();
	//For now, print out the child/parent pairs
	//This is just so I can debug it
	//cout << child << endl;
	//cout << parent << endl;
	//cout << "----------" << endl;
        map.insert(std::pair<Symbol,Symbol>(child,parent));
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
	    ClassTable::semant_error(child,CYCLE);
	}
    }
    //This is my second way to check for cycles
    int counter = 0;
    for(std::map<Symbol,Symbol>::iterator it = map.begin(); it!=map.end(); ++it)
    {
	counter = 0;
	while(parent != Object && counter <= child_count)
	{
	    child = parent;
	    parent = map[child];
	    counter ++;
	}
	if(counter >= child_count)
	{
	  //cout << "CYCLE!!!!1!!11!!!!" << endl;
	  ClassTable::semant_error(child,CYCLE);
	}
    }
    Symbol curChild;
    for(std::set<Symbol>::iterator it = children.begin(); it!= children.end(); ++it)
    {
        curChild = *it;
        if(children.count(map[curChild]) == 0 && map[curChild] != Object)
	{
          Symbol badParent = map[curChild];
	  //This means the parent wasn't defined
	  ClassTable::semant_error(badParent,UNDECLARED);
  	  break;
	}

    }
}

void program_class::semant()
{
    initialize_constants();
    build_inheritance_graph(classes);
    /* ClassTable constructor may do some semantic analysis */
    bool test = test_subclass(classes->nth(0)->get_name(),Object);
    cout << "My two classes were: " << classes->nth(0)->get_name() << " and " << classes->nth(1)->get_name() << endl;
    cout << "I sure hope that this is: " << test << endl;
    test = test_subclass(classes->nth(0)->get_name(),classes->nth(1)->get_name());
    cout << "But this should be :" << test << endl;
    ClassTable *classtable = new ClassTable(classes);

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
}


