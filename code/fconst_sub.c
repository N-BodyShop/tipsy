#include "defs.h"
fconst_sub(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;

    if(sscanf(job,"%s %lf %lf %lf %lf",command,&cstar,&dtime,&temppar,
	    &softpar) == 5){
	form_loaded = YES ;
	starform_loaded = NO ;
    }
    else {
	input_error(command) ;
    }
}
