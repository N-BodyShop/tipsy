/* produces fits files from 3-column input data files

	cfg 7/89
*/
#include <malloc.h>
#include <stdlib.h>
#include "defs.h"
#include "fdefs.h"
#define BLANK 	-32768		/* short blank value */

void
fits(data,xsize,ysize,xmin,ymin,deltax,deltay,zmin,zmax,outfile)
	float **data ;
	double xmin,ymin,deltax,deltay,zmin,zmax ;
	int xsize,ysize ;
	char *outfile ;
{
	short *page = NULL;
	short zero ;
	int fillno ;
	double bscale,bzero ;
	int i,j ;
	void hdrwrite(),gerror() ;
	FILE *fpo ;
	int pmax ;
	/* Are we little or big endian?  From Harbison&Steele.  */
	union
	{
	    short s;
	    char c[sizeof (short)];
	} u;
	int bigendian;

	u.s = 1;
	bigendian = (u.c[sizeof(short) - 1] == 1);

	zero = 0 ;
	pmax = (int)(xsize * ysize) ;

	/* make page,initialize,  and fill it */
	if(page == NULL) 
	  {
	    page = (short *)malloc(sizeof(*page)*pmax);
	    if(page == NULL)
	      {
		printf("<sorry, no memory for page, %s>\n",title) ;
		return ;
	      }
	  }
	bzero = (zmax + zmin)/2.0 ;
	bscale = (zmax - zmin)/65535.0 ;
	for(i=0;i<xsize*ysize;i++) page[i] = BLANK ;

	for(i=0;i<xsize;i++) {
		for(j=0;j<ysize;j++) {
			page[i + j*xsize] = (short)max(floor(0.5 + 
				((double)data[i][j] - bzero)/bscale),BLANK) ;
		}
	}
	if(!bigendian)
	    swab((char *)page,(char *)page,sizeof(*page)*xsize*ysize) ;

	/* output page to fits file outfile */
	fprintf(stderr,"writing fits to file %s\n",outfile) ;
	fpo = fopen(outfile,"w") ;
	hdrwrite(fpo,zmax,zmin,bscale,bzero,xsize,ysize,xmin,ymin,deltax,deltay) ;

	/* write directly onto the file descripter in fpo */
	fflush(fpo) ;
	fwrite(page, sizeof(short), xsize*ysize, fpo) ;

	/* fill out file with zeros */
	fillno = 1440*((int)(1.0 + xsize*ysize/1440)) - xsize*ysize ;
	for(i=0;i<fillno;i++) fwrite(&zero,sizeof(short), 1, fpo) ;
	fclose(fpo) ;
	free(page);
	
}

void hdrwrite(fp,datamax,datamin,bscale,bzero,xsize,ysize,xmin,ymin,deltax,deltay)
FILE *fp ;
double datamax,datamin,bscale,bzero,xmin,ymin,deltax,deltay ;
int xsize,ysize ;
{

	char dum[50] ;
	void cardwrite() ;
	int i ;

	/* absolute minimal fits header */
	cardwrite(fp,"SIMPLE","T") ;
	cardwrite(fp,"BITPIX","16") ;
	cardwrite(fp,"NAXIS","2") ;
	sprintf(dum,"%d",xsize) ;
	cardwrite(fp,"NAXIS1",dum) ;
	sprintf(dum,"%d",ysize) ;
	cardwrite(fp,"NAXIS2",dum) ;
	sprintf(dum,"%lf",xmin) ;
	cardwrite(fp,"CRVAL1",dum) ;
	sprintf(dum,"%lf",ymin) ;
	cardwrite(fp,"CRVAL2",dum) ;
	sprintf(dum,"1.0") ;
	cardwrite(fp,"CRPIX1",dum) ;
	sprintf(dum,"1.0") ;
	cardwrite(fp,"CRPIX2",dum) ;
	sprintf(dum,"%lf",deltax) ;
	cardwrite(fp,"CDELT1",dum) ;
	sprintf(dum,"%lf",deltay) ;
	cardwrite(fp,"CDELT2",dum) ;
	cardwrite(fp,"BLANK","BLANK") ;
	sprintf(dum,"%lf",bscale) ;
	cardwrite(fp,"BSCALE",dum) ;
	sprintf(dum,"%lf",bzero) ;
	cardwrite(fp,"BZERO",dum) ;
	sprintf(dum,"%lf",datamax) ;
	cardwrite(fp,"DATAMAX",dum) ;
	sprintf(dum,"%lf",datamin) ;
	cardwrite(fp,"DATAMIN",dum) ;
	cardwrite(fp,"END","") ;

	/* fill out header block with blanks */
	for(i=0;i<19;i++) cardwrite(fp,"DUMMY",dum) ;

}

void cardwrite(fp,name,val)
FILE *fp ;
char name[50],val[50] ;
{
	int i;

	/* if DUMMY card, emit blank card */
	if(strcmp(name,"DUMMY") == 0) {
		for(i=0;i<80;i++) fprintf(fp," ") ;
	}
	else {

		/* first 8 column are var name */
		fprintf(fp,"%-8s",name) ;

		if(strcmp(name,"END") != 0) fprintf(fp,"= ") ;
		else fprintf(fp,"  ") ;

		/* next twenty columns contain var value */
		if(val[0] == '\'')
			fprintf(fp,"%-20s",val) ;
		else
			fprintf(fp,"%20s",val) ;

		/* ' /' goes in column 31,32 */
		if(strcmp(name,"END") != 0) fprintf(fp," /") ;
		else fprintf(fp,"  ") ;

		/* fill out to column 80 with blanks */
		for(i=0;i<48;i++) fprintf(fp," ") ;
	}
}
