#include <stdlib.h>
#include "defs.h"

oldascii2binary(infile,outfile)
    FILE *infile;
    FILE *outfile;
{
    int ndim ;
    int nbodies ;
    int ngas ;
    int ndark ;
    int nstar ;
    int count ;
    struct gas_particle *gp, *lastgp;
    struct dark_particle *dp, *lastdp ;
    struct star_particle *sp, *lastsp ;

    forever {
	if(file_type == SPH){
	    count=fscanf(infile,
			"%d%*[, \t\n]%d%*[, \t\n]%lf%*[, \t\n]%d%*[, \t\n]%d"
			,&header.nbodies, &header.ndim, &header.time,
			 &header.nsph, &header.nstar); 
	}
	else{
	    count=fscanf(infile,
			"%d%*[, \t\n]%d%*[, \t\n]%lf",
			&header.nbodies, &header.ndim, &header.time) ;
	    header.nsph = 0 ;
	    header.nstar = 0 ;
	}
	if ( (count == EOF) || (count==0) ){
	    break ;
	}
	ndim=header.ndim;
	nbodies=header.nbodies;
	ngas=header.nsph;
	nstar = header.nstar ;
	ndark = header.ndark = nbodies - nstar - ngas ;
        if(gas_particles != NULL) free(gas_particles);
	if(ngas != 0) {
	    gas_particles = (struct gas_particle *)
				malloc(ngas*sizeof(*gas_particles));
	    if(gas_particles == NULL) {
		printf("<sorry, no memory for gas particles, %s>\n",title) ;
		return ;
	    }
	}
	else
	  gas_particles = NULL;
	
        if(dark_particles != NULL) free(dark_particles);
	if(ndark != 0) {
	    dark_particles = (struct dark_particle *)
				malloc(ndark*sizeof(*dark_particles));
	    if(dark_particles == NULL) {
		printf("<sorry, no memory for dark particles, %s>\n",title) ;
		return ;
	    }
	}
	else
	  dark_particles = NULL;
        if(star_particles != NULL) free(star_particles);
	if(nstar != 0) {
	    star_particles = (struct star_particle *)
				    malloc(nstar*sizeof(*star_particles));
	    if(star_particles == NULL) {
		printf("<sorry, no memory for star particles, %s>\n",title) ;
		return ;
	    }
	}
	else
	  star_particles = NULL;

	lastgp = gas_particles + ngas ;
	lastdp = dark_particles + ndark ;
	lastsp = star_particles + nstar ;

	for(gp=gas_particles; gp < lastgp ; gp++){
	    fscanf(infile,"%f%*[, \t\n]",&gp->mass);
	}
	for(dp=dark_particles; dp < lastdp;  dp++){
	    fscanf(infile,"%f%*[, \t\n]",&dp->mass);
	}
	for(sp=star_particles; sp < lastsp; sp++){
	    fscanf(infile,"%f%*[, \t\n]",&sp->mass);
	}
	for(gp=gas_particles; gp < lastgp ; gp++) {
	    fscanf(infile,"%f%*[, \t\n]",&gp->pos[0]);
	}
	for(dp=dark_particles; dp < lastdp ; dp++) {
	    fscanf(infile,"%f%*[, \t\n]",&dp->pos[0]);
	}
	for(sp=star_particles; sp < lastsp ; sp++) {
	    fscanf(infile,"%f%*[, \t\n]",&sp->pos[0]);
	}
	for(gp=gas_particles; gp < lastgp ; gp++) {
	    fscanf(infile,"%f%*[, \t\n]",&gp->pos[1]);
	}
	for(dp=dark_particles; dp < lastdp ; dp++) {
	    fscanf(infile,"%f%*[, \t\n]",&dp->pos[1]);
	}
	for(sp=star_particles; sp < lastsp ; sp++) {
	    fscanf(infile,"%f%*[, \t\n]",&sp->pos[1]);
	}
	if (ndim == 3){
	    for(gp=gas_particles; gp < lastgp ; gp++) {
		fscanf(infile,"%f%*[, \t\n]",&gp->pos[2]);
	    }
	    for(dp=dark_particles; dp < lastdp ; dp++) {
		fscanf(infile,"%f%*[, \t\n]",&dp->pos[2]);
	    }
	    for(sp=star_particles; sp < lastsp ; sp++) {
		fscanf(infile,"%f%*[, \t\n]",&sp->pos[2]);
	    }
	}
	for(gp=gas_particles; gp < lastgp ; gp++) {
	    fscanf(infile,"%f%*[, \t\n]",&gp->vel[0]);
	}
	for(dp=dark_particles; dp < lastdp ; dp++) {
		fscanf(infile,"%f%*[, \t\n]",&dp->vel[0]);
	}
	for(sp=star_particles; sp < lastsp ; sp++) {
	    fscanf(infile,"%f%*[, \t\n]",&sp->vel[0]);
	}
	for(gp=gas_particles; gp < lastgp ; gp++) {
	    fscanf(infile,"%f%*[, \t\n]",&gp->vel[1]);
	}
	for(dp=dark_particles; dp < lastdp ; dp++) {
	    fscanf(infile,"%f%*[, \t\n]",&dp->vel[1]);
	}
	for(sp=star_particles; sp < lastsp ; sp++) {
	    fscanf(infile,"%f%*[, \t\n]",&sp->vel[1]);
	}
	if (ndim == 3){
	    for(gp=gas_particles; gp < lastgp ; gp++) {
		fscanf(infile,"%f%*[, \t\n]",&gp->vel[2]);
	    }
	    for(dp=dark_particles; dp < lastdp ; dp++) {
		fscanf(infile,"%f%*[, \t\n]",&dp->vel[2]);
	    }
	    for(sp=star_particles; sp < lastsp ; sp++) {
		fscanf(infile,"%f%*[, \t\n]",&sp->vel[2]);
	    }
	}
	for(gp=gas_particles; gp < lastgp ; gp++) {
	    fscanf(infile,"%f%*[, \t\n]",&gp->rho);
	}
	for(gp=gas_particles; gp < lastgp ; gp++) {
	    fscanf(infile,"%f%*[, \t\n]",&gp->temp);
	}
	for(gp=gas_particles; gp < lastgp ; gp++) {
	    fscanf(infile,"%f%*[, \t\n]",&gp->hsmooth);
	}
	for(gp=gas_particles; gp < lastgp ; gp++) {
	    fscanf(infile,"%f%*[, \t\n]",&gp->metals);
	}
	for(sp=star_particles; sp < lastsp ; sp++) {
	    fscanf(infile,"%f%*[, \t\n]",&sp->metals);
	}
	for(sp=star_particles; sp < lastsp ; sp++) {
	    fscanf(infile,"%f%*[, \t\n]",&sp->tform);
	}
	for(gp=gas_particles; gp < lastgp ; gp++) {
	    gp->phi = 0. ;
        }
	for(dp=dark_particles; dp < lastdp ; dp++) {
	    dp->phi = 0. ;
        }
	for(sp=star_particles; sp < lastsp ; sp++) {
	    sp->phi = 0. ;
        }
	for(dp=dark_particles; dp < lastdp ; dp++) {
	    dp->eps = 0. ;
        }
	for(sp=star_particles; sp < lastsp ; sp++) {
	    sp->eps = 0. ;
        }
	fwrite((char *)&header,sizeof(header),1,outfile) ;
	fwrite((char *)gas_particles,sizeof(struct gas_particle),ngas,outfile) ;
	fwrite((char *)dark_particles,sizeof(struct dark_particle),
	       ndark,outfile) ;
	fwrite((char *)star_particles,sizeof(struct star_particle),
	   nstar,outfile) ;
	printf("read time %lf\n",header.time) ;
    }
}
