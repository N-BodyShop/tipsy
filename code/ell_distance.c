#include "defs.h"
double ell_distance(x1)
    Real x1[MAXDIM] ;
{
    double seperation ;
    int i ;
    Real dx[MAXDIM] ;
    Real dx_rot[MAXDIM] ;
    double invba2, invca2;

    for (i=0 ;i < MAXDIM ;i++) {
	dx[i] = x1[i] - center_ell[i] ;
    }
    matrix_vector_mult(ell_matrix,dx,dx_rot) ;
    if(ca != 0.0)
	invca2 = 1.0/(ca*ca);
    else
	invca2 = HUGE;
    if(ba != 0.0)
	invba2 = 1.0/(ba*ba);
    else
	invba2 = HUGE;
    seperation = dx_rot[0] * dx_rot[0] + dx_rot[1] * dx_rot[1] *invba2
	    + dx_rot[2] * dx_rot[2] * invca2;
    return(sqrt(seperation)) ;
}
