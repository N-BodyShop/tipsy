#include "defs.h"

scale_sub(job)
    char *job;
{
    char command[MAXCOMM] ;
    double scale;
    int i, j;

    if (sscanf(job,"%s %lf",command,&scale) ==2) {
	for (i=0 ;i < MAXDIM ;i++) {
	    for (j=0 ;j < MAXDIM ;j++) {
		rot_matrix[i][j] *= scale;
	    }
	}
    }
    else {
	input_error(command) ;
    }
}
