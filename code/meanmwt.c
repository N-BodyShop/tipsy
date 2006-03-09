#include <stdlib.h>
#include "defs.h"
#include "fdefs.h"

void
meanmwt_func()
{
    struct gas_particle *gp ;
    int i ;

    if (!redshift_loaded){
	load_redshift() ;
    }
    if (!cool_loaded ){
	load_cool() ;
    }
    if (!uv_loaded ){
	load_uv() ;
    }
    if(meanmwt != NULL) free(meanmwt);
    if(boxlist[0].ngas != 0) {
	meanmwt = (double *)malloc(boxlist[0].ngas
				    *sizeof(*meanmwt));
	if(meanmwt == NULL) {
	    printf("<sorry, no memory for meanmwt, %s>\n",title) ;
	    return ;
	}
    }
    else
      meanmwt = NULL;

    for (i = 0 ;i < boxlist[0].ngas ;i++) {
	gp = boxlist[0].gp[i] ;
	if(!uniform){
	    calc_uv(gp) ;
	}
	meanmwt[i] = calc_meanmwt(gp->temp, gp->rho);
    }
    meanmwt_loaded = YES ;
}
