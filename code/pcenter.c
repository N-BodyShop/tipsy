#include "defs.h"
#include "fdefs.h"

void pcenter(job)
    char *job;
{
    char command[MAXCOMM] ;
    int box ;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    Real center[MAXDIM];
    Real period[MAXDIM];
    Real half_period[MAXDIM];
    int i,j ;

    if(sscanf(job,"%s %d %f %f %f %f %f %f",command,&box,&center[0],
	     &center[1],&center[2],&period[0],&period[1],&period[2]) == 8) {
	if(boxes_loaded[box]) {
	    for (j = 0 ;j < header.ndim ;j++) {
		half_period[j] = period[j]/2. ;
	    }
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		for (j = 0 ;j < header.ndim ;j++) {
		    gp->pos[j] -= center[j] ;
		    if(gp->pos[j] >= half_period[j])gp->pos[j] -= period[j] ;
		    if(gp->pos[j] < -half_period[j])gp->pos[j] += period[j] ;
		}
	    }
	    for (i = 0 ;i < boxlist[box].ndark ;i++) {
		dp = boxlist[box].dp[i] ;
		for (j = 0 ;j < header.ndim ;j++) {
		    dp->pos[j] -= center[j] ;
		    if(dp->pos[j] >= half_period[j])dp->pos[j] -= period[j] ;
		    if(dp->pos[j] < -half_period[j])dp->pos[j] += period[j] ;
		}
	    }
	    for (i = 0 ;i < boxlist[box].nstar ;i++) {
		sp = boxlist[box].sp[i] ;
		for (j = 0 ;j < header.ndim ;j++) {
		    sp->pos[j] -= center[j] ;
		    if(sp->pos[j] >= half_period[j])sp->pos[j] -= period[j] ;
		    if(sp->pos[j] < -half_period[j])sp->pos[j] += period[j] ;
		}
	    }
	}
	else{
	    printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
