/*
 *  cool.y
 *              Parser definition for the COOL language.
 *
 */
%{
/*
   See copyright.h for copyright notice, limitation of liability,
   and disclaimer of warranty provisions.
*/
#include "copyright.h"

#include "cool-io.h" //includes iostream
#include "cool-tree.h"
#include "stringtab.h"
#include "utilities.h"

/* Locations */
#define YYLTYPE int              /* the type of locations */
#define cool_yylloc curr_lineno  /* use the curr_lineno from the lexer
                                    for the location of tokens */
extern int node_lineno;          /* set before constructing a tree node
                                    to whatever you want the line number
                                    for the tree node to be */

/* The default action for locations.  Use the location of the first
   terminal/non-terminal and set the node_lineno to that value. */
#define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current = Rhs[1];                             \
  node_lineno = Current;

#define SET_NODELOC(Current)  \
  node_lineno = Current;
extern char *curr_filename;

void yyerror(char *s);        /*  defined below; called for each parse error */
extern int yylex();           /*  the entry point to the lexer  */
Program ast_root;	      /* the result of the parse  */
Classes parse_results;        /* for use in semantic analysis */
int omerrs = 0;               /* number of erros in lexing and parsing */

%}

/* A union of all the types that can be the result of parsing actions. */
%union {
  Boolean boolean;
  Symbol symbol;
  Program program;
  Class_ class_;
  Classes classes;
  Feature feature;
  Features features;
  Formal formal;
  Formals formals;
  Case case_;
  Cases cases;
  Expression expression;
  Expressions expressions;
  char *error_msg;
}

/* Declare types for the grammar's non-terminals. */
%type <program> program
%type <classes> class_list
%type <class_> class

/* Type declarations that I wrote */
%type <features> feature_list nonempty_feature_list
%type <feature> feature
%type <formals> formal_list nonempty_formal_list
%type <formal> formal
/* I will need three types of expression lists, for each type of delimiting*/
%type <expressions> expression_list_comma expression_list_semi //expression_list
%type <expression> expression let_stmts
/* 
   Declare the terminals; a few have types for associated lexemes.
   The token ERROR is never used in the parser; thus, it is a parse
   error when the lexer returns it.

   The integer following token declaration is the numeric constant used
   to represent that token internally.  Typically, Bison generates these
   on its own, but we give explicit numbers to prevent version parity
   problems (bison 1.25 and earlier start at 258, later versions -- at
   257)

 */
%token CLASS 258 ELSE 259 FI 260 IF 261 IN 262 
%token INHERITS 263 LET 264 LOOP 265 POOL 266 THEN 267 WHILE 268
%token CASE 269 ESAC 270 OF 271 DARROW 272 NEW 273 ISVOID 274
%token <symbol>  STR_CONST 275 INT_CONST 276 
%token <boolean> BOOL_CONST 277
%token <symbol>  TYPEID 278 OBJECTID 279 
%token ASSIGN 280 NOT 281 LE 282 ERROR 283

/* Precedence declarations */
%left LET
%left LETPREC
%right ASSIGN
%left NOT
%nonassoc LE '<' '='
%left '+' '-'
%left '*' '/'
%left ISVOID
%left '~'
%left '@'
%left '.'
%%
/* 
   Save the root of the abstract syntax tree in a global variable.
*/
program	: class_list	{ /* make sure bison computes location information */
                          @$ = @1;
                          ast_root = program($1); }
        ;

class_list
	: class			/* single class */
		{ $$ = single_Classes($1);
                  parse_results = $$; }
	| class_list class	/* several classes */
		{ $$ = append_Classes($1,single_Classes($2)); 
                  parse_results = $$; }
	| error ';'
	;

/* If no parent is specified, the class inherits from the Object class. */
class	: CLASS TYPEID '{' feature_list '}' ';'
		{ $$ = class_($2,idtable.add_string("Object"),$4,
			      stringtable.add_string(curr_filename)); }
	| CLASS TYPEID INHERITS TYPEID '{' feature_list '}' ';'
		{ $$ = class_($2,$4,$6,stringtable.add_string(curr_filename)); }
	;

/* Feature list may be empty, but no empty features in list. */
feature_list
        : 
	 /*empty feature list. This causes 4 shift/reduce errors */
	    { $$ = nil_Features(); }
	| nonempty_feature_list
	    { $$ = $1; }
        ;
nonempty_feature_list
	: feature ';' /*Single feature */
	    { $$ = single_Features($1); }
	| nonempty_feature_list feature ';'
	    { $$ = append_Features($1,single_Features($2)); }
	| error ';'
	;

