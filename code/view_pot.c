/*
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:33  trq
 * Initial revision
 *
 * Revision 2.3  94/04/20  08:57:01  trq
 * Added title variable
 * 
 * Revision 2.1  94/02/16  10:17:22  trq
 * Added array_plot
 * 
 */
#include "defs.h"
#include "fdefs.h"

void
view_pot(job)
    char *job ;
{
    char command[MAXCOMM] ;
    char type[MAXCOMM] ;
    char clip_type[MAXCOMM] ;
    double low ;
    double high ;
    int i ;

    if (boxes_loaded[0]) {
	if((i = sscanf(job,"%s %s %lf %lf %s",command,type,&low,&high,
		clip_type)) >= 4) {
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
	    if(!potential_plot) {
		potential_plot = YES ;
		magnitude_plot = NO ;
		array_plot = NO;
		radial_plot = NO ;
		gas_plot = NO ;
		star_plot = NO ;
		load_color_table() ;
	    }
	    color_slope = 124.0 / (high - low) ;
	    color_offset = 2.0 - color_slope * low ;
	    if ( strcmp(type,"gas") == 0 ) {
		color_type = GAS ;
	    }
	    else if ( strcmp(type,"dark") == 0 ) {
		color_type = DARK ;
	    }
	    else if ( strcmp(type,"star") == 0 ) {
		color_type = STAR ;
	    }
	    else if ( strcmp(type,"all") == 0 ) {
		color_type = ALL ;
	    }
	    else {
		printf("<%s is an unknown particle type, %s>\n",type,title) ;
		return ;
	    }
	    project() ;
	    potential_color() ;
	    plot_sub(job) ;
	}
	else {
	    input_error(command) ;
	}
    }
    else {
	printf("<sorry, no boxes are loaded, %s>\n",title) ;
    }
}
