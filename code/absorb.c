/* $Header$
 * $Log$
 * Revision 1.11  1996/07/30 22:19:48  trq
 * Fixed memory leak.
 *
 * Revision 1.10  1996/06/26  00:34:47  nsk
 * Added deuterium to absorb, HeI, HeII, and xray to vista
 *
 * Revision 1.9  1996/04/24  23:40:32  trq
 * balls_loaded variable to keep track of balls.
 * Fixed smoothing length bug.
 *
 * Revision 1.8  1996/04/24  23:29:16  nsk
 *     changed units etc.
 *
 * Revision 1.7  1996/04/19  19:10:46  nsk
 *       Added all sorts of velocity stuff and divv
 *
 * Revision 1.6  1995/12/11  20:00:41  nsk
 * added helium, dark absorb,  integral for elcetronic heating,
 * and read in 6 numbers for background
 *
 * Revision 1.5  1995/11/07  17:08:00  trq
 * absorb.c: fixed bug with velocities beyond bounds.
 * fits.c: deleted unused variable.
 * hubble.c: fixed argument parsing.
 *
 * Revision 1.4  1995/09/22  23:39:36  nsk
 * fixed boundary bug
 *
 * Revision 1.3  1995/03/02  21:43:01  nsk
 * fixed yet another bug in absorb
 *
 * Revision 1.2  1995/03/02  17:30:21  nsk
 * changed absorption cross section tb be done by integral
 * added optical depth output and fixed bug in absorb
 * added stellar mass plot to view_star
 *
 * Revision 1.1.1.1  1995/01/10  22:57:38  trq
 * Import to CVS
 *
 * Revision 2.15  1994/11/01  00:35:27  nsk
 * added resolution column
 *
 * Revision 2.14  1994/10/12  21:11:27  trq
 * Bug fix for hubble flow.
 *
 * Revision 2.13  1994/10/12  00:30:01  trq
 * Small bug fix to hubble velocity code.
 *
 * Revision 2.12  1994/10/11  23:03:04  nsk
 * added hubble flow to velodity in pencil beam
 *
 * Revision 2.11  1994/09/29  21:46:54  nsk
 * added zero temperature column
 *
 * Revision 2.10  1994/09/29  21:30:15  trq
 * Corrected velocity conversion.
 *
 * Revision 2.9  1994/09/29  18:15:09  nsk
 * *** empty log message ***
 *
 * Revision 2.8  1994/09/27  22:31:57  trq
 * Fixed bug with ibin; removed some debugging statements.
 *
 * Revision 2.7  1994/09/27  17:45:33  nsk
 * added lots of stuff
 *
 * Revision 2.6  1994/09/27  17:24:04  trq
 * small fix to periodic code.
 *
 * Revision 2.5  1994/09/26  23:42:08  nsk
 * made velocity periodic
 *
 * Revision 2.4  1994/09/26  23:05:55  trq
 * Actual working version!! (well, almost)
 *
 * Revision 2.3  1994/09/23  22:38:43  nsk
 * made tons of changes
 *
 * Revision 2.2  1994/09/22  16:24:48  trq
 * Corrected checks on the endpoints of the column.
 *
 * Revision 2.1  1994/09/20  21:56:30  trq
 * Added periodic loop.
 *
 * Revision 2.0  1994/09/19  21:46:05  trq
 * Moved file from vcolumn.c
 *
 * Revision 1.2  1994/04/20  08:46:10  trq
 * Added title variable.
 *
 */
/* absorb() -- routine to calculate the velocity distribution of */
 /* material in a column. */

#include "defs.h"
#include "fdefs.h"
#include <malloc.h>

#define SUBBIN 10
#define SPECTRA 1
#define COLUMN  2
#define VELCOL  3
#define DARKM  4

/* Lyman alpha cross section in cm^2 per atom */
#define LYCS_HI (1.34e-12)
#define LYCS_HeI (4.278e-12)
#define LYCS_HeII (3.358e-13)
#define MASSHE (3.971)
#define MASSD (2.)

