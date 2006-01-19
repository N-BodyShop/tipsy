/*
 * $Header$
 * $Log$
 * Revision 1.7  2006/01/19 17:19:45  trq
 * Added "readpackedvector" command to get vectors ordered x1 y1 z1 ...
 *
 * Revision 1.6  2003/06/13 17:37:37  trq
 * Replaced "include <malloc.h>" with "include <stdlib.h>".  This will allow
 * compilation on MAC OSX.  Also replaced "values.h" with "float.h".
 *
 * Revision 1.5  2002/12/28 17:28:26  trq
 * Added "readbinvector" command for reading in binary vector file.
 *
 * Revision 1.4  2002/08/15 22:33:13  trq
 * Fixed bugs in freeing arrays on bad input.
 *
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
#include <stdlib.h>

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
			vector_size = 0 ;
			free(vector) ;
			vector = NULL;
			break;
		  }
		}
		if(fscanf(infile, "%f", &(vector[count].v[j])) == EOF){
		    printf("<Sorry %s, file format is wrong>\n",title);
		    vector_size = 0 ;
		    free(vector);
		    vector = NULL;
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

void
readpackedvector(job)
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
	for(i = 0, count = 0; i < nbodies; i++){
	    for(j = 0; j < MAXDIM; j++){
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
			vector_size = 0 ;
			free(vector) ;
			vector = NULL;
			break;
		  }
		}
		if(fscanf(infile, "%f", &(vector[count].v[j])) == EOF){
		    printf("<Sorry %s, file format is wrong>\n",title);
		    vector_size = 0 ;
		    free(vector);
		    vector = NULL;
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

void
readbinvector(job)
     char *job;
     
{
  char command[MAXCOMM] ;
  char filename[MAXCOMM] ;
  char type[MAXCOMM] ;
  FILE *infile;
  int j;
  int i;
  int k;
  int count;
  int nbodies;
  int btype;
  int check;

  if (sscanf(job,"%s %s %s",command,filename,type) == 3) {
      if(!strcmp(type, "packedfloat")){
	  btype = -1;
      }
      else if(!strcmp(type, "packeddouble")) {
	  btype = -2;
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
		    switch (btype) {
		    float fdummy[3];
		    double ddummy[3];
		    case -1:
			fread(fdummy, sizeof(float), 3, infile) ;
			break;
		    case -2:
			fread(fdummy, sizeof(double), 3, infile) ;
			break;
		    case 1:
			fread(fdummy, sizeof(float), 1, infile) ;
			break;
		    case 2:
			fread(ddummy, sizeof(double), 1, infile) ;
		    }

		  continue;
		}
		else {
		  if(count >= header.nbodies) {
			printf("<Sorry %s, file format is wrong>\n",title);
			vector_size = 0 ;
			free(vector) ;
			vector = NULL;
			break;
		  }
		}
		switch (btype) {
		float fdummy[3];
		double ddummy[3];
		case -2:
		    check = fread(ddummy, sizeof(double), 3, infile) ;
		    for(k = 0; k < 3; k++)
			vector[count].v[k] = ddummy[k];
		    break;
		case -1:
		    check = fread(fdummy, sizeof(float), 3, infile) ;
		    for(k = 0; k < 3; k++)
			vector[count].v[k] = fdummy[k];
		    break;
		case 1:
		    check = fread(&fdummy, sizeof(float), 1, infile) ;
		    vector[count].v[j] = fdummy[0];
		    break;
		case 2:
		    check = fread(&ddummy, sizeof(double), 1, infile) ;
		    vector[count].v[j] = ddummy[0];
		}
		if(check == EOF) {
		    printf("<Sorry %s, file format is wrong>\n",title);
		    vector_size = 0 ;
		    free(vector) ;
		    vector = NULL;
		    break;
		}
		count++;
	    }
	    if(btype < 0)
		break;
	}
    }
    else {
	input_error(command) ;
    }
}
