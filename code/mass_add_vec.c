#include "defs.h"
#include "fdefs.h"

void
mass_add_vec(a,b,mass_b,c,mass_c)
    Real a[MAXDIM] ;
    Real b[MAXDIM] ;
    Real c[MAXDIM] ;
    double mass_b ;
    double mass_c ;
{
    int i;

    for (i = 0 ;i < MAXDIM ;i++) {
	a[i] = (mass_b * b[i] + mass_c * c[i]) / (mass_b + mass_c) ;
    }
}
