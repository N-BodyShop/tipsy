#include "defs.h"
void
readascii(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    char type[MAXCOMM] ;

    if (binaryopen){
	printf("<sorry, binary file %s already open, %s>\n",
		binaryfile.name,title) ;
    }
    else if (!asciiopen){
	printf("<sorry, ascii file not open, %s>\n",title) ;
    }
    else {
	if (sscanf(job,"%s %s",command,binaryfile.name) == 2) {
	    binaryfile.ptr = fopen(binaryfile.name,"w");
	    binaryopen = OPEN ;
	    ascii2binary(bodfile.ptr,binaryfile.ptr);
	    fclose(bodfile.ptr) ;
	    fclose(binaryfile.ptr) ;
	    binaryfile.ptr = fopen(binaryfile.name,"r");
	    asciiopen = CLOSED ;
	    cool_loaded = NO ;
	    visc_loaded = NO ;
	    form_loaded = NO ;
	    lum_loaded = NO ;
	    starform_loaded = NO ;
	    dudt_loaded = NO ;
	}
	else {
	    input_error(command) ;
	}
    }
}
