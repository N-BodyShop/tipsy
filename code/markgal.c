/*
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:35  trq
 * Initial revision
 *
 * Revision 1.3  94/04/20  08:45:51  trq
 * Added title variable.
 * 
 * Revision 1.1  94/02/16  13:45:04  trq
 * Initial revision
 * 
 */
#include "defs.h"
#include "fdefs.h"

void
markgal(job)
    char *job;
{
    char command[MAXCOMM] ;
    double max_temp ;
    double min_rho ;
    int box ;
    int i ;
    struct gas_particle *gp ;

    if (sscanf(job,"%s %d %lf %lf",command,&box,&max_temp,&min_rho) == 4) {
	if (boxes_loaded[box]) {
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		if(gp->temp <= max_temp && gp->rho >= min_rho){
		    mark_gas[boxlist[box].gpi[i]]++;
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
