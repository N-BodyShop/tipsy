#include "defs.h"
void
cooling_sub(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    int box ;
    char file[MAXCOMM] ;
    int i,j ;
    int nbodies ;
    double cool_vec[COOLVECSIZE] ;
    double c1 ;
    double vol ;
    struct gas_particle *gp ;

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
	    if (!cool_loaded ){
		load_cool() ;
	    }
	    if (!uv_loaded ){
		load_uv() ;
	    }
	    nbodies=boxlist[box].ngas+boxlist[box].ndark+boxlist[box].nstar ;
	    fprintf(hardfile.ptr,"%d %d %d\n",nbodies,boxlist[box].ngas,
		    boxlist[box].nstar) ;
	    fprintf(hardfile.ptr,"%d\n",header.ndim) ;
	    fprintf(hardfile.ptr,"%.6e\n",header.time) ;
	    c1 = cosmof3*kpcunit*kpcunit*kpcunit*KPCCM*KPCCM*KPCCM ;
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		if(!uniform){
		    calc_uv(gp) ;
		}
		lycool(gp,cool_vec);
		vol = c1*gp->mass/gp->rho ;
		fprintf(hardfile.ptr,"%g %g %g %g %g %g %g %g %g\n",
			gp->rho,gp->temp,
			vol*cool_vec[0],vol*cool_vec[1],
			vol*cool_vec[2],vol*cool_vec[3],
			vol*cool_vec[4],vol*cool_vec[5],
			vol*cool_vec[6]) ;
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
