#include "defs.h"
#include "fdefs.h"

void
plane(x1,x2,x3,x4,constant)
    Real x1[MAXDIM] ;
    Real x2[MAXDIM] ;
    Real x3[MAXDIM] ;
    Real x4[MAXDIM] ;
    double constant[5] ;
{
    int i ;
    double dx[MAXDIM] ;
    double dprime ;
    double lower,upper ;

				/* explicity cast to double to avoid */
				/* rounding errors */
    constant[0]=((double)x1[1] * x2[2]) + ((double)x2[1] * x3[2])
      + ((double)x3[1] * x1[2])
	    - ((double)x3[1] * x2[2]) - ((double)x2[1] * x1[2])
	      - ((double)x1[1] * x3[2]) ;
    constant[1]=((double)x3[0] * x2[2]) + ((double)x1[0] * x3[2])
      + ((double)x2[0] * x1[2])
	    - ((double)x1[0] * x2[2]) - ((double)x3[0] * x1[2])
	      - ((double)x2[0] * x3[2]) ;
    constant[2]=((double)x1[0] * x2[1]) + ((double)x2[0] * x3[1])
      + ((double)x3[0] * x1[1])
	    - ((double)x3[0] * x2[1]) - ((double)x2[0] * x1[1])
	      - ((double)x1[0] * x3[1]) ;
    constant[3]=((double)x1[0] * x2[1] * x3[2])
      + ((double)x2[0] * x3[1] * x1[2]) +
	    ((double)x3[0] * x1[1] * x2[2]) - ((double)x3[0] * x2[1] * x1[2]) -
	    ((double)x2[0] * x1[1] * x3[2]) - ((double)x1[0] * x3[1] * x2[2]) ;
    for (i=0 ;i < header.ndim ;i++) {
	dx[i]=x4[i] - x3[i] ;
    }
    dprime = constant[3] + (constant[0] * dx[0])+
	    (constant[1] * dx[1])+ (constant[2] * dx[2]) ;
    lower = min(constant[3],dprime) ;
    upper = max(constant[3],dprime) ;
    constant[3] = lower ;
    constant[4] = upper ;
}
