#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>

void
gasify(job)
    char *job;
{
    char command[MAXCOMM] ;
    char g_type[MAXCOMM] ;
    double temp_y ;
    double temp_slope ;
    double rho_shock ;
    double temp_shock ;
    double gas_frac ;
    double rhobar ;
    double metal ;
    double rho ;
    int i,j ;
    int old_nsph ;
    struct dark_particle *dp ;

/* Gas particles are created from dark matter particles.
 * Masses are gas_frac * masses of dm particles
 * dm particle masses are multiplied by (1-gas_frac).
 * Temperatures are set to t=temp_y*(rho/rhobar)^temp_slope, 
 * or to t=temp_shock if rho/rhobar>rho_shock.
 */

    if (!boxes_loaded[0]){
	printf("<sorry, no boxes are loaded, %s>\n",title) ;
    }
    else {
	if (sscanf(job,"%s %lf %lf %lf %lf %lf %lf %lf %s",command,&gas_frac,
	                &rhobar,&temp_y,&temp_slope,&rho_shock,&temp_shock,
			&metal,g_type) == 8) {

	calc_density(&box0_smx, 1, 0, 0);
	header.nbodies += boxlist[0].ndark ;
	old_nsph = header.nsph ;
        header.nsph += boxlist[0].ndark ;
        if(header.nsph != 0) {
	    gas_particles = (struct gas_particle *) realloc(gas_particles,
				    header.nsph*sizeof(*gas_particles));
	    if(gas_particles == NULL) {
	        printf("<sorry, no memory for gas particles, %s>\n",title) ;
	        return ;
	    }
	    mark_gas = (short *)realloc(mark_gas,header.nsph*sizeof(*mark_gas));
	    if(mark_gas == NULL)  {
	        printf("<sorry, no memory for gas particle markers, %s>\n",
			title) ;
		return ;
	    }
	    for (i = old_nsph; i < header.nsph; i++) mark_gas[i] = 0;
        }
        else
          gas_particles = NULL;
	    
	for (i = 0 ;i < boxlist[0].ndark ;i++) {
	    dp = boxlist[0].dp[i] ;
	    gas_particles[i+old_nsph].mass = gas_frac*(dp->mass) ;
	    dp->mass = (1. - gas_frac)*(dp->mass) ;
	    for(j = 0; j < header.ndim; j++){
	      gas_particles[i+old_nsph].pos[j] = dp->pos[j] ;
	      gas_particles[i+old_nsph].vel[j] = dp->vel[j] ;
	    }
	    gas_particles[i+old_nsph].rho = 
		    gas_frac*(box0_smx->kd->p[i].fDensity);
	    gas_particles[i+old_nsph].temp = temp_y*
		    pow((gas_particles[i+old_nsph].rho/rhobar),temp_slope) ;
	    if (gas_particles[i+old_nsph].rho > rhobar*rho_shock)  {
	        gas_particles[i+old_nsph].temp = temp_shock ;
	    }
	    gas_particles[i+old_nsph].hsmooth = 
		    sqrt(box0_smx->kd->p[i].fBall2)/2.0;
	    gas_particles[i+old_nsph].metals = metal ;
	    gas_particles[i+old_nsph].phi = dp->phi ;
	}

        if(box0_smx) {
	    kdFinish(box0_smx->kd);
	    smFinish(box0_smx);
	    box0_smx = NULL;
        }
	boxes_loaded[0] = NO ;
	unload_all() ;
	active_box = 0 ;
	binary_loaded = LOADED ;
	current_project = NO ;
	current_color = NO ;
	divv_loaded = NO ;
	hneutral_loaded = NO ;
	meanmwt_loaded = NO ;
	xray_loaded = NO ;
	dudt_loaded = NO ;
	starform_loaded = NO ;
      }
	else if (sscanf(job,"%s %lf %lf %lf %lf %lf %lf %lf %s",command,
			&gas_frac,&rhobar,&temp_y,&temp_slope,&rho_shock,
			&temp_shock,&metal,g_type) == 9) {

	if (strcmp(g_type,"destroy") != 0 && strcmp(g_type,"d") != 0){
	     printf("<sorry, %s is not a gasify type, %s",g_type,title) ;
	     return;
	}
	calc_density(&box0_smx, 1, 0, 0);
	printf("<warning, destroying original dark and gas particles, %s>\n",
	title) ;
	header.nbodies -= header.nsph ;
        header.nsph = boxlist[0].ndark ;
        header.ndark = 0 ;
        if(header.nsph != 0) {
	    dark_particles = (struct dark_particle *) realloc(dark_particles,
				    header.nsph*sizeof(*gas_particles));
	    if(dark_particles == NULL) {
	        printf("<sorry, no memory for gas particles, %s>\n",title) ;
	        return ;
	    }
	    gas_particles = (struct gas_particle *)dark_particles ;
	    free(mark_dark) ;
	    if(header.nsph != 0)
		 mark_gas = (short *)calloc(header.nsph, sizeof(*mark_gas));
	    if(mark_gas == NULL && header.nsph != 0)  {
	        printf("<sorry, no memory for gas particle markers, %s>\n",
			title) ;
		return ;
	    }
	    for (i = 0; i < header.nsph; i++) mark_gas[i] = 0;
        }
        else
          gas_particles = NULL;
	    
	for (i =  boxlist[0].ndark - 1 ;i >= 0 ;i--) {
	    dp = boxlist[0].dp[i] ;
	    gas_particles[i].phi = dp->phi ;
	    gas_particles[i].metals = metal ;
	    gas_particles[i].hsmooth = sqrt(box0_smx->kd->p[i].fBall2)/2.0;
	    rho = gas_frac*(box0_smx->kd->p[i].fDensity);
	    gas_particles[i].temp = temp_y*pow((rho/rhobar),temp_slope) ;
	    if (rho > rhobar*rho_shock)  {
	        gas_particles[i].temp = temp_shock ;
	    }
	    gas_particles[i].rho = rho ;
	    for(j = header.ndim - 1; j >= 0; j--){
	      gas_particles[i+old_nsph].vel[j] = dp->vel[j] ;
	    }
	    for(j = header.ndim - 1; j >= 0; j--){
	      gas_particles[i+old_nsph].pos[j] = dp->pos[j] ;
	    }
	    gas_particles[i].mass = gas_frac*(dp->mass) ;
	}

        if(box0_smx) {
	    kdFinish(box0_smx->kd);
	    smFinish(box0_smx);
	    box0_smx = NULL;
        }
        dark_particles = NULL;
	boxes_loaded[0] = NO ;
	unload_all() ;
	active_box = 0 ;
	binary_loaded = LOADED ;
	current_project = NO ;
	current_color = NO ;
	divv_loaded = NO ;
	hneutral_loaded = NO ;
	meanmwt_loaded = NO ;
	xray_loaded = NO ;
	starform_loaded = NO ;
	dudt_loaded = NO ;
      }
      else {
	  input_error(command) ;
      }
    }
}
