#include "defs.h"
#include "fdefs.h"
#include "smooth.h"
#include <math.h>
#include <stdlib.h>

void
cube(job)
     char *job;
{
  char command[MAXCOMM] ;
  char type[MAXCOMM] ;
  double size_pixel ;
  double size_pixel_2 ;
  double xmin ;
  double ymin ;
  double xmax ;
  double ymax ;
  struct gas_particle *gp ;
  struct star_particle *sp;
  double pixel_pos[2] ;
  double part_pos[3] ;
  int i,j,k,l ;
  int kx,ky,kz,kv ;
  int kx_min,kx_max ;
  int ky_min,ky_max ;
  float ***density;		/* array of pixels, also used to
				   normalize other arrays */
  double radius2 ;
  double distance_dim2() ;
  double hsmooth ;
  double thsmooth ;
  double hsmth2pi ;
  double kernel ;
  int vista_type ;
  char name[100] ;
  char name1[100] ;
  char base[100];              /* Holds filename (without .fits) */
  int nameLength;
  double pixel ;
  double low;
  double high ;
  double pixmax;
  double pixmin;
  double c1 ;
  double distnorm ;
  int iwsm;
  double drw;
  int xy_size ;
  int nChannels; /*velocity channels*/
  double msys ;
  double rsys ;
  double vsys ;
  int num_read ;
  double vel_min ;
  double vel_max ;
  double vmin;
  double *vel_ref;
  int irep[MAXDIM];
  int autolim ;
  double vz ;
  double v_hubble ;
  double delta_d ;
  int vindex;
  double dvel;
  int cube_type;


  /* copied from vista.c */
  if (!ikernel_loaded) {
    ikernel_load();
  }
  if (!current_project) {
    dv1_x = -INTMAX;
    dv1_y = -INTMAX;
    dv2_x = INTMAX;
    dv2_y = INTMAX;
    view_size = INTMAX;
    zoom_factor = 1.0;
    scaling = 2. * (double)INTMAX / boxes[active_box].size;
  }

  /*
   * Parse command line arguments.
   * Everything is in this if statement!
   */
  if ((num_read = sscanf(job,"%s %s %s %i %lf %lf %lf",command, type, name, 
			 &xy_size, &vel_min, &vel_max, &dvel)
                         >=7)){

    /* make sure input is okay */
    if (strncmp(type, "star", 4) == 0) {
      if (strlen(type) == 5) {
	if (type[4] == 'u') {
	  color_filter = UBAND;
	}
	if (type[4] == 'b') {
	  color_filter = BBAND;
	}
	if (type[4] == 'v') {
	  color_filter = VBAND;
	}
	cube_type = STAR;
      }
      else {
	  printf("<incorrect star type: staru, starb, starv, or gas>\n");
	  return;
	}
    }
    else if (strcmp(type, "gas") == 0 || strcmp(type, "g") == 0) {
      cube_type = GAS;
    }
    else {
      printf("<type must be staru, starb, starv, or gas>\n");
      return;
    }

    if (vel_min >= vel_max) {
      printf("<minimum velocity not less than maximum>\n");
      return;
    }

    if ((vel_max - vel_min) <= dvel) {
      printf("<deltav larger than velocity range>\n");
      return;
    }

    if (!redshift_loaded) {
      load_redshift();
    }

    if (!cool_loaded ){
      load_cool() ;
    }


    /* Determine pixel sizes? */
    size_pixel = (dv2_x - dv1_x) / scaling / (double)xy_size;
    size_pixel_2 = size_pixel * size_pixel;
    xmin = dv1_x / scaling;
    ymin = dv1_y / scaling ;
    xmax = (dv2_x+1) / scaling ;
    ymax = (dv2_y+1) / scaling ;
    

    /* determine number of channels */
    nChannels = ceil((vel_max - vel_min) / dvel);


    /* allocate velocity reference array */
    vel_ref = (double*)malloc(nChannels * sizeof(*vel_ref));
    if (vel_ref == NULL) {
      printf("<sorry, no memory for image, %s>\n",title);
      return;
    }


    /* initialize velocity reference info. */
    for (i = 0; i < nChannels; i++) {
      vel_ref[i] = vel_min + dvel / 2.0 + (dvel * i);
    }

    vmin = vel_ref[0];


    /* new way of allocating memory */
    density = (float ***)malloc(nChannels * sizeof(*density));
    if (density == NULL) {
      printf("<sorry, no memory for image, %s>", title);
      return;
    }

    *density = (float **) malloc(nChannels * xy_size * sizeof(**density));
    if (*density == NULL) {
      free(density);
      printf("<sorry, no memory for image, %s>", title);
      return;
    }

    **density = (float *)malloc(nChannels * xy_size * xy_size *
				 sizeof(***density));
    if (**density == NULL) {
      free(density);
      free(*density);
      printf("<sorry, no memory for image, %s>", title);
      return;
    }

    for(j = 1; j < nChannels; j++){
      density[j] = &density[0][j * xy_size];
    }


    for (j = 0; j < nChannels; j++) {
      for (i = 0; i < xy_size; i++) {
	density[j][i] = &density[0][0][j * xy_size * xy_size + i * xy_size];
      }
    }

    /* initialize memory */
    for (kv = 0; kv < nChannels; kv++) {
      for (kx = 0; kx < xy_size; kx++) {
	for (ky = 0; ky < xy_size; ky++) {
	  density[kv][kx][ky] = 0.0;
	}
      }
    }

    /* convert to real units */
    msys = msolunit;  /* just want the mass of gas particles in msuns. */
    rsys = cosmof * kpcunit / 1.e3;
           /* in pc? */
    vsys = cosmof * sqrt(msolunit / kpcunit * (GCGS * MSOLG / KPCCM)) / 1.e5;
           /* in km/s */


    /*
     * Gas or stars?
     *
     */

    if (cube_type == GAS) {


      /* load neutral fractions */
      if (!hneutral_loaded) {
	hneutral_func();
      }

      /* determine constants */
      /* The commented line is making all masses off
	 by 0.75 percent.. I think. */
      /* c1 = msys * fhydrogen; */
      c1 = msys;


      /* calculate smoothing length */
      if (balls_loaded != GAS) {
	calc_balls(&box0_smx, 0, 1, 0);
	balls_loaded = GAS;
      }
      
      /* loop over particles */
      printf("<Placing particles, %s>\n", title);
      for (i = 0; i < boxlist[active_box].ngas; i++) {
	int gpi;
	
	gp = boxlist[active_box].gp[i];
	gpi = boxlist[active_box].gpi[i] ;
	hsmooth = sqrt(box0_smx->kd->p[gpi].fBall2)/2.0;
	
	/*
	 * handle periodic boundary conditions
	 */
	for (irep[0] = -1; irep[0] <= 1; irep[0]++) {
	  for (irep[1] = -1; irep[1] <= 1; irep[1]++) {
	    for(irep[2] = -1; irep[2] <= 1; irep[2]++) {
	      if(periodic || (irep[0] == 0 && irep[1] == 0 && irep[2] == 0)) {
		for (part_pos[0] = part_pos[1] = part_pos[2] = 0.0,j = 0 ;
		     j < header.ndim ;j++) {
		  for (k = 0 ; k < 3 ; k++){
		    part_pos[k] += rot_matrix[k][j] * 
		      (irep[j]*period_size + gp->pos[j]
		       - boxes[active_box].center[j]) ;
		  }
		}
		if(part_pos[0]-2.*(hsmooth) > xmax)
		  continue ;
		if(part_pos[0]+2.*(hsmooth) < xmin)
		  continue ;
		if(part_pos[1]-2.*(hsmooth) > ymax)
		  continue ;
		if(part_pos[1]+2.*(hsmooth) < ymin)
		  continue ;
		
		/*
		 * for vista_type = HNEUT in vista.c
		 */
		for (vz = 0.0,j = 0;j < header.ndim ;j++) {
		  vz += rot_matrix[2][j] * (gp->vel[j]) ;
		}
		vz *= vsys ;
		if(comove == YES){
		  v_hubble = hubble_constant * part_pos[2]*rsys ;
		}
		
		/* find what velocity channel the particle belongs in */
		vindex = getVelocityChannel(vz, vel_ref, dvel, nChannels);

		
		/* put it in its pixel(s). */
		if (vindex >= 0 && vindex < nChannels) {
		  delta_d = c1 * hneutral[gp-gas_particles] * (gp->mass);
		  
		  if (hsmooth > size_pixel){
		    /* need to loop over surrounding
		       pixels
		    */
		    thsmooth = 2. * hsmooth ;
		    distnorm = 1. / (hsmooth * hsmooth) ;
		    hsmth2pi = distnorm / PI ;
		    kx_min = max(0,(int)
				 ((part_pos[0]-thsmooth-xmin)/size_pixel + 
				  .499999)) ;
		    kx_max = min(xy_size-1,(int)
				 ((part_pos[0]+thsmooth-xmin)/size_pixel +
				  .499999)) ;
		    ky_min = max(0,(int)
				 ((part_pos[1]-thsmooth-ymin)/size_pixel +
				  .499999)) ;
		    ky_max = min(xy_size-1,(int)
				 ((part_pos[1]+thsmooth-ymin)/ size_pixel +
				  .499999)) ;
		    for(kx = kx_min; kx < kx_max + 1; kx++){
		      for(ky = ky_min; ky < ky_max + 1; ky++){
			pixel_pos[0] = xmin+(kx + .5) * size_pixel ;
			pixel_pos[1] = ymin+(ky + .5) * size_pixel ;
			radius2 = distance_dim2(pixel_pos, part_pos) *
			          distnorm;
			if(radius2 < 4.){
			  radius2 *= deldr2i ;
			  iwsm = (int)radius2 ;
			  drw = radius2 - (double)iwsm ;
			  kernel = ((1. - drw) * iwsmooth[iwsm] + drw * 
				    iwsmooth[1+iwsm]) * hsmth2pi ;
			}
			else {
			  kernel = 0.0;
			}
			
			kernel *= size_pixel_2;
			if (kernel != 0.0) {
			  density[vindex][kx][ky] += (float)(kernel * delta_d);
			}
		      }
		    }
		  }
		  else {
		    /* particle fits in a single pixel */
		    kx = (int)((part_pos[0]-xmin)/size_pixel + 0.499999) ;
		    ky = (int)((part_pos[1]-ymin)/size_pixel + 0.499999) ;
		    if(kx >= 0 && kx < xy_size && ky >= 0 && ky < xy_size){
		      density[vindex][kx][ky] += (float) delta_d ;
		    }
		  }
		  
		}
	      }
	    }
	  }
	}
      }
    }
    else if (cube_type == STAR) {

      /* determine constants */
      c1 = msys;


      /* calculate smoothing length */

      /* 
       *
       * check if it is 32 or 64!
       *
       * The calc_balls command was in vista this way.
       *
       */
      if (balls_loaded != STAR && boxlist[active_box].nstar > 0) {
	calc_balls(&box0_smx, 0, 0, 1);
	balls_loaded = STAR;
      }
      
      /* loop over particles */
      for (i = 0; i < boxlist[active_box].nstar; i++) {
	int spi;

	sp = boxlist[active_box].sp[i];
	spi = boxlist[active_box].spi[i] ;
	hsmooth = sqrt(box0_smx->kd->p[spi].fBall2)/2.0;
	
	/*
	 * handle periodic boundary conditions
	 */
	for (irep[0] = -1; irep[0] <= 1; irep[0]++) {
	  for (irep[1] = -1; irep[1] <= 1; irep[1]++) {
	    for(irep[2] = -1; irep[2] <= 1; irep[2]++) {
	      if(periodic || (irep[0] == 0 && irep[1] == 0 && irep[2] == 0)) {
		for (part_pos[0] = part_pos[1] = part_pos[2] = 0.0,j = 0 ;
		     j < header.ndim ;j++) {
		  for (k = 0 ; k < 3 ; k++){
		    part_pos[k] += rot_matrix[k][j] * 
		      (irep[j]*period_size + sp->pos[j]
		       - boxes[active_box].center[j]) ;
		  }
		}
		if(part_pos[0]-2.*(hsmooth) > xmax)
		  continue ;
		if(part_pos[0]+2.*(hsmooth) < xmin)
		  continue ;
		if(part_pos[1]-2.*(hsmooth) > ymax)
		  continue ;
		if(part_pos[1]+2.*(hsmooth) < ymin)
		  continue ;
		
		/*
		 * for vista_type = HNEUT in vista.c
		 * Not for stars?
		 *
		 * keep for now.
		 */
		for (vz = 0.0,j = 0;j < header.ndim ;j++) {
		  vz += rot_matrix[2][j] * (sp->vel[j]) ;
		}
		vz *= vsys ;
		if(comove == YES){
		  v_hubble = hubble_constant * part_pos[2]*rsys ;
		}
		
		/* find what velocity channel the particle belongs in */
		vindex = getVelocityChannel(vz, vel_ref, dvel, nChannels);

		
		/* put it in its pixel(s). */
		if (vindex >= 0 && vindex < nChannels) {
		  delta_d = star_lum(sp->mass, sp->tform);
		  
		  if (hsmooth > size_pixel){
		    /* need to loop over surrounding
		       pixels
		    */
		    thsmooth = 2. * hsmooth ;
		    distnorm = 1. / (hsmooth * hsmooth) ;
		    hsmth2pi = distnorm / PI ;
		    kx_min = max(0,(int)
				 ((part_pos[0]-thsmooth-xmin)/size_pixel + 
				  .499999)) ;
		    kx_max = min(xy_size-1,(int)
				 ((part_pos[0]+thsmooth-xmin)/size_pixel +
				  .499999)) ;
		    ky_min = max(0,(int)
				 ((part_pos[1]-thsmooth-ymin)/size_pixel +
				  .499999)) ;
		    ky_max = min(xy_size-1,(int)
				 ((part_pos[1]+thsmooth-ymin)/ size_pixel +
				  .499999)) ;
		    for(kx = kx_min; kx < kx_max + 1; kx++){
		      for(ky = ky_min; ky < ky_max + 1; ky++){
			pixel_pos[0] = xmin+(kx + .5) * size_pixel ;
			pixel_pos[1] = ymin+(ky + .5) * size_pixel ;
			radius2 = distance_dim2(pixel_pos, part_pos) *
			          distnorm;
			if(radius2 < 4.){
			  radius2 *= deldr2i ;
			  iwsm = (int)radius2 ;
			  drw = radius2 - (double)iwsm ;
			  kernel = ((1. - drw) * iwsmooth[iwsm] + drw * 
				    iwsmooth[1+iwsm]) * hsmth2pi ;
			}
			else {
			  kernel = 0.0;
			}
			
			kernel *= size_pixel_2;
			if (kernel != 0.0) {
			  density[vindex][kx][ky] += (float)(kernel * delta_d);
			}
		      }
		    }
		  }
		  else {
		    /* particle fits in a single pixel */
		    kx = (int)((part_pos[0]-xmin)/size_pixel + 0.499999) ;
		    ky = (int)((part_pos[1]-ymin)/size_pixel + 0.499999) ;
		    if(kx >= 0 && kx < xy_size && ky >= 0 && ky < xy_size){
		      density[vindex][kx][ky] += (float) delta_d ;
		    }
		  }
		  
		}
	      }
	    }
	  }
	}
      }
    }

    pixmax = -HUGE;
    pixmin = HUGE;
    for (i = 0; i < nChannels; i++) {
      for (j = 0; j < xy_size; j++) {
	for (k = 0; k < xy_size; k++) {
	  if (density[i][j][k] > 0.0) {
	    pixel = log10((double)(density[i][j][k]));
	    if (pixel < 0) {
	      pixel = 0.0;
	    }
	  }
	  else {
	    (pixel = 0.0);
	  }
	  if (pixel > pixmax) {
	    pixmax = pixel;
	  }
	  if (pixel < pixmin) {
	    pixmin = pixel;
	  }
	  density[i][j][k] = pixel;
	}
      }
    }

    printf("Max: %lf\nMin: %lf\n", pixmax, pixmin);
    fits3d(density, xy_size, xy_size, nChannels, xmin * kpcunit, 
	   ymin * kpcunit, vmin, size_pixel * kpcunit,
	   size_pixel * kpcunit, dvel, pixmin, pixmax, name);

    /* write to fits file */


    /* free used memory */
    free(**density);
    free(*density);
    free(density);

    /* end of actual program part */
  }
  else {
    input_error(command);
  }
}

