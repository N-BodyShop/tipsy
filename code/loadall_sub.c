#include "defs.h"
loadall_sub(job)
    char job[MAXCOMM] ;
{
    double xmax[MAXDIM] ;

    if (binary_loaded) {
	loadall() ;
    }
    else {
	printf("<sorry, no binary file is loaded, %s>\n",title) ;
    }
}
