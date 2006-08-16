#include "defs.h"
#include "fdefs.h"

arraymoments(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    char particle_type[MAXCOMM] ;
    char bin_type[MAXCOMM] ;
    int box ;
    int center_box ;
    int number_bins ;
    Real center[MAXDIM] ;
    int i ;
    int bin_count ;

    int p_type;
    int *aindex;
    int aindex_size;
    int k, l, m;
    double p = log(3.0)/log(1.5);   
    double s1;
    double s3;
    int particlemax;
    int particlemin;
    int inc;
    int rbinsize;
    raverage = 0.0;
    particlemax = 0;
    particlemin = 0;
    rbinsize = 0; 

    if (sscanf(job,"%s %d %d %s %s %d %s %d %d %d",command,&box,&center_box,
	    particle_type,bin_type,&number_bins,
	    hardfile.name,&bin_count,&inc,&rbinsize) == 10) { 
	if (boxes_loaded[box] && boxes_loaded[center_box] &&
		number_bins <= 1000) {
	    if (strcmp(bin_type,"bin") == 0) {
	    }
	    else if (strcmp(bin_type,"shell") == 0) {
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
	    if(bin_count < rbinsize) {
		printf("<sorry, need at least %d particles in a bin, %s>\n",rbinsize,title);
		return;
	    }

	    hardfile.ptr = fopen(hardfile.name,"w") ;

	    /* allocating and initializing index array */
	    if(array == NULL) { 
		printf("<sorry, no array read in yet, %s>\n",title);
		return;
	    }

	    if (strcmp(particle_type,"dark") == 0){
	        aindex_size = boxlist[box].ndark;
		p_type = DARK;
	    }
    	    else if (strcmp(particle_type,"star") == 0){
	        aindex_size = boxlist[box].nstar;
		p_type = STAR;
	    }
	    else if (strcmp(particle_type,"gas") == 0){
	        aindex_size = boxlist[box].ngas;
		p_type = GAS;
	    }
	    else{
	        aindex_size = boxlist[box].ndark + boxlist[box].nstar + boxlist[box].ngas;
		p_type = ALL;
	    }

	    if(inc*number_bins + bin_count - inc > aindex_size) {
		printf("<sorry, not enough particles for those bin settings, %s>\n",title);
		return; 
	    }

  	    aindex = (int *)malloc(aindex_size*sizeof(int));
	    if(aindex == NULL) {
		printf("<Error in creating index array, %s>\n",title);
		return;
	    }
	    for(k = 0; k < aindex_size; k++) {
	        aindex[k] = k;
	    }
	    sortarray(aindex, 0, aindex_size - 1, box, p_type);

	    /* for(m = lowershell ; m <= (lowershell + bin_count) && m < boxlist[box].ndark; m++) {
		mark_dark[boxlist[box].dpi[aindex[m]]]++;
    	    } */


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
		if (strcmp(bin_type,"bin") == 0) {
		    particlemin = 0;
		}
		else if (strcmp(bin_type,"shell") == 0) {
		    particlemin = inc*i - inc;	
		}
		particlemax = inc*i + bin_count - inc ;

		find_shape_array(particle_type,box,center,aindex,particlemax,particlemin,rbinsize) ;
		s1 = sin(PI/2.0*pow(1.0 - ca, p)) ;
		s3 = sin(PI/2.0*ba) ;
		fprintf(hardfile.ptr,"%g %g %g ",raverage,ba,ca) ;
		fprintf(hardfile.ptr,"%g %g %g ",phi,theta,psi) ;
		fprintf(hardfile.ptr,"%g %g %g ",center[0],
			center[1],center[2]) ;
		fprintf(hardfile.ptr,"%g %g %i\n",s1, s3, particlemax) ;
	    }
	    fclose(hardfile.ptr) ;
	    free(aindex);
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
