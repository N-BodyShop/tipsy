/*
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:37  trq
 * Initial revision
 *
 * Revision 2.2  94/03/24  11:33:25  nsk
 * Allow only the total number of particles in the header line.
 * 
 * Revision 2.1  94/03/24  11:30:41  trq
 * New function.
 * 
 * 
 */
#include "defs.h"
#include "fdefs.h"
#include <malloc.h>

void
readvector(job)
     char *job;
     
{
  char command[MAXCOMM] ;
  char filename[MAXCOMM] ;
  FILE *infile;
  int j;
  int count;
  int nbodies;
  struct vec *v ;

  if (sscanf(job,"%s %s",command,filename) == 2) {
    infile = fopen(filename, "r");
    if(infile == NULL)
      {
        printf("<Sorry %s, file does not exist>\n",title);
	return;
      }
	count=fscanf(infile, "%d%*[, \t]%*d%*[, \t]%*d",&nbodies) ;
	if ( (count == EOF) || (count==0) ){
	    printf("<Sorry %s, file format is wrong>\n",title);
	    return;
	}
    if(vector != NULL) free(vector);
    vector = (struct vec *)malloc(nbodies*sizeof(*vector));
    vector_size = nbodies;
    if(vector == NULL) 
      {
	printf("<Sorry %s, no room for array>\n",title);
	return;
      }
	for(j = 0; j < MAXDIM; j++){
	    for(v=vector; v < vector + nbodies; v++){
		if(fscanf(infile, "%f", &(v->v[j])) == EOF){
		    printf("<Sorry %s, file format is wrong>\n",title);
		    vector_size = 0 ;
		    free(vector);
		    break;
		}
	    }
	}
    }
    else {
	input_error(command) ;
    }
}
