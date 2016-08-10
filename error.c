#include "error.h"
#include <stdlib.h>
#include <stdio.h>
#include "lex.h"


/*
 ******************************************************
procedure exit_error                    
   prints an error messages msg on the   
   sreen and exits the compiler program 
inputs :                               
    char *msg - the message to be displ 
                ayed                    
 outputs:                               
   prints error messge , program exits    
*******************************************************
*/
void exit_error( char *msg)
{
    fprintf(stderr," \n %s \n", msg);
    print_lex_error_info(); 
    fprintf(stderr,"\n");  
    exit(EXIT_FAILURE) ;      
}
/*
*******************************************************
procedure print_warning                 
   prints warning messages on the screen
inputs :                                
    char *msg - the message to be displ 
                ayed                    
 outputs:                               
    prints some info to erro device       
******************************************************  
*/
void print_warning( char *msg)
{
   fprintf(stderr,"%s",msg);
   fprintf(stderr, "\n ");  
   print_lex_error_info(); 
   fprintf(stderr,"\n");     
}

