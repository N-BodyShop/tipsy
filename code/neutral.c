/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:36  trq
 * Initial revision
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

    if (!cool_loaded ){
	load_cool() ;
    }
    if (!redshift_loaded ){
	load_redshift() ;
    }
    if(hneutral != NULL) free(hneutral);
    if(header.nsph != 0) {
	hneutral = (double *)malloc(header.nsph*sizeof(*hneutral));
	if(hneutral == NULL) {
	    printf("<sorry, no memory for hneutral, %s>\n",title) ;
	    return ;
	}
    }
    else
      hneutral = NULL;

    printf("<calculating neutral fraction, please be patient %s>\n",
	   title);
    
    for (i = 0 ;i < boxlist[0].ngas ;i++) {
	gp = boxlist[0].gp[i] ;
	hneutral[gp-gas_particles] = calc_hneutral(gp->temp, gp->rho);
    }
    hneutral_loaded = YES ;
}
