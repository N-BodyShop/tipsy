#include "defs.h"
#include "fdefs.h"

void
vconst_sub(job)
    char *job;
{
    char command[MAXCOMM] ;

    if(sscanf(job,"%s %lf %lf", command,
	    &alpha,&beta) == 5){
	visc_loaded = YES ;
    }
    else {
	input_error(command) ;
    }
}
