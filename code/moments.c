#include "defs.h"
moments(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    char particle_type[MAXCOMM] ;
    char bin_type[MAXCOMM] ;
    int box ;
    int center_box ;
    int number_bins ;
    double min_radius ;
    Real center[MAXDIM] ;
    double bin_size ;
    int i ;

    if (sscanf(job,"%s %d %d %s %s %d %s %lf",command,&box,&center_box,
	    particle_type,bin_type,&number_bins,
	    hardfile.name,&min_radius) == 8) { 
	if (boxes_loaded[box] && boxes_loaded[center_box] &&
		number_bins <= MAXBIN) {
	    if (strcmp(bin_type,"lin") == 0) {
		bin_size = (boxes[box].size / 2.0  - min_radius) /
			(double)number_bins ;
	    }
	    else if (strcmp(bin_type,"log") == 0) {
		min_radius = log10(min_radius) ;
		bin_size = (log10(boxes[box].size / 2.0 ) - min_radius) /
			(double)number_bins ;
	    }
	    else {
		printf("<sorry, %s not a bin type, %s>\n",bin_type,title) ;
		return ;
	    }
	    if (strcmp(particle_type,"dark") != 0 && 
		    strcmp(particle_type,"gas") != 0 &&
		    strcmp(particle_type,"star") != 0 &&
		    strcmp(particle_type,"all") !=0) {
		printf("<sorry, %s is not a particle type, %s>\n",
			particle_type,title) ;
		return ;
	    }
	    hardfile.ptr = fopen(hardfile.name,"w") ;
	    for (i = 1 ;i <= number_bins ;i++) {
		if (strcmp(particle_type,"dark") == 0){
		    setvec(center,boxes[center_box].dark_com) ;
		}
		else if (strcmp(particle_type,"star") == 0){
		    setvec(center,boxes[center_box].star_com) ;
		}
		else if (strcmp(particle_type,"gas") == 0){
		    setvec(center,boxes[center_box].gas_com) ;
		}
		else{
		    setvec(center,boxes[center_box].total_com) ;
		}
		fit_radius = bin_size * (double)(i) ;
		if (strcmp(bin_type,"lin") == 0) {
		    fit_radius += min_radius ;
		}
		else if (strcmp(bin_type,"log") == 0) {
		    fit_radius = pow(10.,fit_radius + min_radius) ;
		}
		find_shape(particle_type,box,center) ;
		fprintf(hardfile.ptr,"%g %g %g ",fit_radius,ba,ca) ;
		fprintf(hardfile.ptr,"%g %g %g ",phi,theta,psi) ;
		fprintf(hardfile.ptr,"%g %g %g\n",center[0],
			center[1],center[2]) ;
	    }
	    fclose(hardfile.ptr) ;
	}
	else if (!boxes_loaded[box]) {
	    printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
	}
	else if (!boxes_loaded[center_box]) {
	    printf("<sorry, box %d is not loaded, %s>\n",center_box,title) ;
	}
	else {
	    printf("<%d is too many bins, %s>\n",number_bins,title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
