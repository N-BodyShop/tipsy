#include "defs.h"
unload_box(job)
    char *job ;
{
    char command[MAXCOMM] ;
    int box ;

    if (boxes_loaded[0]) {
	if (sscanf(job,"%s %d",command,&box) == 2) {
	    if(box >= 0 && box <= MAXBOX) {
		boxes_loaded[box] = UNLOADED ;
		if(mark_box[box]){
		    unmarkbox(box) ;
		}
	    }
	    else {
		printf("<box %d not a box, %s>\n",box,title) ;
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
