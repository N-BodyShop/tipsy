/*
 * $Header$
 * $Log$
 * Revision 1.2  2000/01/12 22:55:21  nsk
 * Fixed bugs in cooling routines, added cooling damping,
 * fixed bugs in starformation,  regularized dependencies.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:36  trq
 * Import to CVS
 *
 * Revision 1.3  94/04/20  08:46:06  trq
 * Added title variable.
 * 
 * Revision 1.1  94/02/16  14:15:11  trq
 * Initial revision
 * 
 */
#include "defs.h"
#include "fdefs.h"

void
readoldbin(job)
    char *job;
{
    char command[MAXCOMM] ;

    if (!binaryopen){
	printf("<sorry, binary file not open, %s>\n",title) ;
    }
    else {
	if (sscanf(job,"%s %s",command,binaryfile.name) == 2) {
	    FILE *ofile = binaryfile.ptr;
	    binaryfile.ptr = fopen(binaryfile.name,"w");
	    binaryopen = OPEN ;
	    old2binary(ofile,binaryfile.ptr);
	    fclose(ofile) ;
	    fclose(binaryfile.ptr) ;
	    binaryfile.ptr = fopen(binaryfile.name,"r");
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
