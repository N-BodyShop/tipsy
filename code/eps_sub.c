#include "defs.h"
#include "fdefs.h"
void
eps_sub(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;

    if (sscanf(job,"%s %lf",command,&eps_grav) == 2 ) {
	eps_loaded = YES ;
    }
    else {
	input_error(command) ;
    }
}
