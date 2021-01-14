/*	
 *	BISON FILE
 */
 
%code requires {
	#include <stdio.h>
	#include <ctype.h>
	#include <stdlib.h>
	#include <string.h>
	#include "sym.h"
	#include "ast.h"
	
	static int level=0;
	static int offset=0;
	static int goffset=0;
	static int maxoffset=0;
	char filename[100];
	
	extern int yylex();
	extern int yyparse();
	extern char *yytext;
	extern FILE *yyin;
	FILE *intermediate;
	void yyerror(const char* s);
	extern int yylineno;
}
%start program

%union {
	int value;
	char *string;
	ASTnode * node;
	enum OPERATORS op;
}

%left '|'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%left UMINUS
%right "then" ELSE

%token IF WHILE ELSE RETURN GE LE EQ NE READ WRITE INT VOID STRING
%token <value> NUM
%token <string> ID STRINGTEXT
%type <node> declarationList decl varDecl funDecl params
%type <node> paramList param compoundStmt localDecls stmtList stmt expStmt
%type <node> selectStmt iterStmt returnStmt expression
%type <node> assignStmt var simpleExpression addExpression term factor
%type <node> call args argList
%type <op> relop addOP multOP typeSpec


%%
program         : declarationList { prog=$1; }
                ;

declarationList : decl { $$=$1; }
                | decl declarationList	{ $1->left = $2; $$ = $1; }
                ;

decl            : varDecl { $$ = $1; }
                | funDecl { $$ = $1; }
                ;

varDecl         : typeSpec ID ';'
                {
                    if(Search($2,level,0))
						yyerror("Redefined variable declaration");
					if ($1 != VOIDDEC && $1 != INTDEC)
						yyerror("Invalid type specifier");

                    $$=ASTCreateNode(VARDEC);
                    $$->name=$2;
                    $$->op=$1;
                    $$->symbol=Insert($2,$1,0,level,1,offset,NULL);
                    $$->isType=$1;
                    offset += 1;
                    if(offset > maxoffset)
                        maxoffset = offset;
                }
                | typeSpec ID '[' NUM ']' ';'
                {
                    if(Search($2,level,0))
						yyerror("Redefined variable declaration");

                    $$=ASTCreateNode(VARDEC);
                    $$->name=$2;
                    $$->op=$1;
                    $$->value=$4;
                    $$->symbol=Insert($2,$1,2,level,$4,offset,NULL);
                    $$->isType=$1;
                    offset += $4;
                    
                    if (offset>maxoffset)
                        maxoffset = offset;
                }
                ;

typeSpec        : INT   	{ $$=INTDEC;	}
                | VOID  	{ $$=VOIDDEC;	}
                ;

funDecl         : typeSpec ID '('
                {   
                    if(Search($2,level,0))
						yyerror("Redefined function declaration");
                    Insert($2,$1,1,level,1,0,NULL);
                    goffset=offset;
                    offset=2;
                    if(offset>maxoffset)
                        maxoffset = offset;
                }
                params
                { 
                    (Search($2,0,0))->fparms = $5;
                }
                ')' compoundStmt
                {
                    $$=ASTCreateNode(FUNCTIONDEC);
                    $$->name=$2;
                    $$->op=$1;
                    $$->s1=$5;
                    $$->right=$8;
                    $$->symbol=Search($2,0,0);
                    offset -=Delete(1);
                    level = 0;
                    $$->value=maxoffset;                
                    $$->symbol->mysize = maxoffset;
                    offset=goffset;
                    maxoffset=0;
                }
                ;

params          : VOID 		{ $$ = NULL; }
                | paramList { $$ = $1; }
                ;

paramList       : param {$$ = $1;}
                | param ',' paramList
                {  
                    $1->left = $3;
                    $$ = $1;
                }
                ;

param           : typeSpec ID
                {  
                    if(Search($2,level,0))
						yyerror("Redefined variable declaration");
                    $$ = ASTCreateNode(PARAM);
                    $$->name=$2;
                    $$->op=$1;
                    $$->value=0;
                    $$->isType=$1;
                    $$->symbol=Insert($2,$1,0,level+1,1,offset,NULL);
                    offset+=1;
                    
                    if(offset>maxoffset)
                        maxoffset = offset;
                }
                | typeSpec ID '[' ']'
                {
                    if(Search($2,level,0))
						yyerror("Redefined variable declaration");
                    $$ = ASTCreateNode(PARAM);
                    $$->name=$2;
                    $$->op=$1;
                    $$->value=1;
                    $$->isType=$1;
                    $$->symbol=Insert($2,$1,2,level+1,1,offset,NULL);
                    offset+=1;
                    if(offset>maxoffset)
                        maxoffset = offset;
                }
                ;

