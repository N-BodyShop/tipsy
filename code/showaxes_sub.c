#include "defs.h"
showaxes_sub(job)
    char *job;
{
    char command[MAXCOMM] ;
    char reply[15] ;

    if (sscanf(job,"%s %s",command,reply) == 2) {
	if ( strcmp(reply,"yes") == 0 || strcmp(reply,"y") == 0 ) {
	  if(showaxes == OFF) 
	    {
	      current_project = NO;
	      showaxes = ON ;
	    }
	}
	else if ( strcmp(reply,"no") == 0 || strcmp(reply,"n") == 0 ) {
	  if(showaxes == ON) 
	    {
	      current_project = NO;
	      showaxes = OFF ;
	    }
	}
	else {
	    printf("<yes or no, %s>",title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
