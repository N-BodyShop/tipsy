#include "defs.h"
#include "fdefs.h"

void
transpose(mat,trans_mat)
    double mat[MAXDIM][MAXDIM] ;
    double trans_mat[MAXDIM][MAXDIM] ;
{
    int i,j ;

    for (i=0 ;i < MAXDIM ;i++) {
	for (j=0 ;j < MAXDIM ;j++) {
	    trans_mat[i][j] = mat[j][i] ;
	}
    }
}
