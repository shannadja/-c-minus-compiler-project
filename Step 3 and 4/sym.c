
#include <string.h>
#include "sym.h"

char * CreateTemp() {    
char hold[100];
    char *s;
    sprintf(hold,"_t%d",GTEMP++);
    s = strdup(hold);
    return (s);
}


struct SymbTab * Insert(char *name, enum OPERATORS Type, int isafunc, int  level, int mysize, int offset , ASTnode * fparms) {
	struct SymbTab * n;
    n = Search(name,level,0);
    if(n != NULL) {
		//printf("\n\tThe name %s exists at level %d already in the symbol table\n\tDuplicate can.t be inserted",name, level);
		return (NULL);
    }
    else {
		struct SymbTab *p;
		p = malloc(sizeof(struct SymbTab));
		p->name = name;			/* assign the name		*/
		p->offset = offset;  	/* assign the offset 	*/
		p->level = level;  		/* assign the level 	*/
		p->mysize = mysize;  	/* assign the size 		*/
		p->Type = Type;  		/* assign the Type 		*/
		p->IsAFunc = isafunc;  	/* assign the Function  */
		p->fparms = fparms;  	/* assign the Function  */
		p->next = NULL;

		/* Check on how many elements we have in the symbol table */
		if(first == NULL) {
			first = p;
		}
		else {
			p->next = first;
			first = p;
      }
      return (p);
    }
	printf("\n\tLabel inserted\n");
}


struct SymbTab * Search(char * name, int level, int recur) {
	struct SymbTab *p;

	/* Loop through each levels to find the symbol */
	while (level >= 0) {
		p=first;
		while (p != NULL) {
			if((strcmp(p->name,name) == 0) && (p->level == level))
				return p;
			p=p->next;
        }
		if (recur == 0) return (NULL);   /* we did not find it at our level */
		level--; /* check the next level up */
    }
	return  NULL;  /* did not find it, return 0 */
}


int Delete(int level) {
	struct SymbTab *p, *f = NULL;  
    int SIZE = 0;
    p = first;

	/* Loop through the list */
    while (p != NULL) {
        /* If found */
        if (p->level >= level ) { /* if it is the first in the list we have to update first, we know this by f being NULL */
			SIZE += p->mysize;
			if ( f == NULL ) 
				first = p->next;
			else /* not the first element */
				f->next=p->next;
            p = p->next;
        }
        else {
			/* update follow pointer, move the p pointer */
            f = p;
            p = p->next;
        }
    }
    return(SIZE);
}
