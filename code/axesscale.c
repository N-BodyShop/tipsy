#include "defs.h"
axesscale(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    double axes_scale ;

    if (sscanf(job,"%s %lf",command,&axes_scale) ==2) {
	axes_coord[1][0] = axes_scale ;
	axes_coord[3][1] = axes_scale ;
	axes_coord[5][2] = axes_scale ;
    }
    else {
	input_error(command) ;
    }
}
