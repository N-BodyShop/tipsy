/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:35  trq
 * Initial revision
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

int xray_lum_load()
{
    char *units_line ;
    double cm_2 ;
    int count ;
    double *lum_dat ;
    double temp_dat ;
    double last_temp ;
    int i,j ;

    if(!xray_loaded){
	forever{
	    printf("<please enter: xray-data-filename,") ;
	    printf(" column-density-unit(#/cm-2)>\n") ;
	    units_line = my_gets(" ") ;
	    if(sscanf(units_line,"%s %lf",
		    hardfile.name,&cm_2) == 2){
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
	    count=fscanf(hardfile.ptr, "%d %d"
			,&number_xray_data,&number_bands) ;
	    if ( (count == EOF) || (count==0) ){
		printf("<Sorry %s, file format is wrong>\n",title);
		return 1;
	    }
	    xray_lum = (double *)malloc((number_xray_data)*(number_bands)*
		    sizeof(*xray_lum));
	    if(xray_lum == NULL)
	      {
		printf("<sorry, no memory for xray_lum, %s>\n",title) ;
		  return 1;
	      }
	    lum_dat = xray_lum ;
	    if(fscanf(hardfile.ptr, "%lf",
		     &xray_min_temp) == EOF) {
		printf("<Sorry %s, file format is wrong>\n",title);
		return 1;
	    }
	    for(j = 0 ; j < number_bands; j++,lum_dat++){
		fscanf(hardfile.ptr, "%lf",lum_dat) ;
	    }
	    if(fscanf(hardfile.ptr, "%lf",
		     &temp_dat) == EOF) {
		printf("<Sorry %s, file format is wrong>\n",title);
		return 1;
	    }
	    deldtempi = temp_dat - xray_min_temp ;
	    last_temp = temp_dat ;
	    for(j = 0 ; j < number_bands; j++,lum_dat++){
		fscanf(hardfile.ptr, "%lf",lum_dat) ;
	    }
	    for(i = 2 ; i < number_xray_data; i++)
	      {
		if(fscanf(hardfile.ptr, "%lf", &temp_dat) == EOF) {
		    printf("<Sorry %s, file format is wrong>\n",title);
		    return 1;
		}
		if(fabs((temp_dat - last_temp - deldtempi)/deldtempi) > 1.e-3){
		    printf("<Sorry %s, file format is wrong>\n",title);
		    return 1;
		}
		last_temp = temp_dat ;
		for(j = 0 ; j < number_bands; j++,lum_dat++){
		    fscanf(hardfile.ptr,"%lf",lum_dat) ;
		}
	      }
	  temp_dat = xray_min_temp ;
	  count = 0 ;
	  for(i = 0 ; i < number_xray_data; i++){
	      for(j = 0 ; j < number_bands; j++){
		  xray_lum[count] *= cm_2 ;
		  count++ ;
	      }
	      temp_dat += deldtempi ;
	  }
	  number_xray_data -= 1 ;
	  xray_loaded = YES ;
	  }
	return 0 ;
}
