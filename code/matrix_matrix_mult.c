#include "defs.h"
#include "fdefs.h"

void
matrix_matrix_mult(a,b,c)
    double a[MAXDIM][MAXDIM] ;
    double b[MAXDIM][MAXDIM] ;
    double c[MAXDIM][MAXDIM] ;
{
    int i,j,k ;

    for (i=0 ;i < MAXDIM ;i++) {
	for (j=0 ;j < MAXDIM ;j++) {
	    c[i][j] = 0.0 ;
	    for (k=0 ;k < MAXDIM ;k++) {
		c[i][j] += a[i][k] * b[k][j] ;
	    }
	}
    }
}
