#include "defs.h"
void
loadbinary_sub(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    double time ;
    int i ;

    if (!binaryopen){
	printf("<sorry, binary file not open, %s>\n",title) ;
    }
    else {
	if (sscanf(job,"%s %lf",command,&time) == 2) {
	    loadbinary(binaryfile.ptr,time);
	    boxes_loaded[0] = NO ;
	    unload_all() ;
	    active_box = 0 ;
	    binary_loaded = LOADED ;
	    current_project = NO ;
	    current_color = NO ;
	    redshift_loaded = NO ;
	    divv_loaded = NO ;
	    cooling_loaded = NO ;
	    starform_loaded = NO ;
	}
	else {
	    input_error(command) ;
	}
    }
}
