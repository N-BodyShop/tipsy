/* $Header$
 * $Log$
 * Revision 1.2  1995/03/02 17:30:22  nsk
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

    if(sscanf(job,"%s %lf %lf %lf %lf %lf", command, 
	      &kpcunit, &msolunit, &fhydrogen, &jnu21, &alphaj) == 6){
	cool_loaded = YES ;
	cooling_loaded = NO ;
	time_unit = sqrt(pow(kpcunit*KPCCM, 3.0)/(GCGS*msolunit*MSOLG));
	time_unit /= GYRSEC;
	if(jnu21 > 0.0){
	    ionize() ;
	}
	else{
	    gp0_H = 0.0 ;
	    gp0_He = 0.0 ;
	    gp0_Hep = 0.0 ;
	}
	lum_loaded = YES;
    }
    else {
	input_error(command) ;
    }
}
