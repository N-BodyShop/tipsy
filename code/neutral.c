/* $Header$
 * $Log$
 * Revision 1.4  1997/09/05 01:02:23  nsk
 * streamlined vista (I hope it works), added neutralize command, added SZ
 * effect to vista.
 *
 * Revision 1.3  1995/12/11  20:00:48  nsk
 * added helium, dark absorb,  integral for elcetronic heating,
 * and read in 6 numbers for background
 *
 * Revision 1.2  1995/07/24  20:51:16  trq
 * Fixed bug in load_cool.c.
 * vista now has a velocity cut.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:37  trq
 * Import to CVS
 *
 * Revision 2.1  1994/09/22  16:20:14  trq
 * Changed to calculate neutral fraction of all particles in box 0.
 *
 */
#include <stdlib.h>
#include "defs.h"
#include "fdefs.h"

void
hneutral_func()
{
    struct gas_particle *gp ;
    int i ;
    double hneutral_p ;
    double heneutral_p ;
    double heII_p ;

    if (!cool_loaded ){
	load_cool() ;
    }
    if (!redshift_loaded ){
	load_redshift() ;
    }
    max_temp_old = -HUGE ;
    min_rho_old = HUGE ;
    if(hneutral != NULL) free(hneutral);
    if(heneutral != NULL) free(heneutral);
    if(heII != NULL) free(heII);
    if(header.nsph != 0) {
	hneutral = (double *)malloc(header.nsph*sizeof(*hneutral));
	heneutral = (double *)malloc(header.nsph*sizeof(*heneutral));
	heII = (double *)malloc(header.nsph*sizeof(*heII));
	if(hneutral == NULL || heneutral == NULL || heII == NULL)  {
	    printf("<sorry, no memory for hneutral, %s>\n",title) ;
	    free(hneutral) ;
	    free(heneutral) ;
	    free(heII) ;
	    return ;
	}
    }
    else {
      hneutral = NULL;
      heneutral = NULL;
      heII = NULL;
    }

    printf("<calculating neutral fraction, please be patient %s>\n",
	   title);
    
    for (i = 0 ;i < boxlist[0].ngas ;i++) {
	gp = boxlist[0].gp[i] ;
	calc_hneutral(gp->temp, gp->rho, &hneutral_p, &heneutral_p, &heII_p);
	hneutral[gp-gas_particles] = hneutral_p ;
	heneutral[gp-gas_particles] = heneutral_p ;
	heII[gp-gas_particles] = heII_p ;
    }
    printf("<finished calculating neutral fraction, %s>\n",
	   title);
    hneutral_loaded = YES ;
}
