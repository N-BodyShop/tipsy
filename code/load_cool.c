#include "defs.h"

void
load_cool() 
{
    char *cool_const ;
    int number ;
    double c1,c2,c3,c4,c5,c6 ;

    forever{
        printf("<enter: kpcunit msolunit fhydrogen jnu21 alphaj>\n") ;
	cool_const = my_gets(" ") ;
	if((number = sscanf(cool_const,"%lf %lf %lf %lf %lf %lf %lf %lf %lf",
		&kpcunit, &msolunit, &fhydrogen, &c1, &c2,&c3,&c4,&c5,&c6))
		== 5 || number == 9){
	    break ;
	}
	printf("<sorry, inconsistent input, %s>\n",title) ;
    }
    time_unit = sqrt(pow(kpcunit*KPCCM, 3.0)/(GCGS*msolunit*MSOLG));
    time_unit /= GYRSEC;
    if(number == 5){
	jnu21 = c1 ;
	alphaj = c2 ;
	if(jnu21 > 0.0){
	    ionize() ;
	    gp0_H *= jnu21 * 4 * PI * 1.e-21 ;
	    gp0_He *= jnu21 * 4 * PI * 1.e-21 ;
	    gp0_Hep *= jnu21 * 4 * PI * 1.e-21 ;
	    eps_H *= jnu21 * 4 * PI * 1.e-21 ;
	    eps_He *= jnu21 * 4 * PI * 1.e-21 ;
	    eps_Hep *= jnu21 * 4 * PI * 1.e-21 ;
	}
	else{
	    gp0_H = 0.0 ;
	    gp0_He = 0.0 ;
	    gp0_Hep = 0.0 ;
	    eps_H = 0.0 ;
	    eps_He = 0.0 ;
	    eps_Hep = 0.0 ;
	}
    }
    else{
	gp0_H = c1 ;
	gp0_He = c2 ;
	gp0_Hep = c3 ;
	eps_H = c4 ;
	eps_He = c5 ;
	eps_Hep = c6 ;
    }
    cool_loaded = YES ;
    lum_loaded = YES;
    cooling_loaded = NO ;
    hneutral_loaded = NO ;
}
