/* $Header$
 * $Log$
 * Revision 1.2  1997/02/20 02:37:13  trq
 * Make reading and writing marks work with a loaded subbox.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:35  trq
 * Import to CVS
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
	i = 0;
	for(;;)
	  {
	    if(fscanf(infile, "%d", &index) != 1) break;
	    if(index <= 0)
	      {
		printf("<Sorry %s, index is less than 1>\n",title);
		break;
	      }
	    --index;
	    if(box0_pi) {	/* partial box, assume sorted marked
				   particles */
	        while(box0_pi[i] < index) i++;
		if(box0_pi[i] != index) {
		  continue;
		}
	    }
	    else
	        i = index;
	    if(index < nsph && i < header.nsph)
	      {
		mark_gas[i]++;
	      }
	    else if (index < nsph + ndark && i < header.nsph + header.ndark) 
	      {
		mark_dark[i-header.nsph]++;
	      }
	    else if(index < nbodies && i < header.nbodies)
	      {
		mark_star[i-header.nsph-header.ndark]++;
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
