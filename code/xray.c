/* $Header$
 * $Log$
 * Revision 1.6  2008/09/05 00:05:06  trq
 * Changed "values.h" to "float.h" so that it will compile on a MAC.
 *
 * Revision 1.5  2007/01/14 19:43:22  trq
 * 1) load redshift information if needed.
 * 2) add warning about floating point overflow of temp_array.
 *
 * Revision 1.4  2000/01/12 22:55:29  nsk
 * Fixed bugs in cooling routines, added cooling damping,
 * fixed bugs in starformation,  regularized dependencies.
 *
 * Revision 1.3  1998/02/18  22:54:56  trq
 * Correct bounding on temperature.
 *
 * Revision 1.2  1997/12/06  22:00:57  trq
 * Updated xray stuff to use spline emmission files.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:37  trq
 * Import to CVS
 *
 * Revision 2.4  1994/11/01  19:20:04  nsk
 * allowed size of image to change nad call to more general fits.c
 *
 * Revision 2.3  1994/10/12  21:26:39  trq
 * Cleaned up mallocs.
 *
 * Revision 2.2  1994/04/20  08:46:30  trq
 * Added title variable.
 *
 * Revision 2.0  93/10/19  13:24:08  trq
 * *** empty log message ***
 * 
 */

#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>
#include <float.h>
#include <assert.h>
#include "xray.h"

