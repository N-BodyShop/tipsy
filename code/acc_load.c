/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:22  trq
 * Initial revision
 *
 * Revision 2.2  94/04/20  08:45:15  trq
 * Added title variable.
 * 
 */
#include "defs.h"

void *malloc();

void
acc_load()
{
    int i ;
    double xw ;
    double xw2 ;
    double xw3 ;
    double xw4 ;

    deldrg = 2./(double)NINTERP ;
    if(acsmooth == NULL)
      {
	acsmooth = malloc((NINTERP+2)*sizeof(*acsmooth));
	if(acsmooth == NULL)
	  {
	    printf("<sorry, no memory for acsmooth, %s>\n",title) ;
	    return ;
	  }
      }
    for(i = 0; i <= NINTERP + 1; i++){
	xw = (double)i * deldrg ;
	xw2 = xw * xw ;
	xw3 = xw2 * xw ;
	xw4 = xw2 * xw2 ;
	if(xw <= 1.){
	    acsmooth[i] = xw3*(4./3.-6.*xw2/5.+0.5*xw3) ;
	}
	else if(xw > 1. && xw < 2.){
	    acsmooth[i] = -1./15+8.*xw3/3.-3.*xw4+6.*xw3*xw2/5.-xw4*xw2/6. ;
	}
	else{
	    acsmooth[i] = 1. ;
	}
    }
    acc_loaded = YES ;
}
