#include "defs.h"

void
xverbose_sub(job)
    char *job;
{
  char command[MAXCOMM] ;
  char reply[15] ;
  
  if (sscanf(job,"%s %s",command,reply) == 2) {
    if        ( strcmp(reply,"yes") == 0 
		|| strcmp(reply,"y") == 0 
		|| strcmp(reply,"on") == 0 ) {
        if (xverbose == OFF) xverbose = ON;
    } else if ( strcmp(reply,"no") == 0 
		|| strcmp(reply,"n") == 0 
		|| strcmp(reply,"off") == 0 ) {
        if (xverbose == ON) xverbose = OFF;
    } else {
      printf("<yes or no, %s>",title) ;
    }
  } else {
    input_error(command) ;
  }
}