void
xray(job)
    char *job;
{
    char command[MAXCOMM] ;
    double size_pixel ;
    double size_pixel_2 ;
    double xmin ;
    double ymin ;
    struct gas_particle *gp ;
    double pixel_pos[2] ;
    double part_pos[2] ;
    int i,j ;
    int kx,ky ;
    int kx_min,kx_max ;
    int ky_min,ky_max ;
    double ***density;
    double radius2 ;
    double distance_dim2() ;
    double hsmooth ;
    double thsmooth ;
    double hsmth2pi ;
    double kernel ;
    char name[50] ;
    char image_name[50] ;
    float **image;
    float **temp_image;
    double pixel ;
    double low ;
    double high ;
    double distnorm ;
    double drw ;
    int iwsm ;
    int xray_lum_load() ;
    double *lum_xray ;
    int band ;
    double *band_lum ;
    int xray_size ;

    double total_lum ; 

    if (sscanf(job,"%s %lf %lf %s %d",command,&low,&high,name,&xray_size) == 
	    5) {
	if(!ikernel_loaded){
	    ikernel_load() ;
	}
	if (!redshift_loaded ){
	    load_redshift();
	    }
	if (current_project){
	    if(!xray_loaded){
		if(xray_lum_load() != 0){
		    return ;
		}
	    }
	    if (!cool_loaded ){
	      printf("loading cool %d \n",cool_loaded);
	       load_cool() ;
		}
	    
	  density = (double ***)malloc(number_bands*sizeof(*density));
	  if(density == NULL)
	    {
	      printf("<sorry, no memory for image, %s>\n",title) ;
	      return ;
	    }
	  *density = (double **)malloc(number_bands*xray_size*sizeof(**density));
	  if(*density == NULL)
	    {
	      free(density);
	      printf("<sorry, no memory for image, %s>\n",title) ;
	      return ;
	    }
	  image = (float **)malloc(xray_size*sizeof(*image));
	  if(image == NULL)
	    {
	      free(density);
	      free(*density);
	      printf("<sorry, no memory for image, %s>\n",title) ;
	      return ;
	    }
	  temp_image = (float **)malloc(xray_size*sizeof(*temp_image));
	  if(temp_image == NULL)
	    {
	      free(density);
	      free(*density);
	      free(image);
	      printf("<sorry, no memory for image, %s>\n",title) ;
	      return ;
	    }
	  **density = (double *)malloc(number_bands*xray_size*xray_size*
		sizeof(***density));
	  if(**density == NULL)
	    {
	      free(density);
	      free(*density);
	      free(image);
	      free(temp_image);
	      printf("<sorry, no memory for image, %s>\n",title) ;
	      return ;
	    }
	  *image = (float *)malloc(xray_size*xray_size*sizeof(**image));
	  if(*image == NULL)
	    {
	      free(density);
	      free(image);
	      free(temp_image);
	      free(*density);
	      free(**density);
	      printf("<sorry, no memory for image, %s>\n",title) ;
	      return ;
	    }
	  *temp_image = (float *)malloc(xray_size*xray_size*sizeof(**temp_image));
	  if(*temp_image == NULL)
	    {
	      free(density);
	      free(image);
	      free(temp_image);
	      free(*image);
	      free(*density);
	      free(**density);
	      printf("<sorry, no memory for image, %s>\n",title) ;
	      return ;
	    }
	  for(j = 1; j < number_bands; j++)
	      density[j] = &density[0][j*xray_size];
	  
	  for(i = 0; i < xray_size; i++)
	    {
	      for(j = 0; j < number_bands; j++)
		density[j][i] = &density[0][0][j*xray_size*xray_size + 
			i*xray_size];
	      image[i] = &image[0][i*xray_size];
	      temp_image[i] = &temp_image[0][i*xray_size];
	    }
	    size_pixel = (dv2_x - dv1_x) / scaling / (double)xray_size ;
	    size_pixel_2 = size_pixel*size_pixel ;
	    xmin = dv1_x / scaling ;
	    ymin = dv1_y / scaling ;
	    band_lum = (double *)malloc((number_bands)*sizeof(*band_lum)) ;
	    for(band = 0; band < number_bands; band++){
		for(kx = 0; kx < xray_size; kx++){
		    for(ky = 0; ky < xray_size; ky++){
		    density[band][kx][ky] = 0.0 ;
		    }
		}
	    }
	    for(kx = 0; kx < xray_size; kx++){
		for(ky = 0; ky < xray_size; ky++){
		    temp_image[kx][ky] = 0.0 ;
		}
	    }
	    for (i = 0 ;i < boxlist[active_box].ngas;i++) {
		gp = boxlist[active_box].gp[i] ;
		if(log10((double) gp->temp) > xray_min_temp){
		    for(band = 0,lum_xray = band_lum; band < number_bands;
			    band++,lum_xray++){
			 *lum_xray = gp->mass*calc_xemiss((double) gp->temp,
						 (double) gp->rho, band);
			
			if(*lum_xray < 0.){
			    printf("<sorry, negative luminosity %s\n",title) ;
			    return ;
			}
			
		    }
		    hsmooth = gp->hsmooth ;
		    for (part_pos[0] = part_pos[1] = 0.0,j = 0 ;
			 j < header.ndim ;j++) {
			part_pos[0] += rot_matrix[0][j] * (gp->pos[j] -
				    boxes[active_box].center[j]) ;
			part_pos[1] += rot_matrix[1][j] * (gp->pos[j] -
				    boxes[active_box].center[j]) ;
		    }
		    if(hsmooth > size_pixel) {
			thsmooth = 2. * hsmooth ;
			distnorm = 1. / (hsmooth * hsmooth) ;
			hsmth2pi = distnorm / PI ;
			kx_min = max(0,(int)((part_pos[0]-thsmooth-xmin)/
				size_pixel + .499999)) ;
			kx_max = min(xray_size-1,(int)((part_pos[0]+thsmooth-
				xmin)/
				size_pixel + .499999)) ;
			ky_min = max(0,(int)((part_pos[1]-thsmooth-ymin)/
			    size_pixel + .499999)) ;
			ky_max = min(xray_size-1,(int)((part_pos[1]+thsmooth-
				ymin)/
				size_pixel + .499999)) ;
			for(kx = kx_min; kx < kx_max + 1; kx++){
			    for(ky = ky_min; ky < ky_max + 1; ky++){
				pixel_pos[0] = xmin + (kx + .5) * size_pixel ;
				pixel_pos[1] = ymin + (ky + .5) * size_pixel ;
				radius2 = distance_dim2(pixel_pos,part_pos) *
					distnorm ;
				if(radius2 < 4.){
				    radius2 *= deldr2i ;
				    iwsm = (int)radius2 ;
				    drw = radius2 - (double)iwsm ;
				    kernel = ((1. - drw) * iwsmooth[iwsm] +
					    drw * iwsmooth[1 + iwsm]) * hsmth2pi ;
				}
				else{
				    kernel = 0. ;
				}
				kernel *= size_pixel_2 ;
				if(kernel != 0.){
				    for(band = 0; band < number_bands; band++){
					double tTemp
					    = kernel*band_lum[band]*gp->temp;
					if(tTemp > FLT_MAX) {
					    fprintf(stderr,
						    "Warning: %g overflows temperature array\n",
						    tTemp);
					    tTemp = FLT_MAX/10.0;
					    }
					
					density[band][kx][ky] += (kernel*
					    band_lum[band]) ;
					temp_image[kx][ky] += tTemp;
				    }
				}
			    }
			}
		    }
		    else {
			kx = (int)((part_pos[0]-xmin)/size_pixel+ 0.499999) ;
			ky = (int)((part_pos[1]-ymin)/size_pixel+ 0.499999) ;
			if(kx >= 0 && kx < xray_size && ky >= 0 && ky
			   < xray_size){
			    for(band = 0; band < number_bands; band++){
				double tTemp = band_lum[band]*gp->temp;
				if(tTemp > FLT_MAX) {
				    fprintf(stderr, "Warning: %g overflows temperature array\n",
					    tTemp);
				    tTemp = FLT_MAX/10.0;
				    }
				density[band][kx][ky] += band_lum[band];
				temp_image[kx][ky]
					+= tTemp;
			    }
			}
		    }
		}
	    }
	    for(band = 0; band < number_bands; band++){

	      total_lum = 0.0 ;

		for(i = 0; i < xray_size; i++){
		    for(j = 0; j < xray_size; j++){
			if(density[band][i][j] > 0. ){

			  total_lum += (double)(density[band][i][j]) ;

			    pixel = log10((double)(density[band][i][j]));
			}
			else{
			    pixel = low ;
			}
			if(pixel > high)pixel = high ;
			if(pixel < low)pixel = low ;
			image[i][j] = (float)pixel ;
		    }
		}
		printf(" Band Number: %d flux = %f \n",band,log10(total_lum) );
		sprintf(image_name,"%s%03d.fits",name,band) ;
		fits(image,xray_size,xray_size,0.,
			0.,1.,1.,low,high,image_name) ;
	    }
	/* sum up the luminosity in all bands. */
	    for(band = 1; band < number_bands; band++){
		for(i = 0; i < xray_size; i++){
		    for(j = 0; j < xray_size; j++){
			density[0][i][j] += density[band][i][j];
			}
		    }
		}
	    for(i = 0; i < xray_size; i++){
		for(j = 0; j < xray_size; j++){
		    if(density[0][i][j] > 0. ){
			pixel = log10((double)(density[0][i][j]));
		    }
		    else{
			pixel = low ;
		    }
		    if(pixel > high)pixel = high ;
		    if(pixel < low)pixel = low ;
		    image[i][j] = (float)pixel ;
		    temp_image[i][j] /= density[0][i][j];
#define LOW_TEMP 0.0
#define HIGH_TEMP 20.0
		    if(temp_image[i][j] > 0. ){
			pixel = log10((double)(temp_image[i][j]));
		    }
		    else{
			pixel = LOW_TEMP ;
		    }
		    if(pixel > HIGH_TEMP)pixel = HIGH_TEMP ;
		    if(pixel < LOW_TEMP)pixel = LOW_TEMP ;
		    temp_image[i][j] = (float) pixel;
		}
	    }
	    sprintf(image_name,"%stot.fits",name) ;
	    fits(image,xray_size,xray_size,0.,
		    0.,1.,1.,low,high,image_name) ;
	    sprintf(image_name,"%stemp.fits",name) ;
	    fits(temp_image,xray_size,xray_size,0.,
		    0.,1.,1.,LOW_TEMP,HIGH_TEMP,image_name) ;
	    
	}
	else {
	    printf("<plot does not represent the present internal state, %s>",title) ;
	}
      free(**density);
      free(*density);
      free(density);
      free(*image);
      free(image);
      free(*temp_image);
      free(temp_image);
    }
    else {
	input_error(command) ;
    }
}
