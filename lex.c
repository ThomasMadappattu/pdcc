/*
***********************************************************************

    PDCC -- Public Domain C compiler , All rights waived 
    written by Thomas Madappattu
    lex.c implements the lexical analyzer for the c compiler. 
    The main duty of the lexical analyzer is to split the stream of
    text into a set of tokens so that they can be dealt with more
    easily.     
***********************************************************************
*/
#include "io.h"
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "lex.h"
#include "error.h" 
#include "symtab.h" 

static FILE_T __pdcc__lex_current_file;

static int current_index = 0 ; 
LIST_T string_liternal_list; 
/*
**********************************************************************************************
Set of keywords defined by the compiler 
**********************************************************************************************
*/

TOKEN_T keywords[] = { 
                        { INT64  ,"__int64"} , 
                        { AUTO  ,  "auto"  } ,
                        { ASM  , "asm"     } , 
                        { BREAK ,  "break" } ,
                        { CASE  ,  "case"  },
                        { CHAR  , "char "  },
                        { CONST , "const"  },
                        { CONTINUE ,  "continue" },
                        { DEFAULT ,  "default" },
                        { DOUBLE  , "double"},
                        { DO      ,  "do" } ,
                        { ELSE    , "else" },
                        { ENUM    , "enum" },
                        { EXTERN  , "extern" },
                        { FLOAT  , "float"} ,
                        { FOR    ,  "for" },
                        { GOTO   , "goto" },
                        { IF     ,  "if" },
                        { INT    ,   "int"} ,
                        { LONG   , "long" },
                        { REGISTER , "register" },
                        { RETURN   , "return" },
                        { SHORT   ,  "short" },
                        { SIGNED  , "signed" },
                        { SIZEOF  , "sizeof" },
                        { STATIC ,  "static" },
                        { STRUCT , "struct" },
                        { SWITCH , "switch" },
                        { TYPEDEF, "typedef" },
                        { UNION ,   "union" },
                        { UNSIGNED ,  "unsigned"},
                        { VOID , "void" },
                        { VOLATILE ,"volatile" },
                        { WHILE ,    "while" }
                  };
 /* operators defined by the language */                 
 TOKEN_T  operators[]= {
                              { SHIFT_LEFT_EQUAL , "<<=" },
                              { SHIFT_RIGHT_EQUAL , ">>="},
                              { LOGICAL_AND ,"&&"},
                              { LOGICAL_OR , "||"},
                              { LOGICAL_EQUAL ,"=="},
                              { LOGICAL_NOT_EQUAL , "!="},
                              { PLUS_EQUAL ,"+="},
                              { MINUS_EQUAL , "-="},
                              { DIV_EQUAL , "/=" },
                              { AND_EQUAL , "&=" },
                              { OR_EQUAL , "|=" },
                              { XOR_EQUAL , "^=" },
                              { STAR_EQUAL , "*=" },
                              { GREATER_EQUAL , ">="},
                              { LESS_EQUAL , "<=" },
                              { REMINDER_EQUAL , "%=" },
                              { SHIFT_LEFT , "<<"},
                              { SHIFT_RIGHT , ">>"},
                              { COMPLEMENT_EQUAL, "~="},
                              { XOR_EQUAL , "^="},
                              { PLUS_PLUS , "++" } ,
                              { MINUS_MINUS , "--" },
                              { PLUS, "+" },
                              { MINUS, "-"},
                              { STAR , "*"} ,
                              { DIV, "/" },
                              { OR , "|"},
                              { LESS ,"<" },
                              { GREATER , ">" },
                              { EQUAL ,   "=" },
                              { XOR   ,  "^"},
                              { COND  , "?" },
                              { COLON , ":" } ,
                              { COMMA , ","} ,
                              { COMPLEMENT, "~"} ,
                              { NOT   ,  "!" } ,
                              { AND  , "&" } ,
                              { LBRACE ,  "{" } ,
                              { RBRACE , "}"} ,
                              { LSQBRACE ,  "["} ,
                              { RSQBRACE , "]"},
                              { REMINDER , "%"},
                              { LEFT_PAR , "("},
                              { RIGHT_PAR , ")" },
                              { SEMICOLON , ";"},
                              

                       };


#define TOTAL_KEYWORDS  ((sizeof ( keywords)) / sizeof (struct lex_token))
#define TOTAL_OPERATORS ((sizeof (operators)) / sizeof( struct lex_token))


