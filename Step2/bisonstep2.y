
%{
#include<string.h>
#include <stdio.h>
int yylex();
void yyerror(char *s);
%}
 

%token COMMENT
%token WHILE
%token ELSE
%token IF
%token INT
%token RETURN
%token VOID
%token ID 
%token GREATQ
%token PRINT
%token NUMBER
%token SMALLQ
%token NOTEQ
%token EQUALITY
%start program


%% 
program :  			{ printf("program\ndeclaration_list\n"); }
					declaration_list 						
					;
	
declaration : 		         var_declaration	| fun_declaration 	  ;
				
					
					
declaration_list : 		declaration_list declaration  { printf("declaration_list\n"); }	| declaration   	;				
					
					

var_declaration :   		type_specifier ID ';'  	{ printf("val_declaration\n");} 		| type_specifier ID '[' NUMBER ']' ';'  ;
					
	

				
type_specifier :	               INT                      { printf("type_specifier(INT)\n"); }
				| VOID	               { printf("type_specifier(VOID)\n"); }
					;
					
fun_declaration :	type_specifier ID fun1 
					;
					
fun1:				{printf("Fun_definitionparamcompound\n");}
					'(' paramss ')' comp_stmt 
					;
					
paramss : 			param_list	{printf("Params-INT\n");}					 
				| VOID 		{printf("Paramss-Void\n");}
					;
					
					
param : 			type_specifier ID 
				| type_specifier ID '[' ']' 
				;

					
					
param_list : 		param_list ',' param 
				| param 
					;
 


					
local_declarations :           local_declarations var_declaration 
			            |  	{printf("empty\n"); }		
					;
					
					
statement_list : 	            statement_list stmnt 	{printf("statement list\n");}
				| 				{printf("statement list(empty)\n");}
					;
					
stmnt:			               expression_stmt 				
					| comp_stmt 
					| selection_stmt 
					| iteration_stmt 
				        | return_stmt 
			                 ;
					
expression_stmt :	                   expression ';' 	{printf("expression_stmt\n");}
					| ';' 		   
					;
					
						
selection_stmt : 	IF '(' expression ')' stmnt				           {printf("only IF");}
			| IF '(' expression ')' stmnt ELSE stmnt
                              {printf("IF with else");}

					;

comp_stmt :	           '{' local_declarations statement_list '}' 		
                           { printf("compound_stmt\nDeclaration\n"); }
					;

iteration_stmt :	WHILE '(' expression ')' stmnt {printf("WHILE\n");};
						
						
return_stmt :	RETURN ';' | RETURN expression ';' ;


expression :		var '=' expression  {printf("expression\n");} 	
			| simple_expression 
			;
			
 
var :	                ID 	{printf("var\n");}
			| ID '[' expression ']'  	{printf("var_array\n");} 
			;

simple_expression : additive_expression relop additive_expression {printf("simple_expression\n");}
			| additive_expression 	{printf("additive_expression\n");}
					;


relop :	SMALLQ | '<' | '>' | GREATQ | EQUALITY | NOTEQ ;


additive_expression :	 additive_expression addop term | term ;
 
 
addop : 			'+' 	{printf("addop\n");}		|   '-' {printf("minusop\n");};
					

term : 				term mulop factor  {printf("term\n");} 	| factor	   {printf("term\n");} 	;
 
mulop :				'*' 	{printf("muloperation\n");}	| '/' 	{printf("divoperation\n");} ;
					  	

factor :			'(' expression ')'  {printf("factor\n");}
					| var 	  {printf("factor\n");}
					| call 	  {printf("factor\n");}
					| NUMBER  {printf("factor\n");}
					;
					
					
call :				ID '(' args ')' 	{printf("call input\n");}
					;
					
args :				arg_list 			{printf("args(empty)\n");}
					|  
					;


arg_list :			arg_list ',' expression 
					| expression 
					;
			
%%


void yyerror(char* s)
{
printf("%s\n",s);
}
