/* $Header$
 * $Log$
 * Revision 1.3  1995/12/11 20:00:42  nsk
 * added helium, dark absorb,  integral for elcetronic heating,
 * and read in 6 numbers for background
 *
 * Revision 1.2  1995/03/02  17:30:22  nsk
 * changed absorption cross section tb be done by integral
 * added optical depth output and fixed bug in absorb
 * added stellar mass plot to view_star
 *
 * Revision 1.1.1.1  1995/01/10  22:57:23  trq
 * Import to CVS
 *
 * Revision 2.2  94/04/20  08:14:47  trq
 * rationalized units.
 * 
 * Revision 2.1  1992/11/11  15:19:03  trq
 * fixed bug in sscanf
 *
 */
#include "defs.h"
#include "fdefs.h"

void
cconst_sub(job)
    char *job;
{
    char command[MAXCOMM] ;
    double c1,c2,c3,c4,c5,c6 ;
    int number ;

    if((number = sscanf(job,"%s %lf %lf %lf %lf %lf %lf %lf %lf %lf", command, 
	      &kpcunit, &msolunit, &fhydrogen, &c1, &c2,&c3,&c4,&c5,&c6)) == 6
	      || number == 10){
	cool_loaded = YES ;
	cooling_loaded = NO ;
	hneutral_loaded = NO ;
	time_unit = sqrt(pow(kpcunit*KPCCM, 3.0)/(GCGS*msolunit*MSOLG));
	time_unit /= GYRSEC;
	if(number == 6){
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
	lum_loaded = YES;
    }
    else {
	input_error(command) ;
    }
}
