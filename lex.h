#ifndef __LEX_H_
#define __LEX_H_

#define TOKEN_MAX 2048

/* structure used to represent a token */
struct lex_token
{
   int type;
   char* token;


};
typedef struct lex_token TOKEN_T;

 union token_value
 {
      long int_value ;
      double float_value;
 };

 typedef union token_value TOKEN_VALUE_T ;


/* various types of token*/
enum TOKEN_TYPE
{
   INT64 ,
   AUTO,
   BREAK,
   CASE,
   CHAR,
   CONST,
   CONTINUE,
   DEFAULT,
   DO,
   DOUBLE,
   ELSE,
   ENUM,
   EXTERN,
   FLOAT,
   FOR,
   GOTO,
   IF,
   INT,
   LONG,
   REGISTER,
   RETURN,
   SHORT,
   SIGNED,
   SIZEOF,
   STATIC,
   STRUCT,
   SWITCH,
   TYPEDEF,
   UNION,
   UNSIGNED,
   VOID,
   VOLATILE,
   WHILE,
   IDENTIFIER ,
   PLUS ,
   MINUS ,
   STAR,
   DIV,
   AND,
   NOT,
   XOR,
   COMPLEMENT,
   OR,
   COND,
   COMMA,
   LOGICAL_AND,
   LOGICAL_OR,
   LOGICAL_EQUAL,
   LOGICAL_NOT_EQUAL,
   EQUAL,
   PLUS_PLUS,
   MINUS_MINUS,
   PLUS_EQUAL,
   MINUS_EQUAL,
   DIV_EQUAL,
   AND_EQUAL,
   OR_EQUAL,
   XOR_EQUAL,
   STAR_EQUAL,
   LEFT_PAR,
   RIGHT_PAR,
   LBRACE,
   RBRACE,
   LSQBRACE,
   RSQBRACE,
   OCT_NUMBER,
   HEX_NUMBER,
   STRING,
   CHAR_CONST,
   SEMICOLON,
   DOT,
   COMMENT,
   STR_PTR,
   GREATER,
   LESS ,
   GREATER_EQUAL,
   LESS_EQUAL,
   SHIFT_LEFT,
   SHIFT_RIGHT,
   SHIFT_LEFT_EQUAL,
   SHIFT_RIGHT_EQUAL,
   REMINDER ,
   REMINDER_EQUAL,
   COMPLEMENT_EQUAL,
   COLON,
   NOTHING,
   SPACE,
   DQUOTE,
   SQUOTE,
   INT_NUMBER,
   FLOAT_NUMBER,
   NIL_TOKEN ,
   ASM,
   END_OF_FILE
};

/* initializes the current lexer*/
extern void init_lex(char *filename);
extern void init_token(TOKEN_T *token);
int is_keyword(char *str , TOKEN_T *tok);
long  ascii_to_int(char ascii);

extern void skip_comments();
/*skip blanks*/
extern void skip_blanks();
/* checks if source has the given string at the current position*/
extern int match(char *str);
/* increment the current index to advance count */
extern void advance_lex(int advance_count ) ;
/* returns true if lex has reached the end of file */
extern int end_of_file();
/* splits source into a token using space as a delimiter */
extern char* get_token_space(char *token);
/* match and  advance or return FALSE */
extern int  match_advance(char *str ) ;
/* get a character from the current index */
extern char get_current_char(int lookahead );
/* close the lexical analyzer */
extern void close_lex();
/* get the current token */
extern void get_token(TOKEN_T *token,TOKEN_VALUE_T *val);
extern void print_lex_error_info();
extern void backtrack();
#endif



