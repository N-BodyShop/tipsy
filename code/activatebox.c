/*
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:22  trq
 * Initial revision
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
		divv_loaded = NO ;
		cooling_loaded = NO ;
		meanmwt_loaded = NO ;
		starform_loaded = NO ;
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
