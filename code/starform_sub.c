#include "defs.h"
void
starform_sub(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    char file[MAXCOMM] ;
    int i,j ;
    int box ;
    int nbodies ;
    struct gas_particle *gp ;

    if (sscanf(job,"%s %s",command,file) == 2) { 
	box = active_box ;
	if (boxes_loaded[box]) {
	    sprintf(hardfile.name,"%s",file) ;
	    hardfile.ptr = fopen(hardfile.name,"w");
	    if(hardfile.ptr == NULL) 
	      {
		printf("<sorry,  can't open file %s, %s>\n",
		       hardfile.name,title);
		return;
	      }
            if(!starform_loaded){
                starform_func() ;
            }
	    nbodies=boxlist[box].ngas+boxlist[box].ndark+boxlist[box].nstar ;
	    fprintf(hardfile.ptr,"%d %d %d\n",nbodies,boxlist[box].ngas,
		    boxlist[box].nstar) ;
	    fprintf(hardfile.ptr,"%d\n",header.ndim) ;
	    fprintf(hardfile.ptr,"%.6e\n",header.time) ;
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[active_box].gp[i] ;
		fprintf(hardfile.ptr,"%g\n",starform[gp-gas_particles]) ;
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
