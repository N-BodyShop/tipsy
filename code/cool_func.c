#include <stdlib.h>
#include "defs.h"
#include "fdefs.h"

void
cool_func()
{
    struct gas_particle *gp ;
    int i ;

    if (!cool_loaded ){
	load_cool() ;
    }
    if (!uv_loaded ){
	load_uv() ;
    }
    if (!redshift_loaded ){
	load_redshift() ;
    }
    if(cooling != NULL) free(cooling);
    if(boxlist[active_box].ngas != 0) {
	cooling = (double *)malloc(boxlist[active_box].ngas *sizeof(*cooling));
	if(cooling == NULL) {
	    printf("<sorry, no memory for cooling, %s>\n",title) ;
	    return ;
	}
    }
    else
      cooling = NULL;

    for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
	gp = boxlist[active_box].gp[i] ;
	if(!uniform){
	    calc_uv(gp) ;
	}
	cooling[i] = heatcool(gp->temp, gp->rho);
    }
    cooling_loaded = YES ;
}
