/*
***************************************************************************************************

PDCC - Public Domain C Compiler, All rights waived

written  by  Thomas Madappattu

Description: This is responsile for implementing the C parser.  The parser parses the source file
             and creates an AST or abstract syntax tree . The abstract syntax tree is then used
             to check for semantic correctness and later code generation
***************************************************************************************************
*/

#include <stdio.h> 
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include "lex.h"
#include "parser.h"
#include "error.h"
#include "symtab.h"
#include "codegen.h"

/*
initializes the AST tree for most of the data
*/

void init_ast (AST_T *ast)
{
   *ast = NULL;
}


AST_T get_ast_node()
{
   AST_T node = (AST_T)malloc(sizeof(struct ast_tree));
   memset(node,0,sizeof(struct ast_tree)); 	   
   return node;
}
/*
****************************************************************************************'
   parse a c statement 
****************************************************************************************

*/

void parse_statement(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast)
{
    AST_T ret_ast;
    init_ast(&ret_ast);
    get_token(tok,val);
    switch ( tok->type )
    {
          case SEMICOLON:
                get_token(tok,val);
                              
                break;
          case LBRACE:
                get_token(tok,val);
                parse_compound(tok,val,&ret_ast);                 
                break;  
          case IF:
                get_token(tok,val);
                parse_if(tok,val,&ret_ast);
                break;

          case WHILE:
                
		get_token(tok,val);
                parse_while(tok,val, &ret_ast);
                break;
          case DO:
                get_token(tok,val); 
                parse_do(tok,val,&ret_ast);
                break;
          case FOR:
                get_token(tok,val);  
		parse_for(tok,val,&ret_ast);
                break;
          case CASE:
                get_token(tok,val);
		parse_case(tok,val,&ret_ast);
                break;
          case SWITCH:
                get_token(tok,val);
                parse_switch(tok,val,&ret_ast);
                break; 
          case RETURN:
                get_token(tok,val);
		parse_return(tok,val,&ret_ast);
                break;
          case GOTO:
                get_token(tok,val); 
	        parse_goto(tok,val ,&ret_ast);
                break;
          case BREAK:
                get_token(tok,val);
                parse_break(tok,val,&ret_ast);
                break;
          case CONTINUE:
                get_token(tok,val);  
		parse_continue(tok,val,&ret_ast);
                break;     
          default:
                break; 
     

    }  
    *ast = ret_ast; 
}
/*****************************************************************************************
 * parse an expression 
 *
 *****************************************************************************************
 */
void parse_expression(TOKEN_T *tok , TOKEN_VALUE_T *val,AST_T *ast)
{
    AST_T temp_ast ;
    AST_T ret_ast = get_ast_node();
    ret_ast->type = AST_EXPRESSION;
    temp_ast = ret_ast->child[FIRST_CHILD];
    do    
    {
            
      /*parse_equality(tok,val);*/
      get_token(tok,val);
      parse_atom(tok,val,&temp_ast);
      ret_ast->child[NEXT_EXPR] = get_ast_node();       
      ret_ast->child[NEXT_EXPR]->type = AST_EXPRESSION;                 
      temp_ast = ret_ast->child[NEXT_EXPR]; 
    }
    while(tok->type == COMMA);
       
}
/*
 * ****************************************************************************************
 *  parse an equality statement
 *
 *
 * ****************************************************************************************
 */
int parse_equality(TOKEN_T *tok , TOKEN_VALUE_T *val)
{
   int ret_value;
   ret_value = parse_conditional(tok,val);
   switch(tok->type)
   {
            case EQUAL:
		      printf( "EQUAL" );
               	      break;
            case PLUS_EQUAL:
                      printf( "ADD TO" );
		      break;
	    case MINUS_EQUAL:
                      printf( "SUBRACT TO" );
		      break;
	    case STAR_EQUAL:
                      printf(" MULTIPLY TO");
		      break;
	    case DIV_EQUAL:
                      printf( " DIVIDE TO ");
		      break;
	    case AND_EQUAL:
                      printf( " AND TO ");
		      break;
	    case XOR_EQUAL:
                      printf( " XOR TO ");
		      break;
	    case OR_EQUAL:
                     printf(" OR TO");
		     break;
	    case REMINDER_EQUAL:
                     printf( " MODULUS TO ");
		     break;
	    default:
		     return 0;

   }
   get_token(tok,val); 
   parse_equality(tok,val);
   return ret_value;
}

