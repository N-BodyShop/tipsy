#include "defs.h"

void *malloc();

void
dkernel_load()
{
    int i ;
    double radius ;
    double radius2 ;
    double dkernel ;

    deldr2 = 4./(double)NINTERP ;
    deldr2i = 1. / deldr2 ;
    if(dwsmooth == NULL)
      {
	dwsmooth = malloc((NINTERP+2)*sizeof(*dwsmooth));
	if(dwsmooth == NULL)
	  {
	    printf("<sorry, no memory for dwsmooth, %s>\n",title) ;
	    return ;
	  }
      }
    for(i = 0; i <= NINTERP + 1; i++){
	radius2 = (double)i * deldr2 ;
	radius = sqrt(radius2) ;
	if(radius2 < 4. && radius2 > 1.){
	    dkernel = -.75 * (2. - radius) * (2. - radius) / radius ;
	}
	else if(radius2 <= 1.){
	    dkernel = -3. + 2.25 * radius ;
	}
	else{
	    dkernel = 0. ;
	}
	dwsmooth[i] = dkernel ;
    }
    dkernel_loaded = YES ;
}
