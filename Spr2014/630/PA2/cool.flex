/*
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;

/*
 *  Add Your own definitions here
 */
%}

/*
 * Define names for regular expressions here.
 */

WHITESPACE	[ \t]+
COMMENTLINE	"--"[^\n]*$
DARROW          =>
COMMENTBLOC	\(\*(.*[\n])*\*\)
CLASS           [cC][lL][aA][sS][sS]
ELSE            [eE][lL][sS][eE]
FI              [fF][iI]
IF              [iI][fF]
IN		[iI][nN]
INHERITS	[iI][nN][hH][eE][rR][iI][tT][sS]
LET		[lL][eE][tT]
LOOP		[lL][oO][oO][pP]
POOL		[pP][oO][oO][lL]
THEN		[tT][hH][eE][nN]
WHILE		[wW][hH][iI][lL][eE]
OF		[oO][fF]
NEW		[nN][eE][wW]
NOT		[nN][oO][tT]
ISVOID		[iI][sS][vV][oO][iI][dD]
INT_CONST	[0-9]*
BOOL_CONST	(t(r|R)(u|U)(e|E))|(f(a|A)(l|L)(s|S)(e|E))
TYPEID		[A-Z][0-9A-Za-z_]*
OBJECTID	[a-z][A-Za-z_0-9]*
ASSIGN		<-
SEMI		;
COLON		:
AT		@
TILDE		~
STAR		\*
SLASH		\/
PLUS		\+
MINUS		\-
LE		<=
LT		<
EQ		=
LB		\{
RB		\}
DOT		\.
COMMA		,
LP		\(
RP		\)
NEWLINE		[\n]
UNMATCHED       \*\)
 /*
  *   The LEFTOVERS token will catch any otherwise unmatched character
  *   Because it is last, it won't take any other correct regex
  */
LEFTOVERS	.
%x comment
%x str
%%

 /*
  *  Nested comments
  */

 /*
  * Keywords are case-insensitive except for the values true and false,
  * which must begin with a lower-case letter.
  */
{CLASS}         { return (CLASS); }
{ELSE}			{ return (ELSE); }
{FI}			{ return (FI); }
{IF}			{ return (IF); }
{IN}			{ return (IN); }
{INHERITS}		{ return (INHERITS); }
{LET}			{ return (LET); }
{LOOP}			{ return (LOOP); }
{POOL}			{ return (POOL); }
{THEN}			{ return (THEN); }
{WHILE}			{ return (WHILE); }
{OF}			{ return (OF); }
{NEW}			{ return (NEW); }
{NOT}			{ return (NOT); }
{ISVOID}		{ return (ISVOID); }

 /*
  *  String constants (C syntax)
  *  Escape sequence \c is accepted for all characters c. Except for 
  *  \n \t \b \f, the result is c.
  *
  */
{NEWLINE}			{ ++curr_lineno ;}
{SEMI}		        { return (';'); }
{COLON}           	{ return (':'); }
{AT}              	{ return ('@'); }
{TILDE}           	{ return ('~'); }
{STAR}            	{ return ('*'); }
{SLASH}           	{ return ('/'); }
{PLUS}            	{ return ('+'); }
{MINUS}           	{ return ('-'); }
{LB}				{ return ('{'); }
{LP}                { return ('('); }
{RP}                { return (')'); }
{DOT}				{ return ('.'); }
{COMMA}				{ return (','); }
{RB}				{ return ('}'); }
{LE}              	{ return (LE); }
{LT}              	{ return ('<'); }
{EQ}              	{ return ('='); }
{DARROW}			{ return (DARROW); }
{ASSIGN}			{ return (ASSIGN); }
{UNMATCHED}	{ yylval.error_msg ="Unmatched *)"; return(ERROR);}
 /*
  *  The multiple-character operators.
  */

{INT_CONST}		{ yylval.symbol = inttable.add_int(atoi(yytext)); return (INT_CONST); }
{BOOL_CONST}    { yylval.symbol = inttable.add_string(yytext);  return (BOOL_CONST); }
{TYPEID}		{ yylval.symbol = idtable.add_string(yytext); return (TYPEID); }
{OBJECTID}		{ yylval.symbol = idtable.add_string(yytext); return (OBJECTID); }

 /*
  *   Whitespace and single-line comments need a handler
  *   Even though they don't have to have anything done
  */     
     
{WHITESPACE}		{ }
{COMMENTLINE}		{  }

 /*
  *   The start expressions for block comments and strings
  */


"(*"         BEGIN(comment);
     
             <comment>[^*\n]*        /* eat anything that's not a '*' */
             <comment>"*"+[^*)\n]*   /* eat up '*'s not followed by '/'s */
             <comment>\n             ++curr_lineno;
             <comment><<EOF>>   {
				BEGIN(INITIAL);
				yylval.error_msg = "EOF in comment";
				return (ERROR);
				}
             <comment>"*"+")"        BEGIN(INITIAL);



\"      string_buf_ptr = string_buf; BEGIN(str);
     
         <str>\"        { /* saw closing quote - all done */
                 BEGIN(INITIAL);
                 *string_buf_ptr = '\0';
                 yylval.symbol = stringtable.add_string(string_buf);
				 return (STR_CONST);
		         }
     
         <str>\n        {
                 /* error - unterminated string constant */
				 BEGIN(INITIAL);
				 yylval.error_msg = "Unterminated string constant";
		 		 ++curr_lineno;
                                 return (ERROR);
                 }
     
  	 <str>\\0 *string_buf_ptr++ = '0';   
	 	 <str><<EOF>>	{
				BEGIN(INITIAL);
				yylval.error_msg= "EOF in string";
				return (ERROR);
				}
				     
         <str>\\n  *string_buf_ptr++ = '\n';
         <str>\\t  *string_buf_ptr++ = '\t';
         <str>\\r  *string_buf_ptr++ = '\r';
         <str>\\b  *string_buf_ptr++ = '\b';
         <str>\\f  *string_buf_ptr++ = '\f';
         <str>\\(\n) {++curr_lineno; *string_buf_ptr++ = yytext[1];}   
         <str>\\(.|\n)  *string_buf_ptr++ = yytext[1];
       
         <str>[^\\\n\"]+        {
                 char *yptr = yytext;
     
                 while ( *yptr )
                         *string_buf_ptr++ = *yptr++;
                 }



{LEFTOVERS}		{ yylval.error_msg = yytext;  return (ERROR); }




%%