/* 
 *  ****************************************************************************************
 *
 *  parse a conditional statment
 *
 *  ****************************************************************************************
 *
 */
int  parse_conditional(TOKEN_T *tok , TOKEN_VALUE_T *val)
{
   int ret_value ;
   ret_value =  parse_logical_or(tok,val);
   
   while (TRUE)
   {
      if (tok->type == COND )
      {
         printf(" IF PREVIOUS CONDITON TRUE THEN " ) ;
         get_token(tok,val);
         ret_value = parse_logical_or(tok,val) ;
         if (tok->type != COLON )
         {
            exit_error("\n <<Error parsing conditional -> missing colon !>>\n>>");

         }
         else
         {
              printf(" ELSE DO  ");
              get_token(tok,val);
              parse_logical_or(tok,val) ;

         }
      }
      else
      {
        return 0 ;
      }

  }
  return ret_value;
}

int parse_logical_or(TOKEN_T *tok , TOKEN_VALUE_T *val)
{
  int ret_value ;
  ret_value = parse_logical_and(tok,val);
  if ( tok->type != LOGICAL_OR)
  {
     return ret_value;
  }
  
  while (TRUE)
  {
     if (tok->type == LOGICAL_OR )
     {
         printf("  LOGICAL OR ");
         get_token(tok,val);
         parse_logical_and(tok,val) ;
     }
     else
     {
       return 0 ;
     }
  }
}
int parse_logical_and(TOKEN_T *tok , TOKEN_VALUE_T *val)
{
   int ret_value ;
   ret_value = parse_bitwise_or(tok,val);
   if (tok->type != LOGICAL_AND )
   {
     return ret_value;
   }
   while ( TRUE)
   {
      if (tok->type == LOGICAL_AND)
      {
         printf( " LOGICAL AND  " );
         get_token(tok,val);
         parse_bitwise_or(tok,val);

      }
      else
      {
         return 0 ;
      }
   }

}
int parse_bitwise_or(TOKEN_T *tok , TOKEN_VALUE_T *val)
{
   int ret_value ;
   ret_value = parse_bitwise_xor(tok,val) ;
     
   if ( tok->type != OR )
   {
      return ret_value;
   }
   
   while (TRUE )
   {
      if (tok->type == OR  )
      {
         printf(" LOGICAL OR ");
         get_token(tok,val);
         parse_bitwise_xor(tok,val);
      }
      else
      {
         return 0 ;
      }

   }
}

int parse_bitwise_xor(TOKEN_T *tok , TOKEN_VALUE_T *val)
{
   int ret_value;
   ret_value = parse_bitwise_and(tok,val);
   
  
   if (tok->type != XOR  )
   {
     return ret_value;
   }
   while ( TRUE )
   {
       if(tok->type == XOR )
       {
          printf(" BITWISE XOR " ) ;
          get_token(tok,val);
          parse_bitwise_and(tok,val);
       }
       else
       {
          return 0;
       }
   }
}
int parse_bitwise_and(TOKEN_T *tok , TOKEN_VALUE_T *val)
{
   int ret_value ;
   ret_value = parse_logical_equality(tok,val);
   
      
   if (tok->type != AND )
   {
      return ret_value;
   }
   while (TRUE)
   {
      if (tok->type == AND  )
      {
          printf(" BITWISE AND ");
          get_token(tok,val);
          parse_logical_equality(tok,val);
      }
      else
      {
         return 0;
      }
   }

}


