#include "defs.h"
#include "fdefs.h"

void
redshift_sub(job)
    char *job;
{
    char command[MAXCOMM] ;
    char coord[MAXCOMM] ;
    char boundary[MAXCOMM] ;
    double lambda ;
    double zp1 ;

    if (sscanf(job,"%s %lf %s %lf %lf %lf %s %lf", command,&redshift, coord,
	    &hubble_constant,&omega,&lambda,boundary,&period_size) == 8){
	if(strcmp(coord,"physical") == 0 || strcmp(coord,"p") == 0){
	    cosmof3 = 1.0;
	    cosmof = 1.0;
	    comove = NO ;
	}
	else if(strcmp(coord,"comove") == 0 || strcmp(coord,"c") == 0){
	    cosmof = 1.0/(1.0 + redshift);
	    cosmof3 = cosmof*cosmof*cosmof;
	    comove = YES ;
	    zp1 = 1. + redshift ;
	    hubble_constant *= sqrt(omega*zp1*zp1*zp1+
			       (1.-omega-lambda)*zp1*zp1+lambda) ;
	}
	else {
	    printf("<sorry, %s is not a coordinate type, %s>\n",
		    coord,title) ;
	    return ;
	}
	if(strcmp(boundary,"periodic") == 0 ||
		strcmp(boundary,"p") == 0){
	    periodic = YES ;
	}
	else if(strcmp(boundary,"vacuum") == 0 ||
		strcmp(boundary,"v") == 0){
	    periodic = NO ;
	}
	else {
	    printf("<sorry, %s is not a boundary type, %s>\n",
		    boundary,title) ;
            return ;
	}
	redshift_loaded = YES ;
	starform_loaded = NO ;
	dudt_loaded = NO ;
	cooling_loaded = NO ;
	meanmwt_loaded = NO ;
	hneutral_loaded = NO ;
    }
    else {
	input_error(command) ;
    }
}
