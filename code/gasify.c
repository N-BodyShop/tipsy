#include "defs.h"
#include "fdefs.h"
#include <malloc.h>

void
gasify(job)
    char *job;
{
    char command[MAXCOMM] ;
    double temp_y ;
    double temp_slope ;
    double rho_shock ;
    double temp_shock ;
    double gas_frac ;
    double rhobar ;
    double metal ;
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
	if (sscanf(job,"%s %lf %lf %lf %lf %lf %lf %lf",command,&gas_frac,
	                &rhobar,&temp_y,&temp_slope,&rho_shock,&temp_shock,
			&metal) == 8) {

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
      }
      else {
	  input_error(command) ;
      }
    }
}