compoundStmt    : '{' { level++; }
                   localDecls stmtList '}'
                {
                    $$=ASTCreateNode(BLOCK);
                    if( $3 == NULL )
                        $$->right = $4;
                    else {
                        ASTattachleft($3,$4);
                        $$->right = $3;
                   }
                   offset -=Delete(level);
                   level--;
                }
                ;

localDecls      : {$$ = NULL;}
                | varDecl localDecls
                {  
                    if($1 != NULL){
                        $1->left = $2;
                        $$ = $1;
                    }
                    else
                        $$ = $2;
                }
                ;

stmtList        : { $$ = NULL; }
                | stmt stmtList
                {   
                    if( $1 != NULL ) {
                        $1->left = $2;
                        $$ = $1;
                    }
                    else
                        $$ = $2;
                }
                ;

stmt            : expStmt { $$ = $1; }
                | compoundStmt { $$ = $1; }
                | selectStmt { $$ = $1; }
                | iterStmt { $$ = $1; }
                | assignStmt { $$ = $1; }
                | returnStmt { $$ = $1; }
                ;

expStmt         : expression ';'
                {
                    $$=ASTCreateNode(EXPRSTMT);
                    $$->right=$1;
                    $$->isType=$1->isType;
                }
                | ';' {$$ = NULL;}
                ;

selectStmt      : IF '(' expression ')' stmt %prec "then"
                {
                    $$ = ASTCreateNode(IFSTMT);
                    $$->right=$3;
                    $$->s1=$5;
                }
                | IF '(' expression ')' stmt ELSE stmt
                {
                    $$ = ASTCreateNode(IFSTMT);
                    $$->right=$3;
                    $$->s1=$5;
                    $$->s2=$7;
                }
                ;

iterStmt        : WHILE '(' expression ')' stmt
                {
                    $$ = ASTCreateNode(ITERSTMT);
                    $$->right=$3;
                    $$->s1=$5;
                }
                ;

returnStmt      : RETURN ';' {$$ = ASTCreateNode(RETURNSTMT);}
                | RETURN expression ';'
                {
                    $$ = ASTCreateNode(RETURNSTMT);
                    $$->s2=$2;
                }
                ;

expression      : simpleExpression {$$ = $1;}
                ;

assignStmt      : var '=' expStmt
                {
					if (($1->isType != $3->isType) || ($1->isType == VOIDDEC))
						yyerror("Incompatible types in assignment");
                    $$=ASTCreateNode(ASSIGN);
                    /* hold the link to the var node*/
                    $$->right=$1;
                    /* hold the link to the expression statement*/
                    $$->s1=$3;
                    /*inherit the type, already check for equivalence
                    so can just use $1*/
                    $$->isType=$1->isType;
                    $$->name=CreateTemp();
                    $$->symbol=Insert($$->name,$$->isType,0,level,1,offset,NULL);
                    offset+=1;
                    if(offset>maxoffset)
                        maxoffset = offset;
                }
                ;

var             : ID
                {
                    struct SymbTab * p;
                    if ((p=Search($1,level,1)) != NULL)
                    {
                        $$ = ASTCreateNode(IDENT);
                        $$->name=$1;
                        $$->symbol=p;                       
                        $$->isType=p->Type;
                        
                        if (p->IsAFunc == 2)
			    yyerror("Variable is an array");
                    }
                    else
                        yyerror("Undefined variable");
                }
                
                | ID '[' expression ']'
                {
                    struct SymbTab * p;
                    if ((p=Search($1,level,1)) != NULL)
                    {
                        $$ = ASTCreateNode(IDENT);
                        $$->name=$1;                        
                        $$->right=$3;
                        $$->symbol=p;                      
                        $$->isType=p->Type;
                        if (p->IsAFunc != 2)
							yyerror("Variable is not an array");
                    }
                    else
                        yyerror("Undefined variable");
                }
                ;

