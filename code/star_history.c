#include "defs.h"
#include "fdefs.h"

void
star_history(job)
    char *job;
{
    char command[MAXCOMM] ;
    int box ;
    int number_bins ;
    double min_time ;
    double max_time ;
    double time ;
    double bin_size ;
    struct star_particle *sp ;
    int i,j ;
    int i_min ;
    double star_mass ;
    double total_star_mass ;

    if (sscanf(job,"%s %d %d %s %lf %lf",command,&box,
	    &number_bins,hardfile.name,&min_time,&max_time) == 6){
	hardfile.ptr = fopen(hardfile.name,"w") ;
	if (boxes_loaded[box]) {
	    total_star_mass = 0. ;
	    bin_size = (max_time - min_time) / (double)number_bins ;
	    i_min = 0 ;
	    for (j = 1 ;j <= number_bins ; j++) {
		time = j * bin_size + min_time ;
		star_mass = 0. ;
		for (i = i_min ;i < boxlist[box].nstar ; i++) {
		    sp = boxlist[box].sp[i] ;
		    if(sp->tform <= time){
			star_mass += sp->mass ;
			total_star_mass += sp->mass ;
		    }
		    else {
			i_min = i ;
			break ;
		    }
		}
		fprintf(hardfile.ptr,"%g %g %g\n",time,star_mass/bin_size,
			total_star_mass) ;
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
