#include "defs.h"
void
openascii(job) 
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;

    if (asciiopen){
	printf("<sorry, ascii file %s already open, %s>\n",
		bodfile.name,title) ;
    }
    else {
	if (sscanf(job,"%s %s",command,bodfile.name) == 2) { 
	    if( (bodfile.ptr = fopen(bodfile.name,"r")) == NULL) {
		printf("<sorry, ascii file %s does not exist, %s>\n",
			bodfile.name,title) ;
	    }
	    else {
		asciiopen = OPEN ;
	    }
	}
	else {
	    input_error(command) ;
	}
    }
}
