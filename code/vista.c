/* $Header$
 * $Log$
 * Revision 1.9  1996/06/26 00:34:50  nsk
 * Added deuterium to absorb, HeI, HeII, and xray to vista
 *
 * Revision 1.8  1996/04/24  23:40:38  trq
 * balls_loaded variable to keep track of balls.
 * Fixed smoothing length bug.
 *
 * Revision 1.7  1995/07/24  20:51:17  trq
 * Fixed bug in load_cool.c.
 * vista now has a velocity cut.
 *
 * Revision 1.5  1995/06/06  17:48:01  trq
 * dump_pixmap.c: Cleaned up declarations.
 *
 * Added kd.c and smooth.c for variable smoothing; NOW WITH LOSER TREES.
 *
 * Revision 1.4  1995/03/02  17:30:29  nsk
 * changed absorption cross section tb be done by integral
 * added optical depth output and fixed bug in absorb
 * added stellar mass plot to view_star
 *
 * Revision 1.3  1995/02/17  21:58:49  trq
 * Vista: changed ssprintf -> sprintf
 *
 * heatcool: fixed some comments.
 *
 * Revision 1.2  1995/02/17  21:45:13  nsk
 * changed what hneut does so that it outputs nuetral weighted hfrac and temp as
 * well.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:29  trq
 * Import to CVS
 *
 * Revision 2.16  1994/12/02  02:39:50  nsk
 * added kludge factor to hneutral
 *
 * Revision 2.15  1994/12/01  23:15:57  trq
 * Fixed bug in initializing quantity[].
 *
 * Revision 2.14  1994/11/01  22:41:25  nsk
 * made smoothing better
 *
 * Revision 2.13  1994/11/01  19:19:40  nsk
 * allowed size of image to change
 *
 * Revision 2.12  1994/10/12  21:26:39  trq
 * Cleaned up mallocs.
 *
 * Revision 2.11  1994/09/22  22:12:47  nsk
 * fixed hneut
 *
 * Revision 2.10  1994/09/22  16:19:13  trq
 * Changed indexing of hneutral.
 *
 * Revision 2.9  1994/04/20  08:46:25  trq
 * Added title variable.
 *
 * Revision 2.7  93/10/26  11:24:27  trq
 * Slight restructure to prevent memory leaks.
 * 
 * Revision 2.6  1993/05/25  16:31:44  trq
 * Added dynamically allocated arrays
 *
 */
#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>

