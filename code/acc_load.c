/* $Header$
 * $Log$
 * Revision 1.2  2010/09/14 22:06:33  trq
 * Added potential to rotation curve command.
 *
 * Revision 1.1.1.1  1995/01/10 22:57:23  trq
 * Import to CVS
 *
 * Revision 2.2  94/04/20  08:45:15  trq
 * Added title variable.
 * 
 */
#include "defs.h"
#include <stdlib.h>

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
	potsmooth = malloc((NINTERP+2)*sizeof(*potsmooth));
	if(potsmooth == NULL)
	  {
	    printf("<sorry, no memory for potsmooth, %s>\n",title) ;
	    return ;
	  }
      }
    for(i = 0; i <= NINTERP + 1; i++){
	xw = (double)i * deldrg ;
	xw2 = xw * xw ;
	xw3 = xw2 * xw ;
	xw4 = xw2 * xw2 ;
	if(xw <= 1.){
	    potsmooth[i] = xw*(7.0/5.0 - 2.0/3.0*xw2 + 3.0/10.0*xw4
					 - 1.0/10.0*xw4*xw);
	    acsmooth[i] = xw3*(4./3.-6.*xw2/5.+0.5*xw3) ;
	}
	else if(xw > 1. && xw < 2.){
	    potsmooth[i] = -1.0/15.0 + xw*(8.0/5.0 - 4.0/3.0*xw2 + xw3
			              - 3.0/10.0*xw4 + 1.0/30.0*xw4*xw);
	    acsmooth[i] = -1./15+8.*xw3/3.-3.*xw4+6.*xw3*xw2/5.-xw4*xw2/6. ;
	}
	else{
	    acsmooth[i] = 1. ;
	    potsmooth[i] = 1. ;
	}
    }
    acc_loaded = YES ;
}
