#include "defs.h"
showvel_sub(job)
    char *job;
{
    char command[MAXCOMM] ;
    char reply[15] ;

    if (sscanf(job,"%s %s",command,reply) == 2) {
	if ( strcmp(reply,"yes") == 0 || strcmp(reply,"y") == 0 ) {
	  if(showvec == OFF) 
	    {
	      if(showvel == OFF) 
		{
		  current_project = NO;
		  showvel = ON ;
		}
	      else
		{
		  printf("<showvel is already on, %s>\n",title) ;
		}
	    }
	  else
	    {
              printf("<sorry, showvec is already on, %s>\n",title) ;
	    }
	}
	else if ( strcmp(reply,"no") == 0 || strcmp(reply,"n") == 0 ) {
	  if(showvel == ON) 
	    {
	      showvel = OFF ;
	    }
	  else
	    {
	    printf("<showvel is already off, %s>\n",title) ;
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
