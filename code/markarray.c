#include "defs.h"
#include "fdefs.h"
void
markarray(job)
    char *job;
{
    char command[MAXCOMM] ;
    double max_array ;
    double min_array ;
    int box ;
    int i ;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    int aoffset;

    if (sscanf(job,"%s %d %lf %lf",command,&box,&min_array,&max_array) == 4) {
      if(array_size == 0) 
	{
	  printf("<Sorry %s, there is no array loaded\n",title);
	  return;
	}
	if (boxes_loaded[box]) {
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		aoffset = gp - gas_particles;
		if(array[aoffset] <= max_array && array[aoffset] >= min_array){
		    mark_gas[boxlist[box].gpi[i]]++;
		}
	    }
	    for (i = 0 ;i < boxlist[box].ndark ;i++) {
		dp = boxlist[box].dp[i] ;
		aoffset = dp - dark_particles + header.nsph;
		if(array[aoffset] <= max_array && array[aoffset] >= min_array){
		    mark_dark[boxlist[box].dpi[i]]++;
		}
	    }
	    for (i = 0 ;i < boxlist[box].nstar ;i++) {
		sp = boxlist[box].sp[i] ;
		aoffset = sp - star_particles + header.nsph + header.ndark;
		if(array[aoffset] <= max_array && array[aoffset] >= min_array){
		    mark_star[boxlist[box].spi[i]]++;
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
