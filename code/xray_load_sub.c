#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>
#include "xray.h"

/*
 * Read in the spline coefficients for spline interpolating the X-ray
 * luminosity.
 * The format of the table is
 Comment line
 <redshift>
 <number of spline points> <min. temperature> <delta temperature> (the
 temperatures are log10(Kelvin).)
 <number of bands>
<y spline coefficients for band 1>
<y'' spline coefficients for band 1>
<y spline coefficients for band 2>
...
 
 This version allows you to read the spline file from the command line
 with a call to: xrayload splinefile
 
 */
void
xray_load_sub(job)
     char *job ;
{
  int count ;
  int i,j ;
  FILE *fp;
  char command[MAXCOMM] ;
  
  if(sscanf(job,"%s %s",command, hardfile.name) != 2){
    printf("<Useage, xrayload splinefile>\n") ;
    return ;
  }
  
  /* Free any previously allocated stuff */

  if ( temp_spline!=NULL ) free(temp_spline) ;  	

  if ( xray_lums!=NULL ){
    
    for(i = 0; i < number_bands; i++) {
      free( xray_lums[i].yspl );
      free( xray_lums[i].y2spl );
    }
    
    free(xray_lums) ;  
  }
  
  hardfile.ptr = fopen(hardfile.name,"r") ;
  if(hardfile.ptr == NULL)
    {
      printf("<Sorry %s, file does not exist>\n",title);
      return;
    }
  /*
   * Toss first line
   */
  fscanf(hardfile.ptr, "%*s");
  
  count = fscanf(hardfile.ptr, "%lf", &redshift);
  if(count != 1) {
    printf("<Sorry %s, file format wrong>\n", title);
    return;
  }
  count = fscanf(hardfile.ptr, "%d %lf %lf", &xray_spline_n,
		 &xray_min_temp, &deldtempi);
  
  if(count != 3) {
    printf("<Sorry %s, file format wrong>\n", title);
    return;
  }
  count=fscanf(hardfile.ptr, "%d",&number_bands) ;
  if(count != 1) {
    printf("<Sorry %s, file format wrong>\n", title);
    return;
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
      return;
    }
 
  for(i = 0; i < number_bands; i++) {
    xray_lums[i].yspl =
      malloc(xray_spline_n*sizeof(*(xray_lums->yspl)));
    xray_lums[i].y2spl =
      malloc(xray_spline_n*sizeof(*(xray_lums->y2spl)));
  }
  
  for(i = 0 ; i < number_bands; i++){
    for(j = 0 ; j < xray_spline_n; j++){
      count = fscanf(hardfile.ptr, "%lf",&xray_lums[i].yspl[j]) ;
      if(count != 1) {
	printf("<Sorry %s, file format wrong>\n", title);
	return;
      }
    }
    for(j = 0 ; j < xray_spline_n; j++){
      count = fscanf(hardfile.ptr, "%lf",&xray_lums[i].y2spl[j]) ;
      if(count != 1) {
	printf("<Sorry %s, file format wrong>\n", title);
	return;
      }
    }
  }

  /* Close the file */

  close( hardfile.ptr ) ;

  printf(" Spline file: %s loaded \n", hardfile.name ) ;  
  xray_loaded = YES ;
}


