#include "defs.h"
void calc_uv(gp)
    struct gas_particle *gp ;
{
    struct uv_source *uvs, *lastuvs ;
    Real dr[MAXDIM] ;
    double r2 ;
    int j ;
    double c1 ;

    if (!cool_loaded ){
        load_cool() ;
    }
    if (!redshift_loaded ){
        load_redshift() ;
    }
    gp0_H = 0.0 ;
    gp0_He = 0.0 ;
    gp0_Hep = 0.0 ;
    eps_H = 0.0 ;
    eps_He = 0.0 ;
    eps_Hep = 0.0 ;
    c1 = cosmof*cosmof*kpcunit*kpcunit*KPCCM*KPCCM ;
    lastuvs = uv_sources + source_num ;
    for(uvs = uv_sources; uvs < lastuvs; uvs++){
	sub_vec(dr,gp->pos,uvs->pos) ;
/*	if(periodic == YES) {
	    for(j = 0; j < MAXDIM; j++) {
		if(dr[j] >= 0.5*period_size)
		    dr[j] -= period_size;
		if(dr[j] < -0.5*period_size)
		    dr[j] += period_size;
	    }
	} */
	r2 = dr[0]*dr[0] + dr[1]*dr[1] + dr[2]*dr[2] ;
	gp0_H += (uvs->gp0_H)/(r2*c1) ;
	gp0_He += (uvs->gp0_He)/(r2*c1) ;
	gp0_Hep += (uvs->gp0_Hep)/(r2*c1) ;
	eps_H += (uvs->eps_H)/(r2*c1) ;
	eps_He += (uvs->eps_He)/(r2*c1) ;
	eps_Hep += (uvs->eps_Hep)/(r2*c1) ;
    }
}
