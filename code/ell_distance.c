#include "defs.h"
double ell_distance(x1)
    Real x1[MAXDIM] ;
{
    double seperation ;
    int i ;
    Real dx[MAXDIM] ;
    Real dx_rot[MAXDIM] ;

    for (i=0 ;i < MAXDIM ;i++) {
	dx[i] = x1[i] - center_ell[i] ;
    }
    matrix_vector_mult(ell_matrix,dx,dx_rot) ;
    seperation = dx_rot[0] * dx_rot[0] + dx_rot[1] * dx_rot[1] / ba / ba
	    + dx_rot[2] * dx_rot[2] / ca / ca ;
    return(sqrt(seperation)) ;
}
