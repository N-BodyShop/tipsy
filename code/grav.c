#include "defs.h"
void grav(pos,acc_gas,acc_star,acc_dark,box)
    Real pos[MAXDIM] ;
    Real acc_gas[MAXDIM] ;
    Real acc_star[MAXDIM] ;
    Real acc_dark[MAXDIM] ;
    int box ;
{
    struct gas_particle *gp ;
    struct star_particle *sp ;
    struct dark_particle *dp ;
    int i,j ;
    double radius ;
    Real delta_x[MAXDIM] ;
    double dxdotdx ;
    double sdxdotdx ;
    double r3inveff ;
    double dxdeldxg ;
    double drsm ;
    double accsm ;
    double acci ;
    int smindex ;
    char dummy[MAXCOMM] ;
    double dot_product() ;

    if(!acc_loaded){
	acc_load() ;
    }
    for(i = 0; i < MAXDIM ; i++){
	acc_gas[i] = 0. ;
	acc_star[i] = 0. ;
	acc_dark[i] = 0. ;
    }
    if(!epsgas_loaded){
	load_epsgas() ;
    }
    if(!eps_loaded){
	load_eps() ;
    }
    for (i = 0 ;i < boxlist[box].ngas ;i++) {
	gp = boxlist[box].gp[i] ;
	delta_x[0] = pos[0] - gp->pos[0] ;
	delta_x[1] = pos[1] - gp->pos[1] ;
	delta_x[2] = pos[2] - gp->pos[2] ;
	dxdotdx = delta_x[0]*delta_x[0] + delta_x[1]*delta_x[1] +
		delta_x[2]*delta_x[2] ;
	if(dxdotdx == 0.0)	/* Avoid divide by zero */
	    continue;
        sdxdotdx = sqrt(dxdotdx) ;
        r3inveff = 1./sdxdotdx / dxdotdx ;
	if(epsgas_grav != 0.){
	    dxdeldxg = sdxdotdx * (double)NINTERP / epsgas_grav / 2. ;
	    smindex = min(NINTERP,(int)dxdeldxg) ;
	    drsm = min(1.,dxdeldxg-(double)smindex) ;
	    accsm=(1.-drsm)*acsmooth[smindex]+drsm*acsmooth[1+smindex] ;
	    r3inveff = accsm * r3inveff ;
	}
        acci = gp->mass * r3inveff ;
        acc_gas[0] -= delta_x[0] * acci ;
        acc_gas[1] -= delta_x[1] * acci ;
        acc_gas[2] -= delta_x[2] * acci ;
    }
    for (i = 0 ;i < boxlist[box].nstar ;i++) {
	sp = boxlist[box].sp[i] ;
	delta_x[0] = pos[0] - sp->pos[0] ;
	delta_x[1] = pos[1] - sp->pos[1] ;
	delta_x[2] = pos[2] - sp->pos[2] ;
	dxdotdx = delta_x[0]*delta_x[0] + delta_x[1]*delta_x[1] +
		delta_x[2]*delta_x[2] ;
	if(dxdotdx == 0.0)	/* Avoid divide by zero */
	    continue;
        sdxdotdx = sqrt(dxdotdx) ;
        r3inveff = 1./sdxdotdx / dxdotdx ;
	if(epsgas_grav != 0.){
	    dxdeldxg = sdxdotdx * (double)NINTERP / epsgas_grav / 2. ;
	    smindex = min(NINTERP,(int)dxdeldxg) ;
	    drsm = min(1.,dxdeldxg-smindex) ;
	    accsm=(1.-drsm)*acsmooth[smindex]+drsm*acsmooth[1+smindex] ;
	    r3inveff = accsm * r3inveff ;
	}
        acci = sp->mass * r3inveff ;
        acc_star[0] -= delta_x[0] * acci ;
        acc_star[1] -= delta_x[1] * acci ;
        acc_star[2] -= delta_x[2] * acci ;
    }
    for (i = 0 ;i < boxlist[box].ndark ;i++) {
	dp = boxlist[box].dp[i] ;
	delta_x[0] = pos[0] - dp->pos[0] ;
	delta_x[1] = pos[1] - dp->pos[1] ;
	delta_x[2] = pos[2] - dp->pos[2] ;
	dxdotdx = delta_x[0]*delta_x[0] + delta_x[1]*delta_x[1] +
		delta_x[2]*delta_x[2] ;
	if(dxdotdx == 0.0)	/* Avoid divide by zero */
	    continue;
        sdxdotdx = sqrt(dxdotdx) ;
        r3inveff = 1./sdxdotdx / dxdotdx ;
	if(eps_grav != 0.){
	    dxdeldxg = sdxdotdx * (double)NINTERP / eps_grav / 2. ;
	    smindex = min(NINTERP,(int)dxdeldxg) ;
	    drsm = min(1.,dxdeldxg-smindex) ;
	    accsm=(1.-drsm)*acsmooth[smindex]+drsm*acsmooth[1+smindex] ;
	    r3inveff = accsm * r3inveff ;
	}
        acci = dp->mass * r3inveff ;
        acc_dark[0] -= delta_x[0] * acci ;
        acc_dark[1] -= delta_x[1] * acci ;
        acc_dark[2] -= delta_x[2] * acci ;
    }
}
