/*
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:35  trq
 * Initial revision
 *
 * Revision 1.3  94/04/20  08:46:35  trq
 * Added title variable.
 * 
 * Revision 1.1  94/02/16  14:09:25  trq
 * Initial revision
 * 
 */
#include "defs.h"
#include "fdefs.h"

void
yplot_array(job)
    char *job;
{
    if (binary_loaded) {
	if(active_box == 0 && !boxes_loaded[0]) {
	    loadall() ;
	}
	clear_rot() ;
	rotate(UP,90.0) ;
	rotate(LEFT,180.0) ;
	view_array(job) ;
    }
    else {
	printf("<sorry, no binary file is loaded, %s>\n",title) ;
    }
}