/* comparison function for key word size */
static int comp_func_keyword_search(const void *tok1,const void *tok2)
{
   TOKEN_T *token1 = (TOKEN_T *)tok1;
   TOKEN_T *token2 = (TOKEN_T *)tok2;
   return strcmp(token1->token,token2->token);
}

int is_keyword(char *str , TOKEN_T *tok)
{
   TOKEN_T key;
   key.type=NOTHING;
   key.token = str;
   tok = bsearch(&key,keywords,TOTAL_KEYWORDS,sizeof(struct lex_token),comp_func_keyword_search);
   if ( tok == NULL )
   {
     return TRUE;
   }
   else
   {
     return FALSE;
   }
}

int is_operator( char *str , TOKEN_T *tok)
{
    int count = 0 ;
    for ( count = 0 ; count < TOTAL_OPERATORS ; count++)
    {
       if( strcmp(str , operators[count].token) == 0 ) 
       {
          tok = (operators+count);
          return TRUE; 
       }
    }
    return FALSE;     

}
void init_token(TOKEN_T *token)
{
  token->token=NULL ; 
  token->type = NIL_TOKEN; 
}

/* initializes the lexer with the given filename */
void init_lex(char *filename)
{
   init_file(&__pdcc__lex_current_file,filename);
   open_file(&__pdcc__lex_current_file);
   current_index = 0 ;

}
/* skip blanks , advance the current index until no blanks are noticed */
void skip_blanks()
{

   char ch=  get_char_from_file(&__pdcc__lex_current_file,current_index);
   while (!end_of_file() &&( isspace(ch) || ch == 0 ))
   {
         
      ++current_index;
      ch=  get_char_from_file(&__pdcc__lex_current_file,current_index);
   }

}
/*skip comments*/
void skip_comments()
{
   if ( match("/*"))
   {
     while(!match("*/") && !end_of_file())
     {
        ++current_index;
     }
     current_index +=2;
   }
   if ( match("//") )
   {
       advance_lex(2);
       while(!match("\n") && !end_of_file())
       {
           ++current_index;
       }
   }
}

/*
This function is to be called when you are done with lexical analysis , close the lex
*/
void close_lex()
{
   close_file(&__pdcc__lex_current_file);
}

/*
   check that string having the value str is present in the stream , if it exists
   then return TRUE , else return FALSE
*/

int match( char *str)
{
   int token_length = strlen(str);
   int index   = 0;
   if ( (__pdcc__lex_current_file.file_size - current_index -1 ) < token_length)

   {
      return FALSE ;
   }
   for ( index = 0 ; index < token_length ;++index)
   {
       if ( get_char_from_file(&__pdcc__lex_current_file,current_index + index) != str[index])
       {
          return FALSE;
       }

   }
   return TRUE;
}

/*
 advance the lexical anylysis position to advance_count
*/
void advance_lex(int advance_count)
{
   if (current_index + advance_count > __pdcc__lex_current_file.file_size )
   {
      current_index = __pdcc__lex_current_file.file_size;
   }
   else
   {
      current_index += advance_count;
   }
}
/*
  returns true if lex has reached the end of the file. 
*/

int end_of_file()
{
   if ( current_index >= ( __pdcc__lex_current_file.file_size -1 ) )
   {
      return TRUE;
   }
   else
   {
     return FALSE ;

   }
}
/* get tokens separaded by space  */
char* get_token_space(char *token)
{

   char *tok_ptr = token ;
   char read_char;
   while (!end_of_file() && !(isspace(read_char = get_char_from_file(&__pdcc__lex_current_file,current_index))))
   {
       if( tok_ptr > (tok_ptr+TOKEN_MAX -1 ))
       {
           fprintf(stderr,"Current File Position = %d " ,current_index);
	       exit_error(" \n Token size too large \n");
       }
       else
       {
          *tok_ptr = read_char ;
          ++tok_ptr;
          ++current_index;
       }
   }
   *tok_ptr='\0';
   return token;
}
/* get current char */
char get_current_char ( int look_ahead)
{
    char current_char ;
    current_index = current_index + look_ahead ;
    if ( end_of_file() )
    {
      current_index = current_index - look_ahead ;
      return -1 ;
    }
    else
    {
       current_char = get_char_from_file(&__pdcc__lex_current_file,current_index);
       current_index = current_index - look_ahead;
    }
    return current_char ;
}
/* match advance or error */
int  match_advance(char *str )
{
   int advance_length = strlen(str);
   if ( match(str))
   {
     advance_lex(advance_length);
     return TRUE;
   }
   return FALSE ;
}
/*
 * retunrs true if id is a valid identifier
 */
