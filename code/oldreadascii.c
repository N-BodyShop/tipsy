#include "defs.h"
void
oldreadascii(job)
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
	if (sscanf(job,"%s %s %s",command,binaryfile.name,type) == 3) {
	    if(strcmp(type,"nosph") == 0){
		file_type = NOSPH ;
	    }
	    else if(strcmp(type,"sph") == 0){
		file_type = SPH ;
	    }
	    else{
		printf("<sorry, %s is not a file type, %s>\n",type,title) ;
		return ;
	    }
	    binaryfile.ptr = fopen(binaryfile.name,"w");
	    binaryopen = OPEN ;
	    oldascii2binary(bodfile.ptr,binaryfile.ptr);
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