int parse_logical_equality(TOKEN_T *tok , TOKEN_VALUE_T *val)
{
   int ret_value ;
   ret_value  = parse_logical_inequality(tok,val) ;
   if (!(tok->type == LOGICAL_EQUAL || tok->type == LOGICAL_NOT_EQUAL))
   {
      return ret_value ;
   }
   while (TRUE)
   {
    switch(tok->type)
	{
		case LOGICAL_EQUAL:
			printf( "  LOGICALLY  EQUAL ") ;
                        parse_logical_inequality(tok,val);
			break;
		case LOGICAL_NOT_EQUAL:
                        printf(" LOGICALLY NOT EQUAL ") ;
			parse_logical_inequality(tok,val);
			break;
        default:
			return ret_value;
			break;

	}
    get_token(tok,val); 
   }

}
int parse_logical_inequality(TOKEN_T *tok , TOKEN_VALUE_T *val)
{
   int ret_value ;
   ret_value = parse_shift(tok,val) ;
   if (!(tok->type == LESS || tok->type == GREATER || tok->type == LESS_EQUAL || tok->type == GREATER_EQUAL) )
   {
      return ret_value;
   }
   while( TRUE)
   {
      switch(tok->type)
      {
           case LESS:
                printf(" LESS  THAN ");
                break;
           case GREATER:
                printf(" GREATER THAN ");
                break;
           case LESS_EQUAL:
                printf(" LESS THAN OR EQUAL ");
                break;
           case GREATER_EQUAL:
	          	printf(" GREATER THAN OR EQUAL ");
                break;
           default:
	       return 0;
      }
      get_token(tok,val);
      parse_shift(tok,val);
      
   }

}
int parse_shift(TOKEN_T *tok , TOKEN_VALUE_T *val)
{
   int ret_value = 0 ;
   ret_value = parse_plmin(tok,val);
   if (!(tok->type == SHIFT_LEFT || tok->type == SHIFT_RIGHT) )
   {
      return ret_value ;
   }
   while (TRUE)
   {
       switch(tok->type)
       {
            case SHIFT_LEFT:
                         printf(" SHIFT LESS ");
	                 break;
            case SHIFT_RIGHT:
		         printf( " SHIFT RIGHT ");
		         break;
	        default:
		         return 0;

       }
       get_token(tok,val);
       parse_plmin(tok,val);
   }
}
int parse_plmin(TOKEN_T *tok , TOKEN_VALUE_T *val)
{
   int ret_value ;
   ret_value = parse_divmulper(tok,val);
   if (!(tok->type == PLUS || tok->type == MINUS))
   {
      return ret_value;
   }
   while(TRUE)
   {
       switch(tok->type)
       {
            case PLUS:
                 printf("  ADD  ");
                 break;

            case MINUS:
		  printf("  SUBTRACT  ");
		  break;
	        default:
		       return 0;

       }
       get_token(tok,val);
       parse_divmulper(tok,val);
   }
   return  ret_value;

}
int parse_divmulper(TOKEN_T *tok , TOKEN_VALUE_T *val)
{
    int ret_value ;
    ret_value = parse_prepost(tok,val);
    if( !(tok->type == DIV || tok->type == STAR || tok->type == REMINDER) )
    {
        return ret_value;
    }
    while ( TRUE)
    {
       switch(tok->type)
       {
            case DIV:
                 printf("  DIVIDES " );
                 break;

            case STAR:
                 printf("  MULTIPLIES " );
                 break ;
	    case REMINDER:
		         printf(" TAKE REMINDER OF ");
		         break;
		    default:
                 return 0;

       }
       get_token(tok,val);
       parse_prepost(tok,val);
    }
}

