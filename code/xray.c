/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:36  trq
 * Initial revision
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

void
xray(job)
    char *job;
{
    char command[MAXCOMM] ;
    double size_pixel ;
    double xmin ;
    double ymin ;
    struct gas_particle *gp ;
    double pixel_pos[2] ;
    double part_pos[2] ;
    int i,j ;
    int kx,ky ;
    int kx_min,kx_max ;
    int ky_min,ky_max ;
    float ***density;
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
    double temp ;
    double *lum_xray ;
    int band ;
    double *band_lum ;
    int xray_size ;


    if (sscanf(job,"%s %lf %lf %s %d",command,&low,&high,name,&xray_size) == 
	    5) {
	if(!ikernel_loaded){
	    ikernel_load() ;
	}
	if (current_project){
	  density = (float ***)malloc(MAXBANDS*sizeof(*density));
	  if(density == NULL)
	    {
	      printf("<sorry, no memory for image, %s>\n",title) ;
	      return ;
	    }
	  *density = (float **)malloc(MAXBANDS*xray_size*sizeof(**density));
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
	  **density = (float *)malloc(MAXBANDS*xray_size*xray_size*
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
	  for(j = 1; j < MAXBANDS; j++)
	      density[j] = &density[0][j*xray_size];
	  
	  for(i = 0; i < xray_size; i++)
	    {
	      for(j = 0; j < MAXBANDS; j++)
		density[j][i] = &density[0][0][j*xray_size*xray_size + 
			i*xray_size];
	      image[i] = &image[0][i*xray_size];
	    }
	    size_pixel = (dv2_x - dv1_x) / scaling / (double)xray_size ;
	    xmin = dv1_x / scaling ;
	    ymin = dv1_y / scaling ;
	    if(!xray_loaded){
		if(xray_lum_load() != 0){
		    return ;
		}
	    }
	    if(number_bands > MAXBANDS){
		printf("<sorry, too many xray bands, %s>\n",title) ;
		return ;
	    }
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
		    temp = log10(gp->temp) - xray_min_temp ;
		    temp /= deldtempi ;
		    iwsm = (int)temp ;
		    if(iwsm >= number_xray_data){
			printf("<sorry, gas temperature too") ;
			printf(" big, %s>",title) ;
			return ;
		    }
		    drw = temp - (double)iwsm ;
		    iwsm *= number_bands ;
		    for(band = 0,lum_xray = band_lum; band < number_bands;
			    band++,lum_xray++){
			*lum_xray = ((1. - drw) * xray_lum[iwsm + band] +
				drw * xray_lum[number_bands + iwsm + band]) ;
			*lum_xray *= 100./118.1/0.6/0.6*(gp->rho)*(gp->mass) ;
			if(*lum_xray < 0.){
			    printf("<sorry, negative luminosity %s\n",title) ;
			    return ;
			}
		    }
		    hsmooth = max(gp->hsmooth, size_pixel / 2.) ;
		    thsmooth = 2. * hsmooth ;
		    distnorm = 1. / (hsmooth * hsmooth) ;
		    hsmth2pi = distnorm / PI ;
		    for (part_pos[0] = part_pos[1] = 0.0,j = 0 ;
			    j < header.ndim ;j++) {
			part_pos[0] += rot_matrix[0][j] * (gp->pos[j] -
				boxes[active_box].center[j]) ;
			part_pos[1] += rot_matrix[1][j] * (gp->pos[j] -
				boxes[active_box].center[j]) ;
		    }
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
			    if(kernel != 0.){
				for(band = 0; band < number_bands; band++){
				    density[band][kx][ky] += (float)(kernel*
					band_lum[band]) ;
				}
			    }
			}
		    }
		}
	    }
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
	}
	else {
	    input_error(command) ;
	}
      free(density);
      free(image);
      free(*density);
      free(**density);
      free(*image);
    }
    else {
	printf("<plot does not represent the present internal state, %s>",title) ;
    }
}
