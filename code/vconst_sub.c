#include "defs.h"
#include "fdefs.h"

void
vconst_sub(job)
    char *job;
{
    char command[MAXCOMM] ;

    if(sscanf(job,"%s %lf %lf", command,
	    &alpha,&beta) == 3){
	visc_loaded = YES ;
	dudt_loaded = NO ;
    }
    else {
	input_error(command) ;
    }
}
