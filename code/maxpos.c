/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:26  trq
 * Initial revision
 *
 * Revision 2.3  94/04/20  09:05:38  trq
 * Make the box fit more tightly around the particles.
 * 
 * Revision 2.2  94/04/15  15:57:55  trq
 * Make the box slightly larger than the particle distribution.
 * 
 */
#include "defs.h"
void
maxpos(xmax,xmin)
    Real xmax[MAXDIM];
    Real xmin[MAXDIM];
{
    int i ;
    struct gas_particle *gp, *lastgp ;
    struct dark_particle *dp, *lastdp ;
    struct star_particle *sp, *lastsp ;

    lastgp = gas_particles + header.nsph ;
    lastdp = dark_particles + header.ndark ;
    lastsp = star_particles + header.nstar ;

    if(header.nsph){
	for (i = 0; i < header.ndim; i++) {
	    gp = gas_particles ;
	    xmax[i] = gp->pos[i] ;
	    xmin[i] = gp->pos[i] ;
	}
    }
    else if(header.ndark){
	for (i = 0; i < header.ndim; i++) {
	    dp = dark_particles ;
	    xmax[i] = dp->pos[i] ;
	    xmin[i] = dp->pos[i] ;
	}
    }
    else{
	for (i = 0; i < header.ndim; i++) {
	    sp = star_particles ;
	    xmax[i] = sp->pos[i] ;
	    xmin[i] = sp->pos[i] ;
	}
    }

    for (gp = gas_particles ;gp < lastgp ;gp++) {
	for (i = 0; i < header.ndim; i++) {
	    xmax[i] = max(xmax[i],gp->pos[i]) ;
	    xmin[i] = min(xmin[i],gp->pos[i]) ;
	}
    }

    for (dp = dark_particles ;dp < lastdp ;dp++) {
	for ( i = 0; i < header.ndim; i++) {
	    xmax[i] = max(xmax[i],dp->pos[i]) ;
	    xmin[i] = min(xmin[i],dp->pos[i]) ;
	}
    }

    for (sp = star_particles ;sp < lastsp ;sp++) {
	for ( i = 0; i < header.ndim; i++) {
	    xmax[i] = max(xmax[i],sp->pos[i]) ;
	    xmin[i] = min(xmin[i],sp->pos[i]) ;
	}
    }
    for (i = 0; i < header.ndim; i++) {
	xmax[i] += 0.001*(xmax[i] - xmin[i]);
	xmin[i] -= 0.001*(xmax[i] - xmin[i]);
    }
}
