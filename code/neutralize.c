#include "defs.h"
#include "fdefs.h"

void
neutralize(job)
    char *job;
{
    char command[MAXCOMM] ;
    double max_temp ;
    double min_rho ;
    int box ;
    int i ;
    struct gas_particle *gp ;
    double hneutral_p ;
    double heneutral_p ;
    double heII_p ;

    if (sscanf(job,"%s %d %lf %lf",command,&box,&max_temp,&min_rho) == 4) {
	if (boxes_loaded[box]) {
	    if(!hneutral_loaded){                                           
		hneutral_func() ;          
	    }
	    for (i = 0 ;i < boxlist[0].ngas ;i++) {
		gp = boxlist[0].gp[i] ;
		if(gp->temp <= max_temp && gp->rho >= min_rho){
		    hneutral[gp-gas_particles] = 1. ;
		    heneutral[gp-gas_particles] = 1. ;
		    heII[gp-gas_particles] = 0. ;
		}
		else{
		    if(gp->temp <= max_temp_old && gp->rho >= min_rho_old){
			if(!uniform){
			    calc_uv(gp) ;
			}
			calc_hneutral(gp->temp, gp->rho, &hneutral_p,
				&heneutral_p, &heII_p);
			hneutral[gp-gas_particles] = hneutral_p ;
			heneutral[gp-gas_particles] = heneutral_p ;
			heII[gp-gas_particles] = heII_p ;
		    }
		}
	    }
	    max_temp_old = max_temp ;
	    min_rho_old = min_rho ;
	}
	else {
	    printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
