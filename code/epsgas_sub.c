#include "defs.h"
epsgas_sub(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;

    if (sscanf(job,"%s %lf",command,&epsgas_grav) == 2 ) {
	epsgas_loaded = YES ;
	starform_loaded = NO ;
    }
    else {
	input_error(command) ;
    }
}
