/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:37  trq
 * Initial revision
 *
 * Revision 2.0  1994/12/22  01:16:23  trq
 * New command.
 *
 */
#include "defs.h"
#include "fdefs.h"

void
markmass(job)
    char *job;
{
    char command[MAXCOMM] ;
    double max_mass ;
    double min_mass ;
    int box ;
    int i ;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;

    if (sscanf(job,"%s %d %lf %lf",command,&box,&min_mass,&max_mass) == 4) {
	if (boxes_loaded[box]) {
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		if(gp->mass <= max_mass && gp->mass >= min_mass){
		    mark_gas[boxlist[box].gpi[i]]++;
		}
	    }
	    for (i = 0 ;i < boxlist[box].ndark ;i++) {
		dp = boxlist[box].dp[i] ;
		if(dp->mass <= max_mass && dp->mass >= min_mass){
		    mark_dark[boxlist[box].dpi[i]]++;
		}
	    }
	    for (i = 0 ;i < boxlist[box].nstar ;i++) {
		sp = boxlist[box].sp[i] ;
		if(sp->mass <= max_mass && sp->mass >= min_mass){
		    mark_star[boxlist[box].spi[i]]++;
		}
	    }
	}
	else {
	    printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
