#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>

void
starform_func()
{
    int i ;
    struct gas_particle *gp ;
    double tdyn ;
    double tcool ;
    double tsound ;
    double tempvect ;
    double soft ;

    if (!cooling_loaded ){
	cool_func() ;
    }
    if (!meanmwt_loaded ){
	meanmwt_func() ;
    }
    if(!form_loaded ){
	load_form() ;
    }
    if(!epsgas_loaded){
	load_epsgas() ;
    }
    if(!divv_loaded){
	divv() ;
    }
    if(starform != NULL) free(starform);
    if(boxlist[active_box].ngas != 0) {
	starform =
	  (double *)malloc(boxlist[active_box].ngas *sizeof(*starform));
	if(starform == NULL) {
	    printf("<sorry, no memory for starform, %s>\n",title) ;
	    return ;
	}
    }
    else
      starform = NULL;
    
    for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
	gp = boxlist[active_box].gp[i] ;
	if(hsmdivv[i] < 0.) {
				/* G internal == 1 */
 	    tdyn = time_unit/sqrt(4.*(gp->rho/cosmof3)*PI);
	    tcool = 1.5*KBOLTZ*gp->temp
	      *MHYDR*meanmwt[i]*MSOLG*msolunit*gp->rho
		/(cosmof3*pow(kpcunit*KPCCM, 3.0))
		/cooling[i]/GYRSEC;
	    tsound = kpcunit*KPCCM*gp->hsmooth*cosmof
		/ sqrt(GAMMA*KBOLTZ*gp->temp/(MHYDR*meanmwt[i]))/GYRSEC;
				/* see Katz and Gunn (1991) */
	    soft = .89553 * (PI*epsgas_grav*cosmof)*(PI*epsgas_grav*cosmof) /
		    3. * meanmwt[i]*MHYDR*(gp->rho / (KBOLTZ*gp->temp))
		      *(kpcunit*kpcunit*KPCCM*KPCCM
			/time_unit/time_unit/GYRSEC/GYRSEC) ;
	    if((tdyn > fabs(tcool) || gp->temp < temppar) && tcool < 0. &&
		    (tsound > tdyn || soft > softpar)){
		tempvect = tdyn ;
		starform[i] = 1.- exp(-cstar * dtime / tempvect) ;
	    }
	    else if(fabs(tcool) > tdyn && gp->temp >= temppar && tcool < 0. &&
		    (tsound > tdyn || soft > softpar)){
		tempvect = fabs(tcool) ;
		starform[i] = 1.- exp(-cstar * dtime / tempvect) ;
	    }
	    else{
		starform[i] = 0. ;
	    }
	}
	else{
	    starform[i] = 0. ;
	}
    }
    starform_loaded = YES ;
}
