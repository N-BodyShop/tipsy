#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>

PROTO(void, smDivvSym, (SMX smx, int pi, int nSmooth, int *pList,
			float *fList));

static double hsys;

void
divv()
{
    struct gas_particle *gp ;
    KD kd;
    int n_smooth = 64;
    float period[MAXDIM];
    int i;
    double rsys, vsys;

    if(!dkernel_loaded){
	dkernel_load() ;
    }
    if (!cool_loaded ){
	load_cool() ;
    }

    rsys = kpcunit/1.e3 ;
    vsys = sqrt(msolunit/kpcunit*(GCGS*MSOLG/KPCCM))/1.e5 ;
    hsys = rsys*hubble_constant/vsys;

    if(boxlist[0].ngas != header.nsph) {
	printf("<Warning, box 0 does not contain all particles, %s>\n", title);
	printf("<Reloading box 0, %s>\n", title);
	loadall();
    }

    if(hsmdivv != NULL){
	free(hsmdivv);
    }
    if(boxlist[0].ngas != 0) {
	hsmdivv = (double *)malloc(boxlist[0].ngas *sizeof(*hsmdivv));
	if(hsmdivv == NULL) {
	    printf("<sorry, no memory for hsmdivv, %s>\n",title) ;
	    return ;
	}
    }
    else
      hsmdivv = NULL;

    
    if(box0_smx) {
	kdFinish(box0_smx->kd);
	smFinish(box0_smx);
	box0_smx = NULL;
    }

    printf("<Building tree, %s>\n", title);
    kdInit(&kd);
    kdReadBox(kd, &boxlist[0], 0, 1, 0);
    kdBuildTree(kd);
    if(periodic)
	period[0] = period[1] = period[2] = period_size;
    else
	period[0] = period[1] = period[2] = 1e37;

    printf("<Calculating divv, %s>\n", title);

    smInit(&box0_smx,kd,n_smooth,period);
    smSetBallh(box0_smx);
    smReSmooth(box0_smx,smDivvSym);

    kdOrder(kd);
    for(i = 0; i < boxlist[0].ngas; i++) {
	gp = boxlist[0].gp[i];
	hsmdivv[i] = 0.5*gp->hsmooth*kd->p[i].fDensity/gp->rho;
    }
    divv_loaded = YES ;
}

void smDivvSym(smx, pi, nSmooth, pList, fList)
     SMX smx;
     int pi;
     int nSmooth;
     int *pList;
     float *fList;
{
    int i ;
    int iwsm ;
    double dot_product() ;
    double hsminv ;
    double hsminv2 ;
    double dwnorm ;
    double distnorm ;
    double dr2 ;
    double dr2p ;
    double drw ;
    double dwsm ;
    double dwmass ;
    Real dr[MAXDIM] ;
    Real dv[MAXDIM] ;
    double vdotdr ;
    int pj;
    PARTICLE *p = smx->kd->p;

/*
 * N.B.  fBall2 is (2*hsmooth)^2.
 */
	hsminv2 = 4.0 / p[pi].fBall2 ;
	hsminv = sqrt(hsminv2);
	dwnorm = hsminv2 * hsminv2 * hsminv / PI ;
	distnorm = hsminv2 * deldr2i ;
	for (i = 0; i < nSmooth; i++) {

	        pj = pList[i];
		if(pj == pi)
		    continue;
		dr2 = fList[i];
		dr2p = dr2 * distnorm ;
		if(dr2p < NINTERP){
		    int j;

		    iwsm = (int)dr2p ;
		    iwsm = min(NINTERP,iwsm) ;
		    drw = dr2p - iwsm ;
		    dwsm = (1.- drw) * dwsmooth[iwsm] + drw * dwsmooth[1+iwsm] ;
		    dwmass = dwnorm * dwsm ;
		    sub_vec(dr,p[pi].p.gp->pos, p[pj].p.gp->pos) ;
		    sub_vec(dv,p[pi].p.gp->vel, p[pj].p.gp->vel) ;
/*
 * Handle periodic boundary conditions.
 */
		    if(periodic == YES) {
			for(j = 0; j < MAXDIM; j++) {
			    if(dr[j] >= 0.5*period_size)
				dr[j] -= period_size;
			    if(dr[j] < -0.5*period_size)
				dr[j] += period_size;
			}
		    }
/*
 * Handle comoving coordinates.
 */
		    if(comove == YES) {
			for(j = 0; j < MAXDIM; j++)
			    dv[j] += dr[j]*hsys;
		    }
		    vdotdr = dot_product(dv,dr) ;
		    smx->kd->p[pi].fDensity -=
			smx->kd->p[pj].p.gp->mass * dwmass * vdotdr ;
		    smx->kd->p[pj].fDensity -=
			smx->kd->p[pi].p.gp->mass * dwmass * vdotdr ;
		}
	}
}
