/* $Header$
 * $Log$
 * Revision 1.6  2000/01/12 22:55:01  nsk
 * Fixed bugs in cooling routines, added cooling damping,
 * fixed bugs in starformation,  regularized dependencies.
 *
 * Revision 1.5  1998/02/27  19:51:32  nsk
 * Fixed star formation stuff, added starform routine
 *
 * Revision 1.4  1997/09/25  18:35:09  nsk
 * added uv command and discrete uv sources
 *
 * Revision 1.3  1995/12/11  20:00:42  nsk
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

    if((number = sscanf(job,"%s %lf %lf %lf %lf %lf", command, &kpcunit,
	    &msolunit,&fhydrogen,&slowcool,&dtcool)) == 6 || 
	    sscanf(job,"%s %lf %lf %lf", command, &kpcunit,
	    &msolunit,&fhydrogen) == 4){
	if(number == 4){
	    dtcool = 1. ;
	    slowcool = 1. ;
	}
	time_unit = sqrt(pow(kpcunit*KPCCM, 3.0)/(GCGS*msolunit*MSOLG));
	time_unit /= GYRSEC;
	lum_loaded = YES;
	cool_loaded = YES ;
	dudt_loaded = NO ;
	cooling_loaded = NO ;
	hneutral_loaded = NO ;
	meanmwt_loaded = NO ;
	starform_loaded = NO ;
	dudt_loaded = NO ;
    }
    else {
	input_error(command) ;
    }
}
