/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:34  trq
 * Initial revision
 *
 * Revision 2.1  1994/12/29  02:27:52  trq
 * close the file when done.
 *
 * Revision 1.7  1994/04/20  08:46:05  trq
 * Added title variable.
 *
 * Revision 1.5  93/11/13  11:31:04  trq
 * Checked for negative indices.
 * 
 * Revision 1.4  93/10/15  10:33:35  trq
 * Added check for binary_loaded.
 * 
 */

#include <stdlib.h>
#include "defs.h"
#include "fdefs.h"

void
readmark(job)
     char *job;
     
{
  char command[MAXCOMM] ;
  char filename[MAXCOMM] ;
  FILE *infile;
  int i, index;
  int count;
  int nbodies, nsph, nstar, ndark;

  if(binary_loaded != LOADED)
    {
      printf("<Sorry %s, a binary file is not loaded>\n",title);
      return;
    }
      
  if (sscanf(job,"%s %s",command,filename) == 2) {
    infile = fopen(filename, "r");
    if(infile == NULL)
      {
        printf("<Sorry %s, file does not exist>\n",title);
	return;
      }
	count=fscanf(infile, "%d%*[, \t\n]%d%*[, \t\n]%d"
		    ,&nbodies, &nsph, &nstar) ;
	if ( (count == EOF) || (count==0) ){
	    printf("<Sorry %s, file format is wrong>\n",title);
	    return;
	}
	ndark = nbodies - (nsph + nstar);
	if(nsph > header.nsph)
	  {
	    fprintf(stderr, "Increasing nsph to %d\n", nsph);
	    mark_gas = (short *)realloc(mark_gas,
					sizeof(*mark_gas)*nsph);
	    for(i = header.nsph; i < nsph; i++)
	      {
		mark_gas[i] = 0;
	      }
	  }
	if(ndark > header.ndark)
	  {
	    fprintf(stderr, "Increasing ndark to %d\n", ndark);
	    mark_dark = (short *)realloc(mark_dark,
					sizeof(*mark_dark)*ndark);
	    for(i = header.ndark; i < ndark; i++)
	      {
		mark_dark[i] = 0;
	      }
	  }
	if(nstar > header.nstar)
	  {
	    fprintf(stderr, "Increasing nstar to %d\n", nstar);
	    mark_star = (short *)realloc(mark_star,
					sizeof(*mark_star)*nstar);
	    for(i = header.nstar; i < nstar; i++)
	      {
		mark_star[i] = 0;
	      }
	  }
	for(;;)
	  {
	    if(fscanf(infile, "%d", &index) != 1) break;
	    if(index <= 0)
	      {
		printf("<Sorry %s, index is less than 1>\n",title);
		break;
	      }
	    else if(index <= nsph)
	      {
		mark_gas[index-1]++;
	      }
	    else if (index <= nsph + ndark) 
	      {
		mark_dark[index-1-nsph]++;
	      }
	    else if(index <= nbodies)
	      {
		mark_star[index-1-nsph-ndark]++;
	      }
	    else
	      {
		printf("<Sorry %s, index is too large>\n",title);
		break;
	      }
	  }
	fclose(infile);
      }
    else {
	input_error(command) ;
    }
}
