#include "defs.h"
#include "fdefs.h"

void
showvec_sub(job)
    char *job;
{
    char command[MAXCOMM] ;
    char reply[15] ;

    if (sscanf(job,"%s %s",command,reply) == 2) {
	if ( strcmp(reply,"yes") == 0 || strcmp(reply,"y") == 0 ) {
	  if(showvel == OFF) 
	    {
	      if(showvec == OFF) 
		{
		  current_project = NO;
		  showvec = ON ;
		}
	      else
		{
		  printf("<showvec is already on, %s>\n",title) ;
		}
	    }
	  else
	    {
              printf("<sorry, showvel is already on, %s>\n",title) ;
	    }
	}
	else if ( strcmp(reply,"no") == 0 || strcmp(reply,"n") == 0 ) {
	  if(showvec == ON) 
	    {
	      showvec = OFF ;
	    }
	  else
	    {
	    printf("<showvec is already off, %s>\n",title) ;
	    }
	}
	else {
	    printf("<yes or no, %s>\n",title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
