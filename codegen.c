#include "codegen.h"
#include <string.h> 

static FILE* __current_file = NULL; 
static char __comment_start = ';' ; 
static int __pdcc_internal_label_count  = 0 ;
static char *__label_prefix = "__pdcc_label";
char label_string[100];

/*
   generates a label with a given prefix 
*/
char* gen_label( char *prefix)
{
  char *new_label = NULL;
  memset(label_string,0 ,99);
  sprintf(label_string,"%s%s%d",__label_prefix,prefix,__pdcc_internal_label_count);
  if ( __pdcc_internal_label_count > INT_MAX -3 )
  {
         new_label = malloc(sizeof(char) * strlen(__label_prefix));
         strcpy(new_label,__label_prefix);
         strcat(new_label,"1");
         __label_prefix = new_label;
         __pdcc_internal_label_count = 0 ;       
  }  
  ++__pdcc_internal_label_count;  
    
  return label_string;
}
/*
 * intializes the code generator module 
 */
void init_codegen( char* filename)
{
  if ( __current_file != NULL )
  {
    fclose(__current_file);
  }
  printf(" Output File = %s", filename);
  __current_file =  fopen(filename,"w"); 
  printf(" After fopen output file = %s" , filename);
  if ( __current_file  == NULL )
  {
     printf("\n Code Gen : Error opening the file :  %s",filename);
     exit(EXIT_FAILURE);
  }

}
/*
 *  write a comment into the file 
 */

void write_comment(char *str)
{
  fprintf(__current_file,"\n %c %s ", __comment_start , str );
  fflush(__current_file); 

}
/*
 *  write  a  source line into the file 
 */
void write_line(char *str)
{
   fprintf(__current_file,"\n %s " , str);
   fflush(__current_file);
}
/* write a string without a newline delimiter */
void write_string ( char *str )
{
   fprintf(__current_file,"%s" , str ) ; 
   fflush(__current_file);
}
/* write a numnber given the number */
void write_number ( int num )
{
   fprintf( __current_file , "%d " ,  num );
   fflush(__current_file);
}
/* write a number in hex to  the file */
void write_number_hex( int num )
{
   fprintf( __current_file, "%x " , num ) ; 
   fflush(__current_file);
}
/* print a label to the file*/
void write_label(char *str)
{
   write_line(str);
   write_string(":");
}
/*  
 *   shut the code geneator
 */
void shut_codegen()
{
  if( __current_file != NULL ) 
  { 
    fflush(__current_file); 
    fclose(__current_file); 
  }
  __current_file = NULL;
}
/*
 *  Shut the code generator down 
 */
void put_char(char ch)
{
   fprintf(__current_file,"%c",ch);
   fflush(__current_file); 
} 

void set_comment_char ( char comment_char ) 
{
   __comment_start = comment_char ; 

}
