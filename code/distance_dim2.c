#include "defs.h"
double distance_dim2(x1,x2)
    double x1[2] ;
    double x2[2] ;
{
    double seperation ;
    int i ;

    for (i=0, seperation = 0.0 ; i < 2 ;i++) {
	seperation += (x1[i] - x2[i]) * (x1[i] - x2[i]) ;
    }
    return(seperation) ;
}
