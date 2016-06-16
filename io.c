/*

  io.c
  provies the abstraction of a file as an arrary where the file can be indexed like a buffer

*/
#include "io.h"
/*initalizes the file name and other properties*/
void init_file(FILE_T* file , char *filename)
{
   file->filename = filename;  
   file->file_size = 0 ; 
   file->current_index = 0 ; 
   memset(file->file_buffer,0,BUFFER_MAX);

}
/* opens the file name and sets the other parametes of the file*/
void open_file(FILE_T *file)
{
   file->input_file = fopen(file->filename,"r");
   memset(file->file_buffer,0,BUFFER_MAX);
   if ( file->input_file == NULL )
   {
      printf(" \n Error opening file %s" , file->filename);
      exit(EXIT_FAILURE);
      return ;
   }
   fseek(file->input_file, 0 , SEEK_END );
   file->file_size = ftell(file->input_file) ;
   file->current_index = 0 ; 
   fseek(file->input_file, 0 , SEEK_SET) ;
   fread(file->file_buffer,file->file_size, 1,file->input_file);
 
}
/* gives the abstraction of the file as an array of characters. you give  the file
 * reference and the index  and get the correrespoding character as the output
 */
char get_char_from_file(FILE_T* file,int index)
{

    /* check the index is invalid ,if then show the maximum possible index */
    if ( index  > file->file_size )
    {
       fprintf(stderr,"\n Index out of bounds : %d  max possible %d " , index , file->file_size) ;
       exit(EXIT_FAILURE); 
    }
    /* if the given char is already in the buffer, then return it from the buffer*/
    else if( index <  (file->current_index + BUFFER_MAX))
    {
        return file->file_buffer[index - file->current_index] ;
    }
    /* read the buffer from the file */
    else
    {
        memset(file->file_buffer,0,BUFFER_MAX);
        file->current_index = ( index / BUFFER_MAX) * BUFFER_MAX ;
        fseek(file->input_file,file->current_index,SEEK_SET) ;
        fread( file->file_buffer ,(file->file_size-file->current_index * BUFFER_MAX) < BUFFER_MAX ?(file->file_size-file->current_index * BUFFER_MAX)  :  BUFFER_MAX  , 1 , file->input_file ) ;
        return ( file->file_buffer [ index - file->current_index]) ;

    }

}
/*
   returns the length of the file , you can alternatively use the file_size variable
   directly in the file structure
*/
int file_length(FILE_T file)
{
   return file.file_size ;
}
/*
    closes the file and does the necessary clean up 
*/
void close_file(FILE_T* file)
{
   fclose(file->input_file) ;
   file->current_index = 0 ;
   file->file_size = 0 ;
}
