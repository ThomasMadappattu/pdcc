#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>


/* intialize code generator with a file name*/
extern void init_codegen(char *filename);
/* write comment to the output file */
extern void write_comment(char *str);
/* write a line to the output file */
extern void write_line(char *str);
/* shut the code generator module */
extern void shut_codegen();
/* write  a string to the output file*/
extern void write_string(char *str);
/* write a number to the output  file*/
extern void write_number(int num);
/* write hexadecimal number ot the output file*/
extern void write_number_hex(int num);
/* write a label to the output file*/
extern void write_label( char *label); 
/* generate an unsigned with a given prefix */
extern char* gen_label(char * prefix ); 
/* put a charactor to the output file */
extern void put_char(char ch); 
/* set up code gen instructions for the particular platform*/
extern void setup_codegen_instructions(int supported_arch); 

void (*add_func)(); 
void (*sub_func)();
void (*div_func)(); 
void (*gen_func)();
void (*divide_func)(); 
#endif
