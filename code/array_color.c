#include <stdlib.h>
#include "defs.h"
void
array_color() 
{
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    int offset;
    int aoffset;
    int i,k ;
    double round;
    
    k = 0 ;
    if(particle_color != NULL) free(particle_color);
    if(boxlist[active_box].ngas + boxlist[active_box].ndark
       + boxlist[active_box].nstar != 0) {
	particle_color = (int *)malloc((boxlist[active_box].ngas
	    + boxlist[active_box].ndark + boxlist[active_box].nstar)
	    *sizeof(*particle_color));
	if(particle_color == NULL) {
	    printf("<sorry, no memory for particle colors, %s>\n",title) ;
	    return ;
	}
    }
    else
      particle_color = NULL;

    if(arraymap_red == colormap_red + 2)
      {
	round = 5e-6;
      }
    else
      {
	round = 0.5;
      }
    offset = header.nsph;
    if ( color_type == DARK || color_type == ALL ) {
	for (i = 0 ;i < boxlist[active_box].ndark ;i++, k++) {
	    dp = boxlist[active_box].dp[i] ;
	    aoffset = dp-dark_particles+offset;
	    if(aoffset >= array_size)
	      {
		printf("Sorry %s, the loaded array is too small\n",title);
		return;
	      }
            if(log_type == LIN){
	        particle_color[k] =
	          (int)(color_slope * array[aoffset]
		        + color_offset + round) ;
            }
            else{
	        particle_color[k] =
	          (int)(color_slope * log10(array[aoffset])
		        + color_offset + round) ;
            }
	}
    }
    offset = 0;
    if ( color_type == GAS || color_type == ALL ) {
	for (i = 0 ;i < boxlist[active_box].ngas ;i++, k++) {
	    gp = boxlist[active_box].gp[i] ;
	    aoffset = gp-gas_particles+offset;
	    if(aoffset >= array_size)
	      {
		printf("Sorry %s, the loaded array is too small\n",title);
		return;
	      }
            if(log_type == LIN){
	        particle_color[k] =
	          (int)(color_slope * array[aoffset]
		        + color_offset + round) ;
            }
            else{
	        particle_color[k] =
	          (int)(color_slope * log10(array[aoffset])
		        + color_offset + round) ;
            }
	}
    }
    offset = header.ndark + header.nsph;
    if ( color_type == STAR || color_type == ALL ) {
	for (i = 0 ;i < boxlist[active_box].nstar ;i++, k++) {
	    sp = boxlist[active_box].sp[i] ;
	    aoffset = sp-star_particles+offset;
	    if(aoffset >= array_size)
	      {
		printf("Sorry %s, the loaded array is too small\n",title);
		return;
	      }
            if(log_type == LIN){
	        particle_color[k] =
	          (int)(color_slope * array[aoffset]
		        + color_offset + round) ;
            }
            else{
	        particle_color[k] =
	          (int)(color_slope * log10(array[aoffset])
		        + color_offset + round) ;
            }
	}
    }
    particle_color = (int *)realloc(particle_color, k*sizeof(*particle_color));
    for (i = 0 ;i < k ;i++) {
	if(particle_color[i] < 2 && (clip_flag == LOCLIP ||
		clip_flag == BCLIP)){
	    particle_color[i] = 0 ;
	}
	else if(particle_color[i] > 126 && (clip_flag == UPCLIP ||
		clip_flag == BCLIP)){
	    particle_color[i] = 0 ;
	}
	else{
	    particle_color[i] = min(particle_color[i],126) ;
	    particle_color[i] = max(particle_color[i],2) ;
	}
    }
    current_color = YES;
}
