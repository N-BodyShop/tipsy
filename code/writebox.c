#include "defs.h"
void
writebox(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    char file[MAXCOMM] ;
    int box ;
    int i,j ;
    int nbodies ;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;

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
		    fprintf(hardfile.ptr,"%.6e\n",gp->pos[j]) ;
		}
		for (i = 0 ;i < boxlist[box].ndark ;i++) {
		    dp = boxlist[box].dp[i] ;
		    fprintf(hardfile.ptr,"%.6e\n",dp->pos[j]) ;
		}
		for (i = 0 ;i < boxlist[box].nstar ;i++) {
		    sp = boxlist[box].sp[i] ;
		    fprintf(hardfile.ptr,"%.6e\n",sp->pos[j]) ;
		}
	    }
	    for (j = 0 ;j < header.ndim ;j++) {
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    fprintf(hardfile.ptr,"%.6e\n",gp->vel[j]) ;
		}
		for (i = 0 ;i < boxlist[box].ndark ;i++) {
		    dp = boxlist[box].dp[i] ;
		    fprintf(hardfile.ptr,"%.6e\n",dp->vel[j]) ;
		}
		for (i = 0 ;i < boxlist[box].nstar ;i++) {
		    sp = boxlist[box].sp[i] ;
		    fprintf(hardfile.ptr,"%.6e\n",sp->vel[j]) ;
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
