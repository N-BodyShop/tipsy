#include "defs.h"
#include "fdefs.h"

void
rotate(direction,angle)
    int direction ;
    double angle ;
{
    double old_matrix[MAXDIM][MAXDIM] ;
    double new_matrix[MAXDIM][MAXDIM] ;
    int i,j ;

    angle *= PI / 180.0 ;
    for (i=0 ;i < MAXDIM ;i++) {
	for (j=0 ;j < MAXDIM ;j++) {
	    old_matrix[i][j] = rot_matrix[i][j] ;
	    if ( i == j) {
		new_matrix[i][j] = 1.0 ;
	    }
	    else {
		new_matrix[i][j] = 0.0 ;
	    }
	}
    }

    if(direction == LEFT || direction == RIGHT ) {
	if (direction == LEFT ) {
	    angle = -angle ;
	}
	new_matrix[0][0] = cos(angle) ;
	new_matrix[2][2] = cos(angle) ;
	new_matrix[0][2] = sin(angle) ;
	new_matrix[2][0] = -sin(angle) ;
    }
    else if(direction == UP || direction == DOWN ) {
	if (direction == DOWN ) {
	    angle = -angle ;
	}
	new_matrix[1][1] = cos(angle) ;
	new_matrix[2][2] = cos(angle) ;
	new_matrix[1][2] = sin(angle) ;
	new_matrix[2][1] = -sin(angle) ;
    }
    else {
	new_matrix[0][0] = cos(angle) ;
	new_matrix[1][1] = cos(angle) ;
	new_matrix[0][1] = sin(angle) ;
	new_matrix[1][0] = -sin(angle) ;
    }
    matrix_matrix_mult(new_matrix,old_matrix,rot_matrix) ;
}
