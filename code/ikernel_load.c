#include "defs.h"

void *malloc();

void
ikernel_load()
{
    int i ;
    double radius ;
    double radius2 ;
    double radius4 ;
    double ikernel ;
    double d,d2 ;

    deldr2 = 4./(double)NINTERP ;
    deldr2i = 1. / deldr2 ;
    if(iwsmooth == NULL)
      {
	iwsmooth = malloc((NINTERP+2)*sizeof(*iwsmooth));
	if(iwsmooth == NULL)
	  {
	    printf("<sorry, no memory for iwsmooth, %s>\n",title) ;
	    return ;
	  }
      }
    for(i = 0; i <= NINTERP + 1; i++){
	radius2 = (double)i * deldr2 ;
	radius4 = radius2 * radius2 ;
	radius = sqrt(radius2) ;
	ikernel = 0. ;
	if(radius2 < 4.){
	    d2 = 4. - radius2 ;
	    d = sqrt(d2) ;
	    ikernel += (3. * (-1. + .875 * radius2) * d + d * d2 - 3. *
		(radius2 + radius4 * .0625) * log(d + 2.)) ;
	}
	if(radius2 < 1.){
	    d2 = 1. - radius2 ;
	    d = sqrt(d2) ;
	    ikernel += 1.5 * (1. - 3.5 * radius2) * d -2. * d2 * d +
		3. * (radius2 + radius4 *.25) * log(d + 1) ;
	}
	if(radius != 0. && radius2 < 4.){
	    if(radius2 < 1.){
		ikernel -= .5625 * radius4 * log(radius) ;
	    }
	    else{
		ikernel += 3. * (radius2 + .0625 * radius4) * log(radius) ;
	    }
	}
	iwsmooth[i] = ikernel ;
    }
    ikernel_loaded = YES ;
}
