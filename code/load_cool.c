#include "defs.h"

void
load_cool() 
{
    char *cool_const ;

    forever{
        printf("<enter: kpcunit msolunit fhydrogen jnu21 alphaj>\n") ;
	cool_const = my_gets(" ") ;
	if(sscanf(cool_const,"%lf %lf %lf %lf %lf",
		&kpcunit, &msolunit, &fhydrogen, &jnu21, &alphaj)
		== 5){
	    break ;
	}
	printf("<sorry, inconsistent input, %s>\n",title) ;
    }
    time_unit = sqrt(pow(kpcunit*KPCCM, 3.0)/(GCGS*msolunit*MSOLG));
    time_unit /= GYRSEC;
    cool_loaded = YES ;
    lum_loaded = YES;
}
