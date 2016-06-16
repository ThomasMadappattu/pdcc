/*
*****************************************************************************
symtab.c 
  
  written by : Thomas Madappattu
  Description:
  General linked list routines for adding and seaarching items in a linked 
  list. pdcc used linked list for everything and does not use anything else.
  This module contain all the functions related to symbol table handling
*****************************************************************************
*/
#include <stdint.h>
#include  <stdio.h>
#include <stdlib.h>
#include "symtab.h"
#include <ctype.h>
#include "error.h"
#include <string.h>


LIST_T __glob_tab  = NULL;
LIST_T __local_tab = NULL;
LIST_T __struct_tab= NULL;

/*
*****************************************************************************
  get_list_node
  Allocates a list node and the user must pass a data pointer to find
  the total number of linked list modules.

*****************************************************************************
*/
LIST_T get_list_node(void  *data)
{
   
   LIST_T new_list = (LIST_T) malloc (sizeof(struct LIST_NODE));
   if ( new_list == NULL )
   {
      fprintf(stderr,"unable to allocate list");
      return NULL;
   }
   new_list->next = NULL ;
   new_list->prev = NULL ;
   new_list->data = data ; 
   return new_list;
}

/*
*****************************************************************************
 init_list 
 initializes the list with a NULL
*****************************************************************************
*/
void init_list(LIST_T *node)
{
    *node = NULL ; 
}

/*
*****************************************************************************
 insert_head
 adds an item to the head of the list node 
*****************************************************************************
*/
void insert_head(LIST_T *node , void *data) 
{
   LIST_T list_iterator  =  *node ;
   LIST_T new_item = get_list_node(data);
   if ( list_iterator == NULL ) 
   { 
       *node = new_item; 
        return ; 
   }
   while ( list_iterator->prev != NULL ) 
   {
      list_iterator = list_iterator->prev;
   }
   new_item->next = list_iterator;
   list_iterator->prev = new_item;
   *node = new_item;

}
/*
*****************************************************************************
  insert_tail
  inserts and item to the tail of the list node
*****************************************************************************
*/
void insert_tail(LIST_T *node , void *data ) 
{
    LIST_T list_iterator = *node ; 
    LIST_T new_item  = get_list_node(data);
    if ( list_iterator == NULL ) 
    {
        *node = new_item ; 
	return;
    }
    while ( list_iterator->next != NULL ) 
    {
         list_iterator = list_iterator->next; 
    
    } 
    new_item->prev = list_iterator ; 
    list_iterator->next = new_item;
       
}
/*
*****************************************************************************
  delete tail
  insert and item to the tail of the list node
*****************************************************************************
*/
void delete_head(LIST_T *node)
{  
   LIST_T list_iterator  = *node ; 
   LIST_T deleted;
   if( *node == NULL)
   {
     return ;
   }
   while(list_iterator->prev != NULL) 
   {
      list_iterator = list_iterator->prev;
   }
   deleted = list_iterator;
   *node   = list_iterator->next;
   (*node)->prev = NULL;
   free(deleted);
   
}
/*
*****************************************************************************
  delete_tail
  deletes and item from the tail of the linked list
*****************************************************************************
*/
void  delete_tail(LIST_T *node )
{
  LIST_T list_iterator = *node;
  LIST_T deleted ; 
  if ( *node == NULL ) 
  {
     return  ;
  }
  while(list_iterator->next != NULL ) 
  {
     list_iterator = list_iterator->next;
  }
  deleted = list_iterator;
  *node = list_iterator->prev; 
  (*node)->next = NULL; 
  free(deleted);
}

/*
*****************************************************************************
   get_list_item(index)
   gets the nth item from the list , the purpose of this function is to 
   index the linked list like and array 
*****************************************************************************
*/
void* get_list_item(LIST_T *node , int index)
{
   LIST_T ptr = *node;
   while( ptr != NULL && ptr->prev != NULL)
   {
      ptr = ptr->prev; 
   }
   while(index)
   {
      index--;
      if( ptr == NULL)
      {
         return NULL;
      }
      else
      {
         ptr = ptr->next; 
      }
   }
   return ptr;
}
/*
*****************************************************************************  
   print_list
   prints a list . This function is mainly here for the sake of debugging.

*****************************************************************************
*/
void print_list(LIST_T list)
{
    while ( list != NULL )
    {
          printf("\n%s\n",(char *) (list->data) ); 
	      list = list->next; 
    }
}
/*
*****************************************************************************
  is_valid_symbol
  
*****************************************************************************
*/
int is_valid_symbol(char *sym)
{
   char *str_ptr = sym ;
   if ( (*str_ptr >= 'A'  && *str_ptr <= 'Z') || ( *str_ptr  >=  'a'  && *str_ptr <= 'z'   ) || ( *str_ptr == '_')  )
   {
     str_ptr++;    
   }  
   else 
   {
      return FALSE; 
   }
   
   while (*str_ptr != '\0')
   {
      if ( ( *str_ptr >= 'A'  && *str_ptr <= 'Z') ||  ( *str_ptr  >=  'a'  && *str_ptr <= 'z'   ) || *str_ptr == '_' || isalnum(*str_ptr) )
      {
         str_ptr++;      
      } 
      else 
      {
         return FALSE; 
      }     
   }
   return TRUE; 
}
SYMBOL_T *allocate_symbol()
{
  SYMBOL_T *sym = (SYMBOL_T *)malloc(sizeof(struct symbol));
  if(sym == NULL)
  {
     /*
      * Exit with error
      */
  }
  memset(sym,0,sizeof(struct symbol));
  return sym; 
}

void setup_sym_tables()
{
   init_list(&__glob_tab);
   init_list(&__local_tab);
   init_list(&__struct_tab);
}

void insert_symbol(SYMBOL_T *sym , int type)
{
     if ( sym->name == NULL )
     {
          return ;
     }
     if ( !is_valid_symbol(sym->name))
     {
       /*
	* invalid symbol name , handle error here
	*/
     }
     switch(type)
     {
	     case  SYM_GLOBAL:
		  insert_tail(&__glob_tab,sym);
	          break;
	     
             case SYM_LOCAL:
		  insert_tail(&__local_tab,sym);
		  break; 
     
     }	     
}
void dump_symbol_table(LIST_T table)
{

   SYMBOL_T *sym = NULL; 
   printf("\n -------------------------------------- \n");
   printf("\n   Symbol Table contents :");
   printf("\n --------------------------------------- \n");
   while ( table != NULL )
   {
         sym = ( SYMBOL_T *) table->data;
	 printf("\n Symbol Name   = %s",sym->name);
	 printf("\n Type          = %d", sym->type);   
	 printf("\n Is Pointer    = %d", sym->is_ptr); 
	 printf("\n Storage Class = %d",sym->storage_class);
	 printf("\n Function Ptr  = %d" ,  sym->is_func_ptr );
	 printf("\n Is structure  = %d " ,  sym->is_struct);
	 printf("\n Is Array      = %d " , sym->is_array );
	 printf("\n Size          = %d " , sym->size);
	 printf("\n Offset        = %d " , sym->offset);
         printf("\n ------End of Entry----------- \n");
	 table = table->next; 
           
   }

}
void dump_globals ()
{
   dump_symbol_table(__glob_tab);
}
void search_symbol(char *symbol_name , int is_struct_or_un)
{

}

