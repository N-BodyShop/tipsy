/*
 * $Header$
 * $Log$
 * Revision 1.2  1996/12/19 18:03:37  trq
 * Fixed handling of arrays and vectors when a sub-box is loaded.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:36  trq
 * Import to CVS
 *
 * Revision 2.2  94/03/24  11:25:29  nsk
 * Allow for just the total number of particles in the header line.
 * 
 * Revision 2.1  94/03/24  11:15:45  trq
 * Neal: added title to error messages.
 * 
 * Revision 1.1  94/02/16  14:13:24  trq
 * Initial revision
 * 
 */
#include "defs.h"
#include "fdefs.h"
#include <malloc.h>

void
readarray(job)
     char *job;
     
{
  char command[MAXCOMM] ;
  char filename[MAXCOMM] ;
  FILE *infile;
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
    if(array != NULL) free(array);
    array = (Real *)malloc(nbodies*sizeof(*array));
    array_size = nbodies;
    if(array == NULL) 
      {
	printf("<Sorry %s, no room for array>\n",title);
	return;
      }
	for(i = 0, count = 0; i < nbodies; i++)
	  {
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
	    if(fscanf(infile, "%f", &array[count]) == EOF){
		printf("<Sorry %s, file format is wrong>\n",title);
		array_size = 0 ;
		free(array) ;
		break;
	    }
	    count++;
	  }
      }
    else {
	input_error(command) ;
    }
}
