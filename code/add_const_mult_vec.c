#include "defs.h"
void
add_const_mult_vec(a,constant,b)
    Real a[MAXDIM] ;
    Real b[MAXDIM] ;
    double constant ;
{
    int i;

    for (i = 0 ;i < MAXDIM ;i++) {
	a[i] += constant * b[i] ;
    }
}
