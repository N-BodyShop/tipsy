#include "defs.h"
#include "fdefs.h"

void
matrix_vector_mult(mat,a,b)
    double mat[MAXDIM][MAXDIM] ;
    Real a[MAXDIM] ;
    Real b[MAXDIM] ;
{
    int i,j ;

    for (i = 0 ;i < MAXDIM ;i++) {
	b[i] = 0. ;
	for (j = 0 ;j < MAXDIM ;j++) {
	    b[i] += mat[i][j] * a[j] ;
	}
    }
}
