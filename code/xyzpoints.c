#include "defs.h"
xyzpoints(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    char file[MAXCOMM] ;
    char particle_type[MAXCOMM] ;
    struct gas_particle *gp, *lastgp ;
    struct dark_particle *dp, *lastdp ;
    struct star_particle *sp, *lastsp ;

    if (sscanf(job,"%s %s %s",command,particle_type,file) == 3) { 
	if (binary_loaded) {
	    lastgp = gas_particles + header.nsph ;
	    lastdp = dark_particles + header.ndark ;
	    lastsp = star_particles + header.nstar ;
	    sprintf(hardfile.name,"%s",file) ;
	    hardfile.ptr = fopen(hardfile.name,"w");
	    if(strcmp(particle_type,"gas")==0 ||
		    strcmp(particle_type,"all")==0){
		for(gp=gas_particles; gp < lastgp ; gp++) {
		    fprintf(hardfile.ptr,"%g %g %g\n",gp->pos[0],gp->pos[1],
			    gp->pos[2]) ;
		}
	    }
	    if(strcmp(particle_type,"dark")==0 ||
		    strcmp(particle_type,"all")==0){
		for(dp=dark_particles; dp < lastdp;  dp++){
		    fprintf(hardfile.ptr,"%g %g %g\n",dp->pos[0],dp->pos[1],
			    dp->pos[2]) ;
		}
	    }
	    if(strcmp(particle_type,"star")==0 ||
		    strcmp(particle_type,"all")==0){
		for(sp=star_particles; sp < lastsp; sp++){
		    fprintf(hardfile.ptr,"%g %g %g\n",sp->pos[0],sp->pos[1],
			    sp->pos[2]) ;
		}
	    }
	    fclose(hardfile.ptr) ;
	}
	else {
	    printf("<sorry, binary file is not loaded, %s>\n",title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
