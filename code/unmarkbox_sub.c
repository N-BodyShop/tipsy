#include "defs.h"
unmarkbox_sub(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    int i ;

    if (boxes_loaded[0]) {
	if (sscanf(job,"%s %d",command,&i) == 2) {
	    if(mark_box[i] ) {
		unmarkbox(i) ;
	    }
	    else {
		printf("<box %d not marked, %s>\n",i,title) ;
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