void
vista(job)
    char *job ;
{
    char command[MAXCOMM] ;
    char type[MAXCOMM] ;
    double size_pixel ;
    double size_pixel_2 ;
    double xmin ;
    double ymin ;
    struct gas_particle *gp ;
    struct star_particle *sp ;
    struct dark_particle *dp ;
    double pixel_pos[2] ;
    double part_pos[3] ;
    int i,j,k ;
    int kx,ky ;
    int kx_min,kx_max ;
    int ky_min,ky_max ;
    float **density;
    float **quantity;
    float **quantity2;
    double radius2 ;
    double distance_dim2() ;
    double hsmooth ;
    double thsmooth ;
    double hsmth2pi ;
    double kernel ;
    int vista_type ;
    char name[100] ;
    char name1[100] ;
    double pixel ;
    double low ;
    double high ;
    double c1 ;
    double distnorm ;
    double drw ;
    double luminosity ;
    int iwsm ;
    double star_lum_redshift() ;
    double wavelength ;
    double lum_xray ;
    int vista_size ;
    double msys ;
    double rsys ;
    double vsys ;
    int num_read ;
    double vel_min ;
    double vel_max ;
    int irep[MAXDIM];
    int autolim ;
    double vz ;
    double xray_const ;
    double e_lower, e_upper ;
    double t_lower, t_upper ;
    char *energy_const ;

    if(!ikernel_loaded){
	ikernel_load() ;
    }
    if (current_project){
	if ((num_read = sscanf(job,"%s %s %lf %lf %s %d %lf %lf",command,type,
		&low, &high,name, &vista_size, &vel_min, &vel_max)) >= 6) {
	      size_pixel = (dv2_x - dv1_x) / scaling / (double)vista_size ;
	      size_pixel_2 = size_pixel*size_pixel ;
	      xmin = dv1_x / scaling ;
	      ymin = dv1_y / scaling ;
	      if (!cool_loaded ){
		  load_cool() ;
	      }
	      if (!redshift_loaded ){
		  load_redshift() ;
	      }
	      if (strcmp(type,"density") == 0 || strcmp(type,"rho") == 0 ){
		  vista_type = RHO ;
	      }
	      else if(strcmp(type,"temperature") == 0 ||
		    strcmp(type,"temp") == 0){
		  vista_type = TEMP ;
	      }
	      else if(strcmp(type,"pressure") == 0 ||
		    strcmp(type,"press") == 0){
		  vista_type = PRESS ;
	      }
	      else if(strcmp(type,"cooling") == 0 || strcmp(type,"cool") == 0){
		  vista_type = TCOOL ;
	      }
	      else if(strcmp(type,"jeans") == 0){
		  vista_type = JEANS ;
	      }
	      else if(strcmp(type,"formstar") == 0 ||
		    strcmp(type,"fstar") == 0){
		  vista_type = FSTAR ;
	      }
	      else if(strcmp(type,"star") == 0 || strcmp(type,"stars") == 0){
		    vista_type = VSTAR ;
	      }
	      else if(strcmp(type,"lumi") == 0 ||
		      strcmp(type,"luminosityi") == 0  ||
		      strcmp(type,"lumv") == 0  ||
		      strcmp(type,"luminosityv") == 0  ||
		      strcmp(type,"lumb") == 0  ||
		      strcmp(type,"luminosityb") == 0  ||
		      strcmp(type,"lumu") == 0  ||
		        strcmp(type,"luminosityu") == 0 ||
		      strcmp(type,"lumuu") == 0  ||
		      strcmp(type,"luminosityuu") == 0){
		  vista_type = LUMSTAR ;
	      }
	      else if(strcmp(type,"dark") == 0 ){
		  vista_type = VDARK ;
	      }
	      else if(strcmp(type,"threedrho") == 0 ){
		  vista_type = TDRHO ;
	      }
	      else if(strcmp(type,"xray") == 0 || strcmp(type,"xraylum") == 0){
		  vista_type = XRAY ;
	      }
	      else if(strcmp(type,"hneutral") == 0 ||
		    strcmp(type,"hneut") == 0){
		  vista_type = HNEUT ;
	      }
	      else if(strcmp(type,"heI") == 0){
		  vista_type = HEI ;
	      }
	      else if(strcmp(type,"heI") == 0){
		  vista_type = HEII ;
	      }
	      else {
		  printf("<sorry, %s is not a proper type, %s>\n",type,title) ;
		  return ;
	      }
	      density = (float **)malloc(vista_size*sizeof(*density));
	      if(density == NULL)
		{
		  printf("<sorry, no memory for image, %s>\n",title) ;
		  return ;
		}
	      if(vista_type != RHO && vista_type != XRAY){
		  quantity = (float **)malloc(vista_size*sizeof(*quantity));
		  if(quantity == NULL)
		    {
		      free(density);
		      printf("<sorry, no memory for image, %s>\n",title) ;
		      return ;
		    }
	      }
	      *density = (float *)malloc(vista_size*
		    vista_size*sizeof(**density));
	      if(*density == NULL)
		{
		  free(density);
		  if(vista_type != RHO && vista_type != XRAY){
		      free(quantity);
		  }
		  printf("<sorry, no memory for image, %s>\n",title) ;
		  return ;
		}
	      if(vista_type != RHO && vista_type != XRAY){
		  *quantity = (float *)malloc(vista_size*
			vista_size*sizeof(**quantity));
		  if(*quantity == NULL)
		    {
		      free(density);
		      free(quantity);
		      free(*density);
		      printf("<sorry, no memory for image, %s>\n",title) ;
		      return ;
		    }
	      }
	      if(vista_type == HNEUT){
		  quantity2 = (float **)malloc(vista_size*sizeof(*quantity2));
		      if(quantity2 == NULL)
			{
			  free(density);
			  free(quantity);
			  free(*density);
			  printf("<sorry, no memory for image, %s>\n",title) ;
			  return ;
			}
                  *quantity2 = (float *)malloc(vista_size*
                        vista_size*sizeof(**quantity2));
                  if(*quantity2 == NULL)
                    {
                      free(density);
                      free(quantity);
                      free(*density);
                      free(*quantity);
                      free(quantity2);
                      printf("<sorry, no memory for image, %s>\n",title) ;
                      return ;
                    }
	      }
	      for(i = 1; i < vista_size; i++)
		{
		  density[i] = &density[0][i*vista_size];
		  if(vista_type != RHO && vista_type != XRAY)
		      quantity[i] = &quantity[0][i*vista_size];
		  if(vista_type == HNEUT)
		      quantity2[i] = &quantity2[0][i*vista_size];
		}
	    for(kx = 0; kx < vista_size; kx++){
		for(ky = 0; ky < vista_size; ky++){
		density[kx][ky] = 0.0 ;
		}
	    }
	    if(vista_type == TCOOL){
		if(!cooling_loaded){
		    cool_func() ;
		}
	    }
	    else if(vista_type == JEANS){
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		if(!epsgas_loaded){
		    load_epsgas() ;
		}
	    }
	    else if(vista_type == FSTAR){
		if(!starform_loaded){
		    starform_func() ;
		}
	    }
	    else if(strcmp(type,"lumi") == 0 ||
		    strcmp(type,"luminosityi") == 0  ||
		    strcmp(type,"lumv") == 0  ||
		    strcmp(type,"luminosityv") == 0  ||
		    strcmp(type,"lumb") == 0  ||
		    strcmp(type,"luminosityb") == 0  ||
		    strcmp(type,"lumu") == 0  ||
		    strcmp(type,"luminosityu") == 0 ||
		    strcmp(type,"lumuu") == 0  ||
		    strcmp(type,"luminosityuu") == 0){
		if (!redshift_loaded ){
		    load_redshift() ;
		}
		if(strcmp(type,"lumi") == 0 || strcmp(type,"luminosityi") == 0){
		    color_filter = IBAND ;
		    wavelength = 8800./(1.+redshift) ;
		}
		else if(strcmp(type,"lumv") == 0 || 
			strcmp(type,"luminosityv") == 0){
		    color_filter = VBAND ;
		    wavelength = 5500./(1.+redshift) ;
		}
		else if(strcmp(type,"lumb") == 0 || 
			strcmp(type,"luminosityb") == 0){
		    color_filter = BBAND ;
		    wavelength = 4400./(1.+redshift) ;
		}
		else if(strcmp(type,"lumu") == 0 || 
			strcmp(type,"luminosityu") == 0){
		    color_filter = UBAND ;
		    wavelength = 3360./(1.+redshift) ;
		}
		else if(strcmp(type,"lumuu") == 0 || 
			strcmp(type,"luminosityuu") == 0){
		    color_filter = UUBAND ;
		    wavelength = 1940./(1.+redshift) ;
		}
		if(wavelength > 8800 || wavelength < 1940){
		    printf("<sorry, redshift too large for") ;
		    printf(" color band, %s>\n",title);
		    free(density);
		    free(*density);
		    if(vista_type != RHO && vista_type != XRAY &&
		    	    vista_type != HNEUT){
		        free(quantity);
		        free(*quantity);
		    }
		    return ;
		}
		if(!lum_loaded){
		    star_lum(1.,1.) ;
		}
	    }
	    else if(vista_type == XRAY){
		xray_const = msolunit*MSOLG/(cosmof*kpcunit*KPCCM)/
			(cosmof*kpcunit*KPCCM)/(cosmof*kpcunit*KPCCM) ;
		xray_const *= fhydrogen*fhydrogen/MHYDR/MHYDR ;
		xray_const *= 4.2e-27*msolunit*MSOLG/1.e40 ;
                forever{
		   printf("<enter: energy_lower (KeV), energy_upper (Kev)>\n");
		   energy_const = my_gets(" ") ;
		   if(( sscanf(energy_const,"%lf %lf",&e_lower,
			&e_upper)) == 2){
		       break ;
		   }
		   printf("<sorry, inconsistent input, %s>\n",title) ;
	        }
                t_lower = e_lower*(1 + redshift)*1.6e-9/KBOLTZ ;
                t_upper = e_upper*(1 + redshift)*1.6e-9/KBOLTZ ;
	    }
	    else if(vista_type == HNEUT || vista_type == HEI || 
		    vista_type == HEI){
		autolim = NO ;
		if(num_read != 8){
		    vel_min = -HUGE ;
		    vel_max = HUGE ;
		    autolim = YES ;
		}
		if(!hneutral_loaded){
		    hneutral_func() ;
		}
		msys = msolunit/(cosmof*kpcunit*cosmof*kpcunit)*
			(MSOLG/(KPCCM*KPCCM*MHYDR)) ;
		rsys = cosmof*kpcunit/1.e3 ;
		vsys = cosmof*sqrt(msolunit/kpcunit*(GCGS*MSOLG/KPCCM))/1.e5 ;
	    }
	    if(vista_type == RHO || vista_type == TEMP || vista_type == PRESS ||
		    vista_type == TCOOL || vista_type == JEANS || 
		    vista_type == FSTAR || vista_type == XRAY ||
		    vista_type == HNEUT || vista_type == HEI ||
		    vista_type == HEII){
		if(vista_type != RHO && vista_type != XRAY){
		    for(kx = 0; kx < vista_size; kx++){
			for(ky = 0; ky < vista_size; ky++){
			quantity[kx][ky] = 0.0 ;
			}
		    }
		}
		if(vista_type == HNEUT){
		    for(kx = 0; kx < vista_size; kx++){
			for(ky = 0; ky < vista_size; ky++){
			quantity2[kx][ky] = 0.0 ;
			}
		    }
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    if(vista_type != XRAY || (gp->temp) > 30000.){
			if(vista_type == XRAY){
			    lum_xray = xray_const*sqrt(gp->temp)*
				    (exp(-t_lower/(gp->temp)) - 
				    exp(-t_upper/(gp->temp))) ;
			    lum_xray *= (gp->rho)*(gp->mass);
			}
			for(irep[0] = -1; irep[0] <= 1; irep[0]++) {
			  for(irep[1] = -1; irep[1] <= 1; irep[1]++) {
			    for(irep[2] = -1; irep[2] <= 1; irep[2]++) {
			      if(periodic || (irep[0] == 0 && irep[1] == 0 && 
					irep[2] == 0)) {
				for (part_pos[0] = part_pos[1] = 
					part_pos[2] = 0.0,j = 0 ; j <
					header.ndim ;j++) {
				    for (k = 0 ; k < 3 ; k++){
					part_pos[k] += rot_matrix[k][j] *
					  (irep[j]*period_size + gp->pos[j]
					   - boxes[active_box].center[j]) ;
				    }
				}
				if((vista_type == HNEUT || vista_type == HEI ||
					vista_type == HEII) && autolim == NO){
				    for (vz = 0.0,j = 0;j < header.ndim ;j++) {
					vz += rot_matrix[2][j] * (gp->vel[j]) ;
				    }
				    vz *= vsys ;
				    if(comove == YES){
					vz += hubble_constant*part_pos[2]*rsys ;
				    }
				}
				if((!periodic && ((vista_type != HNEUT &&
					vista_type == HEI && vista_type == HEII)
					      || autolim == YES))
				   || (periodic && ((vista_type != HNEUT &&
					vista_type == HEI && vista_type == HEII)
						    || autolim == YES)
				       && (part_pos[2]< period_size/2.&&
					   part_pos[2] >= -period_size/2.))
				   || ((vista_type == HNEUT || vista_type == HEI
				   || vista_type == HEII) && autolim == NO
				       && vz >= vel_min && vz <= vel_max)) {
				hsmooth = gp->hsmooth ;
				if(hsmooth > size_pixel){
				  thsmooth = 2. * hsmooth ;
				  distnorm = 1. / (hsmooth * hsmooth) ;
				  hsmth2pi = distnorm / PI ;
				  kx_min = max(0,(int)((part_pos[0]-thsmooth-
					xmin)/size_pixel + .499999)) ;
				  kx_max = min(vista_size-1,(int)((part_pos[0]+
					thsmooth-xmin)/size_pixel + .499999)) ;
				  ky_min = max(0,(int)((part_pos[1]-thsmooth-
					ymin)/size_pixel + .499999)) ;
				  ky_max = min(vista_size-1,(int)((part_pos[1]+
				        thsmooth-ymin)/ size_pixel + .499999)) ;
				    for(kx = kx_min; kx < kx_max + 1; kx++){
			              for(ky = ky_min; ky < ky_max + 1; ky++){
				        pixel_pos[0] = xmin+(kx + .5) *
						size_pixel ;
					pixel_pos[1] = ymin+(ky + .5) * 
						size_pixel ;
					radius2 = distance_dim2(pixel_pos,
						part_pos)*distnorm ;
					if(radius2 < 4.){
					    radius2 *= deldr2i ;
					    iwsm = (int)radius2 ;
					    drw = radius2 - (double)iwsm ;
					    kernel = ((1. - drw)*iwsmooth[iwsm]+
						    drw*iwsmooth[1+iwsm])*
						    hsmth2pi ;
					}
					else{
					    kernel = 0. ;
					}
					kernel *= size_pixel_2 ;
					if(kernel != 0.){
					    if(vista_type == XRAY){
						density[kx][ky]+=(float)(kernel*
						    lum_xray) ;
					    }
					    else if(vista_type == HNEUT){
						density[kx][ky]+=(float)(kernel*
						    msys*fhydrogen*
						    hneutral[gp-gas_particles]*
						    (gp->mass));
					    }
					    else if(vista_type == HEI){
						density[kx][ky]+=(float)(kernel*
						    msys*(1.-fhydrogen)*
						    heneutral[gp-gas_particles]*
						    (gp->mass));
					    }
					    else if(vista_type == HEII){
						density[kx][ky]+=(float)(kernel*
						    msys*(1.-fhydrogen)*
						    heII[gp-gas_particles]*
						    (gp->mass));
					    }
					    else {
						density[kx][ky]+=(float)(kernel*
						    (gp->mass)) ;
					    }
					    if(vista_type == TEMP){
						quantity[kx][ky] += 
						    (float)(kernel*
						    (gp->mass)*(gp->temp)) ;
					    }
					    else if(vista_type == PRESS){
						quantity[kx][ky] +=
						    (float)(kernel*
						    (gp->mass)*(gp->temp)*
						    (gp->rho)) ;
					    }
					    else if(vista_type == TCOOL){
						if(cooling[i] < 0.){
						    quantity[kx][ky] += 
						    (float)(kernel*
						    gp->mass * -cooling[i]) ;
						}
					    }
					    else if(vista_type == JEANS){
					      double tconst = meanmwt[i]*MHYDR
						*GCGS/KBOLTZ*msolunit*MSOLG
						  /kpcunit/KPCCM;
					      c1 = sqrt(tconst/GAMMA * 4. *PI) ;
						quantity[kx][ky] +=
						    (float)(kernel*
						    gp->mass * gp->hsmooth*c1*
						    sqrt(gp->rho / gp->temp));
					    }
					    else if(vista_type == FSTAR){
						quantity[kx][ky] +=
						    (float)(kernel*
						    gp->mass * starform[i]);
					    }
					    else if(vista_type == HNEUT){
						quantity[kx][ky] +=
						    (float)(kernel*
						    msys*fhydrogen*
						    hneutral[gp-gas_particles]*
						    hneutral[gp-gas_particles]*
						    (gp->mass));
						quantity2[kx][ky] +=
						    (float)(kernel*
						    msys*fhydrogen*
						    hneutral[gp-gas_particles]*
						    (gp->mass)*(gp->temp));
					    }
					}
				      }
				    }
				}
				else{
				    kx = (int)((part_pos[0]-xmin)/size_pixel+
					    0.499999) ;
				    ky = (int)((part_pos[1]-ymin)/size_pixel+
					    0.499999) ;
				    if(kx >= 0 && kx < vista_size && ky >= 0 &&
					    ky < vista_size){
					if(vista_type == XRAY){
					    density[kx][ky]+=(float)(lum_xray);
					}
					else if(vista_type == HNEUT){
						density[kx][ky] += (float)(msys*
						    fhydrogen*
						    hneutral[gp-gas_particles]*
						    (gp->mass));
					}
					else if(vista_type == HEI){
						density[kx][ky] += (float)(msys*
						    (1.-fhydrogen)*
						    heneutral[gp-gas_particles]*
						    (gp->mass));
					}
					else if(vista_type == HEII){
						density[kx][ky] += (float)(msys*
						    (1.-fhydrogen)*
						    heII[gp-gas_particles]*
						    (gp->mass));
					}
					else {
					    density[kx][ky] += 
					    	(float)((gp->mass)) ;
					}
					if(vista_type == TEMP){
					    quantity[kx][ky] += (float)(
						(gp->mass)*(gp->temp)) ;
					}
					else if(vista_type == PRESS){
					    quantity[kx][ky] += (float)(
						(gp->mass)*(gp->temp)*
						(gp->rho)) ;
					}
					else if(vista_type == TCOOL){
					    if(cooling[i] < 0.){
						quantity[kx][ky] += 
							(float)(gp->mass *
							-cooling[i]) ;
					    }
					}
					else if(vista_type == JEANS){
					  double tconst = meanmwt[i]*MHYDR
					    *GCGS/KBOLTZ*msolunit*MSOLG
					      /kpcunit/KPCCM;
					  c1 = sqrt(tconst / GAMMA * 4. *PI) ;
					    quantity[kx][ky] +=(float)(
					    gp->mass * gp->hsmooth * c1 *
					    sqrt(gp->rho / gp->temp));
					}
					else if(vista_type == FSTAR){
					    quantity[kx][ky] +=(float)(
					    gp->mass * starform[i]);
					}
					else if(vista_type == HNEUT){
						quantity[kx][ky] +=
						    (float)(msys*fhydrogen*
						    hneutral[gp-gas_particles]*
						    hneutral[gp-gas_particles]*
						    (gp->mass));
						quantity2[kx][ky] +=
						    (float)(msys*fhydrogen*
						    hneutral[gp-gas_particles]*
						    (gp->mass)*(gp->temp));
					}
				    }
				}
			      }
			      }
			    }
			  }
			}
		    }
		}
		if(vista_type != RHO && vista_type != XRAY &&
			 vista_type != HNEUT && vista_type != HEI &&
			 vista_type != HEII){
		    for(kx = 0; kx < vista_size; kx++){
			for(ky = 0; ky < vista_size; ky++){
			    if(density[kx][ky] != 0.){
				density[kx][ky] = quantity[kx][ky] /
					density[kx][ky] ;
			    }
			    else{
				density[kx][ky] = 0. ;
			    }
			}
		    }
		}
		if(vista_type == HNEUT){
		    for(kx = 0; kx < vista_size; kx++){
			for(ky = 0; ky < vista_size; ky++){
			    if(density[kx][ky] != 0.){
				quantity[kx][ky] = quantity[kx][ky] /
					density[kx][ky] ;
				quantity2[kx][ky] = quantity2[kx][ky] /
					density[kx][ky] ;
			    }
			    else{
				quantity[kx][ky] = 0. ;
				quantity2[kx][ky] = 0. ;
			    }
			}
		    }
		}
	    }
	    else if(vista_type == VSTAR){
		if(balls_loaded != STAR) {
		    calc_balls(&box0_smx, 0, 0 , 1);
		    balls_loaded = STAR;
		}
		for (i = 0 ;i < boxlist[active_box].nstar ;i++) {
		    sp = boxlist[active_box].sp[i] ;
		    for(irep[0] = -1; irep[0] <= 1; irep[0]++) {
		      for(irep[1] = -1; irep[1] <= 1; irep[1]++) {
			for(irep[2] = -1; irep[2] <= 1; irep[2]++) {
			  if(periodic || (irep[0] == 0 && irep[1] == 0 &&
				irep[2] == 0)) {
			    for (part_pos[0] = part_pos[1] = part_pos[2] = 0.0,
				    j = 0 ; j < header.ndim ;j++) {
				for (k = 0 ; k < 3 ; k++){
				    part_pos[k] += rot_matrix[k][j] *
				      (irep[j]*period_size + sp->pos[j]
				       - boxes[active_box].center[j]) ;
				}
			    }
			    if(!(periodic) || (part_pos[2] < period_size/2.
				    && part_pos[2] >= -period_size/2.)){
			    hsmooth = sqrt(box0_smx->kd->p[i].fBall2)/2.0;
			    if(hsmooth > size_pixel){
				thsmooth = 2. * hsmooth ;
				distnorm = 1. / (hsmooth * hsmooth) ;
				hsmth2pi = distnorm / PI ;
				kx_min=max(0,(int)((part_pos[0]-thsmooth-xmin)/
					size_pixel + .499999)) ;
				kx_max = min(vista_size-1,(int)((part_pos[0]+
					thsmooth-xmin)/size_pixel + .499999)) ;
				ky_min=max(0,(int)((part_pos[1]-thsmooth-ymin)/
				    size_pixel + .499999)) ;
				ky_max = min(vista_size-1,(int)((part_pos[1]+
					thsmooth- ymin)/ size_pixel+.499999)) ;
				for(kx = kx_min; kx < kx_max + 1; kx++){
				    for(ky = ky_min; ky < ky_max + 1; ky++){
					pixel_pos[0] = xmin+(kx + .5) *
						size_pixel ;
					pixel_pos[1] = ymin+(ky + .5) *
					        size_pixel ;
					radius2 = distance_dim2(pixel_pos,
						part_pos)*distnorm ;
					if(radius2 < 4.){
					    radius2 *= deldr2i ;
					    iwsm = (int)radius2 ;
					    drw = radius2 - (double)iwsm ;
					    kernel = ((1. - drw) *
						    iwsmooth[iwsm] + drw*
						    iwsmooth[1+iwsm])*hsmth2pi ;
					}
					else{
					    kernel = 0. ;
					}
					kernel *= size_pixel_2 ;
					if(kernel != 0.){
						density[kx][ky]+=(float)(kernel*
						    (sp->mass)) ;
					}
				    }
				}
			    }
			    else{
				kx = (int)((part_pos[0]-xmin)/size_pixel+
					0.499999) ;
				ky = (int)((part_pos[1]-ymin)/size_pixel+
					0.499999) ;
				if(kx >= 0 && kx < vista_size && ky >= 0 &&
					ky < vista_size){
					density[kx][ky] += (float)((sp->mass)) ;
				}
			    }
			  }
			  }
			}
	              }
	            }
		}
	    }
	    else if(vista_type == LUMSTAR){
		if(balls_loaded != STAR) {
		    calc_balls(&box0_smx, 0, 0 , 1);
		    balls_loaded = STAR;
		}
		for (i = 0 ;i < boxlist[active_box].nstar ;i++) {
		    sp = boxlist[active_box].sp[i] ;
		    luminosity = star_lum_redshift(sp->mass,sp->tform,
			    wavelength) ;
		    for(irep[0] = -1; irep[0] <= 1; irep[0]++) {
		      for(irep[1] = -1; irep[1] <= 1; irep[1]++) {
			for(irep[2] = -1; irep[2] <= 1; irep[2]++) {
			  if(periodic || (irep[0] == 0 && irep[1] == 0 &&
				irep[2] == 0)) {
			    for (part_pos[0] = part_pos[1] = part_pos[2] = 0.0,
				    j = 0 ; j < header.ndim ;j++) {
				for (k = 0 ; k < 3 ; k++){
				    part_pos[k] += rot_matrix[k][j] *
				      (irep[j]*period_size + sp->pos[j]
				       - boxes[active_box].center[j]) ;
				}
			    }
			    if(!(periodic) || (part_pos[2] < period_size/2.
				    && part_pos[2] >= -period_size/2.)){
			    hsmooth = sqrt(box0_smx->kd->p[i].fBall2)/2.0;
			    if(hsmooth > size_pixel){
				thsmooth = 2. * hsmooth ;
				distnorm = 1. / (hsmooth * hsmooth) ;
				hsmth2pi = distnorm / PI ;
				kx_min=max(0,(int)((part_pos[0]-thsmooth-xmin)/
					size_pixel + .499999)) ;
				kx_max = min(vista_size-1,(int)((part_pos[0]+
					thsmooth-xmin)/size_pixel + .499999)) ;
				ky_min=max(0,(int)((part_pos[1]-thsmooth-ymin)/
				    size_pixel + .499999)) ;
				ky_max = min(vista_size-1,(int)((part_pos[1]+
					thsmooth-ymin)/ size_pixel + .499999)) ;
				for(kx = kx_min; kx < kx_max + 1; kx++){
				    for(ky = ky_min; ky < ky_max + 1; ky++){
					pixel_pos[0] = xmin+(kx + .5) *
						size_pixel ;
					pixel_pos[1] = ymin+(ky + .5) *
						size_pixel ;
					radius2 = distance_dim2(pixel_pos,
						part_pos)*distnorm ;
					if(radius2 < 4.){
					    radius2 *= deldr2i ;
					    iwsm = (int)radius2 ;
					    drw = radius2 - (double)iwsm ;
					    kernel = ((1. - drw) *
						    iwsmooth[iwsm] + drw*
						    iwsmooth[1+iwsm])*hsmth2pi ;
					}
					else{
					    kernel = 0. ;
					}
					kernel *= size_pixel_2 ;
					if(kernel != 0.){
						density[kx][ky]+=(float)(kernel*
						    luminosity) ;
					}
				    }
				}
			    }
			    else{
				kx = (int)((part_pos[0]-xmin)/size_pixel+
					0.499999) ;
				ky = (int)((part_pos[1]-ymin)/size_pixel+
					0.499999) ;
				if(kx >= 0 && kx < vista_size && ky >= 0 &&
					ky < vista_size){
					density[kx][ky] += (float)(luminosity) ;
				}
			    }
			  }
			  }
			}
		      }
		    }
		}
	    }
	    else if(vista_type == VDARK){
		if(balls_loaded != DARK) {
		    calc_balls(&box0_smx, 1, 0 , 0);
		    balls_loaded = DARK;
		}
		for (i = 0 ;i < boxlist[active_box].ndark ;i++) {
		    dp = boxlist[active_box].dp[i] ;
		    for(irep[0] = -1; irep[0] <= 1; irep[0]++) {
		      for(irep[1] = -1; irep[1] <= 1; irep[1]++) {
			for(irep[2] = -1; irep[2] <= 1; irep[2]++) {
			  if(periodic || (irep[0] == 0 && irep[1] == 0 &&
				irep[2] == 0)) {
			    for (part_pos[0] = part_pos[1] = part_pos[2] = 0.0,
				    j = 0 ; j < header.ndim ;j++) {
				for (k = 0 ; k < 3 ; k++){
				    part_pos[k] += rot_matrix[k][j] *
				      (irep[j]*period_size + dp->pos[j]
				       - boxes[active_box].center[j]) ;
				}
			    }
			    if(!(periodic) || (part_pos[2] < period_size/2.
				    && part_pos[2] >= -period_size/2.)){
			    hsmooth = sqrt(box0_smx->kd->p[i].fBall2)/2.0;
			    if(hsmooth > size_pixel){
				thsmooth = 2. * hsmooth ;
				distnorm = 1. / (hsmooth * hsmooth) ;
				hsmth2pi = distnorm / PI ;
				kx_min=max(0,(int)((part_pos[0]-thsmooth-xmin)/
					size_pixel + .499999)) ;
				kx_max = min(vista_size-1,(int)((part_pos[0]+
					thsmooth-xmin)/size_pixel + .499999)) ;
				ky_min=max(0,(int)((part_pos[1]-thsmooth-ymin)/
				    size_pixel + .499999)) ;
				ky_max = min(vista_size-1,(int)((part_pos[1]+
					thsmooth-ymin)/ size_pixel + .499999)) ;
				for(kx = kx_min; kx < kx_max + 1; kx++){
				    for(ky = ky_min; ky < ky_max + 1; ky++){
					pixel_pos[0] = xmin+(kx + .5) *
						size_pixel ;
					pixel_pos[1] = ymin+(ky + .5) *
						size_pixel ;
					radius2 = distance_dim2(pixel_pos,
						part_pos)*distnorm ;
					if(radius2 < 4.){
					    radius2 *= deldr2i ;
					    iwsm = (int)radius2 ;
					    drw = radius2 - (double)iwsm ;
					    kernel = ((1. - drw) *
						    iwsmooth[iwsm] + drw*
						    iwsmooth[1+iwsm])*hsmth2pi ;
					}
					else{
					    kernel = 0. ;
					}
					kernel *= size_pixel_2 ;
					if(kernel != 0.){
						density[kx][ky]+=(float)(kernel*
						    (dp->mass)) ;
					}
				    }
				}
			    }
			    else{
				kx = (int)((part_pos[0]-xmin)/size_pixel+
					0.499999) ;
				ky = (int)((part_pos[1]-ymin)/size_pixel+
					0.499999) ;
				if(kx >= 0 && kx < vista_size && ky >= 0 &&
					ky < vista_size){
					density[kx][ky] += (float)((dp->mass)) ;
				}
			    }
			  }
			  }
			}
		      }
		    }
		}
	    }
	    else if(vista_type == TDRHO) {
		for(kx = 0; kx < vista_size; kx++){
		    for(ky = 0; ky < vista_size; ky++){
			density[kx][ky] = 0.0 ;
		    }
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    for (part_pos[0] = part_pos[1] = 0.0,j = 0 ;
			    j < header.ndim ;j++) {
			part_pos[0] += rot_matrix[0][j] * (gp->pos[j] -
				boxes[active_box].center[j]) ;
			part_pos[1] += rot_matrix[1][j] * (gp->pos[j] -
				boxes[active_box].center[j]) ;
		    }
		    kx = (int)((part_pos[0]-xmin)/size_pixel + .499999) ;
		    ky = (int)((part_pos[1]-ymin)/size_pixel + .499999) ;
		    if(kx >= 0 && kx < vista_size && ky >= 0 &&
			    ky < vista_size){
			density[kx][ky] += gp->rho ;
		    }
		}
	    }
	    for(i = 0; i < vista_size; i++){
		for(j = 0; j < vista_size; j++){
		    if(density[i][j] > 0. ){
			if(vista_type != TEMP && vista_type != PRESS &&
				vista_type != TCOOL && vista_type != JEANS &&
				vista_type != TDRHO && vista_type != FSTAR &&
				vista_type != XRAY) {
			    pixel = log10((double)(density[i][j])/size_pixel_2);
			}
			else{
			    pixel = log10((double)(density[i][j]));
			    if(vista_type == XRAY){
				pixel += 40. ;
			    }
			}
		    }
		    else{
			pixel = low ;
		    }
		    if(pixel > high)pixel = high ;
		    if(pixel < low)pixel = low ;
		    density[i][j] = (float)pixel ;
		}
	    }
	    fits(density,vista_size,vista_size,xmin,ymin,size_pixel,
		    size_pixel,low,high,name) ;
	    if(vista_type == HNEUT){
                low = -20.0 ;
                high = 0.0 ;
		for(i = 0; i < vista_size; i++){
		    for(j = 0; j < vista_size; j++){
			if(quantity[i][j] > 0. ){
			    pixel = log10((double)(quantity[i][j]));
			}
			else{
			    pixel = low ;
			}
			if(pixel > high)pixel = high ;
			if(pixel < low)pixel = low ;
			quantity[i][j] = (float)pixel ;
		    }
		}
                sprintf(name1,"%s.frac",name) ;
		fits(quantity,vista_size,vista_size,xmin,ymin,size_pixel,
			size_pixel,low,high,name1) ;
                sprintf(name1,"%s.temp",name) ;
                low = 0.0 ;
                high = 10.0 ;
		for(i = 0; i < vista_size; i++){
		    for(j = 0; j < vista_size; j++){
			if(quantity2[i][j] > 0. ){
			    pixel = log10((double)(quantity2[i][j]));
			}
			else{
			    pixel = low ;
			}
			if(pixel > high)pixel = high ;
			if(pixel < low)pixel = low ;
			quantity2[i][j] = (float)pixel ;
		    }
		}
		fits(quantity2,vista_size,vista_size,xmin,ymin,size_pixel,
			size_pixel,low,high,name1) ;
            }
	    free(density);
	    free(*density);
	    if(vista_type != RHO && vista_type != XRAY){
		free(quantity);
		free(*quantity);
	    }
	    if(vista_type == HNEUT){
		free(quantity2);
		free(*quantity2);
	    }
	}
	else {
	    input_error(command) ;
	}
    }
    else {
	printf("<plot does not represent the present internal state, %s>",
		title) ;
    }
}
