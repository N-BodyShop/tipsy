#include <stdlib.h>
#include "defs.h"
#include "fdefs.h"

void
meanmwt_func()
{
    struct gas_particle *gp ;
    int i ;

    if (!cool_loaded ){
	load_cool() ;
    }
    if (!redshift_loaded ){
	load_redshift() ;
    }
    if(meanmwt != NULL) free(meanmwt);
    if(boxlist[active_box].ngas != 0) {
	meanmwt = (double *)malloc(boxlist[active_box].ngas
				    *sizeof(*meanmwt));
	if(meanmwt == NULL) {
	    printf("<sorry, no memory for meanmwt, %s>\n",title) ;
	    return ;
	}
    }
    else
      meanmwt = NULL;

    for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
	gp = boxlist[active_box].gp[i] ;
	meanmwt[i] = calc_meanmwt(gp->temp, gp->rho);
    }
    meanmwt_loaded = YES ;
}
