#include "defs.h"

void
load_cool() 
{
    char *cool_const ;
    double c1,c2,c3,c4,c5,c6 ;
    int number ;

    forever{
        printf("<enter: kpcunit msolunit fhydrogen>\n") ;
	cool_const = my_gets(" ") ;
	if((number=sscanf(cool_const,"%lf %lf %lf",
		&kpcunit,&msolunit,&fhydrogen)) == 3){
	    break ;
	}
	if((number=sscanf(cool_const,"%lf %lf %lf %lf %lf",
		&kpcunit,&msolunit,&fhydrogen,&slowcool,&dtcool)) == 5){
	    break ;
	}
	printf("<sorry, inconsistent input, %s>\n",title) ;
    }
    if(number == 3){
        dtcool = 1. ;
        slowcool = 1. ;
    }
    time_unit = sqrt(pow(kpcunit*KPCCM, 3.0)/(GCGS*msolunit*MSOLG));
    time_unit /= GYRSEC;
    cool_loaded = YES ;
    lum_loaded = YES;
    dudt_loaded = NO ;
    starform_loaded = NO ;
    cooling_loaded = NO ;
    hneutral_loaded = NO ;
    meanmwt_loaded = NO ;
    dudt_loaded = NO ;
}
