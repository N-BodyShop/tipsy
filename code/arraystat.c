/* $Header$
 * $Log$
 * Revision 1.4  2003/06/13 17:37:37  trq
 * Replaced "include <malloc.h>" with "include <stdlib.h>".  This will allow
 * compilation on MAC OSX.  Also replaced "values.h" with "float.h".
 *
 * Revision 1.3  1997/09/19 13:41:26  nsk
 * fixed memeory troubles.
 *
 * Revision 1.2  1995/03/24  18:49:02  trq
 * Added "revrain" colormap.
 *
 * divv.c: included malloc.h.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:36  trq
 * Import to CVS
 *
 * Revision 1.3  94/04/20  08:44:54  trq
 * Added title variable.
 * 
 * Revision 1.1  94/02/16  13:51:25  trq
 * Initial revision
 * 
 */
#include "defs.h"
#include <stdlib.h>

void input_error();
static Real *array_select();

void
arraystat(job)
    char *job;
{
    char command[MAXCOMM] ;
    int box;
    char type[MAXCOMM] ;
    double mean = 0.0;
    double median;
    double uquartile;
    int i, offset;
    int totpart;
    int aoffset;
    Real **medtmp = NULL;

    if(sscanf(job,"%s %d %s",command,&box,type) == 3) {
	if (boxes_loaded[box])
	  {
	    if(array_size != 0)
	      {
		if (strcmp(type,"gas") == 0 ) {
		  medtmp = (Real **)malloc(boxlist[box].ngas*sizeof(*medtmp));
		  for (i = 0 ;i < boxlist[box].ngas ;i++)
		    {
		      aoffset = boxlist[box].gp[i]-gas_particles;
		      medtmp[i] = &array[aoffset];
		      mean += array[aoffset];
		    }
		  mean /= boxlist[box].ngas;
		  uquartile = *(array_select(medtmp, boxlist[box].ngas,
				    3*boxlist[box].ngas/4));
		  median = *(array_select(medtmp, boxlist[box].ngas,
				    boxlist[box].ngas/2));
		  free(medtmp);
		  printf("<array mean, median and upper quartile of the gas in box %d>\n", box);
		}
		else if (strcmp(type,"dark") == 0 ) {
		  offset = header.nsph;
		  medtmp = (Real **)malloc(boxlist[box].ndark*sizeof(*medtmp));
		  for (i = 0 ;i < boxlist[box].ndark ;i++)
		    {
		      aoffset = boxlist[box].dp[i]-dark_particles+offset;
		      medtmp[i] = &array[aoffset];
		      mean += array[aoffset];
		    }
		  mean /= boxlist[box].ndark;
		  uquartile = *(array_select(medtmp, boxlist[box].ndark,
				    3*boxlist[box].ndark/4));
		  median = *(array_select(medtmp, boxlist[box].ndark,
				    boxlist[box].ndark/2));
		  free(medtmp);
		  printf("<array mean, median and upper quartile of the dark matter in box %d>\n",
			 box);
		}
		else if (strcmp(type,"star") == 0 ) {
		  offset = header.nsph + header.ndark;
		  medtmp = (Real **)malloc(boxlist[box].nstar*sizeof(*medtmp));
		  for (i = 0 ;i < boxlist[box].nstar ;i++)
		    {
		      aoffset = boxlist[box].sp[i]-star_particles+offset;
		      medtmp[i] = &array[aoffset];
		      mean += array[aoffset];
		    }
		  mean /= boxlist[box].nstar;
		  uquartile = *(array_select(medtmp, boxlist[box].nstar,
				    3*boxlist[box].nstar/4));
		  median = *(array_select(medtmp, boxlist[box].nstar,
				    boxlist[box].nstar/2));
		  free(medtmp);
		  printf("<array mean, median and upper quartile of the stars in box %d>\n",
			 box);
		}
		else if (strcmp(type,"baryon") == 0 ) {
		  medtmp = (Real **)malloc((boxlist[box].ngas+boxlist[box].nstar)
				  *sizeof(*medtmp));
		  for (i = 0 ;i < boxlist[box].ngas ;i++)
		    {
		      aoffset = boxlist[box].gp[i]-gas_particles;
		      medtmp[i] = &array[aoffset];
		      mean += array[aoffset];
		    }
		  offset = header.nsph + header.ndark;
		  for (i = 0 ;i < boxlist[box].nstar ;i++)
		    {
		      aoffset = boxlist[box].sp[i]-star_particles+offset;
		      medtmp[i] = &array[aoffset];
		      mean += array[aoffset];
		    }
		  totpart = boxlist[box].nstar + boxlist[box].ngas;
		  mean /= totpart;
		  uquartile = *(array_select(medtmp, totpart, 3*totpart/4));
		  median = *(array_select(medtmp, totpart, totpart/2));
		  free(medtmp);
		  printf("<array mean, median and upper quartile of the baryons in box %d>\n",
			 box);
		}
		else if (strcmp(type,"all") == 0 ) {
		  medtmp = (Real **)malloc((boxlist[box].ngas+boxlist[box].nstar
				   +boxlist[box].ndark)*sizeof(*medtmp));
		  for (i = 0 ;i < boxlist[box].ngas ;i++)
		    {
		      aoffset = boxlist[box].gp[i]-gas_particles;
		      medtmp[i] = &array[aoffset];
		      mean += array[aoffset];
		    }
		  offset = header.nsph;
		  for (i = 0 ;i < boxlist[box].ndark ;i++)
		    {
		      aoffset = boxlist[box].dp[i]-dark_particles+offset;
		      medtmp[i] = &array[aoffset];
		      mean += array[aoffset];
		    }
		  offset = header.nsph + header.ndark;
		  for (i = 0 ;i < boxlist[box].nstar ;i++)
		    {
		      aoffset = boxlist[box].sp[i]-star_particles+offset;
		      medtmp[i] = &array[aoffset];
		      mean += array[aoffset];
		    }
		  totpart = boxlist[box].nstar + boxlist[box].ngas
		    + boxlist[box].ndark;
		  mean /= totpart;
		  uquartile = *(array_select(medtmp, totpart, 3*totpart/4));
		  median = *(array_select(medtmp, totpart, totpart/2));
		  free(medtmp);
		  printf("<array mean, median and upper quartile of box %d>\n",
			 box);
		}
		else {
		    printf("<sorry, %s is not a proper data type, %s>\n",type,title) ;
		}
		printf("%g %g %g\n", mean, median, uquartile);
	      }
	    else
	      {
		printf("<sorry, no array loaded, %s>\n",title);
	      }
	}
	else {
	    printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
	}
	  
    }
    else {
	input_error(command) ;
    }
}

static
void swap(p, q)
     Real **p;
     Real **q;
{
  Real *tmp = *p;
  *p = *q;
  *q = tmp;
}

static Real *array_select(a, n, k)
     Real **a;
     int n, k;
{
  int l = 0;
  int u = n-1;
  while(l < u)
    {
      int m;
      int i;
      
      swap(&a[l], &a[(l+u)/2]);
      m = l;

      for(i = l+1; i <= u; i++)
	{
	  if(*a[i] < *a[l])
	    {
	      m++;
	      swap(&a[m], &a[i]);
	    }
	}
      swap(&a[l], &a[m]);
      if(k <= m) u = m;
      if(k >= m) l = m+1;
    }
  return a[k];
}