simpleExpression: addExpression {$$ = $1;}
                | simpleExpression relop addExpression
                {
                    if (($1->isType != $3->isType) || ($1->isType == VOIDDEC))
						yyerror("Type mismatch or void in simpleExpression");
                    $$ = ASTCreateNode(EXPR);
                    $$->op=$2;
                    $$->left=$1;
                    $$->right=$3;
                    $$->isType=$1->isType;
                    $$->name=CreateTemp();
                    $$->symbol=Insert($$->name,$$->isType,0,level,1,offset,NULL);
                    offset+=1;
                    if(offset>maxoffset)
                        maxoffset = offset;
                }
                ;

relop           : LE    {$$=LESSTHANEQUAL;		}
                | '<'   {$$=LESSTHAN;			}
                | '>'   {$$=GREATERTHAN;		}
                | GE    {$$=GREATERTHANEQUAL;	}
                | EQ    {$$=EQUAL;				}
                | NE    {$$=NOTEQUAL;			}
                ;

addExpression   : term {$$ = $1;}
                | addExpression addOP term
                { 
                    if (($1->isType != $3->isType) || ($1->isType == VOIDDEC))
						yyerror("Type mismatch or void in additive exp");
                    $$ = ASTCreateNode(EXPR);
                    $$->op=$2;
                    $$->left=$1;
                    $$->right=$3;
                    /*inherit the type, already check for equivalence
                    so can just use $1*/
                    $$->isType=$1->isType;
                    $$->name=CreateTemp();
                    $$->symbol=Insert($$->name,$$->isType,0,level,1,offset,NULL);
                    offset+=1;
                    if(offset>maxoffset)
                        maxoffset = offset;
                }
                ;

addOP           : '+' {$$ = PLUS;}
                | '-' {$$ = MINUS;}
                ;

term            : factor {$$ = $1;}
                | term multOP factor
                {  
                    if (($1->isType != $3->isType) || ($1->isType == VOIDDEC))
						yyerror("Type mismatch or void in term/factor exp");
                    $$ = ASTCreateNode(EXPR);
                    $$->op=$2;
                    $$->left=$1;
                    $$->right=$3;
                    /*inherit the type, already check for equivalence
                    so can just use $1*/
                    $$->isType=$1->isType;
                    $$->name=CreateTemp();
                    $$->symbol=Insert($$->name,$$->isType,0,level,1,offset,NULL);
                    offset+=1;
                    if(offset>maxoffset)
                        maxoffset = offset;
                }
                ;

multOP          : '*'  {$$ = TIMES;}
                | '/'  {$$ = DIVIDE;}
                ;

factor          : '(' expression ')' {$$ = $2;}
                | NUM
                {
                    $$=ASTCreateNode(NUMBER);
                    $$->value=$1;
                    /*numbers are always ints here*/
                    $$->isType=INTDEC;
                }
                | var  {$$ = $1;}
                | call {$$ = $1;}
                ;

call            : ID '(' args ')'
                {
                    struct SymbTab * p;
                    if ((p=Search($1,0,1)) != NULL)
                    {  
                        if(p->IsAFunc != 1)
				yyerror("Is a variable, but was called as function");
							
                        if (compareFormals(p->fparms,$3) != 1)
			       yyerror("Wrong type of arguments");
			                       
                        $$=ASTCreateNode(CALLSTMT);                    
                        $$->right=$3;
                        $$->name=$1;
                        $$->symbol=p;
                        $$->isType=p->Type;
                    }
                    else
						yyerror("Call to undefined function");
                }
                ;

args            : 
                { $$ = NULL;}
                | argList   {$$ = $1;}
                ;

argList         : expression
                {
                    $$ = ASTCreateNode(ARGLIST);
                    $$->right = $1;

                }
                | expression ',' argList
                {  
                    $$=ASTCreateNode(ARGLIST);
                    $$->left=$3;
                    $$->right=$1;
                }
;
%%

int main(int argc, char *argv[]) {
	strcpy(filename, argv[1]);
	intermediate = fopen("intermediate.txt", "w");
	if (intermediate == NULL) {
		printf("Error! Could not open file.\n");
		exit(-1);
	}
	if (argc == 2)
	{ 
		yyin = fopen(argv[1], "r");
		printf("\n");
       }
		
	else {
		printf("No files - Switching to a.out\n");
		yyin = fopen("a.out", "w");
	}
    yyparse();
  
    
	ASTprint(0, prog);
	
	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "%s-:%d %s\n", filename, yylineno, s);
	//fprintf(stderr, "%s On Line: %d\n", s, yylineno);
}
