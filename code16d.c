
/*------------------------------------------------+
 *  code generator for 16 bit dos machines        |
 *                                                |
 *------------------------------------------------+
 */
#include "codegen.h"
/* swap regsiters */
void gen_swap_regs()
{
  write_line(" XCHG BX , DX " ) ; 

}
/* perform 16 bit addition BX and DX */
void gen_add()
{
  write_line( " ADD BX , DX ") ;
}
/* perform 16 bit substraction*/
void gen_substraction()
{
  write_line( " SUB DX , BX ");
  write_line( " MOV BX , DX ");
	  
}
/* perform 16 bit multiplication */
void gen_multipliication()
{
   write_line( " MOV AX , DX "); 
   write_line( " IMUL BX ") ;
   write_line( " MOV BX , AX ");  
   
}
/* perfrom 16  bit division*/
void gen_divide()
{
   write_line( " MOV  AX , DX " ) ; 
   write_line( " SUB  DX , DX " ) ;
   write_line( " IDIV BX ") ;
   write_line( " MOV AX , BX ");   
}
/* genrate code for getting the reminder in BX register */
void gen_modulus()
{
  gen_divide();
  gen_swap_regs();
}
void gen_or()
{
   write_line( " OR BX , DX ") ; 
		    
}
void gen_xor()
{
  write_line(" XOR BX , DX ") ;  
}
void gen_and()
{
  write_line( " AND BX , DX " ) ; 
}
void gen_logical_negate()
{
  char* label = gen_label("__logical_negate");
  write_line(" OR BX , BX ") ;
  write_line(" MOV BX , CX ") ;
  write_line(" JNZ ");
  write_string(label);
  write_line(" INC BX ");
  write_label(label);
}
void gen_arithemetic_shift_right()
{
  write_line(" MOV CL , BL " );
  write_line(" SAR DX , CL " ); 
  write_line(" MOV BX , DX ");
  write_line(" XOR CX , CX ");  
}
void gen_arithemetic_shift_left()
{
   write_line(" MOV CL , BL ");
   write_line(" SAL DX , CL ");
   write_line(" MOV BX , DX ");
   write_line(" XOR CX , CX ");
}
void gen_negate()
{
   write_line( "NEG BX ");
}
void gen_complement()
{
   write_line( "NOT BX ");
}
void gen_incement(int num ) 
{
   int count = 0 ; 
   if ( num <= 2 ) 
   {
      for ( count = 0 ; count < num ; count++)
      {
         write_line(" INC BX "); 
      }
   }
   else
   {
      write_line (" ADD BX , ");
      write_number(num);      
   }
}
void gen_decrement(int num)
{
   int count  = 0 ; 
   if ( num  <= 2 )  
   {
      for ( count = 0 ; count < num ; count++ )
      {
        write_line ( " DEC BX ") ;
      }
   }
   else
   {
      write_line ( " SUB BX , ");
      write_number(num);
   }

}
void gen_equal()
{
   char *label  = gen_label("__equal");
   write_line ( " CMP DX , BX ") ; 
   write_line ( " MOV BX , CX ") ;
   write_line ( " JNZ  ");
   write_string(label);
   write_line ( " INC BX ");
   write_label(label); 
}
void gen_equal_zero(char *lab_to)
{
   char *label = gen_label("__equal_zero");
   write_line ( " OR BX , BX ") ; 
   write_line ( " JZ ");
   write_string( label);
   write_line (" JMP ");
   write_string ( lab_to);
   write_label(label);

}
void gen_not_equal()
{
   char *label = gen_label(" __not_equal");
   write_line ( " CMP DX , BX ");
   write_line ( " MOV BX , CX ");
   write_line ( " JE "); 
   write_string(label) ; 
   write_line ( "INC BX ") ;
   write_label(label); 

}
void gen_not_equal_zero(char *lab_to)
{
   char *label = gen_label("__equal_zero");
   write_line ( " OR BX , BX ") ; 
   write_line ( " JNZ ");
   write_string( label);
   write_line (" JMP ");
   write_string ( lab_to);
   write_label(label);

}
void gen_less_than_zero()
{
   char *label = gen_label("__lessthan_zero");
   write_line( " CMP DX , BX ") ; 
   write_line("  MOV BX , CX ");
   write_line(" JGE ") ; 
   write_string(label);
   write_line ( " INC BX ") ;   
   write_label(label);

}
void gen_greater_than_zero(char *lab_to)
{
   char *label = gen_label ( "__u_greater_than0");
   write_line( " CMP DX  , BX " ) ; 
   write_line( " MOV BX  , CX " ) ; 
   write_line( " JG ") ;

   write_string(label) ;
   write_line( " JMP " ) ; 
   write_string ( lab_to); 
   write_label(label ); 		    
 
}
void gen_greater_than_equal_zero()
{
   char *label  = gen_label ("__u_greater_than0");
   write_line ( " XOR BX , BX " ) ; 
   write_line ( " JGE  " )  ;
   write_string ( label) ; 
   write_line ( " INC BX " ) ; 
   write_label ( label ) ;  
   
}
void gen_un_less_than()
{
   char *label  = gen_label ( "__u_less_than") ; 
   write_line( " CMP DX  , BX " ) ; 
   write_line( " MOV BX , CX " ) ; 
   write_line( " JAE " ) ; 
   write_string( label) ;
   write_line ( " INC BX " ) ; 
   write_label ( label ) ; 

}
void gen_un_less_than_zero( char *label)
{
  write_line( "JMP " ) ; 
  write_string ( label) ; 

}
void gen_un_less_than_equal()
{
   char *label = gen_label("__u_less_than_equal");
   write_line( " CMP DX , BX " ) ;
   write_line( " MOV BX , CX " ) ;
   write_line( " JA " ) ; 
   write_string( label) ; 
   write_line( "INC BX ") ; 
   write_label ( label) ; 

}

void gen_un_greater_than()
{
   char *label =  gen_label( "___u_greater_than") ; 
   write_line ( " CMP DX , BX " ) ; 
   write_line ( " MOV BX , CX " );
   write_line ( " JBE  ") ; 
   write_string ( label ) ; 
   write_line ( " INC BX "); 
   write_label(label); 

}

void  gen_un_greater_than_equal()
{
   char *label = gen_label("__u_lessgreaterequal"); 
   write_line ( "CMP DX , BX ") ; 
   write_line ( "MOV BX , CX ") ; 
   write_line ( "JB ");
   write_string(label);
   write_line ( " INC BX ") ; 
   write_label (label);
}
