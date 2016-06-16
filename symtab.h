#ifndef   __SYMTAB_H_
#define   __SYMTAB_H_

#define  SYMMAX 100
#define  REGMAX 20
#define  ARGMAX 20

#define SYM_GLOBAL 0
#define SYM_LOCAL  1

#define INT_SIZE            2 
#define INT_PTR_SIZE        2
#define CHAR_SIZE           1 
#define CHAR_PTR_SIZE       2   
#define INT_FNPTR_SIZE      2 

enum ST_CLASS
{
   ST_EXTERN,
   ST_INTERRUPT,
   ST_STATIC,
   ST_AUTO,
   ST_REGISTER
};
enum { FALSE , TRUE } ;  
struct symbol
{
   char *name;
   int storage_class;
   int type;
   int is_func_ptr;
   int is_ptr;
   int is_struct;
   int is_array;
   int size;
   int offset;
   int indirection_level;
   char *typedef_name; 
   struct symbol *member_list;      

};


struct LIST_NODE 
{ 
   void *data ;
   struct LIST_NODE *prev;
   struct LIST_NODE *next;  
};

struct struct_un_entry
{
   char *name; 
   int size;
   struct LIST_NODE *entries;       
};

typedef struct symbol SYMBOL_T; 
typedef struct LIST_NODE* LIST_T;
typedef struct struct_un_entry STRUCT_ENTRY_T; 
extern void init_list(LIST_T *node);
extern void insert_head(LIST_T *node , void *data);
extern void insert_tail(LIST_T *node , void *data);
extern void delete_head(LIST_T *node);
extern void delete_tail(LIST_T *node);
extern LIST_T get_list_node(void *data);
extern void init_global();
extern void print_list(); 
extern int is_valid_symbol(char *symname);
extern SYMBOL_T *allocate_symbol();
extern void insert_symbol(SYMBOL_T *sym , int type);
extern void dump_symbol_table(LIST_T table);
extern void dump_globals();
#endif
