/*
 * $Header$
 * $Log$
 * Revision 1.3  1997/02/20 02:34:13  trq
 * Fixed bug in previous modification.
 *
 * Revision 1.2  1996/12/19  18:03:42  trq
 * Fixed handling of arrays and vectors when a sub-box is loaded.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:38  trq
 * Import to CVS
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
  int i;
  int count;
  int nbodies;

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
	    for(i = 0, count = 0; i < nbodies; i++){
				/* skip line if a partial box was
				   loaded and this particle is not in
				   it */
		if(box0_pi && box0_pi[count] != i) {
		  fscanf(infile, "%*f");
		  continue;
		}
		else {
		  if(count >= header.nbodies) {
			printf("<Sorry %s, file format is wrong>\n",title);
			array_size = 0 ;
			free(array) ;
			break;
		  }
		}
		if(fscanf(infile, "%f", &(vector[count].v[j])) == EOF){
		    printf("<Sorry %s, file format is wrong>\n",title);
		    vector_size = 0 ;
		    free(vector);
		    break;
		}
		count++;
	    }
	}
    }
    else {
	input_error(command) ;
    }
}
