#include "defs.h"
grav(pos,acc_gas,acc_star,acc_dark)
    Real pos[MAXDIM] ;
    Real acc_gas[MAXDIM] ;
    Real acc_star[MAXDIM] ;
    Real acc_dark[MAXDIM] ;
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
    char dummy[80] ;
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
    for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
	gp = boxlist[active_box].gp[i] ;
	sub_vec(delta_x,pos,gp->pos) ;
	dxdotdx = dot_product(delta_x,delta_x) ;
        sdxdotdx = sqrt(dxdotdx) ;
        r3inveff = 1./sdxdotdx / dxdotdx ;
        dxdeldxg = sdxdotdx * (double)NINTERP / epsgas_grav ;
        smindex = min(NINTERP,(int)dxdeldxg) ;
        drsm = min(1.,dxdeldxg-(double)smindex) ;
        accsm=(1.-drsm)*acsmooth[smindex]+drsm*acsmooth[1+smindex] ;
        r3inveff = accsm * r3inveff ;
        acci = gp->mass * r3inveff ;
        acc_gas[0] -= delta_x[0] * acci ;
        acc_gas[1] -= delta_x[1] * acci ;
        acc_gas[2] -= delta_x[2] * acci ;
    }
    for (i = 0 ;i < boxlist[active_box].nstar ;i++) {
	sp = boxlist[active_box].sp[i] ;
	sub_vec(delta_x,pos,sp->pos) ;
	dxdotdx = dot_product(delta_x,delta_x) ;
        sdxdotdx = sqrt(dxdotdx) ;
        r3inveff = 1./sdxdotdx / dxdotdx ;
        dxdeldxg = sdxdotdx * (double)NINTERP / epsgas_grav ;
        smindex = min(NINTERP,(int)dxdeldxg) ;
        drsm = min(1.,dxdeldxg-smindex) ;
        accsm=(1.-drsm)*acsmooth[smindex]+drsm*acsmooth[1+smindex] ;
        r3inveff = accsm * r3inveff ;
        acci = sp->mass * r3inveff ;
        acc_star[0] -= delta_x[0] * acci ;
        acc_star[1] -= delta_x[1] * acci ;
        acc_star[2] -= delta_x[2] * acci ;
    }
    for (i = 0 ;i < boxlist[active_box].ndark ;i++) {
	dp = boxlist[active_box].dp[i] ;
	sub_vec(delta_x,pos,dp->pos) ;
	dxdotdx = dot_product(delta_x,delta_x) ;
        sdxdotdx = sqrt(dxdotdx) ;
        r3inveff = 1./sdxdotdx / dxdotdx ;
        dxdeldxg = sdxdotdx * (double)NINTERP / eps_grav ;
        smindex = min(NINTERP,(int)dxdeldxg) ;
        drsm = min(1.,dxdeldxg-smindex) ;
        accsm=(1.-drsm)*acsmooth[smindex]+drsm*acsmooth[1+smindex] ;
        r3inveff = accsm * r3inveff ;
        acci = dp->mass * r3inveff ;
        acc_dark[0] -= delta_x[0] * acci ;
        acc_dark[1] -= delta_x[1] * acci ;
        acc_dark[2] -= delta_x[2] * acci ;
    }
}
