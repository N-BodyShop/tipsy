#include "defs.h"
rotate_sub(job)
    char *job;
{
    char command[MAXCOMM] ;
    char reply[15] ;
    int direction ;
    double angle ;

    if (sscanf(job,"%s %s %lf",command,reply,&angle) == 3) {
	if(strcmp(reply,"left") == 0 || strcmp(reply,"l") == 0) {
	    direction = LEFT ;
	}
	else if(strcmp(reply,"right") == 0 || strcmp(reply,"r") == 0) {
	    direction = RIGHT ;
	}
	else if(strcmp(reply,"up") == 0 || strcmp(reply,"u") == 0) {
	    direction = UP ;
	}
	else if(strcmp(reply,"down") == 0 || strcmp(reply,"d") == 0) {
	    direction = DOWN ;
	}
	else if(strcmp(reply,"clockwise") == 0 || strcmp(reply,"c") == 0) {
	    direction = CLOCKWISE ;
	}
	else {
	    printf("<direction must be left, right, up, down") ;
	    printf("or clockwise, %s>\n",title) ;
	    return ;
	}
	rotate(direction,angle) ;
	current_project = NO ;
    }
    else {
	input_error(command) ;
    }
}
