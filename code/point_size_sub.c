#include "defs.h"
point_size_sub(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    char type[MAXCOMM] ;
    double size ;

    if(sscanf(job,"%s %s %lf",command,type,&size) == 3) {
	if (strcmp(type,"gas") == 0 ) {
	    point_size_gas = size ;
	}
	else if (strcmp(type,"dark") == 0 ) {
	    point_size_dark = size ;
	}
	else if (strcmp(type,"star") == 0 ) {
	    point_size_star = size ;
	}
	else if (strcmp(type,"mark") == 0 ) {
	    point_size_mark = size ;
	}
	else {
	    printf("<sorry, %s is not a proper data type, %s>\n",type,title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
