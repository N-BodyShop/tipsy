/*
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:31  trq
 * Initial revision
 *
 * Revision 2.4  1994/12/22  01:15:11  trq
 * Reset after magnitude plot.
 *
 * Revision 2.3  1994/04/20  08:46:24  trq
 * Added title variable.
 *
 * Revision 2.1  94/02/16  10:05:36  trq
 * Added array_plot to conditional.
 * 
 */
#include "defs.h"
view_all(job)
    char job[MAXCOMM] ;
{

    if (boxes_loaded[0]) {
	if(gas_plot || radial_plot || array_plot || star_plot ||
		potential_plot || magnitude_plot) {
	  	color_type = ALL;
		array_plot = NO;
		gas_plot = NO ;
		radial_plot = NO ;
		star_plot = NO ;
		potential_plot = NO ;
		magnitude_plot = NO ;
		load_color_table() ;
	}
	project() ;
	all_color();
	plot_sub(job) ;
    }
    else {
	printf("<sorry, no boxes are loaded, %s>\n",title) ;
    }
}
