#include "error.h"
#include <stdlib.h>
#include <stdio.h>
#include "lex.h"


/*

Function: exit_error
   prints an error messages msg on the
   screen and exits the compiler program
Parameters:
    char *msg - the message to be displ
                ayed
Returns:
   None

*/
void exit_error( char *msg)
{
    fprintf(stderr," \n %s \n", msg);
    print_lex_error_info();
    fprintf(stderr,"\n");
    exit(EXIT_FAILURE) ;
}
/*
Function: print_warning
   prints warning messages on the screen

Parameters:
    char *msg - the message to be displ
                ayed
Returns:
    None
*/
void print_warning( char *msg)
{
   fprintf(stderr,"%s",msg);
   fprintf(stderr, "\n ");
   print_lex_error_info();
   fprintf(stderr,"\n");
}

