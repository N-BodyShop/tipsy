/* $Header$
 * $Log$
 * Revision 1.1.1.1.2.1  1997/12/06 00:36:30  trq
 * New spline based routine.
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
    FILE *fptst;

    if (sscanf(job,"%s %lf %lf %s %d",command,&low,&high,name,&xray_size) == 
	    5) {
	if(!ikernel_loaded){
	    ikernel_load() ;
	}
	if (current_project){
	    if(!xray_loaded){
		if(xray_lum_load() != 0){
		    return ;
		}
	    }
	    if (!cool_loaded ){
		  load_cool() ;
		}
	    if (!redshift_loaded ){
		  load_redshift() ;
		}
	    fptst = fopen("tst.xem", "w");
	    assert(fptst != NULL);
	    
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
	  **density = (double *)malloc(number_bands*xray_size*xray_size*
		sizeof(***density));
	  if(**density == NULL)
	    {
	      free(density);
	      free(*density);
	      free(image);
	      printf("<sorry, no memory for image, %s>\n",title) ;
	      return ;
	    }
	  *image = (float *)malloc(xray_size*xray_size*sizeof(**image));
	  if(*image == NULL)
	    {
	      free(density);
	      free(image);
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
	    for (i = 0 ;i < boxlist[active_box].ngas;i++) {
		gp = boxlist[active_box].gp[i] ;
		if(log10(gp->temp) > xray_min_temp){
		    fprintf(fptst, "%g %g", gp->rho, gp->temp);
		    for(band = 0,lum_xray = band_lum; band < number_bands;
			    band++,lum_xray++){
			 *lum_xray = gp->mass*calc_xemiss((double) gp->temp,
						 (double) gp->rho, band);
			
			fprintf(fptst, " %g", *lum_xray);
			
			if(*lum_xray < 0.){
			    printf("<sorry, negative luminosity %s\n",title) ;
			    return ;
			}
			
		    }
		    fprintf(fptst, "\n");
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
					density[band][kx][ky] += (kernel*
					    band_lum[band]) ;
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
				density[band][kx][ky] += band_lum[band];
			    }
			}
		    }
		}
	    }
	    fclose(fptst);
	    for(band = 0; band < number_bands; band++){
		for(i = 0; i < xray_size; i++){
		    for(j = 0; j < xray_size; j++){
			if(density[band][i][j] > 0. ){
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
		sprintf(image_name,"%s%03d.fits",name,band) ;
		fits(image,xray_size,xray_size,0.,
			0.,1.,1.,low,high,image_name) ;
	    }
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
		}
	    }
	    sprintf(image_name,"%stot.fits",name) ;
	    fits(image,xray_size,xray_size,0.,
		    0.,1.,1.,low,high,image_name) ;
	    
	}
	else {
	    printf("<plot does not represent the present internal state, %s>",title) ;
	}
      free(**density);
      free(*density);
      free(density);
      free(*image);
      free(image);
    }
    else {
	input_error(command) ;
    }
}
