#include "defs.h"

int
load_redshift() 
{
    char *red_const ;
    char coord[MAXCOMM] ;
    char boundary[MAXCOMM] ;
    double lambda ;
    char response[MAXCOMM] ;

    redshift = 0. ;
    hubble_constant = 50. ;
    omega = 1. ;
    lambda = 0. ;
    period_size = 1. ;
    cosmof3 = 1.0;
    cosmof = 1.0;
    comove = NO ;
    periodic = NO ;

    printf("<Assuming z = %g, physical coordinates, H0 = %g km/sec/Mpc>\n",
	    redshift, hubble_constant);
    printf("<Omega = %g, Lambda = %g, vacuum boundary>\n",omega,lambda);
    printf("<Is this alright, %s (y/n)?>\n",title) ;
    red_const = my_gets(" ") ;
    sscanf(red_const,"%s",response) ;
    if(strcmp(response,"yes")==0 || strcmp(response,"y")==0){
	redshift_loaded = YES ;
	starform_loaded = NO ;
	cooling_loaded = NO ;
	hneutral_loaded = NO ;
	meanmwt_loaded = NO ;
	dudt_loaded = NO ;
	return 1 ;
    }
    else {
	printf("Use the redshift command to change these values, %s\n",title);
	return 1 ;
    }
}
