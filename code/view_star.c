/*
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:32  trq
 * Initial revision
 *
 * Revision 2.3  94/04/20  08:46:20  trq
 * Added title variable.
 * 
 * Revision 2.1  94/02/16  10:19:10  trq
 * Added star formation plots
 * 
 */
#include "defs.h"
view_star(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    char type[MAXCOMM] ;
    char clip_type[MAXCOMM] ;
    double low ;
    double high ;
    double star_lum() ;
    struct star_particle *sp ;
    int i;
    double mag_offset ;

    if (boxes_loaded[0]) {
	if((i = sscanf(job,"%s %s %lf %lf %s",command,type,&low,&high,
		clip_type)) >= 4) {
            if(particle_color != NULL) free(particle_color);
	    if(boxlist[active_box].nstar != 0) {
		particle_color = (int *)malloc(boxlist[active_box].nstar
		    *sizeof(*particle_color));
		if(particle_color == NULL) {
		  printf("<sorry, no memory for particle colors, %s>\n",title) ;
		    return ;
		}
	    }
	    else
	      particle_color = NULL;
	    if(i == 5){
	        if(strcmp(clip_type,"clip") == 0 ||
			strcmp(clip_type,"c") == 0 ||
			strcmp(clip_type,"clipboth") == 0 ||
			strcmp(clip_type,"cb") == 0 ){
		    clip_flag = BCLIP ;
		}
	        else if(strcmp(clip_type,"clipup") == 0 ||
			strcmp(clip_type,"cup") == 0 ){
		    clip_flag = UPCLIP ;
		}
	        else if(strcmp(clip_type,"cliplo") == 0 ||
			strcmp(clip_type,"clo") == 0 ){
		    clip_flag = LOCLIP ;
		}
		else{
		    printf("<sorry, %s is not a proper option, %s>\n",
			    clip_type,title) ;
		    return ;
		}
	    }
	    else{
		clip_flag = NOCLIP ;
	    }
	    if(!star_plot) {
		star_plot = YES ;
		color_type = STAR;
		array_plot = NO;
		radial_plot = NO ;
		gas_plot = NO ;
		potential_plot = NO ;
		magnitude_plot = NO ;
		load_color_table() ;
	    }
	    project() ;
	    color_slope = 124.0 / (high - low) ;
	    color_offset = 2.0 - color_slope * low ;
	    if(strcmp(type,"lumv") == 0 || strcmp(type,"luminosityv") == 0  ||
		    strcmp(type,"lumb") == 0  ||
		    strcmp(type,"luminosityb") == 0  ||
		    strcmp(type,"lumu") == 0  ||
		    strcmp(type,"luminosityu") == 0 ||
		    strcmp(type,"lumuu") == 0  ||
		    strcmp(type,"luminosityuu") == 0){
		if(strcmp(type,"lumv") == 0 || strcmp(type,"luminosityv") == 0){
		    color_filter = VBAND ;
		}
		else if(strcmp(type,"lumb") == 0 || 
			strcmp(type,"luminosityb") == 0){
		    color_filter = BBAND ;
		}
		else if(strcmp(type,"lumu") == 0 || 
			strcmp(type,"luminosityu") == 0){
		    color_filter = UBAND ;
		}
		else if(strcmp(type,"lumuu") == 0 || 
			strcmp(type,"luminosityuu") == 0){
		    color_filter = UUBAND ;
		}
		for (i = 0 ;i < boxlist[active_box].nstar ;i++) {
		    sp = boxlist[active_box].sp[i] ;
		    particle_color[i] = (int)(color_slope *
			    star_lum(sp->mass,sp->tform) + color_offset +0.5) ;
		}
	    }
	    else if(strcmp(type,"magv") == 0 ||
		    strcmp(type,"magnitudev") == 0  ||
		    strcmp(type,"magb") == 0  ||
		    strcmp(type,"magnitudeb") == 0  ||
		    strcmp(type,"magu") == 0  ||
		    strcmp(type,"magnitudeu") == 0 ||
		    strcmp(type,"maguu") == 0  ||
		    strcmp(type,"magnitudeuu") == 0){
		if(strcmp(type,"magv") == 0 || strcmp(type,"magnitudev") == 0){
		    color_filter = VBAND ;
		    mag_offset = V0 ;
		}
		else if(strcmp(type,"magb") == 0 || 
			strcmp(type,"magnitudeb") == 0){
		    color_filter = BBAND ;
		    mag_offset = B0 ;
		}
		else if(strcmp(type,"magu") == 0 || 
			strcmp(type,"magnitudeu") == 0){
		    color_filter = UBAND ;
		    mag_offset = U0 ;
		}
		else if(strcmp(type,"maguu") == 0 || 
			strcmp(type,"magnitudeuu") == 0){
		    color_filter = UUBAND ;
		    mag_offset = UU0 ;
		}
		for (i = 0 ;i < boxlist[active_box].nstar ;i++) {
		    sp = boxlist[active_box].sp[i] ;
		    particle_color[i] = (int)(color_slope * 
			    (-2.5 * log10(star_lum(sp->mass,sp->tform))+4.80 -
			    mag_offset) + color_offset +0.5) ;
		}
	    }
	    else if(strcmp(type,"tform") == 0){
		for (i = 0 ;i < boxlist[active_box].nstar ;i++) {
		    sp = boxlist[active_box].sp[i] ;
		    particle_color[i] = (int)(color_slope * 
			    (sp->tform) + color_offset +0.5) ;
		}
	    }
	    else if(strcmp(type,"logtform") == 0 ||
		    strcmp(type,"ltform") == 0){
		for (i = 0 ;i < boxlist[active_box].nstar ;i++) {
		    sp = boxlist[active_box].sp[i] ;
		    particle_color[i] = (int)(color_slope * 
			    log10(sp->tform) + color_offset +0.5) ;
		}
	    }
	    else if(strcmp(type,"age") == 0){
		for (i = 0 ;i < boxlist[active_box].nstar ;i++) {
		    sp = boxlist[active_box].sp[i] ;
		    particle_color[i] = (int)(color_slope * 
			    (header.time-(sp->tform)) + color_offset +0.5) ;
		}
	    }
	    else if(strcmp(type,"logage") == 0 ||
		    strcmp(type,"logage") == 0){
		for (i = 0 ;i < boxlist[active_box].nstar ;i++) {
		    sp = boxlist[active_box].sp[i] ;
		    particle_color[i] = (int)(color_slope * 
			    log10(header.time-(sp->tform)) + color_offset +0.5);
		}
	    }
	    else {
		printf("<%s is an unknown option, %s>\n",type,title) ;
		return ;
	    }
	    for (i = 0 ;i < boxlist[active_box].nstar ;i++) {
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
	    plot_sub(job) ;
	}
	else {
	    printf("<inconsistent input for a star plot, %s>\n",title) ;
	}
    }
    else {
	printf("<sorry, no boxes are loaded, %s>\n",title) ;
    }
}
