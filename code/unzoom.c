/*
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:31  trq
 * Initial revision
 *
 * Revision 2.3  94/04/20  08:46:14  trq
 * Added title variable.
 * 
 * Revision 2.1  94/02/24  11:18:34  trq
 * Added zoom_factor adjustment.
 * 
 */
#include "defs.h"
#include "fdefs.h"

void
unzoom(job)
    char *job ;
{
    if(current_project && current_color) {
	if(dv1_x != -INTMAX || dv1_y != -INTMAX || dv2_x != INTMAX ||
		dv2_y != INTMAX) {
	    dv1_x = -INTMAX ;
	    dv1_y = -INTMAX ;
	    dv2_x = INTMAX ;
	    dv2_y = INTMAX ;
	    view_size = INTMAX ;
	    zoom_factor = 1.0;
	    reset_zoom_scroll();
	    pixel_size = ((double)(dv2_y - dv1_y)) / window_pixels ;
	    plot_sub(job) ;
	}
	else {
	    printf("<not currently zoomed, %s>\n",title) ;
	}
    }
    else {
	printf("<plot does not represent the present internal state") ;
    }
}
