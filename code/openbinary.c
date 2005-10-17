#define _LARGEFILE_SOURCE
#define _FILE_OFFSET_BITS 64
#include "defs.h"
void
openbinary(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
	
    if (binaryopen){
	printf("<sorry, binary file %s already open, %s>\n",
		binaryfile.name,title) ;
    }
    else {
	if (sscanf(job,"%s %s",command,binaryfile.name) == 2) {
	    if( (binaryfile.ptr = fopen(binaryfile.name,"r")) 
		       == NULL) {
		printf("<sorry, binary file %s does not exist, %s>\n",
			binaryfile.name,title) ;
	    }
	    else {
		binaryopen = OPEN ;
		cool_loaded = NO ;
		visc_loaded = NO ;
		form_loaded = NO ;
		eps_loaded = NO ;
		epsgas_loaded = NO ;
		lum_loaded = NO ;
		array_size = 0;
	    }
	}
	else {
	    input_error(command) ;
	}
    }
}
