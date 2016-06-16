#ifndef __IO_H_
#define __IO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 8k buffer size for each file*/
#define BUFFER_MAX 32768

/* the file structure*/
struct file_st 
{
  FILE *input_file ;
  char *filename ;
  int file_size ;
  int  current_index;
  char file_buffer[BUFFER_MAX];
};
typedef struct file_st FILE_T; 
/* initialize the file name */
extern void init_file(FILE_T* file , char *filename);
/* open the file name */
extern void open_file(FILE_T*	file) ;
/* get a character for the file , given the file name and the index */
extern  char get_char_from_file(FILE_T* file , int index ) ;
/* closes the given file*/
extern void close_file(FILE_T* file);
/* calculate the lenght of the file */
extern int file_length(FILE_T file); 

#endif





