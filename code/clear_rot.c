#include "defs.h"

void reset_rotate_scroll();

void
clear_rot()
{
    int i,j ;

    reset_rotate_scroll();
    for (i=0 ;i < MAXDIM ;i++) {
	for (j=0 ;j < MAXDIM ;j++) {
	    if ( i == j) {
		rot_matrix[i][j] = 1.0 ;
	    }
	    else {
		rot_matrix[i][j] = 0.0 ;
	    }
	}
    }
}