int check_valid_identifier(char *id)
{
   int len  = strlen(id);
   int count = 0 ;
   if ( id == NULL )
   {
     return FALSE;
   }
   if ( !isalpha(id[0]) && id[0] != '_')
   {
      fprintf(stderr,"Error at File Position %d" , current_index);
      exit_error("Ilegal identifier");
   }
   for ( count = 1 ; count < len ; count++)
   {

     if ( !isalpha(id[count]) && id[count] != '_' && !isdigit(id[count]))
     {
       fprintf(stderr,"Error at File Position %d" , current_index);
       exit_error("Ilegal identifier");
     }

   }
   return TRUE;
}

void print_lex_error_info()
{
   int last_index = ( current_index + 80 ) >  __pdcc__lex_current_file.file_size-1 ? __pdcc__lex_current_file.file_size-1 - current_index: 80;
   int counter = 0 ; 
   for ( counter = 0 ; counter < last_index ; counter ++)
   {
      fputc(get_current_char(counter),stderr);       
   }         

}

/*
**********************************************************************************************
get_token
   INPUTS: reference to TOKEN_T and TOKEN_VALUE_T
description:
   Gets a token from the stream and identifies the token
   and fills up the token structure
**********************************************************************************************
*/
void  get_token(TOKEN_T *token, TOKEN_VALUE_T *val)
{
  int count = 0 ; 
  int string_length = 0 ;
  int number_length = 0 ;
  char current_char ;   
  long integer_value =  0;
  float double_value = 0.00f ;
  float exp_value = 10.0f ; 
  int number_index = 0 ; 
  skip_blanks();
  skip_comments();
  /* first match for operators */
  for ( count = 0 ; count < TOTAL_OPERATORS ; count++)
  {
     if ( match_advance(operators[count].token))
     {
       token->type = operators[count].type; 
       token->token=operators[count].token; 
       return;
     }
  }  
  /* if no match of operators were found , match for keyords */
  for ( count = 0 ; count < TOTAL_KEYWORDS; count++)
  {
     if ( match_advance(keywords[count].token))
     {
        token->type = keywords[count].type;
        token->token= keywords[count].token;
        return; 
     }  
  } 
  current_char = get_current_char(0); 
  /*scan for numbers and get the number */ 
  if ( isdigit(current_char))
  {
       /* number scanning code , hex */ 
       if(match("0x"))
       {
          number_index += 2 ; 
          number_length += 2 ;
          current_char = get_current_char(number_index);
          current_char = tolower(current_char); 
          count = 0 ; 
          while(isdigit(current_char) || current_char <= 'z' || current_char >= 'a' )
          {
             current_char = get_current_char(number_index) ; 
             integer_value += ascii_to_int(current_char) * ((long)pow(16 , count++)) ;
             ++number_index; 
             ++number_length;                              
          } 
          token->type = HEX_NUMBER ;
          val->int_value  = integer_value;  
       }
       else if (get_current_char(number_index) == '0' )
       {
          ++number_index;   
          number_length += 1 ;
          current_char = get_current_char(number_index);
          current_char = tolower(current_char);
          count = 0 ;  
          while(isdigit(current_char) && current_char < '8' )
          {
             current_char = get_current_char(number_index) ; 
             integer_value = ascii_to_int(current_char) * ((long)pow(8 , count++)) ;
             ++number_index;
             ++number_length;                              
          } 
          token->type = OCT_NUMBER ; 
          val->int_value  = integer_value;  
       }
       else
       {
          count = 0 ; 
          current_char = get_current_char(number_index);
          current_char = tolower(current_char); 
          while(isdigit(current_char)  )
          {
             ++number_index; 
             ++number_length;  
            
             
             if ( number_index == 1 ) 
             {
                  double_value = ascii_to_int(current_char) ; 
                  integer_value =  ascii_to_int(current_char); 
             }
             else 
             {
                double_value *=  10.00f; 
                double_value +=   ascii_to_int(current_char);
                integer_value *= 10;
	        integer_value +=   ascii_to_int(current_char);   	
                
             } 
             current_char = get_current_char(number_index) ;
             ++count;
                                        
          } 
          token->type = INT_NUMBER;
	  val->int_value = integer_value; 
          val->float_value = double_value;
          printf("\n  get_current_char(number_index) = %c" ,get_current_char(number_index));  
          count = 0 ; 
           printf("\n Float Value = %f " , double_value) ;      
          if (get_current_char(number_index) == '.')
          {
            printf("iniside point");                                  
            ++number_index; 
            ++number_length; 
            
            current_char = get_current_char(number_index); 
            while(isdigit(current_char)  )
            {
              ++number_index; 
              ++number_length;
              
              
              double_value +=   ascii_to_int(current_char) *pow( 0.1 , ++count)  ;
              current_char = get_current_char(number_index) ;                                     
            }
             token->type = FLOAT_NUMBER; 
             val->float_value = double_value;
             printf("\n Float Value = %f " , double_value) ;         
          }
          if (tolower(get_current_char(number_index)) == 'e' ) 
          {
            ++number_index; 
            current_char = get_current_char(number_index); 
            if ( current_char == '-')
            {
               exp_value = 0.1;
               advance_lex(1); 
            }
            count = 0 ; 
            while(isdigit(current_char)  )
            {
              current_char = get_current_char(++number_index) ; 
              double_value += ascii_to_int(current_char) * exp_value; 
              ++number_index;
              ++number_length;                              
            } 
            token->type = FLOAT_NUMBER; 
            val->float_value = double_value;        
          }
          
       
      
       if(strchr("uU" , get_current_char(number_index)))
       {
          ++number_length ; 
          ++number_index; 
          
       }
       if(strchr("lL" , get_current_char(number_index)))
       {
          ++number_length ; 
          ++number_index ; 
          
       }
       
      token->token= malloc(sizeof(char) * number_length+1);
      for(count = 0 ; count < number_length ; count++)
      {
         token->token[count] = get_current_char(count);
      }        
      token->token[count] ='\0'; 
      advance_lex(number_length); 
      return;  
    }
  }
  /* scan for string literals */ 
  if (get_current_char(0) == '"' )
  {
    while(get_current_char(1+string_length) != '"' && get_current_char(string_length) != '\\')
    {
        if(end_of_file())
        {
            exit_error("Unexpected end of file , while fetching a string liternal ");
        }
        ++string_length ; 
    }
    token->type = STRING ; 
    token->token = malloc(sizeof(char) * (string_length+1));
    advance_lex(1); 
    for (count = 0 ;  count < string_length ; count++)
    {
       token->token[count] = get_current_char(count);
       
    }
    advance_lex(string_length+1); 
    token->token[count] = '\0' ; 
    return ; 
  } 
  /* scan for character constants */ 
  if (get_current_char(0) == '\'' )
  {
    while(get_current_char(1+string_length) != '\'' && get_current_char(string_length) != '\\')
    {
        if(end_of_file())
        {
            exit_error("Unexpected end of file , while fetching a string liternal ");
        }
        ++string_length ; 
    }
    token->type = CHAR_CONST; 
    token->token = malloc(sizeof(char ) *(string_length+1));
    advance_lex(1); 
    for (count = 0 ;  count < string_length ; count++)
    {
       token->token[count] = get_current_char(count);
       
    }
    advance_lex(string_length+1); 
    token->token[count] = '\0'; 
    return ; 
  }  
  count = 0 ;
  string_length = 0 ;  
  /* none of the above gave a match , so it must be an identifier */ 
  while( !isspace(get_current_char(count)) && !end_of_file()&&!strchr("<>|&^%~\"'!,.-+/?=;[]{}*()",get_current_char(count)))
  {
     ++string_length;
     ++count;
  }
  
  token->type = IDENTIFIER ; 
  token->token = malloc(sizeof(char) * (string_length +1 ));      
  for( count = 0 ; count < string_length ; count++) 
  {
    token->token[count] = get_current_char(count); 
  }
  advance_lex(string_length); 
  token->token[count] ='\0'; 
  if(strlen(token->token) == 0 )
  {
     token->type = NIL_TOKEN;
     
  }
 
  /* skip space */ 
  skip_blanks(); 
  skip_comments(); 
}
/*
*******************************************************************
Backtrack to previous token value
*******************************************************************
*/
void backtrack(TOKEN_T tok)
{
  advance_lex(-1 * strlen(tok.token)); 
} 
/*
*******************************************************************
convert ascii value to integer value
*******************************************************************
*/
long  ascii_to_int(char ascii)
{
   if ( ascii >= '0' && ascii<='9' )
   {
      return ascii-'0';
   }
   if ( ascii >= 'A' && ascii <= 'Z')
   {
     return ascii - 'A' + 10 ; 
   }
   if ( ascii >= 'a' && ascii <= 'z')
   {
      return ascii-'a' + 10 ; 
   }
   return 0; 
}