int parse_prepost(TOKEN_T *tok , TOKEN_VALUE_T *val)
{
   
   
   switch(tok->type)
   {
          case PLUS_PLUS:
               printf( " INCREMENT AND USE");
               get_token(tok,val);
               parse_divmulper(tok,val);
               break;

          case MINUS_MINUS:
               printf(" DECREMENT AND USE ");
               get_token(tok,val);
               parse_divmulper(tok,val);
               break;
	  case MINUS:
               printf(" TAKE NEGATIVE OF ");
               get_token(tok,val);
               parse_divmulper(tok,val);
               break;
	  case  NOT:
               printf( " TAKE NOT OF ");
               get_token(tok,val);
               parse_divmulper(tok,val);
               break;
          case  STAR:
               printf( " TAKE CONTENTS AT ADRESS ");
               get_token(tok,val); 
               parse_divmulper(tok,val);
               break;
          case AND:
               printf( " TAKE ADRESS OF ");
               get_token(tok,val);
               parse_divmulper(tok,val);
               break;
	  case COMPLEMENT:
               printf("  TAKE COMPLEMENT OF ");
               get_token(tok,val);
               parse_divmulper(tok,val);
               break;
	  default:
	       parse_atom(tok,val,NULL);
    	       switch(tok->type)
	       {
		          case PLUS_PLUS:
		                printf(" USE AND INCREMENT ");
		                get_token(tok,val);
                                break;

		          case MINUS_MINUS:
		                printf(" USE AND DECREMENT ");
		                get_token(tok,val);
		                break;
	                  
			  default:
	                        return 0;

	       }
   }

  return 0;
}


int parse_atom(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast) 
{
   
   
   AST_T ret_ast;
   init_ast(&ret_ast);
   get_token(tok,val);
   /* PLACE HOLDER atom code */
   switch(tok->type)
   {
        case IDENTIFIER:
                       ret_ast = get_ast_node();
                       ret_ast->type =  AST_IDENTIFIER;
                       ret_ast->name = tok->token;
                       break;
        default:
		       ret_ast = get_ast_node();
                       ret_ast->type = AST_IDENTIFIER;
                       break;	

   }
   *ast = ret_ast;
   
   #ifdef DEBUG 
      printf(" Token = %s " , tok->token);    
   #endif 
   /*
   skip_blanks();
   if (match("id"))
   {
      advance_lex(2) ; 
      skip_blanks();
   }
   else if (match("("))
   {
      advance_lex(1);
      skip_blanks(); 
      parse_expression();  
      skip_blanks();
      if ( !match_advance(")"))
      {
         exit_error("\n <<Expected ) in Experssion >>");  
      }
   }
   else 
   {
      exit_error("\n <<Invalid Atom >> "); 
   }
   */
   return 0;
}
int declare_global( int type ,int storage , SYMBOL_T *mtag,SYMBOL_T *otag, int  is_struct)
{

   return 0; 
}

int define_struct(char *sname,int  storage,int  sflag)
{
    

 return 0;
}
int declare_local(int typ,SYMBOL_T *otag)
{

  return 0 ;
}
void get_ident_type(TOKEN_T *token , TOKEN_VALUE_T *val , SYMBOL_T *sym);
void parse_type_decl(TOKEN_T *token, TOKEN_VALUE_T *val, SYMBOL_T *sym)
{
            

            #ifdef DEBUG 
                 printf("\n On Declation token = %s " ,token->token);            
            #endif 
            if(token->type == INT)
            {   
                       sym->type = INT;
                       get_ident_type(token,val,sym); 
            } 
            else if (token->type == CHAR)
            {
		       sym->type = CHAR;
                       get_ident_type(token,val,sym); 
            }
            else if ( token->type == STRUCT )
            {
                       sym->type = STRUCT;
                       
                     
            }
            else if ( token->type == UNION)
            {
                       sym->type = UNION;
                     
            }        
        

}

