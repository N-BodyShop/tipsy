/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:22  trq
 * Initial revision
 *
 * Revision 2.6  94/04/20  08:45:33  trq
 * Added title variable.
 * 
 * Revision 2.5  1993/04/05  11:17:50  trq
 * Neal's changes for tipsy to conform to the new star formation algorithm.
 *
 * Revision 2.4  1993/04/05  11:06:11  trq
 * Deleted declaration of free() - conflicts with some OS headers.
 *
 * Revision 2.3  1993/03/30  16:02:14  trq
 * Corrected "%lf" formats and starforming code.
 *
 * Revision 2.2  91/06/02  12:21:32  trq
 * Added star stuff from Neal
 * 
 * Revision 2.1  1991/04/18  18:11:35  trq
 * Added checks for malloc(0).
 *
 */
#include "defs.h"

void *malloc();

void
ascii2binary(infile,outfile)
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
	count=fscanf(infile, "%d%*[, \t\n]%d%*[, \t\n]%d"
		    ,&header.nbodies, &header.nsph, &header.nstar) ;
	if ( (count == EOF) || (count==0) ){
	    break ;
	}
	fscanf(infile,"%d",&header.ndim) ;
	fscanf(infile,"%lf",&header.time) ;
	ndim=header.ndim;
	nbodies=header.nbodies;
	ngas=header.nsph;
	nstar = header.nstar ;
	ndark = header.ndark = nbodies - nstar - ngas ;
	if(gas_particles != NULL) free(gas_particles);
	if(ngas != 0) {
	    gas_particles =
		(struct gas_particle *) malloc(ngas*sizeof(*gas_particles));
	    if(gas_particles == NULL) {
		printf("<sorry, no memory for gas particles, %s>\n",title) ;
		return ;
	    }
	}
	else
	  gas_particles = NULL;
	if(dark_particles != NULL) free(dark_particles);
	if(ndark != 0) {
	    dark_particles =
		(struct dark_particle *) malloc(ndark*sizeof(*dark_particles));
	    if(dark_particles == NULL) {
		printf("<sorry, no memory for dark particles, %s>\n",title) ;
		return ;
	    }
	}
	else
	  dark_particles = NULL;
	if(star_particles != NULL) free(star_particles);
	if(nstar != 0) {
	    star_particles =
		 (struct star_particle *)malloc(nstar*sizeof(*star_particles));
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
	for(dp=dark_particles; dp < lastdp ; dp++) {
	    fscanf(infile,"%f%*[, \t\n]",&dp->eps);
	}
	for(sp=star_particles; sp < lastsp ; sp++) {
	    fscanf(infile,"%f%*[, \t\n]",&sp->eps);
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
	for(gp=gas_particles; gp < lastgp ; gp++){
	    fscanf(infile,"%f%*[, \t\n]",&gp->phi);
	}
	for(dp=dark_particles; dp < lastdp;  dp++){
	    fscanf(infile,"%f%*[, \t\n]",&dp->phi);
	}
	for(sp=star_particles; sp < lastsp; sp++){
	    fscanf(infile,"%f%*[, \t\n]",&sp->phi);
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
