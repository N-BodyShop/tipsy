#include "defs.h"
char *find(string,target)
    char *string ;
    char *target ;
{
    char *p ;

    for ( p = string ; p[0] != '\0' ; p++) {
	if (p[0] == target[0]) {
	    return p ;
	}
    }
    p = NULL ;
    return p ;
}