feature 
        : OBJECTID ':' TYPEID
               { $$ = attr($1,$3, no_expr()); } /*this is where I need to change the line number */
        | OBJECTID ':' TYPEID ASSIGN expression
               { $$ = attr($1,$3,$5); }
        | OBJECTID '(' formal_list ')' ':' TYPEID '{' expression '}'
               { $$ = method($1,$3,$6,$8); } 
        ;

formal_list 
	: nonempty_formal_list
	  {$$ = $1; } 
	| /*Empty*/
	    { $$ = nil_Formals(); }
	;
nonempty_formal_list
	: formal
	    { $$ = single_Formals($1); }
	| nonempty_formal_list ',' formal
	    { $$ = append_Formals($1, single_Formals($3)); }
	| nonempty_formal_list  ',' error
	;


formal : OBJECTID ':' TYPEID
	{ $$ = formal($1,$3); };

expression_list_comma
	: expression
	   { $$ = single_Expressions($1); }
	| expression_list_comma ',' expression
	   { $$ = append_Expressions($1,single_Expressions($3)); }
	| /* Empty */
	   { $$ = nil_Expressions(); }
	;

expression_list_semi
	: expression ';'
	   { $$ = single_Expressions($1); }
	| expression_list_semi expression ';'
	   { $$ = append_Expressions($1,single_Expressions($2)); }
	| error ';' /*Allow recovery from error within a block */
	   { $$ = nil_Expressions(); }
	;


expression 
    	: OBJECTID ASSIGN expression
	    { $$ = assign($1,$3); }
	/* Three types of dispatch */
	| expression '.' OBJECTID '(' expression_list_comma ')'
	    { $$ = dispatch($1,$3,$5); }
	| expression '@' TYPEID '.' OBJECTID '(' expression_list_comma ')'
	    { $$ = static_dispatch($1,$3,$5,$7); }
	| OBJECTID '(' expression_list_comma ')'
	    { $$ = dispatch(object(idtable.add_string("self")),$1,$3); }

	/* loops, conditionals, blocks, and let */
	| IF expression THEN expression ELSE expression FI
	    { $$ = cond($2,$4,$6); }		
	| WHILE expression LOOP expression POOL
	    { $$ = loop($2,$4); }
	| '{' expression_list_semi '}'  
	    { $$ = block($2); }
	/* The simple ones from the end of the list */
	| NEW TYPEID
	    { $$ = new_($2); }
	| ISVOID expression
	    { $$ = isvoid($2); }
	| expression '+' expression
	    { $$ = plus($1,$3); }
	| expression '-' expression
	    { $$ = sub($1,$3); }
	| expression '*' expression
	    { $$ = mul($1,$3); }
	| expression '/' expression
	    { $$ = divide($1,$3); }
	| '~' expression
	    { $$ = neg($2); }
	| expression '<' expression
	    { $$ = lt($1,$3); }
	| expression LE expression
	    { $$ = leq($1,$3); }
	| expression '=' expression
	    { $$ = eq($1,$3); }
	| NOT expression
	    { $$ = comp($2); }
	| INT_CONST
	    { $$ = int_const($1); }
    	| STR_CONST
            { $$ = string_const($1); }
	| BOOL_CONST
	    { $$ = bool_const($1); } 
	| OBJECTID
	    { $$ = object($1); }
	| '(' expression ')'
	    { $$ = $2; }
        | LET let_stmts
            { $$ = $2; }
        ;

let_stmts
        : OBJECTID ':' TYPEID IN expression %prec LETPREC
            { $$ = let($1,$3,no_expr(),$5); }
        | OBJECTID ':' TYPEID ASSIGN expression IN expression %prec LETPREC
            { $$ = let($1,$3,$5,$7); }
        | OBJECTID ':' TYPEID ',' let_stmts %prec LETPREC
            { $$ = let($1,$3,$5,no_expr()); } //Pass let_stmts as the expression after the comma
        | OBJECTID ':' TYPEID ASSIGN expression ',' let_stmts %prec LETPREC
            { $$ = let($1,$3,$5,$7); } //pass on the let_stmts as an expression to let()
 	;	
    
/* end of grammar */
%%

/* This function is called automatically when Bison detects a parse error. */
void yyerror(char *s)
{
  extern int curr_lineno;

  cerr << "\"" << curr_filename << "\", line " << curr_lineno << ": " \
    << s << " at or near ";
  print_cool_token(yychar);
  cerr << endl;
  omerrs++;
  if(omerrs>50) { cerr << "More than 50 errors" << endl; exit(1);}
}


