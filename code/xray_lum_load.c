/* $Header$
 * $Log$
 * Revision 1.1.1.1.2.1  1997/12/06 00:37:56  trq
 * Added spline based emmissivity.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:36  trq
 * Import to CVS
 *
 * Revision 2.1  1994/12/05  17:49:20  trq
 * Fixed call to my_gets().
 *
 * Revision 1.3  1994/04/20  08:46:34  trq
 * Added title variable.
 *
 */

#include "defs.h"
#include <stdlib.h>
#include "xray.h"
struct xray_table *xray_lums;
double *temp_spline;
int number_bands;
int xray_spline_n;

/*
 * Read in the spline coefficients for spline interpolating the X-ray
 * luminosity.
 * The format of the table is
Comment line
<redshift>
<number of spline points> <min. temperature> <delta temperature> (the
temperatures are log10(Kelvin).)
<number of bands>

 */
int xray_lum_load()
{
    char *units_line ;
    int count ;
    int i,j ;
    FILE *fp;

    if(!xray_loaded){
	forever{
	    printf("<please enter: xray-spline-filename,") ;
	    units_line = my_gets(" ") ;
	    if(sscanf(units_line,"%s", hardfile.name) == 1){
		break ;
	    }
	    printf("<sorry, inconsistent input, %s>\n",title) ;
	}
	hardfile.ptr = fopen(hardfile.name,"r") ;
	if(hardfile.ptr == NULL)
	  {
	    printf("<Sorry %s, file does not exist>\n",title);
	    return 1;
	  }
	/*
	 * Toss first line
	 */
	fscanf(hardfile.ptr, "%*s");
	
	count = fscanf(hardfile.ptr, "%lf", &redshift);
	if(count != 1) {
	    printf("<Sorry %s, file format wrong>\n", title);
	    return 1;
	    }
	count = fscanf(hardfile.ptr, "%d %lf %lf", &xray_spline_n,
		       &xray_min_temp, &deldtempi);
	
	if(count != 3) {
	    printf("<Sorry %s, file format wrong>\n", title);
	    return 1;
	    }
	count=fscanf(hardfile.ptr, "%d",&number_bands) ;
	if(count != 1) {
	    printf("<Sorry %s, file format wrong>\n", title);
	    return 1;
	    }

	temp_spline = (double
		       *)malloc(xray_spline_n*sizeof(*temp_spline));
	for(i = 0; i < xray_spline_n; i++) {
	    temp_spline[i] = xray_min_temp + i*deldtempi;
	    }
	
	xray_lums = malloc(number_bands*sizeof(*xray_lums));
	    if(xray_lums == NULL)
	      {
		printf("<sorry, no memory for xray_lum, %s>\n",title) ;
		  return 1;
	      }
	for(i = 0; i < xray_spline_n; i++) {
	    xray_lums[i].yspl =
		malloc(xray_spline_n*sizeof(*(xray_lums->yspl)));
	    xray_lums[i].y2spl =
		malloc(xray_spline_n*sizeof(*(xray_lums->y2spl)));
	    }
	
	for(i = 0 ; i < number_bands; i++){
	    for(j = 0 ; j < xray_spline_n; j++){
		count = fscanf(hardfile.ptr, "%lf",&xray_lums[i].yspl[j]) ;
		if(count != 1)
		    printf("<Sorry %s, file format wrong>\n", title);
		    return 1;
		}
	    for(j = 0 ; j < xray_spline_n; j++){
		count = fscanf(hardfile.ptr, "%lf",&xray_lums[i].y2spl[j]) ;
		if(count != 1)
		    printf("<Sorry %s, file format wrong>\n", title);
		    return 1;
		}
	    }
	
	  xray_loaded = YES ;
	  }
	return 0 ;
}
