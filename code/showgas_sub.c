#include "defs.h"
showgas_sub(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    char reply[15] ;

    if (sscanf(job,"%s %s",command,reply) == 2 ) {
	if ( strcmp(reply,"yes") == 0 || strcmp(reply,"y") == 0 ) {
	    showgas = ON ;
	}
	else if ( strcmp(reply,"no") == 0 || strcmp(reply,"n") == 0 ) {
	    showgas = OFF ;
	}
	else {
	    printf("<must be yes or no, %s>",title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
