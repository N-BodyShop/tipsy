#include "defs.h"
xplot_all(job)
    char job[MAXCOMM] ;
{
    if (binary_loaded) {
	if(active_box == 0 && !boxes_loaded[0]) {
	    loadall() ;
	}
	clear_rot() ;
	rotate(LEFT,90.0) ;
	rotate(CLOCKWISE,90.0) ;
	view_all(job) ;
    }
    else {
	printf("<sorry, no binary file is loaded, %s>\n",title) ;
    }
}
