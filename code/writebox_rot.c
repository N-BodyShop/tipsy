#include "defs.h"
void
writebox_rot(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    char file[MAXCOMM] ;
    int box ;
    int i,j,k ;
    int nbodies ;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    float x;

    if (sscanf(job,"%s %d %s",command,&box,file) == 3) { 
	if (boxes_loaded[box]) {
	    sprintf(hardfile.name,"%s",file) ;
	    hardfile.ptr = fopen(hardfile.name,"w");
	    if(hardfile.ptr == NULL) 
	      {
		printf("<sorry,  can't open file %s, %s>\n",
		       hardfile.name,title);
		return;
	      }
	    nbodies=boxlist[box].ngas+boxlist[box].ndark+boxlist[box].nstar ;
	    fprintf(hardfile.ptr,"%d %d %d\n",nbodies,boxlist[box].ngas,
		    boxlist[box].nstar) ;
	    fprintf(hardfile.ptr,"%d\n",header.ndim) ;
	    fprintf(hardfile.ptr,"%.6e\n",header.time) ;
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		fprintf(hardfile.ptr,"%.6e\n",gp->mass) ;
	    }
	    for (i = 0 ;i < boxlist[box].ndark ;i++) {
		dp = boxlist[box].dp[i] ;
		fprintf(hardfile.ptr,"%.6e\n",dp->mass) ;
	    }
	    for (i = 0 ;i < boxlist[box].nstar ;i++) {
		sp = boxlist[box].sp[i] ;
		fprintf(hardfile.ptr,"%.6e\n",sp->mass) ;
	    }
	    for (j = 0 ;j < header.ndim ;j++) {
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
                    for (x = 0.0,k = 0 ;k < header.ndim ;k++) {
                        x += rot_matrix[j][k] * (gp->pos[k] -
                             boxes[active_box].center[k]) ;
                    }
		    fprintf(hardfile.ptr,"%.6e\n",x) ;
		}
		for (i = 0 ;i < boxlist[box].ndark ;i++) {
		    dp = boxlist[box].dp[i] ;
                    for (x = 0.0,k = 0 ;k < header.ndim ;k++) {
                        x += rot_matrix[j][k] * (dp->pos[k] -
                             boxes[active_box].center[k]) ;
                    }
		    fprintf(hardfile.ptr,"%.6e\n",x) ;
		}
		for (i = 0 ;i < boxlist[box].nstar ;i++) {
		    sp = boxlist[box].sp[i] ;
                    for (x = 0.0,k = 0 ;k < header.ndim ;k++) {
                        x += rot_matrix[j][k] * (sp->pos[k] -
                             boxes[active_box].center[k]) ;
                    }
		    fprintf(hardfile.ptr,"%.6e\n",x) ;
		}
	    }
	    for (j = 0 ;j < header.ndim ;j++) {
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
                    for (x = 0.0,k = 0 ;k < header.ndim ;k++) {
                        x += rot_matrix[j][k] * (gp->vel[k] -
                             boxes[active_box].center[k]) ;
                    }
		    fprintf(hardfile.ptr,"%.6e\n",x) ;
		}
		for (i = 0 ;i < boxlist[box].ndark ;i++) {
		    dp = boxlist[box].dp[i] ;
                    for (x = 0.0,k = 0 ;k < header.ndim ;k++) {
                        x += rot_matrix[j][k] * (dp->vel[k] -
                             boxes[active_box].center[k]) ;
                    }
		    fprintf(hardfile.ptr,"%.6e\n",x) ;
		}
		for (i = 0 ;i < boxlist[box].nstar ;i++) {
		    sp = boxlist[box].sp[i] ;
                    for (x = 0.0,k = 0 ;k < header.ndim ;k++) {
                        x += rot_matrix[j][k] * (sp->vel[k] -
                             boxes[active_box].center[k]) ;
                    }
		    fprintf(hardfile.ptr,"%.6e\n",x) ;
		}
	    }
	    for (i = 0 ;i < boxlist[box].ndark ;i++) {
		dp = boxlist[box].dp[i] ;
		fprintf(hardfile.ptr,"%.6e\n",dp->eps) ;
	    }
	    for (i = 0 ;i < boxlist[box].nstar ;i++) {
		sp = boxlist[box].sp[i] ;
		fprintf(hardfile.ptr,"%.6e\n",sp->eps) ;
	    }
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		fprintf(hardfile.ptr,"%.6e\n",gp->rho) ;
	    }
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		fprintf(hardfile.ptr,"%.6e\n",gp->temp) ;
	    }
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		fprintf(hardfile.ptr,"%.6e\n",gp->hsmooth) ;
	    }
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		fprintf(hardfile.ptr,"%.6e\n",gp->metals) ;
	    }
	    for (i = 0 ;i < boxlist[box].nstar ;i++) {
		sp = boxlist[box].sp[i] ;
		fprintf(hardfile.ptr,"%.6e\n",sp->metals) ;
	    }
	    for (i = 0 ;i < boxlist[box].nstar ;i++) {
		sp = boxlist[box].sp[i] ;
		fprintf(hardfile.ptr,"%.6e\n",sp->tform) ;
	    }
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		fprintf(hardfile.ptr,"%.6e\n",gp->phi) ;
	    }
	    for (i = 0 ;i < boxlist[box].ndark ;i++) {
		dp = boxlist[box].dp[i] ;
		fprintf(hardfile.ptr,"%.6e\n",dp->phi) ;
	    }
	    for (i = 0 ;i < boxlist[box].nstar ;i++) {
		sp = boxlist[box].sp[i] ;
		fprintf(hardfile.ptr,"%.6e\n",sp->phi) ;
	    }
	    fclose(hardfile.ptr) ;
	}
	else {
	    printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
