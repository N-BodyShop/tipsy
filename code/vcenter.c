/* $Header$
 * $Log$
 * Revision 1.2  2000/01/12 22:55:26  nsk
 * Fixed bugs in cooling routines, added cooling damping,
 * fixed bugs in starformation,  regularized dependencies.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:37  trq
 * Import to CVS
 *
 * Revision 2.2  94/04/20  08:46:13  trq
 * Added title variable.
 * 
 * Revision 2.0  93/10/19  13:18:40  trq
 * Initial revision
 * 
 */

#include "defs.h"
#include "fdefs.h"

void vcenter(job)
    char *job;
{
    char command[MAXCOMM] ;
    int box ;
    int center_box;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    char type[MAXCOMM];
    Real cvel[MAXDIM];
    int i ;

    if(sscanf(job,"%s %d %d %s",command,&box,&center_box, type) == 4) {
	if (boxes_loaded[center_box] && boxes_loaded[box]) {
	    if (strcmp(type,"gas") == 0 ) {
		setvec(cvel, boxes[center_box].gas_com_vel) ;
	    }
	    else if (strcmp(type,"dark") == 0 ) {
		setvec(cvel, boxes[center_box].dark_com_vel) ;
	    }
	    else if (strcmp(type,"star") == 0 ) {
		setvec(cvel, boxes[center_box].star_com_vel) ;
	    }
	    else if (strcmp(type,"baryon") == 0 ) {
		mass_add_vec(cvel, boxes[center_box].gas_com_vel,
			     boxes[center_box].gas_mass,
			     boxes[center_box].star_com_vel,
			     boxes[center_box].star_mass) ;
	    }
	    else if (strcmp(type,"all") == 0 ) {
		setvec(cvel, boxes[center_box].total_com_vel) ;
	    }
	    else {
		printf("<sorry, %s is not a proper data type, %s>\n",type,title) ;
	    }
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		gp->vel[0] -= cvel[0] ;
		gp->vel[1] -= cvel[1] ;
		gp->vel[2] -= cvel[2] ;
	    }
	    for (i = 0 ;i < boxlist[box].ndark ;i++) {
		dp = boxlist[box].dp[i] ;
		dp->vel[0] -= cvel[0] ;
		dp->vel[1] -= cvel[1] ;
		dp->vel[2] -= cvel[2] ;
	    }
	    for (i = 0 ;i < boxlist[box].nstar ;i++) {
		sp = boxlist[box].sp[i] ;
		sp->vel[0] -= cvel[0] ;
		sp->vel[1] -= cvel[1] ;
		sp->vel[2] -= cvel[2] ;
	    }
	    divv_loaded = NO ;
	    starform_loaded = NO ;
	    dudt_loaded = NO ;
	}
	else {
	  if (boxes_loaded[center_box]) {
	    printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
	  }
	  else
	    {
	      printf("<sorry, box %d is not loaded, %s>\n",center_box,title) ;
	    }
	}
    }
    else {
	input_error(command) ;
    }
}
