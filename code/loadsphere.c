/*
 * $Header$
 * $Log$
 * Revision 1.3  2003/08/04 23:51:03  trq
 * Reset current_plot if we have reloaded an active box.
 *
 * Revision 1.2  1996/12/20 01:25:12  trq
 * Added catbox command.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:37  trq
 * Import to CVS
 *
 * Revision 1.3  94/04/20  09:00:53  trq
 * Check for no particles in the box.
 * 
 * Revision 1.2  94/03/14  15:12:51  trq
 * Check for bad malloc returns.
 * 
 */

#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>

void
loadsphere(box,center,radius)
    int box ;
    Real center[MAXDIM] ;
    Real radius ;
{ 
    int i,j ;
    double d ;
    struct gas_particle *gp, *lastgp ;
    struct dark_particle *dp, *lastdp ;
    struct star_particle *sp, *lastsp ;

    lastgp = gas_particles + header.nsph ;
    lastdp = dark_particles + header.ndark ;
    lastsp = star_particles + header.nstar ;
    if(boxlist_alloc(box, header.nsph, header.ndark, header.nstar) == 0)
      return;
    if(box == 0) {

	boxlist[box].ngas = 0 ;
	for (gp = gas_particles, j = 0 ;gp < lastgp ;gp++, j++) {
	    d=distance(center,gp->pos) ;
	    if (d <= radius) {
		boxlist[box].gpi[boxlist[box].ngas] = j ;
		boxlist[box].gp[boxlist[box].ngas++] = gp ;
	    }
	}

	boxlist[box].ndark = 0 ;
	for (dp = dark_particles, j = 0 ;dp < lastdp ;dp++, j++) {
	    d=distance(center,dp->pos) ;
	    if (d <= radius) {
		boxlist[box].dpi[boxlist[box].ndark] = j ;
		boxlist[box].dp[boxlist[box].ndark++] = dp ;
	    }
	}

	boxlist[box].nstar = 0 ;
	for (sp = star_particles, j = 0 ;sp < lastsp ;sp++, j++) {
	    d=distance(center,sp->pos) ;
	    if (d <= radius) {
		boxlist[box].spi[boxlist[box].nstar] = j ;
		boxlist[box].sp[boxlist[box].nstar++] = sp ;
	    }
	}
    }
    else {
	boxlist[box].ngas = 0 ;
	for (i = 0 ;i < boxlist[0].ngas ;i++) {
	    gp = boxlist[0].gp[i] ;
	    d=distance(center,gp->pos) ;
	    if (d <= radius) {
		boxlist[box].gp[boxlist[box].ngas] = gp ;
		boxlist[box].gpi[boxlist[box].ngas++] = boxlist[0].gpi[i] ;
	    }
	}
	boxlist[box].ndark = 0 ;
	for (i = 0 ;i < boxlist[0].ndark ;i++) {
	    dp = boxlist[0].dp[i] ;
	    d=distance(center,dp->pos) ;
	    if (d <= radius) {
		boxlist[box].dp[boxlist[box].ndark] = dp ;
		boxlist[box].dpi[boxlist[box].ndark++] = boxlist[0].dpi[i] ;
	    }
	}
	boxlist[box].nstar = 0 ;
	for (i = 0 ;i < boxlist[0].nstar ;i++) {
	    sp = boxlist[0].sp[i] ;
	    d=distance(center,sp->pos) ;
	    if (d <= radius) {
		boxlist[box].sp[boxlist[box].nstar] = sp ;
		boxlist[box].spi[boxlist[box].nstar++] = boxlist[0].spi[i] ;
	    }
	}
    }
    boxlist_realloc(box);
    boxes_loaded[box] = LOADED ;

    boxes[box].size = 2.*radius ;
    boxes[box].volume = 4.*PI/3.*radius*radius*radius ;
    setvec(boxes[box].center,center) ;
    box_cumulate(box);
    if(box == active_box) {
	current_project = NO;
	}
}