void get_ident_type(TOKEN_T *token, TOKEN_VALUE_T *val , SYMBOL_T *sym)
{
   SYMBOL_T *new_sym;
   get_token(token,val);  
   
   /* we are handling the case that an identifier has been 
      declared without specifiying the type , therefore 
      compiler defaults to int and proceeds 
   */
   if  ( token->type == IDENTIFIER )  
   {
          sym->name = token->token;
   
          get_token(token,val);
          if ( token->type == COMMA)
          {
             insert_symbol(sym,SYM_GLOBAL);
             new_sym = allocate_symbol();
             new_sym->type = sym->type; 
             get_ident_type(token,val,new_sym); 

          }
          else if ( token->type == LSQBRACE )
          { 
             sym->is_array = TRUE;
             get_token(token,val);
             if ( token->type != HEX_NUMBER  && token->type != INT_NUMBER && token->type != OCT_NUMBER )
             {
                exit_error(" Expected number for arrays");       
                      
             } 
             sym->size = val->float_value; 
             insert_symbol(sym,SYM_GLOBAL);
             get_token(token,val);
             if ( token->type != RSQBRACE)
             {
                exit_error(" Missing matching squarce bracket for array subscript ");
             }
	     new_sym = allocate_symbol();
	     new_sym->type = sym->type;
	     get_ident_type(token,val,new_sym);
          }
          else if ( token->type == SEMICOLON )
	  {
             insert_symbol(sym,SYM_GLOBAL);   
                   
	  }	  
   }
   else if ( token->type == STAR )
   {
       sym->is_ptr = TRUE  ;
       sym->indirection_level += 1;
       get_ident_type(token,val,sym);  
   }
   else if ( token->type == LEFT_PAR )
   {
       get_token(token,val);
       if ( token->type == STAR ) 
       {  
           sym->is_func_ptr = TRUE ; 
          
       }
        
   }
   else if ( token->type == COMMA )
   {
       get_ident_type(token,val,sym);       
             
   }
    
    

}

int parse_declaration(TOKEN_T *token, TOKEN_VALUE_T *tok_val, int is_global )
{
     
     SYMBOL_T *sym = allocate_symbol();
     sym->type = -1 ;  
     parse_type_decl(token,tok_val,sym); 
              
     return FALSE;  
}

int parse_function(TOKEN_T *tok,TOKEN_VALUE_T *val)
{

  return 0;
}

void parse_asm(TOKEN_T *token , TOKEN_VALUE_T *value)
{
   skip_blanks();
   skip_comments();
   get_token(token,value); 
   if(token->type !=  LBRACE)
   {
      exit_error("\n << Expected { after asm  >>");
   }
   else
   {
   
       while(!match("}") && !end_of_file())
       {
          advance_lex(1); 
          if(get_current_char(0) != '}' && !end_of_file() )
          {
            put_char(get_current_char(0));
          } 
       }
       get_token(token, value); 
   }
}
int is_end_of_statement(TOKEN_T *tok , int exit_on_error)
{
   skip_blanks();
   if(tok->type == SEMICOLON ) 
   {
     if( exit_on_error)
     {
        exit_error("<<Semicolon expected>>"); 
     }
     return TRUE;
   }
   else
   {
      return FALSE; 
   }
}

int parse_new_function()
{
   return 0;
}
int parse_unit(char *filename)
{
    char  *filename_ptr;
    char* output_file= malloc ( sizeof(filename) + 5 );
    TOKEN_T current_token;
    TOKEN_VALUE_T token_value; 
    
    init_token(&current_token);
    init_lex(filename);
    strcpy(output_file,filename);
    filename_ptr = output_file;
    
    while( *filename_ptr != '\0' && *filename_ptr !='.')
    {
       ++filename_ptr; 
    }
    
    ++filename_ptr;
    *filename_ptr = '\0';
    strcpy(filename_ptr,"asm");
    strcat(filename_ptr,"\0"); 
    printf("Output File = %s " , output_file); 
    init_codegen(output_file);     
    while(!end_of_file())
    {
        skip_blanks();
        skip_comments(); 
        get_token(&current_token, &token_value); 
        switch(current_token.type)
        {
             case EXTERN:
                 
                  break;
	     case INT:
		  break;
	     case ASM:
                  parse_asm(&current_token,&token_value);
                  break; 
             default:
                  parse_function(&current_token,&token_value); 
                  break;                           
                                   
        } 
    }
    close_lex();
    shut_codegen(); 
    return 0;
}

void enforce_token( TOKEN_T *tok,int token_type, char *msg)
{
    TOKEN_VALUE_T val;
    if ( tok->type != token_type)
    {
          exit_error(msg);
    }       
    get_token(tok,&val);
       
}

