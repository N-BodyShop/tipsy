#include "defs.h"
#include "fdefs.h"

void
lconst_sub(job)
    char *job;
{
    char command[MAXCOMM] ;

    if(sscanf(job,"%s %lf %lf",command,&msolunit,&kpcunit) == 3){
	time_unit = sqrt(pow(kpcunit*KPCCM, 3.0)/(GCGS*msolunit*MSOLG));
	time_unit /= GYRSEC;
	lum_loaded = YES ;
	hneutral_loaded = NO ;
	meanmwt_loaded = NO ;
	cooling_loaded = NO ;
    }
    else{
	input_error(command) ;
    }
}