/*
 * This is the OLD linear searcher.
 *
 * Searches through the velocity reference array to find what channel a
 * given pixel belongs to.
 *
 * Returns -1 if the particle is outside the given velocity range.
 */
/*
int getVelocityChannel(double velocity, double *vel_ref, double dvel, int nChannels) {
  int found;
  int i;


  if ((velocity < vel_ref[0]) || (velocity > vel_ref[nChannels-1] + dvel/2.0)){
    return -1;
  }


  found = 0;
  i = 0; 
  while (!found || i < nChannels) {
     
    if (velocity < (vel_ref[i] + dvel / 2.0)) { 
      return i; 
    } 
    i++;
  }

}
*/


/*
 * Here's a new binary searcher!
 *
 * Searches through the velocity reference array to find what channel a
 * given pixel belongs to.
 *
 * Returns -1 if the particle is outside the given velocity range.
 */
int getVelocityChannel(double velocity, double *vel_ref, double dvel, int nChannels) {

  double dv = dvel / 2.0;

  int low = 0;
  int high = nChannels - 1;

  int mid = 0;

  if ((velocity < vel_ref[0]) || (velocity > vel_ref[high]))
    return -1;

  while (low <= high) {
    mid = (low + high) / 2;

    if ((velocity > vel_ref[mid] - dv) && (velocity < vel_ref[mid] + dv))
      return mid;
    else if (vel_ref[mid] < velocity)
      low = mid + 1;
    else if (vel_ref[mid] > velocity)
      high = mid - 1;
  }
}
