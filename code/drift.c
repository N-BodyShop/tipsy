#include "defs.h"
#include "fdefs.h"

void drift(job)
    char *job;
{
    char command[MAXCOMM] ;
    int box ;
    char type[MAXCOMM];
    double delta_t ;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    int i ;

    if(sscanf(job,"%s %d %s %lf",command,&box,type,&delta_t) == 4) {
	if (boxes_loaded[box]) {
	    if (strcmp(type,"gas") != 0  && strcmp(type,"dark") != 0 && 
	    	    strcmp(type,"star") != 0 && strcmp(type,"baryon") != 0 && 
	            strcmp(type,"all") != 0 ) {
		printf("<sorry, %s is not a proper data type,%s>\n",type,title);
	    }
	    if (strcmp(type,"gas") == 0 || strcmp(type,"all") == 0 ||
		    strcmp(type,"baryon") == 0) {
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    gp->pos[0] += delta_t*gp->vel[0] ;
		    gp->pos[1] += delta_t*gp->vel[1] ;
		    gp->pos[2] += delta_t*gp->vel[2] ;
		}
	    }
	    if (strcmp(type,"dark") == 0 || strcmp(type,"all") == 0 ) {
		for (i = 0 ;i < boxlist[box].ndark ;i++) {
		    dp = boxlist[box].dp[i] ;
		    dp->pos[0] += delta_t*dp->vel[0] ;
		    dp->pos[1] += delta_t*dp->vel[1] ;
		    dp->pos[2] += delta_t*dp->vel[2] ;
		}
	    }
	    if (strcmp(type,"star") == 0 || strcmp(type,"all") == 0 ||
		    strcmp(type,"baryon") == 0) {
		for (i = 0 ;i < boxlist[box].nstar ;i++) {
		    sp = boxlist[box].sp[i] ;
		    sp->pos[0] += delta_t*sp->vel[0] ;
		    sp->pos[1] += delta_t*sp->vel[1] ;
		    sp->pos[2] += delta_t*sp->vel[2] ;
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
