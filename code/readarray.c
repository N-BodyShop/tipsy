/*
 * $Header$
 * $Log$
 * Revision 1.6  2003/06/13 17:37:37  trq
 * Replaced "include <malloc.h>" with "include <stdlib.h>".  This will allow
 * compilation on MAC OSX.  Also replaced "values.h" with "float.h".
 *
 * Revision 1.5  2002/12/23 19:44:05  trq
 * Added "readbinarray" command to read binary arrays.  Only native formats
 * are supported.
 *
 * Revision 1.4  2002/08/15 22:33:14  trq
 * Fixed bugs in freeing arrays on bad input.
 *
 * Revision 1.3  2001/06/21 18:56:55  trq
 * Added optional argument to raise values to a power as they are read in.
 *
 * Revision 1.2  1996/12/19  18:03:37  trq
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
#include <math.h>
#include <stdlib.h>

void
readarray(job)
     char *job;
     
{
  char command[MAXCOMM] ;
  char filename[MAXCOMM] ;
  double power;
  Real tmp;
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
		    array = NULL;
		    break;
	      }
	    }
	    if(fscanf(infile, "%f", &array[count]) == EOF){
		printf("<Sorry %s, file format is wrong>\n",title);
		array_size = 0 ;
		free(array) ;
		array = NULL;
		break;
	    }
	    count++;
	  }
      }
  else if (sscanf(job,"%s %s %lf",command,filename,&power) == 3) {
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
    if(array == NULL) {
      array = (Real *)malloc(nbodies*sizeof(*array));
      array_size = nbodies;
      if(array == NULL) 
	{
	  printf("<Sorry %s, no room for array>\n",title);
	  return;
	}
      for(i = 0, count = 0; i < nbodies; i++) array[i]=1;
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
		    array = NULL;
		    break;
	      }
	    }
	    if(fscanf(infile, "%f", &tmp) == EOF){
		printf("<Sorry %s, file format is wrong>\n",title);
		array_size = 0 ;
		free(array) ;
		array = NULL;
		break;
	    }
	    array[count++] *= pow(tmp,power);
	  }
      }
    else {
	input_error(command) ;
    }
}

void
readbinarray(job)
     char *job;
     
{
  char command[MAXCOMM] ;
  char filename[MAXCOMM] ;
  char type[MAXCOMM] ;
  double power;
  Real tmp;
  FILE *infile;
  int i;
  int count;
  int nbodies;
  int btype;
  int check;

  if (sscanf(job,"%s %s %s",command,filename, type) == 3) {
      if(!strcmp(type, "int")) {
	  btype = 0;
      }
      else if(!strcmp(type, "float")){
	  btype = 1;
      }
      else if(!strcmp(type, "double")){
	  btype = 2;
      }
      else {
	  input_error(command) ;
      }
      
	  
    infile = fopen(filename, "r");
    if(infile == NULL)
      {
        printf("<Sorry %s, file does not exist>\n",title);
	return;
      }
	count=fread(&nbodies, sizeof(int), 1, infile) ;
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
		switch (btype) {
		int idummy;
		float fdummy;
		double ddummy;
		case 0:
		    fread(&idummy, sizeof(int), 1, infile) ;
		    break;
		case 1:
		    fread(&fdummy, sizeof(float), 1, infile) ;
		    break;
		case 2:
		    fread(&ddummy, sizeof(double), 1, infile) ;
		}
		
	      continue;
	    }
	    else {
	      if(count >= header.nbodies) {
		    printf("<Sorry %s, file format is wrong>\n",title);
		    array_size = 0 ;
		    free(array) ;
		    array = NULL;
		    break;
	      }
	    }
		switch (btype) {
		int idummy;
		float fdummy;
		double ddummy;
		case 0:
		    check = fread(&idummy, sizeof(int), 1, infile) ;
		    array[count] = idummy;
		    break;
		case 1:
		    check = fread(&fdummy, sizeof(float), 1, infile) ;
		    array[count] = fdummy;
		    break;
		case 2:
		    check = fread(&ddummy, sizeof(double), 1, infile) ;
		    array[count] = ddummy;
		}
	    if(check == EOF) {
		printf("<Sorry %s, file format is wrong>\n",title);
		array_size = 0 ;
		free(array) ;
		array = NULL;
		break;
	    }
	    count++;
	  }
      }
    else {
	input_error(command) ;
    }
}
