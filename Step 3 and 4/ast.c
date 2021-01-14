#include "ast.h"

extern FILE *intermediate;
int fflush (FILE *stream);
int lineCounter = 0;
int labelCounter = 0;
int tempCounter = 0;


ASTnode *ASTCreateNode(enum ASTtype mytype) {
    ASTnode * p;
    p=(ASTnode *)malloc(sizeof(ASTnode));
    p->type=mytype;
    p->left=NULL;
    p->right=NULL;
    p->s1=NULL;
    p->s2=NULL;
    p->value=0;
    p->name=NULL;
    p->str = NULL;
    p->symbol=NULL;
    p->isType=null;
    return(p);
}


void ASTattachleft(ASTnode* p,ASTnode* q) {
    while (p->left != NULL)
        p = p->left;
    p->left = q; 
}

void PT(int howmany){
    int i;
    for (i=0; i < howmany; i++)
        printf(" ");
}


void ASTprint(int level,ASTnode* p) {
    if (p == NULL ) return;
    else {
        PT(level);
        switch (p->type) {
			case VARDEC :
				printf("Variable  "); 
				if ((p->op) == INTDEC)
					printf("INT ");

				if (p->op == VOIDDEC)
					printf("VOID ");
 
				printf("%s\n", p->name);
				if (p->value > 0) {
					printf("[%d]\n",p->value);
					fprintf(intermediate, "%d: VARDECL %s %d\x0D\x0A", lineCounter, p->name, (p->value)*4);
					fflush(intermediate);
				} else {
					fprintf(intermediate, "%d: VARDECL %s %d\x0D\x0A", lineCounter, p->name, 4);
					fflush(intermediate);
				}
				lineCounter++;
				break;

			case FUNCTIONDEC:
				if (p->op == INTDEC)
					printf("INT ");
					
				if (p->op == VOIDDEC)
					printf("VOID ");
					
				printf("FUNCTION %s \n",p->name);
				
				if (p->s1 == NULL) {
					PT(level+2);
					printf( "(VOID)\n" );
				}
				else {
					PT(level+2);
					printf("( \n");
					ASTprint(level+2,p->s1);
					PT(level+2);
					printf(") \n");
				}
				ASTprint(level+2,p->right);
				
				fprintf(intermediate, "%d: LABEL %s\x0D\x0A", lineCounter, p->name);
				fflush(intermediate);
				lineCounter++;
				break;

			case EXPR: printf("EXPR ");
				if (p->name != NULL)
					printf("  %s = ", p->name);
				ASTnode* node1 = p->right;
				ASTnode* node2 = p->left;
				
				switch (p->op){
					case PLUS: 
						printf("+");
						if (node1->value == 0 && node2->value == 0) {
							fprintf(intermediate, "%d: A2PLUS %s %s \x0D\x0A", lineCounter, node1->name, node2->name);
							fflush(intermediate);
							lineCounter++;
							break;
						} else if (node1->value > 0 && node2->value > 0) {
							fprintf(intermediate, "%d: A2PLUS %d %d \x0D\x0A", lineCounter, node1->value, node2->value);
							fflush(intermediate);
							lineCounter++;
							break;
						} else if (node1->value > 0 && node2->value == 0) {
							fprintf(intermediate, "%d: A2PLUS %d %s \x0D\x0A", lineCounter, node1->value, node2->name);
							fflush(intermediate);
							lineCounter++;
							break;
						} else if (node1 ->value == 0 && node2->value > 0) {
							fprintf(intermediate, "%d: A2PLUS %s %d \x0D\x0A", lineCounter, node1->name, node2->value);
							fflush(intermediate);
							lineCounter++;
							break;
						}
						break;

					case MINUS: 
						printf("-");
						if (node1->value == 0 && node2->value == 0) {
							fprintf(intermediate, "%d: A2MINUS %s %s \x0D\x0A", lineCounter, node1->name, node2->name);
							fflush(intermediate);
							lineCounter++;
							break;
						} else if (node1->value > 0 && node2->value > 0) {
							fprintf(intermediate, "%d: A2MINUS %d %d \x0D\x0A", lineCounter, node1->value, node2->value);
							fflush(intermediate);
							lineCounter++;
							break;
						} else if (node1->value > 0 && node2->value == 0) {
							fprintf(intermediate, "%d: A2MINUS %d %s \x0D\x0A", lineCounter, node1->value, node2->name);
							fflush(intermediate);
							lineCounter++;
							break;
						} else if (node1 ->value == 0 && node2->value > 0) {
							fprintf(intermediate, "%d: A2MINUS %s %d \x0D\x0A", lineCounter, node1->name, node2->value);
							fflush(intermediate);
							lineCounter++;
							break;
						}
						break;

					case TIMES: 
						printf("*");
						if (node1->value == 0 && node2->value == 0) {
							fprintf(intermediate, "%d: A2MULT %s %s \x0D\x0A", lineCounter, node1->name, node2->name);
							fflush(intermediate);
							lineCounter++;
							break;
						} else if (node1->value > 0 && node2->value > 0) {
							fprintf(intermediate, "%d: A2MULT %d %d \x0D\x0A", lineCounter, node1->value, node2->value);
							fflush(intermediate);
							lineCounter++;
							break;
						} else if (node1->value > 0 && node2->value == 0) {
							fprintf(intermediate, "%d: A2MULT %d %s \x0D\x0A", lineCounter, node1->value, node2->name);
							fflush(intermediate);
							lineCounter++;
							break;
						} else if (node1 ->value == 0 && node2->value > 0) {
							fprintf(intermediate, "%d: A2MULT %s %d \x0D\x0A", lineCounter, node1->name, node2->value);
							fflush(intermediate);
							lineCounter++;
							break;
						}
						break;

					case DIVIDE: 
						printf("/");
						if (node1->value == 0 && node2->value == 0) {
							fprintf(intermediate, "%d: A2DIV %s %s \x0D\x0A", lineCounter, node1->name, node2->name);
							fflush(intermediate);
							lineCounter++;
							break;
						} else if (node1->value > 0 && node2->value > 0) {
							fprintf(intermediate, "%d: A2DIV %d %d \x0D\x0A", lineCounter, node1->value, node2->value);
							fflush(intermediate);
							lineCounter++;
							break;
						} else if (node1->value > 0 && node2->value == 0) {
							fprintf(intermediate, "%d: A2DIV %d %s \x0D\x0A", lineCounter, node1->value, node2->name);
							fflush(intermediate);
							lineCounter++;
							break;
						} else if (node1 ->value == 0 && node2->value > 0) {
							fprintf(intermediate, "%d: A2DIV %s %d \x0D\x0A", lineCounter, node1->name, node2->value);
							fflush(intermediate);
							lineCounter++;
							break;
						}
						break;

					case LESSTHANEQUAL: 
						printf("<=");
						if (node1->value == 0 && node2->value == 0) {
							fprintf(intermediate, "%d: IFSE %s %s _l%d\x0D\x0A", lineCounter, node1->name, node2->name, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1->value > 0 && node2->value > 0) {
							fprintf(intermediate, "%d: IFSE %d %d _l%d\x0D\x0A", lineCounter, node1->value, node2->value, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1->value > 0 && node2->value == 0) {
							fprintf(intermediate, "%d: IFSE %d %s _l%d\x0D\x0A", lineCounter, node1->value, node2->name, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1 ->value == 0 && node2->value > 0) {
							fprintf(intermediate, "%d: IFSE %s %d _l%d\x0D\x0A", lineCounter, node1->name, node2->value, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						}
						break;

					case LESSTHAN: 
						printf("<");
						if (node1->value == 0 && node2->value == 0) {
							fprintf(intermediate, "%d: IFST %s %s _l%d\x0D\x0A", lineCounter, node1->name, node2->name, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1->value > 0 && node2->value > 0) {
							fprintf(intermediate, "%d: IFST %d %d _l%d\x0D\x0A", lineCounter, node1->value, node2->value, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1->value > 0 && node2->value == 0) {
							fprintf(intermediate, "%d: IFST %d %s _l%d\x0D\x0A", lineCounter, node1->value, node2->name, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1 ->value == 0 && node2->value > 0) {
							fprintf(intermediate, "%d: IFST %s %d _l%d\x0D\x0A", lineCounter, node1->name, node2->value, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						}
						break;

					case GREATERTHAN: 
						printf(">");
						if (node1->value == 0 && node2->value == 0) {
							fprintf(intermediate, "%d: IFGT %s %s _l%d\x0D\x0A", lineCounter, node1->name, node2->name, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1->value > 0 && node2->value > 0) {
							fprintf(intermediate, "%d: IFGT %d %d _l%d\x0D\x0A", lineCounter, node1->value, node2->value, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1->value > 0 && node2->value == 0) {
							fprintf(intermediate, "%d: IFGT %d %s _l%d\x0D\x0A", lineCounter, node1->value, node2->name, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1 ->value == 0 && node2->value > 0) {
							fprintf(intermediate, "%d: IFGT %s %d _l%d\x0D\x0A", lineCounter, node1->name, node2->value, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						}
						break;

					case GREATERTHANEQUAL: 
						printf(">=");
						if (node1->value == 0 && node2->value == 0) {
							fprintf(intermediate, "%d: IFGE %s %s _l%d\x0D\x0A", lineCounter, node1->name, node2->name, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1->value > 0 && node2->value > 0) {
							fprintf(intermediate, "%d: IFGE %d %d _l%d\x0D\x0A", lineCounter, node1->value, node2->value, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1->value > 0 && node2->value == 0) {
							fprintf(intermediate, "%d: IFGE %d %s _l%d\x0D\x0A", lineCounter, node1->value, node2->name, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1 ->value == 0 && node2->value > 0) {
							fprintf(intermediate, "%d: IFGE %s %d _l%d\x0D\x0A", lineCounter, node1->name, node2->value, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						}
						break;

					case NOTEQUAL: 
						printf("!=");
						if (node1->value == 0 && node2->value == 0) {
							fprintf(intermediate, "%d: IFNEQ %s %s _l%d\x0D\x0A", lineCounter, node1->name, node2->name, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1->value > 0 && node2->value > 0) {
							fprintf(intermediate, "%d: IFNEQ %d %d _l%d\x0D\x0A", lineCounter, node1->value, node2->value, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1->value > 0 && node2->value == 0) {
							fprintf(intermediate, "%d: IFNEQ %d %s _l%d\x0D\x0A", lineCounter, node1->value, node2->name, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1 ->value == 0 && node2->value > 0) {
							fprintf(intermediate, "%d: IFNEQ %s %d _l%d\x0D\x0A", lineCounter, node1->name, node2->value, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						}
						break;

					case EQUAL: 
						printf("==");
						if (node1->value == 0 && node2->value == 0) {
							fprintf(intermediate, "%d: IFEQ %s %s _l%d\x0D\x0A", lineCounter, node1->name, node2->name, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1->value > 0 && node2->value > 0) {
							fprintf(intermediate, "%d: IFEQ %d %d _l%d\x0D\x0A", lineCounter, node1->value, node2->value, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1->value > 0 && node2->value == 0) {
							fprintf(intermediate, "%d: IFEQ %d %s _l%d\x0D\x0A", lineCounter, node1->value, node2->name, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						} else if (node1 ->value == 0 && node2->value > 0) {
							fprintf(intermediate, "%d: IFEQ %s %d _l%d\x0D\x0A", lineCounter, node1->name, node2->value, labelCounter);
							fflush(intermediate);
							int f = labelCounter;
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: A0 _t%d 0\x0D\x0A", lineCounter, tempCounter);
							fflush(intermediate);
							int r = tempCounter;
							lineCounter++;
							
							fprintf(intermediate, "%d: GOTO _l%d\x0D\x0A", lineCounter, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							int u = labelCounter;
							
							fprintf(intermediate, "%d: LABEL _l%d\x0D\x0A", lineCounter, f);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							fprintf(intermediate, "%d: IFEQ _t%d 0 _l%d\x0D\x0A", lineCounter, r, labelCounter);
							fflush(intermediate);
							lineCounter++;
							labelCounter++;
							
							break;
						}
						break;

					case INTDEC:
					case VOIDDEC:
					case null:
					break;

			   }
			   printf("\n");
			   ASTprint(level+1, p->left);
			   ASTprint(level+1, p->right);
			   break;

			case  RETURNSTMT:
				printf("Return statement\n");
				fprintf(intermediate, "%d: RETURNOUT _t%d\x0D\x0A", lineCounter, tempCounter);
				fflush(intermediate);
				lineCounter++;
				tempCounter++;
				ASTprint(level+1, p->right);
				break;

			case IDENT:
				printf("IDENTIFIER %s\n", p->name);
				if (p->right != NULL) {
					PT(level);
					printf("Array reference [\n");
					ASTprint(level+1,p->right);
					PT(level);
					printf("] end array\n");
				}
				break;

			case BLOCK:
				printf("compound_stmt\n");
				ASTprint(level+1,p->right);
				break;

			case ASSIGN:
				printf("Assignment STATEMENT\n");
				ASTprint(level+1,p->right);
				ASTprint(level+1,p->s1);
				break;

			case NUMBER:
				printf("NUMBER with value %d\n", p->value);
				break;

			case ITERSTMT:
				printf("WHILE STATEMENT\n");

				ASTprint(level+1,p->right);
				printf("\n");

				ASTprint(level+1,p->s1);
				printf("\n");
				break;

			case PARAM:
				printf("PARAMETER ");
				if( p->op == INTDEC ) {
					printf("INT");
					fprintf(intermediate, "%d: PARAMOUT _t%d\x0D\x0A", lineCounter, tempCounter);
					fflush(intermediate);
					tempCounter++;
					lineCounter++;
				}
					
				if( p->op == VOIDDEC ) {
					printf("VOID");
					fprintf(intermediate, "%d: PARAMOUT _t%d\x0D\x0A", lineCounter, tempCounter);
					fflush(intermediate);
					tempCounter++;
					lineCounter++;
				}
				
				printf(" %s ",p->name);
				if(p->value == -1)
					printf("[]");

				printf("\n");
				break;

			case IFSTMT:
				printf("IF STATEMENT\n");

				ASTprint(level+1,p->right);
				printf("\n");

				ASTprint(level+1,p->s1);
				printf("\n");

				if(p->s2 != NULL){
					PT(level);
					printf("ELSE \n");
					ASTprint(level+2,p->s2);
				}
				break;

			case CALLSTMT:
				printf("Function Call  %s\n" , p->name);
				if (p->right != NULL){
					ASTprint(level+2, p->right);
					printf("\n");
				} else {
					PT(level+2);
					printf("(VOID)\n");
				}
				break;
			case ARGLIST:
				printf("ARG\n");
				ASTnode* node4 = p->right;
				fprintf(intermediate, "%d: PARAMIN %d\n", lineCounter, node4->value);
				fflush(intermediate);
				lineCounter++;
				ASTprint(level+1, p->right);
				break;

			case EXPRSTMT:
				printf("Expression Statement\n");
				ASTnode* node3 = p->right;
				fprintf(intermediate, "%d: A0 _t%d %d\x0D\x0A", lineCounter, tempCounter, node3->value);
				fflush(intermediate);
				tempCounter++;
				lineCounter++;
				ASTprint(level+1, p->right);
				break;

			default: printf("Unknown type in ASTprint\n");
				break;
        }
        if (p->type != EXPR)
            ASTprint(level, p->left);
    }
}


int compareFormals(ASTnode * p, ASTnode * q) {
    if((p==NULL) && (q==NULL))
        return(1); 
    else if ((p==NULL) || (q==NULL))
        return (0); 
    else if(p->isType == q->right->isType)
        compareFormals(p->left, q->left); 
    else
        return(0);
}
