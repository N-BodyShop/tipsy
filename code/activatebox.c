/*
 * $Header$
 * $Log$
 * Revision 1.3  2001/07/11 19:45:42  nsk
 *       Fixed bugs with array sizes for meanmwt, cooling, and starformation.
 *       Used to be only for active box now for all of box zero.
 *
 * Revision 1.2  1996/04/11  21:27:50  trq
 * view_star.c: fixed bug in abox vs. density calculation.
 * activatebox.c: hsmdivv is independent of abox.
 * divv.c, smooth.c, smooth.h, view_gas.c: Use tree to calculate hsmdivv[].
 *
 * Revision 1.1.1.1  1995/01/10  22:57:23  trq
 * Import to CVS
 *
 * Revision 2.4  1994/09/22  16:21:07  trq
 * Removed hneutral_loaded.  Gone to unload_all()
 *
 * Revision 2.3  1994/04/20  08:45:12  trq
 * Added title variable.
 *
 * Revision 2.1  94/02/22  16:27:03  trq
 * Added hneutral_loaded and meanwt_loaded.
 * 
 */
#include "defs.h"
#include "fdefs.h"

void
activatebox(job)
    char *job ;
{
    char command[MAXCOMM] ;
    int i ;

    if (boxes_loaded[0]) {
	if (sscanf(job,"%s %d",command,&i) == 2) {
	    if(active_box >= 0 && active_box <= MAXBOX
		    && boxes_loaded[i] ) {
		active_box = i ;
		current_project = NO;
		current_color = NO;
	    }
	    else {
		printf("<box %d not loaded, %s>\n",i,title) ;
	    }
	}
	else {
	    input_error(command) ;
	}
    }
    else {
	printf("<sorry, no boxes are loaded, %s>\n",title) ;
    }
}
