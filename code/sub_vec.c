#include "defs.h"
sub_vec(a,b,c)
    Real a[MAXDIM] ;
    Real b[MAXDIM] ;
    Real c[MAXDIM] ;
{
    int i;

    for (i = 0 ;i < MAXDIM ;i++) {
	a[i] = b[i] - c[i] ;
    }
}
