#include "defs.h"
#include <stdlib.h>

void
magnitude_color() 
{
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    int i,k ;
    double mag ;
    int offset;
    int aoffset;
    struct vec *v ;

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

    if(vector_plot_type == VELOCITY){
	if ( color_type == DARK || color_type == ALL ) {
	    for (i = 0 ;i < boxlist[active_box].ndark ;i++, k++) {
		dp = boxlist[active_box].dp[i] ;
		mag = sqrt((dp->vel[0])*(dp->vel[0])+(dp->vel[1])*(dp->vel[1])+
			(dp->vel[2])*(dp->vel[2])) ;
		if(log_type == LOG){
		    mag = log10(mag) ;
		}
		particle_color[k] = (int)(color_slope * mag +
			color_offset +0.5) ;
	    }
	}
	if ( color_type == GAS || color_type == ALL ) {
	    for (i = 0 ;i < boxlist[active_box].ngas ;i++, k++) {
		gp = boxlist[active_box].gp[i] ;
		mag = sqrt((gp->vel[0])*(gp->vel[0])+(gp->vel[1])*(gp->vel[1])+
		        (gp->vel[2])*(gp->vel[2])) ;
		if(log_type == LOG){
		    mag = log10(mag) ;
		}
		particle_color[k] = (int)(color_slope * mag +
			color_offset +0.5) ;
	    }
	}
	if ( color_type == STAR || color_type == ALL ) {
	    for (i = 0 ;i < boxlist[active_box].nstar ;i++, k++) {
		sp = boxlist[active_box].sp[i] ;
		mag = sqrt((sp->vel[0])*(sp->vel[0])+(sp->vel[1])*(sp->vel[1])+
			(sp->vel[2])*(sp->vel[2])) ;
		if(log_type == LOG){
		    mag = log10(mag) ;
		}
		particle_color[k] = (int)(color_slope * mag +
			color_offset +0.5) ;
	    }
	}
    }
    else if(vector_plot_type == VECTOR){
	if ( color_type == DARK || color_type == ALL ) {
	    offset = header.nsph;
	    for (i = 0 ;i < boxlist[active_box].ndark ;i++, k++) {
		dp = boxlist[active_box].dp[i] ;
		aoffset = dp-dark_particles+offset;
		if(aoffset >= vector_size)
		  {
		    printf("<sorry %s, the loaded vector is too small>\n",title);
		    return;
		  }
		v = &vector[aoffset] ;
		mag = sqrt((v->v[0])*(v->v[0])+(v->v[1])*(v->v[1])+
			(v->v[2])*(v->v[2])) ;
		if(log_type == LOG){
		    mag = log10(mag) ;
		}
		particle_color[k] = (int)(color_slope * mag +
			color_offset +0.5) ;
	    }
	}
	if ( color_type == GAS || color_type == ALL ) {
	    offset = 0;
	    for (i = 0 ;i < boxlist[active_box].ngas ;i++, k++) {
		gp = boxlist[active_box].gp[i] ;
		aoffset = gp-gas_particles+offset;
		if(aoffset >= vector_size)
		  {
		    printf("<sorry %s, the loaded vector is too small>\n",title);
		    return;
		  }
		v = &vector[aoffset] ;
		mag = sqrt((v->v[0])*(v->v[0])+(v->v[1])*(v->v[1])+
			(v->v[2])*(v->v[2])) ;
		if(log_type == LOG){
		    mag = log10(mag) ;
		}
		particle_color[k] = (int)(color_slope * mag +
			color_offset +0.5) ;
	    }
	}
	if ( color_type == STAR || color_type == ALL ) {
	    offset = header.ndark + header.nsph;
	    for (i = 0 ;i < boxlist[active_box].nstar ;i++, k++) {
		sp = boxlist[active_box].sp[i] ;
		aoffset = sp-star_particles+offset;
		if(aoffset >= vector_size)
		  {
		    printf("<sorry %s, the loaded vector is too small>\n",title);
		    return;
		  }
		v = &vector[aoffset] ;
		mag = sqrt((v->v[0])*(v->v[0])+(v->v[1])*(v->v[1])+
			(v->v[2])*(v->v[2])) ;
		if(log_type == LOG){
		    mag = log10(mag) ;
		}
		particle_color[k] = (int)(color_slope * mag +
			color_offset +0.5) ;
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
