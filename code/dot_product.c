#include "defs.h"
double dot_product(x1,x2)
    Real x1[MAXDIM] ;
    Real x2[MAXDIM] ;
{
    double dot ;
    int i ;

    for (i=0, dot = 0.0 ; i < MAXDIM ;i++) {
	dot += (x1[i] * x2[i]) ;
    }
    return(dot) ;
}
