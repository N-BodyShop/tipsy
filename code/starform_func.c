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
    double formtime ;
    double adiabatic ;
    double srhomin ;

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
    if(!dudt_loaded){
	calc_dudt() ;
    }
    srhomin = rhomin*pow(kpcunit*KPCCM, 3.0)/(MSOLG*msolunit)/fhydrogen*MHYDR ;
    if(starform != NULL) free(starform);
    if(boxlist[active_box].ngas != 0) {
	starform =
	  (double *)malloc(boxlist[0].ngas *sizeof(*starform));
	if(starform == NULL) {
	    printf("<sorry, no memory for starform, %s>\n",title) ;
	    return ;
	}
    }
    else
      starform = NULL;
    
    for (i = 0 ;i < boxlist[0].ngas ;i++) {
	gp = boxlist[0].gp[i] ;
	if(hsmdivv[gp-gas_particles] < 0.) {
 	    tdyn = GYRSEC*time_unit/sqrt(4.*(gp->rho/cosmof3)*PI);
	    adiabatic = -KBOLTZ*gp->temp*
		    gp->rho/(MHYDR*meanmwt[gp-gas_particles])*
	    	    (hsmdivv[gp-gas_particles]/gp->hsmooth)*MSOLG*msolunit/
		    (cosmof3*pow(kpcunit*KPCCM, 3.0))/time_unit/GYRSEC ;
	    adiabatic = dudt[gp-gas_particles]*(gp->rho) /(kpcunit*KPCCM) /
		    (time_unit*GYRSEC*time_unit*GYRSEC*time_unit*GYRSEC) ;
	    tcool = 1.5*KBOLTZ*gp->temp*MSOLG*msolunit*gp->rho/
		    (MHYDR*meanmwt[gp-gas_particles])/
		    (cosmof3*pow(kpcunit*KPCCM, 3.0))/
		     (cooling[gp-gas_particles] + adiabatic) ;
	    tsound = kpcunit*KPCCM*gp->hsmooth*cosmof
		/sqrt(GAMMA*KBOLTZ*gp->temp/(MHYDR*meanmwt[gp-gas_particles])) ;
	    soft = .89553 * (PI*epsgas_grav*cosmof)*(PI*epsgas_grav*cosmof) /
		    2. * meanmwt[gp-gas_particles]*MHYDR*(gp->rho /
		    (KBOLTZ*gp->temp)) *(kpcunit*kpcunit*KPCCM*KPCCM
			/time_unit/time_unit/GYRSEC/GYRSEC) ;
	    if((tdyn > fabs(tcool) || gp->temp <= temppar)){
		formtime = tdyn ;
	    }
	    else{
		formtime = fabs(tcool) ;
	    }
	    formtime /= time_unit*GYRSEC ;
	    if((tcool < 0. || gp->temp <= temppar)
		    && (tsound > tdyn || soft > softpar) && 
		    gp->rho/cosmof3 >= srhomin && gp->rho >= overmin){
		tempvect = formtime ;
		starform[i] = (1.- exp(-cstar * dtime / tempvect))*gp->mass*
			stareff*msolunit/dtime/time_unit/1.e9;
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
