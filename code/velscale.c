#include "defs.h"
velscale(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    double input ;

    if (sscanf(job,"%s %lf",command,&input) ==2) {
	vel_scale = input ;
    }
    else {
	input_error(command) ;
    }
}
