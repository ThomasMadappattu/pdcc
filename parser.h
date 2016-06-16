#ifndef __PARSER_H__
#define __PARSER_H__
#include "symtab.h"
#include "lex.h"

#define MAX_AST_CHILD  10
#define EXPR           0
#define S1             1 
#define S2             2
#define NEXT_STMT      3
#define S3             4
#define S4             5
#define FIRST_CHILD    0
#define NEXT_EXPR      2

enum INBUILT_TYPES
{
   TY_CHAR , 
   TY_SHORT,
   TY_INT ,  
   TY_LONG , 
   TY_FLOAT, 
   TY_DOUBLE ,
   TY_POINTER,
   TY_ULONG , 
   TY_STRUCT , 
   TY_UNION ,
   TY_FUNC ,
   TY_VOID , 
   TY_UCHAR ,
   TY_USHORT ,
   TY_BITFIELD

};

enum AST_TYPES
{
    AST_STATMENT ,
    AST_IF ,
    AST_WHILE ,
    AST_DO,
    AST_CASE,
    AST_SWITCH,
    AST_GOTO ,
    AST_LABEL,
    AST_FOR ,
    AST_IDENTIFIER, 
    AST_RETURN,
    AST_COMMA, 
    AST_EXPRESSION,
    AST_BREAK,
    AST_CONTINUE 
};

struct ast_tree
{
  int type;
  char *name;
  union
  {
        int int_val;
	double float_val;
	char   char_val;
        char *string_name; 
  }value;  
  struct ast_tree *child[MAX_AST_CHILD];
};
typedef struct ast_tree* AST_T; 

/* parse expression */
extern  void parse_expression(TOKEN_T *tok , TOKEN_VALUE_T *val ,AST_T *ast);
/* parse equality */
extern  int parse_equality(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse conditional */
extern  int  parse_conditinal(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse logical or */
extern  int  parse_logical_or(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse logical and */
extern  int  parse_logical_and(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse bitwise and */
extern  int  parse_bitwise_and(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse logical equality */
extern  int parse_logical_equality(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse shift */
extern  int parse_shift(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse arithmetic */
extern  int parse_arithmetic(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse unary arithmetic */
extern  int parse_unary_aritmetic(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse atom */
extern  int parse_atom(TOKEN_T *tok , TOKEN_VALUE_T *val ,AST_T *ast);
/* parse logical inequality */
extern int parse_logical_inequality(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse plus and minus */
extern int parse_plmin(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse division muliplier and reminder */
extern int parse_divmulper(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse  prefix and postfix unary operators */
extern int parse_prepost(TOKEN_T *tok , TOKEN_VALUE_T *val); 
/* parse bitwise or operator */
extern int parse_bitwise_or(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse bitwise xor operator */
extern int parse_bitwise_xor(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse bitwise and operator */
extern int parse_bitwise_and(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse continal expression */
extern int parse_conditional(TOKEN_T *tok , TOKEN_VALUE_T *val);
/* parse a declaration */
extern int parse_declaration(TOKEN_T *token , TOKEN_VALUE_T *value , int is_global);
/* parse a function */
extern int parse_function(TOKEN_T *tok , TOKEN_VALUE_T *val);

/* parse a statement */
extern void parse_statement( TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast);

/* parse a unit , given the filename */
extern int parse_unit(char *filename); 
extern void parse_continue(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast);
extern void parse_compound ( TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast);
extern void parse_break(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast);
extern void parse_default(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast);
extern void parse_case(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast);
extern void parse_switch(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast);
extern void parse_for(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast);
extern void parse_while(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast);
extern void parse_if(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast);
extern void parse_do(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast);
extern void parse_return(TOKEN_T *tok ,TOKEN_VALUE_T *val , AST_T *ast); 
extern void parse_goto(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast);


#endif 
