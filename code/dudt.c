/* routines to calculate the mechanical heating (adiabatic and viscous)
   of the particles */
/* to implement this fully: 
   
   check code
   test against sphinctr
     adiabatic only 
     viscous only
     */


#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>

PROTO(void, smDudtSym, (SMX smx, int pi, int nSmooth, int *pList,
			float *fList));

static double hsys;

void
calc_dudt()
{
  struct gas_particle *gp ;
  KD kd;
  int n_smooth = 64;
  float period[MAXDIM];
  int i;
  double rsys, vsys;

  if (boxlist[0].ngas != header.nsph) {
    printf("<Warning, box 0 does not contain all particles, %s>\n", title);
    printf("<Reloading box 0, %s>\n", title);
    loadall();
  }
  if (!dkernel_loaded){
    dkernel_load() ;
  }
  if (!redshift_loaded){
    load_redshift() ;
  }
  if (!cool_loaded ){
    load_cool() ;
  }
  if (!visc_loaded) {
    load_visc();
  }
  if (!divv_loaded) {
    divv();
  }
  if (!meanmwt_loaded) {
    meanmwt_func() ;
  }

  rsys = kpcunit/1.e3 ;
  vsys = sqrt(msolunit/kpcunit*(GCGS*MSOLG/KPCCM))/1.e5 ;
  hsys = rsys*hubble_constant/vsys;


  if (dudt != NULL){
      free(dudt);
  }
  if (boxlist[0].ngas != 0) {
    dudt = (double *)malloc(boxlist[0].ngas *sizeof(*dudt));
    if (dudt == NULL) {
      printf("<sorry, no memory for hsmdivv, %s>\n",title) ;
      return ;
    }
  }
  else
    dudt = NULL;
    
  if (box0_smx) {
    kdFinish(box0_smx->kd);
    smFinish(box0_smx);
    box0_smx = NULL;
  }

  printf("<Building tree, %s>\n", title);
  kdInit(&kd);
  kdReadBox(kd, &boxlist[0], 0, 1, 0);
  kdBuildTree(kd);
  if (periodic)
    period[0] = period[1] = period[2] = period_size;
  else
    period[0] = period[1] = period[2] = 1e37;

  printf("<Calculating dudt, %s>\n", title);

  smInit(&box0_smx,kd,n_smooth,period);
  smSetBallh(box0_smx);
  smReSmooth(box0_smx,smDudtSym);

  kdOrder(kd);
  for(i = 0; i < boxlist[0].ngas; i++) {
    gp = boxlist[0].gp[i];
    dudt[i] = 0.5 * kd->p[i].fDensity;
  }
  dudt_loaded = YES ;
  starform_loaded = NO ;

}


void smDudtSym(smx, pi, nSmooth, pList, fList)
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
  double adiabatic ;
  double viscous ;
  Real dr[MAXDIM] ;
  Real dv[MAXDIM] ;
  double vdotdr ;
  int pj;
  PARTICLE *p = smx->kd->p;
  struct gas_particle *gi, *gj;
  double hdiv;
  double v_unit;
  double ad_unit;
  double qi, qj;
  double cisound, cjsound;

  /*
   * N.B.  fBall2 is (2*hsmooth)^2.
   */
  gi = p[pi].p.gp;
  hsminv2 = 4.0 / p[pi].fBall2 ;
  hsminv = sqrt(hsminv2);
  dwnorm = hsminv2 * hsminv2 * hsminv / PI ;
  distnorm = hsminv2 * deldr2i ;
  v_unit = (kpcunit * KPCCM) / (time_unit * GYRSEC);
  ad_unit = KBOLTZ / MHYDR / v_unit / v_unit;

  if (hsmdivv[gi-gas_particles] < 0. ) {
    hdiv = hsmdivv[gi-gas_particles];
    cisound = sqrt(GAMMA * ad_unit * gi->temp / meanmwt[gi-gas_particles]);
    qi = (alpha * (-hdiv) * cisound * cosmof 
        + beta * hdiv * hdiv * cosmof*cosmof) / gi->rho;
  } else {
    qi = 0.;
  }

  for (i = 0; i < nSmooth; i++) {

    pj = pList[i];
    gj = p[pj].p.gp;
		
    if (pj == pi)
      continue;
    dr2 = fList[i];
    dr2p = dr2 * distnorm ;
    if (dr2p < NINTERP){
      int j;

      iwsm = (int)dr2p ;
      iwsm = min(NINTERP,iwsm) ;
      drw = dr2p - iwsm ;
      dwsm = (1.- drw) * dwsmooth[iwsm] + drw * dwsmooth[1+iwsm] ;
      dwmass = dwnorm * dwsm ;
      sub_vec(dr,gi->pos, gj->pos) ;
      sub_vec(dv,gi->vel, gj->vel) ;
      /*
       * Handle periodic boundary conditions.
       */
      if (periodic == YES) {
	for(j = 0; j < MAXDIM; j++) {
	  if (dr[j] >= 0.5*period_size)
	    dr[j] -= period_size;
	  if (dr[j] < -0.5*period_size)
	    dr[j] += period_size;
	}
      }
      /*
       * Handle comoving coordinates.
       */
      if (comove == YES) {
	for(j = 0; j < MAXDIM; j++)
	  dv[j] += dr[j]*hsys;
      }
      vdotdr = dot_product(dv,dr) ;

      adiabatic = sqrt(gi->temp*gj->temp 
		       / (meanmwt[gi-gas_particles]*meanmwt[gj-gas_particles]) )
	* ad_unit / sqrt(gi->rho * gj->rho);
      /*adiabatic = 0.; *//*test*/

      if (vdotdr <= 0) {
	if (hsmdivv[gj-gas_particles] < 0.) {
	  hdiv = hsmdivv[gj-gas_particles];
	  cjsound = 
             sqrt(GAMMA * ad_unit * gj->temp / meanmwt[gj-gas_particles]);
	  qj = (alpha * (-hdiv) * cjsound * cosmof 
              + beta * hdiv * hdiv * cosmof*cosmof) / gj->rho;
	} else {
	  qj = 0.;
	}
	viscous = 0.5 * (qi + qj);
      } else {
	viscous = 0.;
      }
      smx->kd->p[pi].fDensity += (adiabatic + viscous)
	* smx->kd->p[pj].p.gp->mass * dwmass * vdotdr ;
      smx->kd->p[pj].fDensity += (adiabatic + viscous)
	* smx->kd->p[pi].p.gp->mass * dwmass * vdotdr ;
    }
  }

}






