#include "defs.h"
#include "fdefs.h"

void
sift(constant,box)
    double constant[5] ;
    int box ;
{
    int ngas ;
    int ndark ;
    int nstar ;
    int i,j ;
    double upper,lower ;
    double d ;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;

    lower = constant[3] ;
    upper = constant[4] ;

    ngas = boxlist[box].ngas ;
    boxlist[box].ngas = 0 ;
    for (i = 0 ;i < ngas ;i++) {
	gp = boxlist[box].gp[i] ;
	d = 0.0 ;
	for(j = 0 ;j < header.ndim ;j++) {
	    d += constant[j] * gp->pos[j] ;
	}
	if (d >= lower && d <= upper) {
	    boxlist[box].gpi[boxlist[box].ngas] = boxlist[box].gpi[i] ;
	    boxlist[box].gp[boxlist[box].ngas++] = gp ;
	}
    }

    ndark = boxlist[box].ndark ;
    boxlist[box].ndark = 0 ;
    for (i = 0 ;i < ndark ;i++) {
	dp = boxlist[box].dp[i] ;
	d = 0.0 ;
	for(j = 0 ;j < header.ndim ;j++) {
	    d += constant[j] * dp->pos[j] ;
	}
	if (d >= lower && d <= upper) {
	    boxlist[box].dpi[boxlist[box].ndark] = boxlist[box].dpi[i] ;
	    boxlist[box].dp[boxlist[box].ndark++] = dp ;
	    
	}
    }

    nstar = boxlist[box].nstar ;
    boxlist[box].nstar = 0 ;
    for (i = 0 ;i < nstar ;i++) {
	sp = boxlist[box].sp[i] ;
	d = 0.0 ;
	for(j = 0 ;j < header.ndim ;j++) {
	    d += constant[j] * sp->pos[j] ;
	}
	if (d >= lower && d <= upper) {
	    boxlist[box].spi[boxlist[box].nstar] = boxlist[box].spi[i] ;
	    boxlist[box].sp[boxlist[box].nstar++] = sp ;
	}
    }
}
