#include "defs.h"
vec_add_const_mult_vec(a,b,constant,c)
    Real a[MAXDIM] ;
    Real b[MAXDIM] ;
    double constant ;
    Real c[MAXDIM] ;
{
    int i;

    for (i = 0 ;i < MAXDIM ;i++) {
	a[i] = b[i] + constant * c[i] ;
    }
}
