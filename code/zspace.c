#include "defs.h"
#include "fdefs.h"

#define SQR(X)	((X)*(X))

void
zspace(job)
    char *job ;
{
    char command[MAXCOMM] ;
    int box ;
    double hubble_const ;
    double hubble_center[3] ;
    double r2;
    double vr;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    int i, j;

    if(sscanf(job,"%s %d %lf %lf %lf %lf",command,&box,&hubble_const,
	      &hubble_center[0], &hubble_center[1],&hubble_center[2]) == 6) {
	if (boxes_loaded[box]) {
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		r2 = 0;
		vr = 0;
		for(j = 0; j < MAXDIM; j++) {
		    r2 += SQR((gp->pos[j] - hubble_center[j]));
		    vr += (gp->pos[j] - hubble_center[j])*gp->vel[j];
		}
		vr /= hubble_const;
		for(j = 0; j < MAXDIM; j++)
		    gp->pos[j] += (gp->pos[j] - hubble_center[j])*vr/r2;
	    }
	    for (i = 0 ;i < boxlist[box].ndark ;i++) {
		dp = boxlist[box].dp[i] ;
		r2 = 0;
		vr = 0;
		for(j = 0; j < MAXDIM; j++) {
		    r2 += SQR((dp->pos[j] - hubble_center[j]));
		    vr += (dp->pos[j] - hubble_center[j])*dp->vel[j];
		}
		vr /= hubble_const;
		for(j = 0; j < MAXDIM; j++)
		    dp->pos[j] += (dp->pos[j] - hubble_center[j])*vr/r2;
	    }
	    for (i = 0 ;i < boxlist[box].nstar ;i++) {
		sp = boxlist[box].sp[i] ;
		r2 = 0;
		vr = 0;
		for(j = 0; j < MAXDIM; j++) {
		    r2 += SQR((sp->pos[j] - hubble_center[j]));
		    vr += (sp->pos[j] - hubble_center[j])*sp->vel[j];
		}
		vr /= hubble_const;
		for(j = 0; j < MAXDIM; j++)
		    sp->pos[j] += (sp->pos[j] - hubble_center[j])*vr/r2;
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