void parse_if(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast)
{
    AST_T temp_ast;
    init_ast(&temp_ast);
    get_token(tok,val);
    temp_ast = get_ast_node();
    temp_ast->type = AST_IF;
    enforce_token(tok,LEFT_PAR , "Expected ("); 
    parse_expression(tok,val,&(temp_ast->child[EXPR]));
    get_token(tok,val);
    enforce_token(tok,RIGHT_PAR , "Expected )");     
    parse_statement(tok,val,&(temp_ast->child[S1]));
    get_token(tok,val);
    if ( tok->type == ELSE)
    {
       parse_statement(tok,val,&(temp_ast->child[S2])); 
    }
    else 
    {
        backtrack(*tok);
    }    
    *ast = temp_ast;      
}

void parse_while(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast)
{
    AST_T temp_ast;
    init_ast(&temp_ast);
    get_token(tok,val);
    temp_ast = get_ast_node();    
    temp_ast->type = AST_WHILE ;
    enforce_token(tok,LEFT_PAR , "Expected ("); 
    parse_expression(tok,val,&(temp_ast->child[EXPR])); 
    get_token(tok,val);
    enforce_token(tok,RIGHT_PAR ,"Expected )");
    parse_statement(tok,val,&(temp_ast->child[S1]));  
    *ast=temp_ast;               
}

void parse_do(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast)
{
     
     AST_T ret_ast;
     ret_ast = get_ast_node();
     ret_ast->type = AST_DO;
     get_token(tok,val);
     parse_statement(tok,val,&(ret_ast->child[S1]));
     get_token(tok,val);
     enforce_token(tok,WHILE ,"expected while");
     enforce_token(tok,LEFT_PAR , "Expected ("); 
     parse_expression(tok,val,&(ret_ast->child[EXPR])); 
     get_token(tok,val);
     enforce_token(tok,RIGHT_PAR ,"Expected )");
     *ast = ret_ast; 
         
}

void parse_for(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast)
{
    AST_T ret_ast;
    ret_ast = get_ast_node();
    ret_ast->type = AST_FOR;
    get_token(tok,val);
    enforce_token(tok , LEFT_PAR , "Expected (");
    parse_expression(tok,val,&(ret_ast->child[S1]));  
    enforce_token(tok,SEMICOLON,"Expected ;");
    parse_expression(tok,val,&(ret_ast->child[S2]));
    enforce_token(tok,SEMICOLON,"Expected ;");
    parse_expression(tok,val,&(ret_ast->child[S3]));
    enforce_token(tok,RBRACE,"Expected )");
    parse_statement(tok,val,&(ret_ast->child[S4])); 
    *ast = ret_ast; 
           
}

void parse_switch(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast)
{
     

}
void parse_case(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast)
{
     
}

void parse_default(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast)
{
     
}

void parse_return(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast)
{
       AST_T temp_ast= get_ast_node();
       temp_ast->type = AST_RETURN;
       parse_expression(tok,val,&(temp_ast->child[EXPR]));
       enforce_token(tok,SEMICOLON,"Expected ;");
      *ast =temp_ast;           
}
void parse_break(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast)
{
      AST_T temp_ast = get_ast_node() ;
      temp_ast->type =  AST_BREAK;                
      get_token(tok,val);
      enforce_token(tok,SEMICOLON,"Expected ;"); 
      *ast =temp_ast; 
}
void parse_compound ( TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast)
{
     AST_T temp , ret_ast;
     init_ast(&temp);
     init_ast(&ret_ast);
     
     while( tok->type != RBRACE)
     { 
         if(ret_ast == NULL)
         {
               parse_statement(tok,val,&temp);
               ret_ast =temp; 
         }
         else 
         {
              temp = temp->child[NEXT_STMT];
              parse_statement(tok,val,&temp); 
         } 
                 
     } 
     get_token(tok,val); 
    *ast = ret_ast;    
}

void parse_continue(TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast)
{
    AST_T temp_ast = get_ast_node();
    temp_ast->type = AST_CONTINUE;
    get_token(tok,val);  
    enforce_token(tok,SEMICOLON,"Expected ;");
    *ast = temp_ast;
}

void parse_goto( TOKEN_T *tok , TOKEN_VALUE_T *val , AST_T *ast)
{

}

