/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:35  trq
 * Initial revision
 *
 * Revision 1.3  94/04/20  08:45:47  trq
 * Added title variable.
 * 
 * Revision 1.1  1993/03/30  16:03:36  trq
 * Initial revision
 *
 */
#include "defs.h"
#include <malloc.h>

void
old2binary(infile,outfile)
    FILE *infile;
    FILE *outfile;
{
    int ndim ;
    int ngas ;
    int ndark ;
    int nstar ;
    int count ;
    int i;
    struct gas_particle *gp, *lastgp;
    struct dark_particle *dp, *lastdp ;
    struct star_particle *sp, *lastsp ;
    struct old_gas {
	double mass;
	double pos[MAXDIM];
	double vel[MAXDIM];
	double rho;
	double temp;
	double hsmooth;
	double metals ;
	double phi ;
    } old_gass;
    struct old_dark {
	double mass;
	double pos[MAXDIM];
	double vel[MAXDIM];
	double eps;
	double phi ;
    } old_darks;
    struct old_star {
	double mass;
	double pos[MAXDIM];
	double vel[MAXDIM];
	double metals ;
	double tform ;
	double eps;
	double phi ;
    } old_stars;

    forever {
	count = fread((char *)&header,sizeof(header),1,infile) ;
	if ( count == 0 ){
	    break ;
	}
	ndim=header.ndim;
	ngas=header.nsph;
	nstar = header.nstar ;
	ndark = header.ndark;
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
	  fread((char *)&old_gass,sizeof(struct old_gas),1,infile) ;
	  gp->mass = old_gass.mass;
	  for(i = 0; i < ndim; i++)
	    {
	      gp->pos[i] = old_gass.pos[i];
	      gp->vel[i] = old_gass.vel[i];
	    }
	  gp->rho = old_gass.rho;
	  gp->temp = old_gass.temp;
	  gp->hsmooth = old_gass.hsmooth;
	  gp->metals = old_gass.metals;
	  gp->phi = old_gass.phi;
	}
	for(dp=dark_particles; dp < lastdp;  dp++){
	  fread((char *)&old_darks,sizeof(struct old_dark),1,infile) ;
	  dp->mass = old_darks.mass;
	  for(i = 0; i < ndim; i++)
	    {
	      dp->pos[i] = old_darks.pos[i];
	      dp->vel[i] = old_darks.vel[i];
	    }
	  dp->eps = old_darks.eps;
	  dp->phi = old_darks.phi;
	}
	for(sp=star_particles; sp < lastsp; sp++){
	  fread((char *)&old_stars,sizeof(struct old_star),1,infile) ;
	  sp->mass = old_stars.mass;
	  for(i = 0; i < ndim; i++)
	    {
	      sp->pos[i] = old_stars.pos[i];
	      sp->vel[i] = old_stars.vel[i];
	    }
	  sp->metals = old_stars.metals;
	  sp->tform = old_stars.tform;
	  sp->eps = old_stars.eps;
	  sp->phi = old_stars.phi;
	}
	fwrite((char *)&header,sizeof(header),1,outfile) ;
	fwrite((char *)gas_particles,sizeof(struct gas_particle),ngas,outfile) ;
	fwrite((char *)dark_particles,sizeof(struct dark_particle),
	       ndark,outfile) ;
	fwrite((char *)star_particles,sizeof(struct star_particle),
	   nstar,outfile) ;
	printf("read time %f\n",header.time) ;
    }
}
