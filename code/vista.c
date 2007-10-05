/* $Header$
 * $Log$
 * Revision 1.25  2007/10/05 19:11:08  trq
 * Adrienne Stilp:
 *
 * Added "cube" command to produce velocity cubes for HI. (cube.c, comm.h,
 * arguments.c, Makefile.in)
 *
 * fits.c:  added comments and fits3d subroutine.
 *
 * neutral.c: added option to read in ionization fractions from gasoline
 * outputs.
 *
 * vista.c: automatically add "fits" to file names if not present.
 *
 * Revision 1.24  2007/08/03 18:08:11  trq
 * Fixed bug in 2nd moment calculation; added diagnostic prints.
 *
 * Revision 1.23  2007/08/02 22:55:11  trq
 * Added v^2 map to HNEUT_VEL.
 *
 * Revision 1.22  2007/08/02 19:31:28  trq
 * Added a "hneutralvel" plot to vista.  It produces the 1st moment of the
 * HI velocity field.
 *
 * Revision 1.21  2005/03/18 13:03:21  trq
 * Load redshift info if needed.
 *
 * Revision 1.20  2001/07/11 19:45:54  nsk
 *       Fixed bugs with array sizes for meanmwt, cooling, and starformation.
 *       Used to be only for active box now for all of box zero.
 *
 * Revision 1.19  2000/01/12  22:55:28  nsk
 * Fixed bugs in cooling routines, added cooling damping,
 * fixed bugs in starformation,  regularized dependencies.
 *
 * Revision 1.18  1999/08/25  22:05:30  nsk
 * added center to boxstat, checks for periodic in smooth, prints out
 * cooling stuff, vista makes plots
 *
 * Revision 1.17  1998/06/11  20:26:28  trq
 * Allow vista to run without a display: check for current project and
 * set appropriate variables.
 *
 * Revision 1.16  1998/03/28  02:09:12  trq
 * Fixed bug in determining smoothing length for dark and star particles.
 *
 * Revision 1.15  1998/03/27  17:12:12  trq
 * Free memory in correct order.
 *
 * Revision 1.14  1997/12/18  17:57:14  trq
 * Added VALL vista plot: total mass density.
 *
 * Revision 1.13  1997/10/24  22:14:45  trq
 * Fixed bug in handling hsmooth < size_pixel.
 *
 * Revision 1.12  1997/09/25  21:46:09  trq
 * Fix bug in summing "quantity".
 *
 * Revision 1.11  1997/09/05  01:02:25  nsk
 * streamlined vista (I hope it works), added neutralize command, added SZ
 * effect to vista.
 *
 * Revision 1.10  1996/07/30  22:20:25  trq
 * Fixed HeII logic bugs.
 *
 * Revision 1.9  1996/06/26  00:34:50  nsk
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
#include "smooth.h"
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
    double xmax ;
    double ymax ;
    struct gas_particle *gp ;
    struct star_particle *sp ;
    struct dark_particle *dp ;
    double pixel_pos[2] ;
    double part_pos[3] ;
    int i,j,k ;
    int kx,ky ;
    int kx_min,kx_max ;
    int ky_min,ky_max ;
    float **density;		/* array of pixels, also used to
				   normalize other arrays */
    float **quantity;		/* auxillary arrays */
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
    char base[100];              /* Holds filename (without .fits) */
    int nameLength;
    double pixel ;
    double low ;
    double high ;
    double pixmax;
    double pixmin;
    double c1 ;
    double c2 ;
    double rsz ;
    double c1_i ;
    double tconst ;
    double distnorm ;
    double drw ;
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
    double v_hubble ;
    double xray_const ;
    double e_lower, e_upper ;
    double t_lower, t_upper ;
    char *energy_const ;
    double delta_d ;
    double delta_q ;
    double delta_q2 ;
    double electron ;
    double cool_vec[COOLVECSIZE] ;
    double cool_tot ;
    double vol ;
    int aoffset;
    double hneut_velmin;
    double hneut_velmax;
    double hneut_vel2max;

    if(!ikernel_loaded){
	ikernel_load() ;
    }
    if (!current_project){
	dv1_x = -INTMAX ;
	dv1_y = -INTMAX ;
	dv2_x = INTMAX ;
	dv2_y = INTMAX ;
	view_size = INTMAX ;
	zoom_factor = 1.0;
	scaling = 2. * (double)INTMAX / boxes[active_box].size ;
    }
    /*
     * Parse command arguments
     */
	if ((num_read = sscanf(job,"%s %s %lf %lf %s %d %lf %lf",command,type,
		&low, &high,name, &vista_size, &vel_min, &vel_max)) >= 6) {
	      if (!redshift_loaded ){
		  load_redshift();
	      }



	      /* 
	       * Remove ".fits" if given.. will be appended later.
	       */
	      nameLength = strlen(name);
	      if (nameLength > 5) {
		if (name[nameLength-5] == '.' && name[nameLength-4] == 'f' && 
		    name[nameLength-3] == 'i' && name[nameLength-2] == 't' &&
		    name[nameLength-1] == 's') {
		  strncpy(base,name,nameLength-6);
		  base[nameLength-5] = '\0';
		}
		else {
		  strcpy(base, name);
		}
	      }
	      else {
		strcpy(base, name);
	      }
	      printf("%s\n", base);

	      size_pixel = (dv2_x - dv1_x) / scaling / (double)vista_size ;
	      size_pixel_2 = size_pixel*size_pixel ;
	      xmin = dv1_x / scaling ;
	      ymin = dv1_y / scaling ;
	      xmax = (dv2_x+1) / scaling ;
	      ymax = (dv2_y+1) / scaling ;
	      if (!cool_loaded ){
		  load_cool() ;
	      }
	      /*
	       * discover plot type
	       */
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
		  vista_type = COOL ;
	      }
	      else if(strcmp(type,"lycooling") == 0 ||
		    strcmp(type,"lycool") == 0){
		  vista_type = LYA ;
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
	      else if(strcmp(type,"all") == 0 ){
		  vista_type = VALL ;
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
	      else if(strcmp(type,"hneutralvel") == 0 ||
		    strcmp(type,"hneutv") == 0){
		  vista_type = HNEUT_VEL ;
	      }
	      else if(strcmp(type,"heI") == 0){
		  vista_type = HEI ;
	      }
	      else if(strcmp(type,"heII") == 0){
		  vista_type = HEII ;
	      }
	      else if(strcmp(type,"sz") == 0){
		  vista_type = SZ ;
	      }
	      else if(strcmp(type,"array") == 0){
		  vista_type = ARRAY ;
	      }
	      else {
		  printf("<sorry, %s is not a proper type, %s>\n",type,title) ;
		  return ;
	      }
	      /*
	       * Allocate memory
	       */
	      density = (float **)malloc(vista_size*sizeof(*density));
	      if(density == NULL)
		{
		  printf("<sorry, no memory for image, %s>\n",title) ;
		  return ;
		}
	      *density = (float *)malloc(vista_size*
		    vista_size*sizeof(**density));
	      if(*density == NULL)
		{
		  free(density);
		  printf("<sorry, no memory for image, %s>\n",title) ;
		  return ;
		}
	      /*
	       * Set up row pointers
	       */
	      for(i = 1; i < vista_size; i++)
		  density[i] = &density[0][i*vista_size];
	      /*
	       * allocate auxillary arrays
	       */
	      if(vista_type != RHO && vista_type != XRAY && vista_type
		 != VDARK && vista_type != VSTAR && vista_type != VALL
		 && vista_type != LUMSTAR && vista_type != COOL &&
		 vista_type != LYA && vista_type != ARRAY){
		  quantity = (float **)malloc(vista_size*sizeof(*quantity));
		  if(quantity == NULL)
		    {
		      free(density);
		      printf("<sorry, no memory for image, %s>\n",title) ;
		      return ;
		    }
		  *quantity = (float *)malloc(vista_size*
			vista_size*sizeof(**quantity));
		  if(*quantity == NULL)
		    {
		      free(*density);
		      free(density);
		      free(quantity);
		      printf("<sorry, no memory for image, %s>\n",title) ;
		      return ;
		    }
		  for(i = 1; i < vista_size; i++)
		      quantity[i] = &quantity[0][i*vista_size];
		  for(kx = 0; kx < vista_size; kx++){
			for(ky = 0; ky < vista_size; ky++){
			    quantity[kx][ky] = 0.0 ;
			}
		  }
		  
	      }
	      /*
	       * some require a second auxilary array
	       */
	      if(vista_type == HNEUT || vista_type == HNEUT_VEL){
		  quantity2 = (float **)malloc(vista_size*sizeof(*quantity2));
		      if(quantity2 == NULL)
			{
			  free(*density);
			  free(density);
			  free(*quantity);
			  free(quantity);
			  printf("<sorry, no memory for image, %s>\n",title) ;
			  return ;
			}
                  *quantity2 = (float *)malloc(vista_size*
                        vista_size*sizeof(**quantity2));
                  if(*quantity2 == NULL)
                    {
                      free(*density);
                      free(density);
                      free(*quantity);
                      free(quantity);
                      free(quantity2);
                      printf("<sorry, no memory for image, %s>\n",title) ;
                      return ;
                    }
		  for(i = 1; i < vista_size; i++)
		      quantity2[i] = &quantity2[0][i*vista_size];
		  for(kx = 0; kx < vista_size; kx++){
			for(ky = 0; ky < vista_size; ky++){
			    quantity2[kx][ky] = 0.0 ;
			}
		  }
	      }
	      /* initialize */
	    for(kx = 0; kx < vista_size; kx++){
		for(ky = 0; ky < vista_size; ky++){
		density[kx][ky] = 0.0 ;
		}
	    }
	    if(vista_type == LYA || vista_type == COOL){
		if (!cool_loaded ){
		    load_cool() ;
		}
		if (!uv_loaded ){
		    load_uv() ;
		}
	    }
	    else if(vista_type == JEANS){
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		if(!epsgas_loaded){
		    load_epsgas() ;
		}
		tconst = MHYDR*GCGS/KBOLTZ*msolunit*MSOLG/kpcunit/KPCCM;
		c1 = sqrt(tconst/GAMMA * 4. *PI) ;
	    }
	    else if(vista_type == FSTAR){
		if(!starform_loaded){
		    starform_func() ;
		}
	    }
	    else if(vista_type == LUMSTAR){
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
		    free(*density);
		    free(density);
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
	    /*
	     * select velocity channels if requested.
	     */
	    else if(vista_type == HNEUT || vista_type == HEI || 
		    vista_type == HEII || vista_type == SZ ||
		    vista_type == HNEUT_VEL){
		autolim = NO ;
		if(num_read != 8){
		    /* get all velocities by default */
		    vel_min = -HUGE ;
		    vel_max = HUGE ;
		    autolim = YES ;
		}
		if(!hneutral_loaded){
		    hneutral_func() ;
		}
		/* convert to real units */
		msys = msolunit/(cosmof*kpcunit*cosmof*kpcunit)*
			(MSOLG/(KPCCM*KPCCM*MHYDR)) ;
		rsys = cosmof*kpcunit/1.e3 ;
		vsys = cosmof*sqrt(msolunit/kpcunit*(GCGS*MSOLG/KPCCM))/1.e5 ;
		/* determine constants */
		if(vista_type == HNEUT || vista_type == HNEUT_VEL){
		    c1 = msys*fhydrogen ;
		}
		else if(vista_type == HEI || vista_type == HEII){
		    c1 = msys*(1.-fhydrogen) ;
		}
		else if(vista_type == SZ){
		    rsz = (1.-fhydrogen)/4./fhydrogen ;
		    c1 = msys*fhydrogen*SIGMAES*KBOLTZ/ME/C/C ;
		    c2 = msys*fhydrogen*SIGMAES*1.e5/C ;
		}
	    }
	    else if(vista_type == ARRAY){
		if(array_size == 0){
		    printf("<Sorry %s, there is no array loaded\n",title);
		    return;
                }
	    }
	    if(vista_type == RHO || vista_type == TEMP || vista_type == PRESS ||
		    vista_type == COOL || vista_type == JEANS || 
		    vista_type == FSTAR || vista_type == XRAY ||
		    vista_type == HNEUT || vista_type == HEI ||
		    vista_type == HEII || vista_type == SZ ||
		    vista_type == VALL || vista_type == LYA ||
		    vista_type == ARRAY || vista_type == HNEUT_VEL){
		if(vista_type == LYA || vista_type == COOL){
		    c1 = cosmof3*kpcunit*kpcunit*kpcunit*KPCCM*KPCCM*KPCCM ;
		}
		/* calculate smoothing length if needed */
		if(balls_loaded != GAS) {
		    calc_balls(&box0_smx, 0, 1 , 0);
		    balls_loaded = GAS;
		}
		/* loop over particles */
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    int gpi;
		    
		    gp = boxlist[active_box].gp[i] ;
		    gpi = boxlist[active_box].gpi[i] ;
		    hsmooth = sqrt(box0_smx->kd->p[gpi].fBall2)/2.0;
		    if(vista_type != XRAY || (gp->temp) > 30000.){
			if(vista_type == XRAY){
			    lum_xray = xray_const*sqrt(gp->temp)*
				    (exp(-t_lower/(gp->temp)) - 
				    exp(-t_upper/(gp->temp))) ;
			    lum_xray *= (gp->rho)*(gp->mass);
			}
			if(vista_type == LYA){
			    if(!uniform){
				calc_uv(gp) ;
			    }
			    lycool(gp,cool_vec);
			    vol = c1*gp->mass/gp->rho ;
			    cool_tot = vol*(cool_vec[0]+cool_vec[1])/1.e40 ;
			}
			if(vista_type == COOL){
			    if(!uniform){
				calc_uv(gp) ;
			    }
			    lycool(gp,cool_vec);
			    vol = c1*gp->mass/gp->rho ;
			    cool_tot = vol*(cool_vec[0]+cool_vec[1] +
				    cool_vec[2]+cool_vec[3] +
				    cool_vec[4]+cool_vec[5] +
				    cool_vec[6])/1.e40 ;
			}
			/*
			 * handle periodic boundary conditions
			 */
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
				if(part_pos[0]-2.*(hsmooth) > xmax)
				    continue ;
				if(part_pos[0]+2.*(hsmooth) < xmin)
				    continue ;
				if(part_pos[1]-2.*(hsmooth) > ymax)
				    continue ;
				if(part_pos[1]+2.*(hsmooth) < ymin)
				    continue ;
				if(((vista_type == HNEUT || vista_type == HEI ||
				     vista_type == HEII) && autolim == NO)
				   || vista_type == HNEUT_VEL
				   || vista_type == SZ){
				    for (vz = 0.0,j = 0;j < header.ndim ;j++) {
					vz += rot_matrix[2][j] * (gp->vel[j]) ;
				    }
				    vz *= vsys ;
				    if(comove == YES){
					v_hubble = hubble_constant*
						part_pos[2]*rsys ;
				    }
				}
				if((!periodic && ((vista_type != HNEUT &&
					vista_type != HEI && vista_type != HEII
					&& vista_type != SZ
				        && vista_type != HNEUT_VEL)
					|| autolim == YES))
					|| (periodic && ((vista_type != HNEUT &&
					vista_type != HEI && vista_type != HEII
					&& vista_type != SZ
					&& vista_type != HNEUT_VEL)
					|| autolim == YES) &&
					(part_pos[2]< period_size/2.&&
					part_pos[2] >= -period_size/2.))
					|| ((vista_type == HNEUT ||
					vista_type == HEI || vista_type == HEII
					|| vista_type == SZ
					|| vista_type == HNEUT_VEL)
					&& autolim == NO
					&& vz + v_hubble >= vel_min &&
					vz + v_hubble <= vel_max)) {
				/*
				 * determine quantities to be
				 * added to density array.
				 */
				if(vista_type == XRAY){
				    delta_d = lum_xray ;
				}
				else if(vista_type == COOL ||vista_type == LYA){
				    delta_d = cool_tot ;
				}
				else if(vista_type == FSTAR){
				    delta_d = starform[gp-gas_particles] ;
				}
				else if(vista_type == HNEUT){
				    /* weight by HI mass */
				    delta_d = c1*hneutral[gp-gas_particles]*
					(gp->mass);
				}
				else if(vista_type == HNEUT_VEL){
				    delta_d = c1*hneutral[gp-gas_particles]*
					(gp->mass);
				}
				else if(vista_type == HEI){
				    delta_d = c1*
					heneutral[gp-gas_particles]*(gp->mass);
				}
				else if(vista_type == HEII){
				    delta_d = c1*heII[gp-gas_particles]*
					(gp->mass);
				}
				else if(vista_type == ARRAY){
				    aoffset = gp-gas_particles ;
				    delta_d = array[aoffset] ;
				}
				else {
				    delta_d = (gp->mass) ;
				}
				/* determine quantities for auxillary
				 *  arrays
				 */
				if(vista_type == TEMP){
				    delta_q = (gp->mass)*(gp->temp) ;
				}
				else if(vista_type == PRESS){
				    delta_q = (gp->mass)*(gp->temp)*(gp->rho) ;
				}
				else if(vista_type == JEANS){
				    c1_i = sqrt(meanmwt[gp-gas_particles])*c1 ;
				    delta_q = gp->mass * hsmooth*c1_i*
					sqrt(gp->rho / gp->temp);
				}
				else if(vista_type == HNEUT){
				    delta_q = c1*hneutral[gp-gas_particles]*
					hneutral[gp-gas_particles]*(gp->mass);
				    delta_q2 = c1*hneutral[gp-gas_particles]*
					(gp->mass)*(gp->temp);
				}
				else if(vista_type == HNEUT_VEL){
				    delta_q = c1*hneutral[gp-gas_particles]*
					vz*(gp->mass);
				    /* velocity^2 */
				    delta_q2 = c1*hneutral[gp-gas_particles]*
					vz*vz*(gp->mass);
				}
				else if(vista_type == SZ){
				    electron = (1. - 
					hneutral[gp-gas_particles] + rsz*
					(2. - 2*heneutral[gp-gas_particles]-
					heII[gp-gas_particles])) ;
				    delta_d = c1*electron*
					(gp->mass)*(gp->temp);
				    delta_q = c2*electron*(gp->mass)*vz ;
				}
				if(hsmooth > size_pixel){
				    /* need to loop over surrounding
				       pixels
				    */
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
					    density[kx][ky]+=(float)(kernel*
						    delta_d) ;
					    if(vista_type == TEMP ||
						vista_type == PRESS ||
						vista_type == JEANS ||
						vista_type == HNEUT
					       || vista_type == HNEUT_VEL
						|| vista_type == SZ){
					    	quantity[kx][ky]
						    += (float)(kernel*delta_q) ;
					    }
					    if(vista_type == HNEUT
					       || vista_type == HNEUT_VEL){
						quantity2[kx][ky] +=
						    (float)(kernel*delta_q2) ;
					    }
					}
				      }
				    }
				}
				else{
				    /*
				     * particle fits in a single pixel
				     */
				    kx = (int)((part_pos[0]-xmin)/size_pixel+
					    0.499999) ;
				    ky = (int)((part_pos[1]-ymin)/size_pixel+
					    0.499999) ;
				    if(kx >= 0 && kx < vista_size && ky >= 0 &&
					    ky < vista_size){
					density[kx][ky]+=(float) delta_d ;
					if(vista_type == TEMP ||
					    vista_type == PRESS ||
					    vista_type == JEANS ||
					    vista_type == HNEUT
					    || vista_type == HNEUT_VEL
					    || vista_type == SZ){
					    quantity[kx][ky] += (float) delta_q ;
					}
					if(vista_type == HNEUT
					    || vista_type == HNEUT_VEL){
					    quantity2[kx][ky] +=
						(float) delta_q2 ;
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
			 vista_type != HEII && vista_type != SZ &&
		         vista_type != VALL && vista_type != COOL &&
			 vista_type != LYA  && vista_type != FSTAR &&
			 vista_type != HNEUT_VEL && vista_type != ARRAY){
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
		/*
		 * Normalize
		 */
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
		if(vista_type == HNEUT_VEL){
		    for(kx = 0; kx < vista_size; kx++){
			for(ky = 0; ky < vista_size; ky++){
			    if(density[kx][ky] != 0.){
				/* 1st moment */
				quantity[kx][ky] = quantity[kx][ky] /
					density[kx][ky] ;
				/* dispersion */
				quantity2[kx][ky] = sqrt(quantity2[kx][ky] /
					density[kx][ky] 
				    - quantity[kx][ky]*quantity[kx][ky]);
			    }
			    else{
				quantity[kx][ky] = 0. ;
				quantity2[kx][ky] = 0. ;
			    }
			}
		    }
		}
	    }
	    if(vista_type == VSTAR || vista_type == VALL ||
		    vista_type == LUMSTAR){
		if(balls_loaded != STAR && boxlist[0].nstar > 0) {
		    calc_balls(&box0_smx, 0, 0 , 1);
		    balls_loaded = STAR;
		}
		for (i = 0 ;i < boxlist[active_box].nstar ;i++) {
		    int spi;
		    
		    sp = boxlist[active_box].sp[i] ;
		    spi = boxlist[active_box].spi[i] ;
		    if(vista_type == LUMSTAR) {
			delta_d = star_lum_redshift(sp->mass,sp->tform,
						    wavelength) ;
		    }
		    else {
			delta_d = sp->mass;
		    }
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
			    hsmooth = sqrt(box0_smx->kd->p[spi].fBall2)/2.0;
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
						    delta_d) ;
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
					density[kx][ky] += (float)(delta_d) ;
				}
			    }
			  }
			  }
			}
	              }
	            }
		}
	    }
	    if(vista_type == VDARK || vista_type == VALL){
		if(balls_loaded != DARK && boxlist[0].ndark > 0) {
		    calc_balls(&box0_smx, 1, 0 , 0);
		    balls_loaded = DARK;
		}
		for (i = 0 ;i < boxlist[active_box].ndark ;i++) {
		    int dpi;
		    
		    dp = boxlist[active_box].dp[i] ;
		    dpi = boxlist[active_box].dpi[i];
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
			    hsmooth = sqrt(box0_smx->kd->p[dpi].fBall2)/2.0;
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
	    if(vista_type == TDRHO) {
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
	    pixmax = -HUGE;
	    pixmin = HUGE;
	    hneut_velmin = HUGE;
	    hneut_velmax = -HUGE;
	    hneut_vel2max = 0.0;
	    


	    for(i = 0; i < vista_size; i++){
		for(j = 0; j < vista_size; j++){
		  if(density[i][j] > 0. && vista_type == HNEUT_VEL){
			if(vista_type != TEMP && vista_type != PRESS &&
				vista_type != COOL && vista_type != JEANS &&
				vista_type != TDRHO && vista_type != FSTAR &&
				vista_type != XRAY && vista_type != LYA) {
			    pixel = log10((double)(density[i][j])/size_pixel_2);
			}
			else{
			    pixel = log10((double)(density[i][j]));
			    if(vista_type == XRAY || vista_type == COOL ||
				    vista_type == LYA){
				pixel += 40. ;
			    }
			}
		    }
		    else{
			pixel = low ;
		    }
		    /*
		     * calculate limits on velocity.
		     * N.B. this overrides the command arguments and
		     * could cause problems.
		    */
		    if(vista_type == HNEUT_VEL) {
			if(quantity[i][j] < hneut_velmin) hneut_velmin = quantity[i][j];
			if(quantity[i][j] > hneut_velmax) hneut_velmax = quantity[i][j];
			if(quantity2[i][j] > hneut_vel2max)
			    hneut_vel2max = quantity2[i][j];
			}
		    
		    /* determine extremes and perform clipping */
		    if(pixel > pixmax) pixmax = pixel;
		    if(pixel < pixmin) pixmin = pixel;
		    if(pixel > high)pixel = high ;
		    if(pixel < low)pixel = low ;
		    density[i][j] = (float)pixel ;

		}
	    }
	    printf("Pixel extremes: %g %g\n", pixmin, pixmax);
	    if(vista_type != SZ){
	      strcpy(name,base);
	      strcat(name,".fits");
		fits(density,vista_size,vista_size,xmin,ymin,size_pixel,
			size_pixel,low,high,name) ;
	    }
	    else{
                sprintf(name1,"%s.th.fits",base) ;
		fits(density,vista_size,vista_size,xmin,ymin,size_pixel,
			size_pixel,low,high,name1) ;
	    }
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
                sprintf(name1,"%s.frac.fits",base) ;
		fits(quantity,vista_size,vista_size,xmin,ymin,size_pixel,
			size_pixel,low,high,name1) ;
                sprintf(name1,"%s.temp.fits",base) ;
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
	    if(vista_type == HNEUT_VEL){
	      printf("velmin: %g, velmax: %g, vel2max: %g \n", 
		     hneut_velmin, hneut_velmax, hneut_vel2max);
                sprintf(name1,"%s.mom1.fits",base) ;
		fits(quantity,vista_size,vista_size,xmin,ymin,size_pixel,
			size_pixel,hneut_velmin,hneut_velmax,name1) ;
                low = 0.0 ;
                sprintf(name1,"%s.mom2.fits",base) ;
		fits(quantity2,vista_size,vista_size,xmin,ymin,size_pixel,
		     size_pixel,low,hneut_vel2max,name1) ;
            }
	    if(vista_type == SZ){
		for(i = 0; i < vista_size; i++){
		    for(j = 0; j < vista_size; j++){
			if(quantity[i][j] > 0. ){
			    pixel = log10((double)(quantity[i][j])/
				    size_pixel_2);
			    density[i][j] = 1. ;
			}
			else if(quantity[i][j] < 0. ){
			    pixel = log10((double)(-quantity[i][j])/
				    size_pixel_2);
			    density[i][j] = -1. ; ;
			}
			else{
			    pixel = low ;
			    density[i][j] = 0. ;
			}
			if(pixel > high)pixel = high ;
			if(pixel < low)pixel = low ;
			quantity[i][j] = (float)pixel ;
		    }
		}
                sprintf(name1,"%s.kin.fits",base) ;
		fits(quantity,vista_size,vista_size,xmin,ymin,size_pixel,
			size_pixel,low,high,name1) ;
                low = -2.0 ;
                high = 2.0 ;
                sprintf(name1,"%s.sgn.fits",base) ;
		fits(density,vista_size,vista_size,xmin,ymin,size_pixel,
			size_pixel,low,high,name1) ;
	    }
	    free(*density);
	    free(density);
	    if(vista_type != RHO && vista_type != XRAY && vista_type
		 != VDARK && vista_type != VSTAR && vista_type != VALL
		 && vista_type != LUMSTAR && vista_type != COOL &&
		 vista_type != LYA && vista_type != ARRAY){
		free(*quantity);
		free(quantity);
	    }
	    if(vista_type == HNEUT || vista_type ==  HNEUT_VEL){
		free(*quantity2);
		free(quantity2);
	    }
	}
	else {
	    input_error(command) ;
	}
}
