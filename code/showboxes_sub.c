#include "defs.h"
showboxes_sub(job)
    char *job;
{
    char command[MAXCOMM] ;
    char reply[15] ;

    if (sscanf(job,"%s %s",command,reply) == 2) {
	if ( strcmp(reply,"yes") == 0 || strcmp(reply,"y") == 0 ) {
	  if(showboxes == OFF) 
	    {
	      current_project = NO;
	      showboxes = ON ;
	    }
	}
	else if ( strcmp(reply,"no") == 0 || strcmp(reply,"n") == 0 ) {
	  if(showboxes == ON) 
	    {
	      current_project = NO;
	      showboxes = OFF ;
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
