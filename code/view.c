/* 
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:34  trq
 * Initial revision
 *
 * Revision 1.3  94/04/20  08:46:22  trq
 * Added title variable.
 * 
 * Revision 1.1  94/02/16  14:19:24  trq
 * Initial revision
 * 
 */
#include "defs.h"
#include "fdefs.h"

void
view(job)
    char *job;
{

    if (boxes_loaded[0]) {
	project() ;
	if (radial_plot) {
	    radial_color() ;
	}
	plot_sub(job) ;
    }
    else {
	printf("<sorry, no boxes are loaded, %s>\n",title) ;
    }
}
