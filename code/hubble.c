#include "defs.h"
#include "fdefs.h"

void
hubble(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    int box ;
    double hubble_const ;
    double hubble_center[3] ;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    int i ;

    if(sscanf(job,"%s %d %lf %lf %lf %lf",command,&box,&hubble_const,
	      &hubble_center[0], &hubble_center[1],&hubble_center[2]) == 6) {
	if (boxes_loaded[box]) {
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		gp->vel[0] += hubble_const * (gp->pos[0] - hubble_center[0]) ;
		gp->vel[1] += hubble_const * (gp->pos[1] - hubble_center[1]) ;
		gp->vel[2] += hubble_const * (gp->pos[2] - hubble_center[2]) ;
	    }
	    for (i = 0 ;i < boxlist[box].ndark ;i++) {
		dp = boxlist[box].dp[i] ;
		dp->vel[0] += hubble_const * (dp->pos[0] - hubble_center[0]) ;
		dp->vel[1] += hubble_const * (dp->pos[1] - hubble_center[1]) ;
		dp->vel[2] += hubble_const * (dp->pos[2] - hubble_center[2]) ;
	    }
	    for (i = 0 ;i < boxlist[box].nstar ;i++) {
		sp = boxlist[box].sp[i] ;
		sp->vel[0] += hubble_const * (sp->pos[0] - hubble_center[0]) ;
		sp->vel[1] += hubble_const * (sp->pos[1] - hubble_center[1]) ;
		sp->vel[2] += hubble_const * (sp->pos[2] - hubble_center[2]) ;
	    }
	    divv_loaded = NO;
	    starform_loaded = NO ;
	    dudt_loaded = NO ;
	}
	else {
	    printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
