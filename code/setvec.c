#include "defs.h"
void
setvec(a,b)
    Real a[MAXDIM] ;
    Real b[MAXDIM] ;
{
    int i;

    for (i = 0 ;i < MAXDIM ;i++) {
	a[i] = b[i] ;
    }
}
