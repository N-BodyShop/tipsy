#include "defs.h"
#include <malloc.h>

void
divv()
{
    struct gas_particle *pp ;
    struct gas_particle *gp ;
    int i,j ;
    int iwsm ;
    int iwsm1 ;
    double dot_product() ;
    double hsminv ;
    double dwnorm ;
    double distnorm ;
    double dr2 ;
    double dr2i ;
    double dr2p ;
    double drw ;
    double drw1 ;
    double dwsm ;
    double dwsm1 ;
    double dwmass ;
    double dwmass1 ;
    Real dr[MAXDIM] ;
    Real dv[MAXDIM] ;
    double vdotdr ;

    if(!dkernel_loaded){
	dkernel_load() ;
    }
    if(hsmdivv != NULL) free(hsmdivv);
    if(boxlist[active_box].ngas != 0) {
	hsmdivv = (double *)malloc(boxlist[active_box].ngas *sizeof(*hsmdivv));
	if(hsmdivv == NULL) {
	    printf("<sorry, no memory for hsmdivv, %s>\n",title) ;
	    return ;
	}
    }
    else
      hsmdivv = NULL;
    
    for (i = 0 ;i < boxlist[active_box].ngas ;i++) {

	pp = boxlist[active_box].gp[i] ;
	hsmdivv[i] = 0. ;
	hsminv = 1.0 / pp->hsmooth ;
	dwnorm = hsminv * hsminv * hsminv * hsminv * hsminv / PI ;
	distnorm = hsminv * hsminv * deldr2i ;
	for (j = 0 ;j < boxlist[active_box].ngas ;j++) {
	    if(j != i){
		gp = boxlist[active_box].gp[j] ;
		sub_vec(dr,pp->pos,gp->pos) ;
		dr2 = dot_product(dr,dr) ;
		dr2p = dr2 * distnorm ;
		if(dr2p < NINTERP){
		    iwsm = (int)dr2p ;
		    iwsm = min(NINTERP,iwsm) ;
		    drw = dr2p - iwsm ;
		    dwsm = (1.- drw) * dwsmooth[iwsm] + drw * dwsmooth[1+iwsm] ;
		    dwmass = dwnorm * dwsm ;
		}
		else{
		    dwmass = 0. ;
		}
		dr2i = dr2 * deldr2i / (gp->hsmooth) / (gp->hsmooth) ;
		if(dr2i < NINTERP){
		    iwsm1 = (int)dr2i ;
		    iwsm1 = min(NINTERP,iwsm1) ;
		    drw1 = dr2i - iwsm1 ;
		    dwsm1 = (1.-drw1) * dwsmooth[iwsm1] + drw1 *
			    dwsmooth[1+iwsm1] ;
		    dwmass1 = 1. / PI / ((gp->hsmooth) * (gp->hsmooth) *
			    (gp->hsmooth) * (gp->hsmooth) * (gp->hsmooth)) *
			    dwsm1 ;
		}
		else{
		    dwmass1 = 0. ;
		}
		if(dwmass != 0. || dwmass1 != 0.){
		    sub_vec(dv,pp->vel,gp->vel) ;
		    vdotdr = dot_product(dv,dr) ;
		    hsmdivv[i] -= gp->mass * 0.5 * (dwmass + dwmass1) * vdotdr ;
		}
	    }
	}
	hsmdivv[i] /= pp->rho ;
    }
    divv_loaded = YES ;
}
