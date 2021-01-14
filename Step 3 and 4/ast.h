
#ifndef ASTH
#define ASTH
#include<stdio.h>
#include<malloc.h>

enum ASTtype {
   PROGRAM,
   VARDEC,
   ARGLIST,
   IDENT,
   BLOCK,
   NUMBER,
   ASSIGN,
   EXPRSTMT,
   FUNCTIONDEC,
   CALLSTMT,
   EXPR,
   IFSTMT,
   ITERSTMT,
   RETURNSTMT,
   READSTMT,
   WRITESTMT,
   PARAM,
   STRINGNODE
};


enum OPERATORS {
   PLUS,
   MINUS,
   TIMES,
   DIVIDE,
   LESSTHANEQUAL,
   LESSTHAN,
   GREATERTHAN,
   GREATERTHANEQUAL,
   EQUAL,
   NOTEQUAL,
   INTDEC,
   VOIDDEC,
   STRINGDEC,
   null
};


typedef struct ASTnodetype {
    enum ASTtype type;
    enum OPERATORS op;
    enum OPERATORS isType;
    char * name;
    char * str;
    int value;
    struct SymbTab * symbol;
    struct ASTnodetype *left,*right;
    struct ASTnodetype *s1,*s2 ;
} ASTnode;

ASTnode *prog;

ASTnode *ASTCreateNode(enum ASTtype mytype);

void ASTattachleft(ASTnode *p,ASTnode *q);

void ASTprint(int level,ASTnode *p);

int compareFormals(ASTnode * p, ASTnode * q);

#endif