void
absorb(job)
     char *job;
{
    char command[MAXCOMM] ;
    char type[MAXCOMM] ;        /* type of velocity column */
    char name[MAXCOMM] ;	/* file name for output */
    char col_name[MAXCOMM] ;	/* file name for output */
    int nvbin;			/* number of velocity bins */
    double vmin, vmax;          /* minimum and maximum velocity */
    int zbin;			/* number of spatial bins */
    char x_string[MAXCOMM] ;
    char y_string[MAXCOMM] ;
    Real x1[MAXDIM];            /* x and y coordinates of pencil beam */
    double *vbins_HI;
    double *vbins_D;
    double *vbins_HeI;
    double *vbins_HeII;
    double *vbins_t_HI;
    double *vbins_tot;
    double *vbins_neut_HI;
    double *vbins_temp_HI;
    double *vbins_rho_HI;
    double *vbins_res_HI;
    double *vbins_temp_tot;
    double *vbins_temp2_tot;
    double *vbins_divv_tot;
    double *mass_tot ;
    double *mass_HI ;
    double *mass_HeI ;
    double *mass_HeII ;
    double *vel_tot ;
    double *vel_HI ;
    double *vel_HeI ;
    double *vel_HeII ;
    double *temp_tot ;
    double *temp_HI ;
    double *temp_HeI ;
    double *temp_HeII ;
    double *divv_tot ;
    double *res ;
    int plot_type;
    int i,j,k ;
    double part_pos[MAXDIM] ;
    double distnorm ;
    double radius ;
    double radius2 ;
    double zo ;
    double zo2 ;
    double zi ;
    double zi2 ;
    Real box_coord[BOXPTS][MAXDIM] ;
    double zbox ;
    double zmax ;
    double zmin ;
    double bound_max[MAXDIM];
    double bound_min[MAXDIM];
    double rot_box[MAXDIM] ;
    double zbox_min ;
    double zbox_max ;
    void plane();
    double constant[5] ;
    int bin_box_min ;
    int bin_box_max ;
    double bin_size ;
    int bin ;
    int bin_max ;
    int bin_min ;
    double zlower ;
    double zupper ;
    double kernel ;
    double vkernel ;
    double abs_zlower ;
    double abs_zupper ;
    double d2 ;
    double d ;
    double vz ;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    double offset_x, offset_y ;
    double span_x, span_y ;
    double rsys ;
    double vsys ;
    double msys ;
    double bsys_H ;
    double bsys_D ;
    double bsys_He ;
    double z ;
    double b ;
    double vbin_size ;
    double vlower ;
    double vupper ;
    double abs_vlower ;
    double abs_vupper ;
    double voffset ;
    double v_interp ;
    double t_interp ;
    double hsmooth ;
    double dvcol ;
    double dvcol_HI ;
    double dvcol_tot ;
    int irep[MAXDIM];
    int autolim ;
    int ibin;
    FILE *fp;
    FILE *fp2;

    if (current_project && current_color){
      /* command takes plot type, box of column, minimum and maximum */
      /* velocity, number of velocity bins and output file name as */
      /* arguments. */
      if (sscanf(job,"%s %s %d %lf %lf %d %s %s %s",command,type,&zbin,
	    &vmin,&vmax,&nvbin, name,x_string,y_string) >= 8) {
	if(strcmp(type,"spectra") == 0){
	  plot_type = SPECTRA ;
	}
	else if(strcmp(type,"column") == 0 || strcmp(type,"col") == 0){
	  plot_type = COLUMN ;
	}
	else if(strcmp(type,"velocity") == 0 || strcmp(type,"vel") == 0){
	  plot_type = VELCOL ;
	}
	else if(strcmp(type,"dark") == 0){
	  plot_type = DARKM ;
	}
	else
	  {
	    printf("<sorry, %s is not an absorption type, %s>\n",type,title) ;
	    return;
	  }
	if ( plotted_box != active_box ) {
	    printf("<the box that is plotted is not the ") ;
	    printf("active box, %s>\n",title) ;
	    return ;
	}
	if(strcmp(x_string, "auto") != 0 && strcmp(x_string, "a") != 0){
	    if(sscanf(x_string,"%f", &x1[0]) != 1){
		input_error(command);
		return;
	    }
	    if(sscanf(y_string,"%f", &x1[1]) != 1){
		input_error(command);
		return;
	    }
	}
	if(plot_type == DARKM){
	    mass_tot = (double *)malloc(zbin*sizeof(*mass_tot));
	    if(mass_tot == NULL)
	      {
		printf("<sorry, no memory for column bins, %s>\n",title) ;
		return ;
	      }
	    vel_tot = (double *)malloc(zbin*sizeof(*vel_tot));
	    if(vel_tot == NULL)
	      {
		printf("<sorry, no memory for column bins, %s>\n",title) ;
		free(mass_tot);
		return ;
	      }
	    res = (double *)malloc(zbin*sizeof(*res));
	    if(res == NULL)
	      {
		printf("<sorry, no memory for column bins, %s>\n",title) ;
		free(mass_tot);
		free(vel_tot);
		return ;
	      }
	    if (!cool_loaded ){
		load_cool() ;
	    }
	    if (!redshift_loaded ){
		load_redshift() ;
	    }
	    for(i = 0; i < zbin; i++){
		mass_tot[i] = 0.0;
		vel_tot[i] = 0.0;
		res[i] = 0.0;
	    }
	}
	else {
	    vbins_t_HI = (double *)malloc(nvbin*sizeof(*vbins_t_HI));
	    if(vbins_t_HI == NULL)
	      {
		printf("<sorry, no memory for velocity bins, %s>\n",title) ;
		return ;
	      }
	    vbins_HI = (double *)malloc(nvbin*sizeof(*vbins_HI));
	    vbins_D = (double *)malloc(nvbin*sizeof(*vbins_D));
	    vbins_HeI = (double *)malloc(nvbin*sizeof(*vbins_HeI));
	    vbins_HeII = (double *)malloc(nvbin*sizeof(*vbins_HeII));
	    if(vbins_HI == NULL || vbins_D  == NULL || vbins_HeI == NULL
	    	|| vbins_HeII == NULL)
	      {
		printf("<sorry, no memory for velocity bins, %s>\n",title) ;
		free(vbins_t_HI);
		return ;
	      }
	    mass_tot = (double *)malloc(zbin*sizeof(*mass_tot));
	    mass_HI = (double *)malloc(zbin*sizeof(*mass_HI));
	    mass_HeI = (double *)malloc(zbin*sizeof(*mass_HeI));
	    mass_HeII = (double *)malloc(zbin*sizeof(*mass_HeII));
	    if(mass_tot == NULL || mass_HI == NULL || mass_HeI == NULL ||
		    mass_HeII == NULL)
	      {
		printf("<sorry, no memory for column bins, %s>\n",title) ;
		free(vbins_HI);
		free(vbins_D);
		free(vbins_HeI);
		free(vbins_HeII);
		free(vbins_t_HI);
		return ;
	      }
	    vel_tot = (double *)malloc(zbin*sizeof(*vel_tot));
	    vel_HI = (double *)malloc(zbin*sizeof(*vel_HI));
	    vel_HeI = (double *)malloc(zbin*sizeof(*vel_HeI));
	    vel_HeII = (double *)malloc(zbin*sizeof(*vel_HeII));
	    if(vel_tot == NULL || vel_HI == NULL || vel_HeI == NULL ||
		    vel_HeII == NULL)
	      {
		printf("<sorry, no memory for column bins, %s>\n",title) ;
		free(vbins_t_HI);
		free(vbins_HI);
		free(vbins_D);
		free(vbins_HeI);
		free(vbins_HeII);
		free(mass_tot);
		free(mass_HI);
		free(mass_HeI);
		free(mass_HeII);
		return ;
	      }
	    temp_tot = (double *)malloc(zbin*sizeof(*temp_tot));
	    temp_HI = (double *)malloc(zbin*sizeof(*temp_HI));
	    temp_HeI = (double *)malloc(zbin*sizeof(*temp_HeI));
	    temp_HeII = (double *)malloc(zbin*sizeof(*temp_HeII));
	    if(temp_tot == NULL || temp_HI == NULL || temp_HeI == NULL ||
		    temp_HeII == NULL)
	      {
		printf("<sorry, no memory for column bins, %s>\n",title) ;
		free(vbins_t_HI);
		free(vbins_HI);
		free(vbins_D);
		free(vbins_HeI);
		free(vbins_HeII);
		free(mass_tot);
		free(mass_HI);
		free(mass_HeI);
		free(mass_HeII);
		free(vel_tot);
		free(vel_HI);
		free(vel_HeI);
		free(vel_HeII);
		return ;
	      }
	    divv_tot = (double *)malloc(zbin*sizeof(*divv_tot));
	    res = (double *)malloc(zbin*sizeof(*res));
	    if(divv_tot == NULL || res == NULL)
	      {
		printf("<sorry, no memory for column bins, %s>\n",title) ;
		free(vbins_t_HI);
		free(vbins_HI);
		free(vbins_D);
		free(vbins_HeI);
		free(vbins_HeII);
		free(mass_tot);
		free(mass_HI);
		free(mass_HeI);
		free(mass_HeII);
		free(vel_tot);
		free(vel_HI);
		free(vel_HeI);
		free(vel_HeII);
		free(temp_tot);
		free(temp_HI);
		free(temp_HeI);
		free(temp_HeII);
		return ;
	      }
	    if (!cool_loaded ){
		load_cool() ;
	    }
	    if (!redshift_loaded ){
		load_redshift() ;
	    }
	    if(!hneutral_loaded){
	      hneutral_func() ;
	    }
	    if (!divv_loaded ){
		divv() ;
	    }
	    for(i = 0; i < zbin; i++){
		mass_tot[i] = 0.0;
		mass_HI[i] = 0.0;
		mass_HeI[i] = 0.0;
		mass_HeII[i] = 0.0;
		vel_tot[i] = 0.0;
		vel_HI[i] = 0.0;
		vel_HeI[i] = 0.0;
		vel_HeII[i] = 0.0;
		temp_tot[i] = 0.0;
		temp_HI[i] = 0.0;
		temp_HeI[i] = 0.0;
		temp_HeII[i] = 0.0;
		divv_tot[i] = 0.0;
		res[i] = 0.0;
	    }
	}
	if(strcmp(x_string, "auto") == 0 || strcmp(x_string, "a") == 0){
		  /* N.B.  I am reusing variables make_box_flag and */
		  /* make_box_[xy]1 */
	    setival(&make_box_flag,1) ;
	    printf("<please click mouse at location, %s>\n", title);
	    wait_on_X();
	    span_x = dv2_x - dv1_x ;
	    offset_x = can_width/2.
			- can_size * ((dv2_x + dv1_x)/ (double)span_x)/2. ;
	    span_y = dv1_y - dv2_y ;
	    offset_y = can_height/2.
			- can_size * ((dv2_y + dv1_y)/ (double)span_y)/2. ;
	    make_box_x1 = (int)((make_box_x1 - offset_x) * (double) span_x /
		    (double)can_size) ;
	    make_box_y1 = (int)((make_box_y1 - offset_y) * (double) span_y /
		    (double)can_size) ;
	    x1[0] = (double)make_box_x1 / scaling ;
	    x1[1]  = (double)make_box_y1 / scaling ;
	    printf("<used position (%g, %g), %s>\n", x1[0], x1[1],
		   title);
	}
	setvec(&box_coord[0][0],boxes[active_box].x1) ;
	setvec(&box_coord[1][0],boxes[active_box].x2) ;
	setvec(&box_coord[2][0],boxes[active_box].x3) ;
	setvec(&box_coord[3][0],boxes[active_box].x4) ;
	setvec(&box_coord[4][0],boxes[active_box].x5) ;
	setvec(&box_coord[5][0],boxes[active_box].x6) ;
	diff_add_vec(&box_coord[6][0],boxes[active_box].x5,
		boxes[active_box].x3,boxes[active_box].x1) ;
	diff_add_vec(&box_coord[7][0],boxes[active_box].x6,
		boxes[active_box].x4,boxes[active_box].x2) ;
	for(k = 0; k < MAXDIM; k++) {
	  bound_max[k] = box_coord[0][k];
	  bound_min[k] = box_coord[0][k];
	}
	for (j = 1 ;j < BOXPTS ;j++) {
	    for(k = 0; k < MAXDIM; k++) {
	      bound_max[k] = max(bound_max[k], box_coord[j][k]);
	      bound_min[k] = min(bound_min[k], box_coord[j][k]);
	    }
	}
	for (j = 0 ;j < BOXPTS ;j++) {
	    for(i = 0; i < header.ndim; i++){
		for (rot_box[i] = 0.0,k = 0 ;k < header.ndim ;k++) {
		    rot_box[i] += rot_matrix[i][k] * (box_coord[j][k] -
			    boxes[active_box].center[k]) ;
		}
	    }
	    for(i = 0; i < header.ndim; i++){
	      box_coord[j][i] = rot_box[i] ;
	    }
	}
	zmax = box_coord[0][2] ;
	zmin = box_coord[0][2] ;
	for (j = 1 ;j < BOXPTS ;j++) {
	    zmin = min(zmin,box_coord[j][2]) ;
	    zmax = max(zmax,box_coord[j][2]) ;
	}
	bin_size = (zmax - zmin)/zbin ;
	zbox_min = HUGE ;
	zbox_max = -HUGE ;
	plane(&box_coord[0][0],&box_coord[1][0],&box_coord[2][0],
		&box_coord[4][0],constant);
        if(constant[2] != 0.0){
	    zbox=(constant[3]-constant[0]*x1[0]-constant[1]*x1[1])/constant[2] ;
	    if(zbox <= zmax && zbox >= zmin){
		zbox_min = min(zbox, zbox_min) ;
		zbox_max = max(zbox, zbox_max) ;
	    }
	    zbox=(constant[4]-constant[0]*x1[0]-constant[1]*x1[1])/constant[2] ;
	    if(zbox <= zmax && zbox >= zmin){
		zbox_min = min(zbox, zbox_min) ;
		zbox_max = max(zbox, zbox_max) ;
	    }
        }
	plane(&box_coord[0][0],&box_coord[2][0],&box_coord[4][0],
		&box_coord[5][0],constant);
        if(constant[2] != 0.0){
	    zbox=(constant[3]-constant[0]*x1[0]-constant[1]*x1[1])/constant[2] ;
	    if(zbox <= zmax && zbox >= zmin){
		zbox_min = min(zbox, zbox_min) ;
		zbox_max = max(zbox, zbox_max) ;
	    }
	    zbox=(constant[4]-constant[0]*x1[0]-constant[1]*x1[1])/constant[2] ;
	    if(zbox <= zmax && zbox >= zmin){
		zbox_min = min(zbox, zbox_min) ;
		zbox_max = max(zbox, zbox_max) ;
	    }
	}
	plane(&box_coord[5][0],&box_coord[4][0],&box_coord[2][0],
		&box_coord[0][0],constant);
        if(constant[2] != 0.0){
	    zbox=(constant[3]-constant[0]*x1[0]-constant[1]*x1[1])/constant[2] ;
	    if(zbox <= zmax && zbox >= zmin){
		zbox_min = min(zbox, zbox_min) ;
		zbox_max = max(zbox, zbox_max) ;
	    }
	    zbox=(constant[4]-constant[0]*x1[0]-constant[1]*x1[1])/constant[2] ;
	    if(zbox <= zmax && zbox >= zmin){
		zbox_min = min(zbox, zbox_min) ;
		zbox_max = max(zbox, zbox_max) ;
	    }
	}
	bin_box_min = floor((zbox_min - zmin)/bin_size) ;
	bin_box_max = floor((zbox_max - zmin)/bin_size) ;
	if((zbox_max - zmin)/bin_size == (double) bin_box_max)
	  bin_box_max--;

	rsys = cosmof*kpcunit/1.e3 ;
	vsys = cosmof*sqrt(msolunit/kpcunit*(GCGS*MSOLG/KPCCM))/1.e5 ;
	if(plot_type == DARKM){
	if(balls_loaded != DARK) {
	    calc_balls(&box0_smx, 1, 0 , 0);
	    balls_loaded = DARK;
	}
	for (i = 0 ;i < boxlist[0].ndark ;i++) {
	    dp = boxlist[0].dp[i] ;
	    hsmooth = sqrt(box0_smx->kd->p[i].fBall2)/2.0;
	    for(irep[0] = -1; irep[0] <= 1; irep[0]++) {
	      for(irep[1] = -1; irep[1] <= 1; irep[1]++) {
		for(irep[2] = -1; irep[2] <= 1; irep[2]++) {
		  if(periodic
		     || (irep[0] == 0 && irep[1] == 0 && irep[2] == 0)) {

		    if(irep[0]*period_size + dp->pos[0] > bound_max[0]
		       && irep[0]*period_size+dp->pos[0]-hsmooth
		       > bound_max[0])
		      continue;
		    if(irep[0]*period_size + dp->pos[0] < bound_min[0]
		       && irep[0]*period_size+dp->pos[0]+hsmooth
		       < bound_min[0])
		      continue;
		    if(irep[1]*period_size + dp->pos[1] > bound_max[1]
		       && irep[1]*period_size+dp->pos[1]-hsmooth
		       > bound_max[1])
		      continue;
		    if(irep[1]*period_size + dp->pos[1] < bound_min[1]
		       && irep[1]*period_size+dp->pos[1]+hsmooth
		       < bound_min[1])
		      continue;
		    if(irep[2]*period_size + dp->pos[2] > bound_max[2]
		       && irep[2]*period_size+dp->pos[2]-hsmooth
		       > bound_max[2])
		      continue;
		    if(irep[2]*period_size + dp->pos[2] < bound_min[2]
		       && irep[2]*period_size+dp->pos[2]+hsmooth
		       < bound_min[2])
		      continue;
		    
		    for (part_pos[0] = part_pos[1] = part_pos[2] = 0.0,j = 0 ;
			    j < header.ndim ;j++) {
			for (k = 0 ; k < header.ndim ; k++){
			    part_pos[k] += rot_matrix[k][j] *
			      (irep[j]*period_size + dp->pos[j]
			       - boxes[active_box].center[j]) ;
			}
		    }
		    distnorm = 1. / (hsmooth * hsmooth) ;
		    if((radius2 = ((part_pos[0] - x1[0])*(part_pos[0] - x1[0])+
			    (part_pos[1] - x1[1])*(part_pos[1] - x1[1]))*
			    distnorm) < 4.0){
			radius = sqrt(radius2) ;
			zo2 = 4. - radius2 ;
			zo = sqrt(zo2) ;
			if(radius2 < 1.){
			    zi2 = 1. - radius2 ;
			    zi = sqrt(zi2) ;
			}
			else{
			    zi2 = zi = 0.0;
			}
			for (vz = 0.0,j = 0 ; j < header.ndim ;j++) {
			    vz += rot_matrix[2][j] * (dp->vel[j]) ;
			}
			bin_min = floor((part_pos[2] - zo*hsmooth-zmin)
					/bin_size) ;
			bin_max = floor((part_pos[2] + zo*hsmooth-zmin)
					/bin_size) ;
			if((part_pos[2] + zo*hsmooth-zmin)/bin_size
			   == (double) bin_max)
			  bin_max--;
			bin_min = max(bin_min, bin_box_min);
			bin_max = min(bin_max, bin_box_max);
			if(bin_min > bin_box_max || bin_max < bin_box_min)
			  continue;

			for(bin = bin_min; bin <= bin_max; bin++){
			    zlower = ((double)(bin)*bin_size + zmin
				      - part_pos[2])/ hsmooth ;
			    zupper = ((double)(bin+1)*bin_size + zmin
				      - part_pos[2])/ hsmooth ;
			    kernel = 0.0 ;
			    vkernel = 0.0 ;
			    abs_zlower = fabs(zlower) ;
			    if(abs_zlower >= zo){
				kernel += 1.3125*radius2*zo - 1.5*zo + 
					0.5*zo2*zo - (1.5*radius2 +
					0.09375*radius2*radius2)*log(zo + 2.0);
				/*kernel at zo in region 2 */
				if(comove == YES){
				    vkernel -= zo2 + 0.75*radius2*zo2 +
					    0.375*zo2*zo2 - 9.6 ;
				}
			    }
			    else if(abs_zlower < zo && (abs_zlower > zi ||
				    zlower == zi)){
				d2 = radius2 + zlower*zlower ;
				d = sqrt(d2) ;
				kernel -= copysign(1.0,zlower)*((2.0 +
				1.5*radius2)* abs_zlower + 
				0.5*abs_zlower*abs_zlower*abs_zlower -
				0.0625*abs_zlower*d*d2 - (1.5 + 0.09375*
				radius2)* abs_zlower*d - (1.5*radius2 +
			        0.09375*radius2* radius2)*log(abs_zlower + d));
			/* sign(zlower)*kernel at abs_zlower in region 2 */
				if(comove == YES){
				    vkernel -= zlower*zlower*(1. +
					    0.75*radius2) + 0.375*zlower*
					    zlower*zlower*zlower - d*d2 -
					    0.05*d2*d2*d ;
				}
			    }
			    else {
				d2 = radius2 + zlower*zlower ;
				d = sqrt(d2) ;
				kernel -= copysign(1.0,zlower)*((1.0 -
					1.5*radius2)*abs_zlower - 0.5*
					abs_zlower*abs_zlower*abs_zlower +
					0.1875*abs_zlower*d2*d + 0.28125*
					radius2*abs_zlower*d + 0.28125*radius2*
					radius2*log(abs_zlower + d)) ;
			/* sign(zlower)* kernel at abs_zlower in region 1 */
				if(comove == YES){
				    vkernel -= (0.5 - 0.75*radius2)*zlower*
					    zlower - 0.375*zlower*zlower*
					    zlower*zlower + 0.15*d2*d2*d ;
				}
			    }
			    abs_zupper = fabs(zupper) ;
			    if(zlower*zupper <= 0.0){  /* bin stradles zero */
				if(radius2 < 1.){
				    kernel -= 0.5625*radius2*radius2*
					    log(radius) ;
				    /* 2.0 * kernel at zero in region 1 */
				}
				else {
				    kernel -= -(3.0*radius2
					    + 0.1875*radius2*radius2)*
					    log(radius) ;
				    /* 2.0 *kernel at zero in region 2 */
				}
			    }
			    if(radius2 < 1.){
				if(zlower*zupper < 0.0 && abs_zupper > zi &&
					abs_zlower > zi) {
				    kernel += 2.375*zi - 2.4375*radius2*zi -
					    zi2*zi + 0.5625*radius2*radius2*
					    log(zi+1) ;
				    /* 2.0 * kernel at zi in region 1 */
				    kernel -= 0.875*zi + 2.8125*radius2*zi +
					    zi2*zi - (3.0*radius2 + 0.1875*
					    radius2*radius2)*log(zi + 1) ;
				    /* 2.0 * kernel at zi in region 2 */
				}
				else {
				    if((zlower < -zi && zupper > -zi) ||
					    (zlower < zi && zupper > zi)){
					 kernel += 1.1875*zi - 1.21875*radius2*
						zi - 0.5*zi2*zi + 0.28125*
						radius2*radius2*log(zi+1) ;
					 /* kernel at zi in region 1 */
					 if(comove == YES){
					    vkernel += copysign(1.0, zlower)
					      *((0.5 - 0.75*radius2)*
						    zi2 - 0.375*zi2*zi2 + 0.15) ;
					 }
					 kernel -= 0.4375*zi + 1.40625*radius2*
					 	zi + 0.5*zi2*zi - (1.5*radius2 +
						0.09375*radius2*radius2)*
						log(zi + 1) ;
					 /* kernel at zi in region 2 */
					 if(comove == YES){
					    vkernel -= copysign(1.0, zlower)
					      *((1.0 + 0.75*radius2)*
						    zi2 + 0.375*zi2*zi2 - 1.05) ;
					 }
				    }
				}
			    }

			    if(abs_zupper >= zo){
				kernel += 1.3125*radius2*zo - 1.5*zo + 
					0.5*zo2*zo - (1.5*radius2 +
					0.09375*radius2*radius2)*log(zo + 2.0) ;
					  /* kernel at zo in region 2 */
			        if(comove == YES){
				    vkernel += zo2 + 0.75*radius2*zo2 +
					    0.375*zo2*zo2 - 9.6 ;
				}
			    }
			    else if(abs_zupper < zo && abs_zupper >= zi){
				d2 = radius2 + zupper*zupper ;
				d = sqrt(d2) ;
				kernel += copysign(1.0,zupper)*((2.0 + 1.5*
					radius2)*abs_zupper + 0.5*abs_zupper*
					abs_zupper*abs_zupper - 0.0625*
					abs_zupper*d*d2 - (1.5 + 0.09375*
					radius2)* abs_zupper*d - (1.5*radius2 +
					0.09375*radius2* radius2)*
					log(abs_zupper + d)) ;
			/* sign(zupper)*kernel at abs_zupper in region 2 */
			        if(comove == YES){
				    vkernel += zupper*zupper*(1. +
					    0.75*radius2) + 0.375*zupper*
					    zupper*zupper*zupper - d*d2 -
					    0.05*d2*d2*d ;
				}
			    }
			    else {
				d2 = radius2 + zupper*zupper ;
				d = sqrt(d2) ;
				kernel += copysign(1.0,zupper)*((1.0 -
					1.5*radius2)*abs_zupper - 0.5*
					abs_zupper*abs_zupper*abs_zupper +
					0.1875*abs_zupper*d2*d + 0.28125*
					radius2*abs_zupper*d + 0.28125*radius2*
					radius2*log(abs_zupper + d)) ;
			/* sign(zupper)* kernel at abs_zupper in region 1 */
			        if(comove == YES){
				    vkernel += (0.5 - 0.75*radius2)*zupper*
					    zupper - 0.375*zupper*zupper*
					    zupper*zupper + 0.15*d2*d2*d ;
				}
			    }
			    kernel *= distnorm/PI ;
			    if(comove == YES){
				vkernel /= PI*hsmooth ;
			    }
			    mass_tot[bin] += kernel*dp->mass ;
			    vel_tot[bin] += kernel*(dp->mass)*vsys*vz ;
			    if(comove == YES){
				vel_tot[bin] += vkernel*(dp->mass)*rsys*
					hubble_constant ;
			    }
			    res[bin] += kernel*(dp->mass)*(hsmooth) ;
			}
		    }
		}
	      }
	    }
	  }
	}
	for(i = 0; i < zbin; i++){
	    if(mass_tot[i] != 0.0){
		vel_tot[i] /= mass_tot[i] ;
		res[i] /= mass_tot[i] ;
	    }
	}
	msys = msolunit/(cosmof*kpcunit*cosmof*kpcunit)*
                (MSOLG/(KPCCM*KPCCM)) ;
	for(i = 0; i < zbin; i++){
	    mass_tot[i] *= msys ;
	}
	fp = fopen(name, "w");
	if(fp == NULL)
	  {
	    printf("<sorry, cannot open file %s, %s>\n",name, title) ;
	    return;
	  }
	for(i = 0; i < zbin; i++)
	  {
	    fprintf(fp, "%g %g %g %g\n",
		    rsys*(bin_size*(i+0.5) + zmin),
		    mass_tot[i]/(bin_size*cosmof*kpcunit*KPCCM),
		    vel_tot[i], rsys*res[i]);
	  }
	fclose(fp);
	}
	else {
	for (i = 0 ;i < boxlist[0].ngas ;i++) {
	    gp = boxlist[0].gp[i] ;
	    for(irep[0] = -1; irep[0] <= 1; irep[0]++) {
	      for(irep[1] = -1; irep[1] <= 1; irep[1]++) {
		for(irep[2] = -1; irep[2] <= 1; irep[2]++) {
		  if(periodic
		     || (irep[0] == 0 && irep[1] == 0 && irep[2] == 0)) {

		    if(irep[0]*period_size + gp->pos[0] > bound_max[0]
		       && irep[0]*period_size+gp->pos[0]-gp->hsmooth
		       > bound_max[0])
		      continue;
		    if(irep[0]*period_size + gp->pos[0] < bound_min[0]
		       && irep[0]*period_size+gp->pos[0]+gp->hsmooth
		       < bound_min[0])
		      continue;
		    if(irep[1]*period_size + gp->pos[1] > bound_max[1]
		       && irep[1]*period_size+gp->pos[1]-gp->hsmooth
		       > bound_max[1])
		      continue;
		    if(irep[1]*period_size + gp->pos[1] < bound_min[1]
		       && irep[1]*period_size+gp->pos[1]+gp->hsmooth
		       < bound_min[1])
		      continue;
		    if(irep[2]*period_size + gp->pos[2] > bound_max[2]
		       && irep[2]*period_size+gp->pos[2]-gp->hsmooth
		       > bound_max[2])
		      continue;
		    if(irep[2]*period_size + gp->pos[2] < bound_min[2]
		       && irep[2]*period_size+gp->pos[2]+gp->hsmooth
		       < bound_min[2])
		      continue;
		    
		    for (part_pos[0] = part_pos[1] = part_pos[2] = 0.0,j = 0 ;
			    j < header.ndim ;j++) {
			for (k = 0 ; k < header.ndim ; k++){
			    part_pos[k] += rot_matrix[k][j] *
			      (irep[j]*period_size + gp->pos[j]
			       - boxes[active_box].center[j]) ;
			}
		    }
		    distnorm = 1. / (gp->hsmooth * gp->hsmooth) ;
		    if((radius2 = ((part_pos[0] - x1[0])*(part_pos[0] - x1[0])+
			    (part_pos[1] - x1[1])*(part_pos[1] - x1[1]))*
			    distnorm) < 4.0){
			radius = sqrt(radius2) ;
			zo2 = 4. - radius2 ;
			zo = sqrt(zo2) ;
			if(radius2 < 1.){
			    zi2 = 1. - radius2 ;
			    zi = sqrt(zi2) ;
			}
			else{
			    zi2 = zi = 0.0;
			}
			for (vz = 0.0,j = 0 ; j < header.ndim ;j++) {
			    vz += rot_matrix[2][j] * (gp->vel[j]) ;
			}
			bin_min = floor((part_pos[2] - zo*gp->hsmooth-zmin)
					/bin_size) ;
			bin_max = floor((part_pos[2] + zo*gp->hsmooth-zmin)
					/bin_size) ;
			if((part_pos[2] + zo*gp->hsmooth-zmin)/bin_size
			   == (double) bin_max)
			  bin_max--;
			bin_min = max(bin_min, bin_box_min);
			bin_max = min(bin_max, bin_box_max);
			if(bin_min > bin_box_max || bin_max < bin_box_min)
			  continue;

			for(bin = bin_min; bin <= bin_max; bin++){
			    zlower = ((double)(bin)*bin_size + zmin
				      - part_pos[2])/ gp->hsmooth ;
			    zupper = ((double)(bin+1)*bin_size + zmin
				      - part_pos[2])/ gp->hsmooth ;
			    kernel = 0.0 ;
			    vkernel = 0.0 ;
			    abs_zlower = fabs(zlower) ;
			    if(abs_zlower >= zo){
				kernel += 1.3125*radius2*zo - 1.5*zo + 
					0.5*zo2*zo - (1.5*radius2 +
					0.09375*radius2*radius2)*log(zo + 2.0);
				/*kernel at zo in region 2 */
				if(comove == YES){
				    vkernel -= zo2 + 0.75*radius2*zo2 +
					    0.375*zo2*zo2 - 9.6 ;
				}
			    }
			    else if(abs_zlower < zo && (abs_zlower > zi ||
				    zlower == zi)){
				d2 = radius2 + zlower*zlower ;
				d = sqrt(d2) ;
				kernel -= copysign(1.0,zlower)*((2.0 +
				1.5*radius2)* abs_zlower + 
				0.5*abs_zlower*abs_zlower*abs_zlower -
				0.0625*abs_zlower*d*d2 - (1.5 + 0.09375*
				radius2)* abs_zlower*d - (1.5*radius2 +
			        0.09375*radius2* radius2)*log(abs_zlower + d));
			/* sign(zlower)*kernel at abs_zlower in region 2 */
				if(comove == YES){
				    vkernel -= zlower*zlower*(1. +
					    0.75*radius2) + 0.375*zlower*
					    zlower*zlower*zlower - d*d2 -
					    0.05*d2*d2*d ;
				}
			    }
			    else {
				d2 = radius2 + zlower*zlower ;
				d = sqrt(d2) ;
				kernel -= copysign(1.0,zlower)*((1.0 -
					1.5*radius2)*abs_zlower - 0.5*
					abs_zlower*abs_zlower*abs_zlower +
					0.1875*abs_zlower*d2*d + 0.28125*
					radius2*abs_zlower*d + 0.28125*radius2*
					radius2*log(abs_zlower + d)) ;
			/* sign(zlower)* kernel at abs_zlower in region 1 */
				if(comove == YES){
				    vkernel -= (0.5 - 0.75*radius2)*zlower*
					    zlower - 0.375*zlower*zlower*
					    zlower*zlower + 0.15*d2*d2*d ;
				}
			    }
			    abs_zupper = fabs(zupper) ;
			    if(zlower*zupper <= 0.0){  /* bin stradles zero */
				if(radius2 < 1.){
				    kernel -= 0.5625*radius2*radius2*
					    log(radius) ;
				    /* 2.0 * kernel at zero in region 1 */
				}
				else {
				    kernel -= -(3.0*radius2
					    + 0.1875*radius2*radius2)*
					    log(radius) ;
				    /* 2.0 *kernel at zero in region 2 */
				}
			    }
			    if(radius2 < 1.){
				if(zlower*zupper < 0.0 && abs_zupper > zi &&
					abs_zlower > zi) {
				    kernel += 2.375*zi - 2.4375*radius2*zi -
					    zi2*zi + 0.5625*radius2*radius2*
					    log(zi+1) ;
				    /* 2.0 * kernel at zi in region 1 */
				    kernel -= 0.875*zi + 2.8125*radius2*zi +
					    zi2*zi - (3.0*radius2 + 0.1875*
					    radius2*radius2)*log(zi + 1) ;
				    /* 2.0 * kernel at zi in region 2 */
				}
				else {
				    if((zlower < -zi && zupper > -zi) ||
					    (zlower < zi && zupper > zi)){
					 kernel += 1.1875*zi - 1.21875*radius2*
						zi - 0.5*zi2*zi + 0.28125*
						radius2*radius2*log(zi+1) ;
					 /* kernel at zi in region 1 */
					 if(comove == YES){
					    vkernel += copysign(1.0, zlower)
					      *((0.5 - 0.75*radius2)*
						    zi2 - 0.375*zi2*zi2 + 0.15) ;
					 }
					 kernel -= 0.4375*zi + 1.40625*radius2*
					 	zi + 0.5*zi2*zi - (1.5*radius2 +
						0.09375*radius2*radius2)*
						log(zi + 1) ;
					 /* kernel at zi in region 2 */
					 if(comove == YES){
					    vkernel -= copysign(1.0, zlower)
					      *((1.0 + 0.75*radius2)*
						    zi2 + 0.375*zi2*zi2 - 1.05) ;
					 }
				    }
				}
			    }

			    if(abs_zupper >= zo){
				kernel += 1.3125*radius2*zo - 1.5*zo + 
					0.5*zo2*zo - (1.5*radius2 +
					0.09375*radius2*radius2)*log(zo + 2.0) ;
					  /* kernel at zo in region 2 */
			        if(comove == YES){
				    vkernel += zo2 + 0.75*radius2*zo2 +
					    0.375*zo2*zo2 - 9.6 ;
				}
			    }
			    else if(abs_zupper < zo && abs_zupper >= zi){
				d2 = radius2 + zupper*zupper ;
				d = sqrt(d2) ;
				kernel += copysign(1.0,zupper)*((2.0 + 1.5*
					radius2)*abs_zupper + 0.5*abs_zupper*
					abs_zupper*abs_zupper - 0.0625*
					abs_zupper*d*d2 - (1.5 + 0.09375*
					radius2)* abs_zupper*d - (1.5*radius2 +
					0.09375*radius2* radius2)*
					log(abs_zupper + d)) ;
			/* sign(zupper)*kernel at abs_zupper in region 2 */
			        if(comove == YES){
				    vkernel += zupper*zupper*(1. +
					    0.75*radius2) + 0.375*zupper*
					    zupper*zupper*zupper - d*d2 -
					    0.05*d2*d2*d ;
				}
			    }
			    else {
				d2 = radius2 + zupper*zupper ;
				d = sqrt(d2) ;
				kernel += copysign(1.0,zupper)*((1.0 -
					1.5*radius2)*abs_zupper - 0.5*
					abs_zupper*abs_zupper*abs_zupper +
					0.1875*abs_zupper*d2*d + 0.28125*
					radius2*abs_zupper*d + 0.28125*radius2*
					radius2*log(abs_zupper + d)) ;
			/* sign(zupper)* kernel at abs_zupper in region 1 */
			        if(comove == YES){
				    vkernel += (0.5 - 0.75*radius2)*zupper*
					    zupper - 0.375*zupper*zupper*
					    zupper*zupper + 0.15*d2*d2*d ;
				}
			    }
			    kernel *= distnorm/PI ;
			    if(comove == YES){
				vkernel /= PI*gp->hsmooth ;
			    }
			    mass_tot[bin] += kernel*gp->mass ;
			    mass_HI[bin] += kernel*fhydrogen*
				    hneutral[gp-gas_particles]*gp->mass ;
			    mass_HeI[bin] += kernel*(1.0 - fhydrogen)*
				    heneutral[gp-gas_particles]*gp->mass ;
			    mass_HeII[bin] += kernel*(1.0 - fhydrogen)*
				    heII[gp-gas_particles]*gp->mass ;
			    vel_tot[bin] += kernel*(gp->mass)*vsys*vz ;
			    vel_HI[bin] += kernel*fhydrogen*
				    hneutral[gp-gas_particles]*
				    (gp->mass)*vsys*vz ;
			    vel_HeI[bin] += kernel*(1.0 - fhydrogen)*
				    heneutral[gp-gas_particles]*
				    (gp->mass)*vsys*vz ;
			    vel_HeII[bin] += kernel*(1.0 - fhydrogen)*
				    heII[gp-gas_particles]*
				    (gp->mass)*vsys*vz ;
			    if(comove == YES){
				vel_tot[bin] += vkernel*(gp->mass)*rsys*
					hubble_constant ;
				vel_HI[bin] += vkernel*fhydrogen*
					hneutral[gp-gas_particles]*
					(gp->mass)*rsys*hubble_constant ;
				vel_HeI[bin] += vkernel*(1.0 - fhydrogen)*
					heneutral[gp-gas_particles]*
					(gp->mass)*rsys*hubble_constant ;
				vel_HeII[bin] += vkernel*(1.0 - fhydrogen)*
					heII[gp-gas_particles]*
					(gp->mass)*rsys*hubble_constant ;
			    }
			    temp_tot[bin] += kernel*(gp->mass)*(gp->temp) ;
			    temp_HI[bin] += kernel*fhydrogen*
				    hneutral[gp-gas_particles]*
				    (gp->mass)*(gp->temp) ;
			    temp_HeI[bin] += kernel*(1.0 - fhydrogen)*
				    heneutral[gp-gas_particles]*
				    (gp->mass)*(gp->temp) ;
			    temp_HeII[bin] += kernel*(1.0 - fhydrogen)*
				    heII[gp-gas_particles]*
				    (gp->mass)*(gp->temp) ;
			    divv_tot[bin] += kernel*(gp->mass)*hsmdivv[i] ;
			    res[bin] += kernel*(gp->mass)*(gp->hsmooth) ;
			}
		    }
		}
	      }
	    }
	  }
	}
	for(i = 0; i < zbin; i++){
	    if(mass_tot[i] != 0.0){
		vel_tot[i] /= mass_tot[i] ;
		temp_tot[i] /= mass_tot[i] ;
		res[i] /= mass_tot[i] ;
		divv_tot[i] *= vsys/rsys/mass_tot[i] ;
	    }
	    if(mass_HI[i] != 0.0){
		vel_HI[i] /= mass_HI[i] ;
		temp_HI[i] /= mass_HI[i] ;
	    }
	    if(mass_HeI[i] != 0.0){
		vel_HeI[i] /= mass_HeI[i] ;
		temp_HeI[i] /= mass_HeI[i] ;
	    }
	    if(mass_HeII[i] != 0.0){
		vel_HeII[i] /= mass_HeII[i] ;
		temp_HeII[i] /= mass_HeII[i] ;
	    }
	}
	msys = msolunit/(cosmof*kpcunit*cosmof*kpcunit)*
                (MSOLG/(KPCCM*KPCCM*MHYDR)) ;
	for(i = 0; i < zbin; i++){
	    mass_tot[i] *= msys ;
	    mass_HI[i] *= msys ;
	    mass_HeI[i] *= msys/MASSHE ;
	    mass_HeII[i] *= msys/MASSHE ;
	}
	if (plot_type == COLUMN){
	    sprintf(col_name,"%s.col",name) ;
	    fp = fopen(col_name, "w");
	    if(fp == NULL)
	      {
		printf("<sorry, cannot open file %s, %s>\n",col_name, title) ;
		return;
	      }
	    for(i = 0; i < zbin; i++)
	      {
		fprintf(fp, "%g %g %g %g %g %g %g %g %g %g %g %g %g %g %g\n",
			rsys*(bin_size*(i+0.5) + zmin),
			mass_tot[i]/(bin_size*cosmof*kpcunit*KPCCM)*MHYDR,
			vel_tot[i], temp_tot[i],
			mass_HI[i]/(bin_size*cosmof*kpcunit*KPCCM),
			vel_HI[i], temp_HI[i],
			mass_HeI[i]/(bin_size*cosmof*kpcunit*KPCCM),
			vel_HeI[i], temp_HeI[i],
			mass_HeII[i]/(bin_size*cosmof*kpcunit*KPCCM),
			vel_HeII[i], temp_HeII[i],rsys*res[i],divv_tot[i]);
	      }
	    fclose(fp);
	}
	fp = fopen(name, "w");
	if(fp == NULL)
	  {
	    printf("<sorry, cannot open file %s, %s>\n",name, title) ;
	    return;
	  }
	bsys_H = sqrt(2.*KBOLTZ/MHYDR)/1.e5 ;
	bsys_D = sqrt(2.*KBOLTZ/(MASSD*MHYDR))/1.e5 ;
	bsys_He = sqrt(2.*KBOLTZ/(MASSHE*MHYDR))/1.e5 ;
	vmin *= vsys ;
	vmax *= vsys ;
	voffset = 0.0 ;
	autolim = NO;
	if(vmin == vmax){
	    autolim = YES ;
	    if(comove == NO){
		vmin = HUGE ;
		vmax = -HUGE ;
		for(i = 0; i < zbin; i++){
		    vmin = min(vmin,vel_tot[i]) ;
		    vmax = max(vmax,vel_tot[i]) ;
		}
	    }
	    else{
		voffset = vmin ;
		vmax = voffset + (zmax - zmin)*rsys*hubble_constant ;
	    }
	}
	vbin_size = (vmax - vmin)/(double)nvbin ;
	for(i = 0; i < nvbin; i++){
	    vbins_HI[i] = 0.0;
	    vbins_D[i] = 0.0;
	    vbins_HeI[i] = 0.0;
	    vbins_HeII[i] = 0.0;
	    vbins_t_HI[i] = 0.0;
	}
	if(plot_type == VELCOL){
            vbins_tot = (double *)malloc(nvbin*sizeof(*vbins_tot));
            if(vbins_tot == NULL)
              {
                printf("<sorry, no memory for velocity bins, %s>\n",title) ;
                free(vbins_t_HI);
                free(vbins_HI);
		free(vbins_D);
                free(vbins_HeI);
                free(vbins_HeII);
                free(mass_tot);
                free(mass_HI);
                free(mass_HeI);
                free(mass_HeII);
                free(vel_tot);
                free(vel_HI);
                free(vel_HeI);
                free(vel_HeII);
                free(temp_tot);
                free(temp_HI);
                free(temp_HeI);
                free(temp_HeII);
		free(divv_tot) ;
                free(res);
                return ;
              }
            vbins_neut_HI = (double *)malloc(nvbin*sizeof(*vbins_neut_HI));
            if(vbins_neut_HI == NULL)
              {
                printf("<sorry, no memory for velocity bins, %s>\n",title) ;
                free(vbins_t_HI);
                free(vbins_HI);
		free(vbins_D);
                free(vbins_HeI);
                free(vbins_HeII);
                free(mass_tot);
                free(mass_HI);
                free(mass_HeI);
                free(mass_HeII);
                free(vel_tot);
                free(vel_HI);
                free(vel_HeI);
                free(vel_HeII);
                free(temp_tot);
                free(temp_HI);
                free(temp_HeI);
                free(temp_HeII);
		free(divv_tot) ;
                free(res);
		free(vbins_tot);
                return ;
              }
            vbins_temp_HI = (double *)malloc(nvbin*sizeof(*vbins_temp_HI));
            if(vbins_temp_HI == NULL)
              {
                printf("<sorry, no memory for velocity bins, %s>\n",title) ;
                free(vbins_t_HI);
                free(vbins_HI);
		free(vbins_D);
                free(vbins_HeI);
                free(vbins_HeII);
                free(mass_tot);
                free(mass_HI);
                free(mass_HeI);
                free(mass_HeII);
                free(vel_tot);
                free(vel_HI);
                free(vel_HeI);
                free(vel_HeII);
                free(temp_tot);
                free(temp_HI);
                free(temp_HeI);
                free(temp_HeII);
		free(divv_tot) ;
                free(res);
		free(vbins_tot);
		free(vbins_neut_HI);
                return ;
              }
            vbins_rho_HI = (double *)malloc(nvbin*sizeof(*vbins_rho_HI));
            if(vbins_rho_HI == NULL)
              {
                printf("<sorry, no memory for velocity bins, %s>\n",title) ;
                free(vbins_t_HI);
                free(vbins_HI);
		free(vbins_D);
                free(vbins_HeI);
                free(vbins_HeII);
                free(mass_tot);
                free(mass_HI);
                free(mass_HeI);
                free(mass_HeII);
                free(vel_tot);
                free(vel_HI);
                free(vel_HeI);
                free(vel_HeII);
                free(temp_tot);
                free(temp_HI);
                free(temp_HeI);
                free(temp_HeII);
		free(divv_tot) ;
                free(res);
		free(vbins_tot);
		free(vbins_neut_HI);
		free(vbins_temp_HI);
	      }
            vbins_res_HI = (double *)malloc(nvbin*sizeof(*vbins_res_HI));
            if(vbins_res_HI == NULL)
              {
                printf("<sorry, no memory for velocity bins, %s>\n",title) ;
                free(vbins_t_HI);
                free(vbins_HI);
		free(vbins_D);
                free(vbins_HeI);
                free(vbins_HeII);
                free(mass_tot);
                free(mass_HI);
                free(mass_HeI);
                free(mass_HeII);
                free(vel_tot);
                free(vel_HI);
                free(vel_HeI);
                free(vel_HeII);
                free(temp_tot);
                free(temp_HI);
                free(temp_HeI);
                free(temp_HeII);
		free(divv_tot) ;
                free(res);
		free(vbins_tot);
		free(vbins_neut_HI);
		free(vbins_temp_HI);
		free(vbins_rho_HI);
                return ;
              }
            vbins_temp_tot = (double *)malloc(nvbin*sizeof(*vbins_temp_tot));
            if(vbins_temp_tot == NULL)
              {
                printf("<sorry, no memory for velocity bins, %s>\n",title) ;
                free(vbins_t_HI);
                free(vbins_HI);
		free(vbins_D);
                free(vbins_HeI);
                free(vbins_HeII);
                free(mass_tot);
                free(mass_HI);
                free(mass_HeI);
                free(mass_HeII);
                free(vel_tot);
                free(vel_HI);
                free(vel_HeI);
                free(vel_HeII);
                free(temp_tot);
                free(temp_HI);
                free(temp_HeI);
                free(temp_HeII);
		free(divv_tot) ;
                free(res);
		free(vbins_tot);
		free(vbins_neut_HI);
		free(vbins_temp_HI);
		free(vbins_rho_HI);
		free(vbins_res_HI);
                return ;
              }
            vbins_temp2_tot = (double *)malloc(nvbin*sizeof(*vbins_temp2_tot));
            if(vbins_temp2_tot == NULL)
              {
                printf("<sorry, no memory for velocity bins, %s>\n",title) ;
                free(vbins_t_HI);
                free(vbins_HI);
		free(vbins_D);
                free(vbins_HeI);
                free(vbins_HeII);
                free(mass_tot);
                free(mass_HI);
                free(mass_HeI);
                free(mass_HeII);
                free(vel_tot);
                free(vel_HI);
                free(vel_HeI);
                free(vel_HeII);
                free(temp_tot);
                free(temp_HI);
                free(temp_HeI);
                free(temp_HeII);
		free(divv_tot) ;
                free(res);
		free(vbins_tot);
		free(vbins_neut_HI);
		free(vbins_temp_HI);
		free(vbins_rho_HI);
		free(vbins_res_HI);
		free(vbins_temp_tot);
                return ;
              }
            vbins_divv_tot = (double *)malloc(nvbin*sizeof(*vbins_divv_tot));
            if(vbins_divv_tot == NULL)
              {
                printf("<sorry, no memory for velocity bins, %s>\n",title) ;
                free(vbins_t_HI);
                free(vbins_HI);
		free(vbins_D);
                free(vbins_HeI);
                free(vbins_HeII);
                free(mass_tot);
                free(mass_HI);
                free(mass_HeI);
                free(mass_HeII);
                free(vel_tot);
                free(vel_HI);
                free(vel_HeI);
                free(vel_HeII);
                free(temp_tot);
                free(temp_HI);
                free(temp_HeI);
                free(temp_HeII);
		free(divv_tot) ;
                free(res);
		free(vbins_tot);
		free(vbins_neut_HI);
		free(vbins_temp_HI);
		free(vbins_rho_HI);
		free(vbins_res_HI);
		free(vbins_temp_tot);
		free(vbins_temp2_tot);
                return ;
              }
	    for(i = 0; i < nvbin; i++){
		vbins_tot[i] = 0.0;
		vbins_neut_HI[i] = 0.0;
		vbins_temp_HI[i] = 0.0;
		vbins_rho_HI[i] = 0.0;
		vbins_res_HI[i] = 0.0;
		vbins_temp_tot[i] = 0.0;
		vbins_temp2_tot[i] = 0.0;
		vbins_divv_tot[i] = 0.0;
	    }
	}
	for(i = 0; i < zbin; i++){
	    if(mass_HI[i] == 0.0)
		continue ;
	    for(j = 0; j < SUBBIN; j++){
		z = ((double)(i) + ((double)(j))/((double)(SUBBIN)))*bin_size ;
		if(2*j < SUBBIN){
		    k = i - 1 ;
		}
		else {
		    k = i ;
		}
		k = max(0,k) ;
		k = min(zbin-2,k) ;
		v_interp = (vel_HI[k+1] - vel_HI[k])*(i + ((double)(j))/
			((double)(SUBBIN)) - (k + 0.5)) + vel_HI[k] ;
		t_interp = (temp_HI[k+1] - temp_HI[k])*(i + ((double)(j))/
			((double)(SUBBIN)) - (k + 0.5)) + temp_HI[k] ;
		b = bsys_H*sqrt(t_interp) ;
		if(comove == YES){
		    v_interp += hubble_constant*z*rsys + voffset ;
		}
		if((v_interp < vmax && v_interp >= vmin) ||
			(autolim == YES && periodic == YES)){
	            bin = floor((v_interp - vmin)/vbin_size) ;
		    ibin = bin%nvbin;
		    if(ibin < 0) {
		      ibin += nvbin;
		    }
		    vbins_t_HI[ibin] += mass_HI[i]/(double)SUBBIN ;
		}
		bin_min = floor((v_interp - 6.0*b - vmin)/vbin_size) ;
		bin_max = floor((v_interp + 6.0*b - vmin)/vbin_size) ;
		if((v_interp + 6.0*b - vmin)/vbin_size == (double) bin_max)
		  bin_max--;
		for(bin = bin_min; bin <= bin_max; bin++){
		    if((bin >= 0 && bin < nvbin) || 
			    (autolim == YES && periodic == YES)){
			if(bin == bin_min){
			    vlower = -6.0*b ;
			}
			else{
			    vlower = ((double)(bin))*vbin_size+vmin -
				    v_interp ;
			}
			if(bin == bin_max){
			    vupper = 6.0*b ;
			}
			else{
			    vupper = ((double)(bin+1))*vbin_size+vmin -
				    v_interp ;
			}
			vlower /= b ;
			vupper /= b ;
			abs_vlower = fabs(vlower) ;
			abs_vupper = fabs(vupper) ;
			ibin = bin%nvbin;
			if(ibin < 0) {
			  ibin += nvbin;
			}
			if(vupper*vlower < 0){
			    dvcol = 1./(double)SUBBIN*0.5*
				    (erf(abs_vlower) + erf(vupper)) ;
			}
			else{
			    if(abs_vlower < abs_vupper){
				dvcol = 1./(double)SUBBIN*
					0.5*(erf(abs_vupper) -
					erf(abs_vlower)) ;
			    }
			    else{
				dvcol = 1./(double)SUBBIN*
					0.5*(erf(abs_vlower) -
					erf(abs_vupper)) ;
			    }
			}
			dvcol_HI = mass_HI[i]*dvcol ;
			vbins_HI[ibin] += dvcol_HI ;
			if(plot_type == VELCOL){
			    dvcol_tot = mass_tot[i]*dvcol ;
			    vbins_tot[ibin] += dvcol_tot ;
			    vbins_neut_HI[ibin] += dvcol_HI*mass_HI[i]/
				    (fhydrogen*mass_tot[i]) ;
			    vbins_temp_HI[ibin] += dvcol_HI*temp_HI[i] ;
			    vbins_rho_HI[ibin] += dvcol_HI*mass_tot[i] ;
			    vbins_res_HI[ibin] += dvcol_HI*res[i] ;
			    vbins_temp_tot[ibin] += dvcol_tot*temp_tot[i] ;
			    vbins_temp2_tot[ibin] += dvcol_tot*temp_tot[i]*
				    temp_tot[i];
			    vbins_divv_tot[ibin] += dvcol_tot*divv_tot[i] ;
			}
		    }
		}
	    }
	}
	for(i = 0; i < zbin; i++){
	    if(mass_HI[i] == 0.0)
		continue ;
	    for(j = 0; j < SUBBIN; j++){
		z = ((double)(i) + ((double)(j))/((double)(SUBBIN)))*bin_size ;
		if(2*j < SUBBIN){
		    k = i - 1 ;
		}
		else {
		    k = i ;
		}
		k = max(0,k) ;
		k = min(zbin-2,k) ;
		v_interp = (vel_HI[k+1] - vel_HI[k])*(i + ((double)(j))/
			((double)(SUBBIN)) - (k + 0.5)) + vel_HI[k] ;
		t_interp = (temp_HI[k+1] - temp_HI[k])*(i + ((double)(j))/
			((double)(SUBBIN)) - (k + 0.5)) + temp_HI[k] ;
		b = bsys_D*sqrt(t_interp) ;
		if(comove == YES){
		    v_interp += hubble_constant*z*rsys + voffset ;
		}
		if((v_interp < vmax && v_interp >= vmin) ||
			(autolim == YES && periodic == YES)){
	            bin = floor((v_interp - vmin)/vbin_size) ;
		    ibin = bin%nvbin;
		    if(ibin < 0) {
		      ibin += nvbin;
		    }
		}
		bin_min = floor((v_interp - 6.0*b - vmin)/vbin_size) ;
		bin_max = floor((v_interp + 6.0*b - vmin)/vbin_size) ;
		if((v_interp + 6.0*b - vmin)/vbin_size == (double) bin_max)
		  bin_max--;
		for(bin = bin_min; bin <= bin_max; bin++){
		    if((bin >= 0 && bin < nvbin) || 
			    (autolim == YES && periodic == YES)){
			if(bin == bin_min){
			    vlower = -6.0*b ;
			}
			else{
			    vlower = ((double)(bin))*vbin_size+vmin -
				    v_interp ;
			}
			if(bin == bin_max){
			    vupper = 6.0*b ;
			}
			else{
			    vupper = ((double)(bin+1))*vbin_size+vmin -
				    v_interp ;
			}
			vlower /= b ;
			vupper /= b ;
			abs_vlower = fabs(vlower) ;
			abs_vupper = fabs(vupper) ;
			ibin = bin%nvbin;
			if(ibin < 0) {
			  ibin += nvbin;
			}
			if(vupper*vlower < 0){
			    dvcol = 1./(double)SUBBIN*0.5*
				    (erf(abs_vlower) + erf(vupper)) ;
			}
			else{
			    if(abs_vlower < abs_vupper){
				dvcol = 1./(double)SUBBIN*
					0.5*(erf(abs_vupper) -
					erf(abs_vlower)) ;
			    }
			    else{
				dvcol = 1./(double)SUBBIN*
					0.5*(erf(abs_vlower) -
					erf(abs_vupper)) ;
			    }
			}
			dvcol_HI = mass_HI[i]*dvcol ;
			vbins_D[ibin] += dvcol_HI ;
		    }
		}
	    }
	}
	for(i = 0; i < zbin; i++){
	    if(mass_HeI[i] == 0.0)
		continue ;
	    for(j = 0; j < SUBBIN; j++){
		z = ((double)(i) + ((double)(j))/((double)(SUBBIN)))*bin_size ;
		if(2*j < SUBBIN){
		    k = i - 1 ;
		}
		else {
		    k = i ;
		}
		k = max(0,k) ;
		k = min(zbin-2,k) ;
		v_interp = (vel_HeI[k+1] - vel_HeI[k])*(i + ((double)(j))/
			((double)(SUBBIN)) - (k + 0.5)) + vel_HeI[k] ;
		t_interp = (temp_HeI[k+1] - temp_HeI[k])*(i + ((double)(j))/
			((double)(SUBBIN)) - (k + 0.5)) + temp_HeI[k] ;
		b = bsys_He*sqrt(t_interp) ;
		if(comove == YES){
		    v_interp += hubble_constant*z*rsys + voffset ;
		}
		if((v_interp < vmax && v_interp >= vmin) ||
			(autolim == YES && periodic == YES)){
	            bin = floor((v_interp - vmin)/vbin_size) ;
		    ibin = bin%nvbin;
		    if(ibin < 0) {
		      ibin += nvbin;
		    }
		}
		bin_min = floor((v_interp - 6.0*b - vmin)/vbin_size) ;
		bin_max = floor((v_interp + 6.0*b - vmin)/vbin_size) ;
		if((v_interp + 6.0*b - vmin)/vbin_size == (double) bin_max)
		  bin_max--;
		for(bin = bin_min; bin <= bin_max; bin++){
		    if((bin >= 0 && bin < nvbin) || 
			    (autolim == YES && periodic == YES)){
			if(bin == bin_min){
			    vlower = -6.0*b ;
			}
			else{
			    vlower = ((double)(bin))*vbin_size+vmin -
				    v_interp ;
			}
			if(bin == bin_max){
			    vupper = 6.0*b ;
			}
			else{
			    vupper = ((double)(bin+1))*vbin_size+vmin -
				    v_interp ;
			}
			vlower /= b ;
			vupper /= b ;
			abs_vlower = fabs(vlower) ;
			abs_vupper = fabs(vupper) ;
			ibin = bin%nvbin;
			if(ibin < 0) {
			  ibin += nvbin;
			}
			if(vupper*vlower < 0){
			    vbins_HeI[ibin] += mass_HeI[i]/(double)SUBBIN*0.5*
				    (erf(abs_vlower) + erf(vupper)) ;
			}
			else{
			    if(abs_vlower < abs_vupper){
				vbins_HeI[ibin] += mass_HeI[i]/(double)SUBBIN*
					0.5*(erf(abs_vupper) -
					erf(abs_vlower)) ;
			    }
			    else{
				vbins_HeI[ibin] += mass_HeI[i]/(double)SUBBIN*
					0.5*(erf(abs_vlower) -
					erf(abs_vupper)) ;
			    }
			}
		    }
		}
	    }
	}
	for(i = 0; i < zbin; i++){
	    if(mass_HeII[i] == 0.0)
		continue ;
	    for(j = 0; j < SUBBIN; j++){
		z = ((double)(i) + ((double)(j))/((double)(SUBBIN)))*bin_size ;
		if(2*j < SUBBIN){
		    k = i - 1 ;
		}
		else {
		    k = i ;
		}
		k = max(0,k) ;
		k = min(zbin-2,k) ;
		v_interp = (vel_HeII[k+1] - vel_HeII[k])*(i + ((double)(j))/
			((double)(SUBBIN)) - (k + 0.5)) + vel_HeII[k] ;
		t_interp = (temp_HeII[k+1] - temp_HeII[k])*(i + ((double)(j))/
			((double)(SUBBIN)) - (k + 0.5)) + temp_HeII[k] ;
		b = bsys_He*sqrt(t_interp) ;
		if(comove == YES){
		    v_interp += hubble_constant*z*rsys + voffset ;
		}
		if((v_interp < vmax && v_interp >= vmin) ||
			(autolim == YES && periodic == YES)){
	            bin = floor((v_interp - vmin)/vbin_size) ;
		    ibin = bin%nvbin;
		    if(ibin < 0) {
		      ibin += nvbin;
		    }
		}
		bin_min = floor((v_interp - 6.0*b - vmin)/vbin_size) ;
		bin_max = floor((v_interp + 6.0*b - vmin)/vbin_size) ;
		if((v_interp + 6.0*b - vmin)/vbin_size == (double) bin_max)
		  bin_max--;
		for(bin = bin_min; bin <= bin_max; bin++){
		    if((bin >= 0 && bin < nvbin) || 
			    (autolim == YES && periodic == YES)){
			if(bin == bin_min){
			    vlower = -6.0*b ;
			}
			else{
			    vlower = ((double)(bin))*vbin_size+vmin -
				    v_interp ;
			}
			if(bin == bin_max){
			    vupper = 6.0*b ;
			}
			else{
			    vupper = ((double)(bin+1))*vbin_size+vmin -
				    v_interp ;
			}
			vlower /= b ;
			vupper /= b ;
			abs_vlower = fabs(vlower) ;
			abs_vupper = fabs(vupper) ;
			ibin = bin%nvbin;
			if(ibin < 0) {
			  ibin += nvbin;
			}
			if(vupper*vlower < 0){
			    vbins_HeII[ibin] += mass_HeII[i]/(double)SUBBIN*0.5*
				    (erf(abs_vlower) + erf(vupper)) ;
			}
			else{
			    if(abs_vlower < abs_vupper){
				vbins_HeII[ibin] += mass_HeII[i]/(double)SUBBIN*
					0.5*(erf(abs_vupper) -
					erf(abs_vlower)) ;
			    }
			    else{
				vbins_HeII[ibin] += mass_HeII[i]/(double)SUBBIN*
					0.5*(erf(abs_vlower) -
					erf(abs_vupper)) ;
			    }
			}
		    }
		}
	    }
	}
	if(plot_type == VELCOL){
	    sprintf(col_name,"%s.vel",name) ;
	    fp2 = fopen(col_name, "w");
	    if(fp2 == NULL)
	      {
		printf("<sorry, cannot open file %s, %s>\n",col_name, title) ;
		return;
	      }
	    for(i = 0; i < nvbin; i++){
		if(vbins_HI[i] > 0.0){
		    vbins_neut_HI[i] /= vbins_HI[i] ;
		    vbins_temp_HI[i] /= vbins_HI[i] ;
		    vbins_rho_HI[i] /= vbins_HI[i] ;
		    vbins_res_HI[i] /= vbins_HI[i] ;
		}
		if(vbins_tot[i] > 0.0){
		    vbins_temp_tot[i] /= vbins_tot[i] ;
		    vbins_temp2_tot[i] /= vbins_tot[i] ;
		    vbins_divv_tot[i] /= vbins_tot[i] ;
		}
		fprintf(fp2, "%g %g %g %g %g %g %g %g %g %g\n",
			vmin + (i+0.5)*vbin_size, vbins_HI[i]/vbin_size,
			vbins_tot[i]*MHYDR/vbin_size,vbins_neut_HI[i],
			vbins_temp_HI[i],
			vbins_rho_HI[i]*MHYDR/(bin_size*cosmof*kpcunit*KPCCM),
			vbins_res_HI[i]*rsys, vbins_temp_tot[i],
			vbins_temp2_tot[i], vbins_divv_tot[i]) ;
		}
	    fclose(fp2);
	}
	for(i = 0; i < nvbin; i++){
	    vbins_HI[i] = LYCS_HI*vbins_HI[i]/vbin_size ;
	    vbins_D[i] = LYCS_HI*vbins_D[i]/vbin_size ;
	    vbins_HeI[i] = LYCS_HeI*vbins_HeI[i]/vbin_size ;
	    vbins_HeII[i] = LYCS_HeII*vbins_HeII[i]/vbin_size ;
	    vbins_t_HI[i] = LYCS_HI*vbins_t_HI[i]/vbin_size ;
	    fprintf(fp, "%g %g %g %g %g %g\n", vmin + (i+0.5)*vbin_size,
		    vbins_HI[i],vbins_HeI[i],vbins_HeII[i],vbins_t_HI[i],
		    vbins_D[i]) ;
	}
	fclose(fp);
	free(vbins_t_HI);
	free(vbins_HI);
	free(vbins_D);
	free(vbins_HeI);
	free(vbins_HeII);
	free(mass_tot);
	free(mass_HI);
	free(mass_HeI);
	free(mass_HeII);
	free(vel_tot);
	free(vel_HI);
	free(vel_HeI);
	free(vel_HeII);
	free(temp_tot);
	free(temp_HI);
	free(temp_HeI);
	free(temp_HeII);
	free(divv_tot) ;
	free(res);
	if(plot_type == VELCOL){
	    free(vbins_tot);
	    free(vbins_neut_HI);
	    free(vbins_temp_HI);
	    free(vbins_rho_HI);
	    free(vbins_res_HI);
	    free(vbins_temp_tot);
	    free(vbins_temp2_tot);
	    free(vbins_divv_tot);
	}
      }
      }
      else {
	input_error(command) ;
      }
    }
    else {
      printf("<plot does not represent the present internal state, %s>",title) ;
    }
}
