#include "defs.h"
double distance(x1,x2)
    Real x1[MAXDIM] ;
    Real x2[MAXDIM] ;
{
    double seperation ;
    int i ;

    for (i=0, seperation = 0.0 ; i < MAXDIM ;i++) {
	seperation += (x1[i] - x2[i]) * (x1[i] - x2[i]) ;
    }
    return(sqrt(seperation)) ;
}
