#include "defs.h"
#include "fdefs.h"

void
redshift_sub(job)
    char *job;
{
    char command[MAXCOMM] ;
    char coord[MAXCOMM] ;
    char boundary[MAXCOMM] ;

    if (sscanf(job,"%s %lf %s %lf %lf %s %lf", command,&redshift, coord,
	    &hubble_constant,&omega,boundary,&period_size) == 7){
	if(strcmp(coord,"physical") == 0 || strcmp(coord,"p") == 0){
	    cosmof3 = 1.0;
	    cosmof = 1.0;
	    comove = NO ;
	}
	else if(strcmp(coord,"comove") == 0 || strcmp(coord,"c") == 0){
	    cosmof = 1.0/(1.0 + redshift);
	    cosmof3 = cosmof*cosmof*cosmof;
	    comove = YES ;
	    hubble_constant *= sqrt(1 + omega*redshift)*(1 + redshift) ;
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
	cooling_loaded = NO ;
    }
    else {
	input_error(command) ;
    }
}
