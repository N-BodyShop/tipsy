#include "defs.h"

void
load_redshift() 
{
    char *red_const ;
    char coord[MAXCOMM] ;
    char boundary[MAXCOMM] ;

    forever{
	printf("<enter: redshift, coordinates (comove or physical,") ;
	printf(" hubble constant, omega, boundary, and period)>\n") ;
	red_const = my_gets(" ") ;
	if(sscanf(red_const,"%lf %s %lf %lf %s %lf",&redshift, coord,
		&hubble_constant,&omega,boundary,&period_size) == 6){
	    if(strcmp(coord,"physical") == 0 || strcmp(coord,"p") == 0){
		cosmof3 = 1.0;
		cosmof = 1.0;
		comove = NO ;
		if(strcmp(boundary,"periodic") == 0 ||
			strcmp(boundary,"p") == 0){
		    periodic = YES ;
		    break ;
		}
		else if(strcmp(boundary,"vacuum") == 0 || 
			strcmp(boundary,"v") == 0){
		    periodic = NO ;
		    break ;
		}
		else {
		    printf("<sorry, %s is not a boundary type, %s>\n",
			    boundary,title) ;
		}
	    }
	    else if (strcmp(coord,"comove") == 0 || strcmp(coord,"c") == 0){
		cosmof = 1.0/(1.0 + redshift);
		cosmof3 = cosmof*cosmof*cosmof;
		comove = YES ;
		hubble_constant *= sqrt(1 + omega*redshift)*(1 + redshift) ;
		if(strcmp(boundary,"periodic") == 0 ||
			strcmp(boundary,"p") == 0){
		    periodic = YES ;
		    break ;
		}
		else if(strcmp(boundary,"vacuum") == 0 || 
			strcmp(boundary,"v") == 0){
		    periodic = NO ;
		    break ;
		}
		else {
		    printf("<sorry, %s is not a boundary type, %s>\n",
			    boundary,title) ;
		}
	    }
	    else {
		printf("<sorry, %s is not a coordinate type, %s>\n",
			coord,title) ;
	    }
	}
	printf("<sorry, inconsistent input, %s>\n",title) ;
    }
    redshift_loaded = YES ;
    starform_loaded = NO ;
    cooling_loaded = NO ;
    hneutral_loaded = NO ;
    meanmwt_loaded = NO ;
}
