/* $Header$
 * $Log$
 * Revision 1.3  1995/03/02 21:43:01  nsk
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

/* Lyman alpha cross section in cm^2 per atom */
#define LYCS (1.34e-12)

void
absorb(job)
     char *job;
{
    char command[MAXCOMM] ;
    char type[MAXCOMM] ;        /* type of velocity column */
    char name[MAXCOMM] ;	/* file name for output */
    int nvbin;			/* number of velocity bins */
    double vmin, vmax;          /* minimum and maximum velocity */
    int zbin;			/* number of spatial bins */
    char x_string[MAXCOMM] ;
    char y_string[MAXCOMM] ;
    Real x1[MAXDIM];            /* x and y coordinates of pencil beam */
    Real *vbins;
    Real *vbins_t;
    Real *mass ;
    Real *vel ;
    Real *temp ;
    Real *res ;
    Real tau ;
    int plot_type;
    int i,j,k ;
    double part_pos[MAXDIM] ;
    double distnorm ;
    double radius ;
    double radius2 ;
    double radius4 ;
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
    double offset_x, offset_y ;
    double span_x, span_y ;
    double rsys ;
    double vsys ;
    double msys ;
    double bsys ;
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
    int irep[MAXDIM];
    int autolim ;
    int ibin;
    FILE *fp;

    if (current_project && current_color){
      /* command takes plot type, box of column, minimum and maximum */
      /* velocity, number of velocity bins and output file name as */
      /* arguments. */
      if (sscanf(job,"%s %s %d %lf %lf %d %s %s %s",command,type,&zbin,
	    &vmin,&vmax,&nvbin, name,x_string,y_string) >= 8) {
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
	fp = fopen(name, "w");
	if(fp == NULL)
	  {
	    printf("<sorry, cannot open file %s, %s>\n",name, title) ;
	    return;
	  }
	vbins = (Real *)malloc(nvbin*sizeof(*vbins));
	if(vbins == NULL)
	  {
	    printf("<sorry, no memory for velocity bins, %s>\n",title) ;
	    return ;
	  }
	vbins_t = (Real *)malloc(nvbin*sizeof(*vbins_t));
	if(vbins_t == NULL)
	  {
	    printf("<sorry, no memory for velocity bins, %s>\n",title) ;
	    free(vbins);
	    return ;
	  }
	mass = (Real *)malloc(zbin*sizeof(*mass));
	if(mass == NULL)
	  {
	    printf("<sorry, no memory for column bins, %s>\n",title) ;
	    free(vbins);
	    free(vbins_t);
	    return ;
	  }
	vel = (Real *)malloc(zbin*sizeof(*vel));
	if(vel == NULL)
	  {
	    printf("<sorry, no memory for column bins, %s>\n",title) ;
	    free(vbins);
	    free(vbins_t);
	    free(mass);
	    return ;
	  }
	temp = (Real *)malloc(zbin*sizeof(*temp));
	if(temp == NULL)
	  {
	    printf("<sorry, no memory for column bins, %s>\n",title) ;
	    free(vbins);
	    free(vbins_t);
	    free(mass);
	    free(vel);
	    return ;
	  }
	res = (Real *)malloc(zbin*sizeof(*res));
	if(res == NULL)
	  {
	    printf("<sorry, no memory for column bins, %s>\n",title) ;
	    free(vbins);
	    free(vbins_t);
	    free(mass);
	    free(vel);
	    free(temp);
	    return ;
	  }
	if (!cool_loaded ){
	    load_cool() ;
	}
	if (!redshift_loaded ){
	    load_redshift() ;
	}
	if (strcmp(type,"all") == 0 || strcmp(type,"allcol") == 0 ||
		strcmp(type,"allcolumn") == 0){
	  plot_type = RHO ;
	}
	else if(strcmp(type,"hneutral") == 0 || strcmp(type,"hneut") == 0 ||
		strcmp(type,"hneutralcolumn") == 0 ||
		strcmp(type,"hneutralcol") == 0 ||
		strcmp(type,"hneutcol") == 0){
	  plot_type = HNEUT ;
	  if(!hneutral_loaded){
	    hneutral_func() ;
	  }
	}
	else
	  {
	    printf("<sorry, %s is not an absorption type, %s>\n",type,title) ;
	    free(vbins);
	    free(vbins_t);
	    free(mass);
	    free(vel);
	    free(temp);
	    free(res);
	    return;
	  }
	for(i = 0; i < zbin; i++){
	    mass[i] = 0.0;
	    vel[i] = 0.0;
	    temp[i] = 0.0;
	    res[i] = 0.0;
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
			radius4 = radius2 * radius2 ;
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
			    if(zlower*zupper < 0.0){  /* bin stradles zero */
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
			    if(plot_type == HNEUT) {
			        kernel *= fhydrogen*hneutral[gp-gas_particles];
			    }
			    if(comove == YES){
				vkernel /= PI*gp->hsmooth ;
				if(plot_type == HNEUT) {
				    vkernel *= fhydrogen*
					    hneutral[gp-gas_particles];
				}
			    }
			    mass[bin] += kernel*gp->mass ;
			    vel[bin] += kernel*(gp->mass)*vsys*vz ;
			    if(comove == YES){
				vel[bin] += vkernel*(gp->mass)*rsys*
					hubble_constant ;
			    }
			    temp[bin] += kernel*(gp->mass)*(gp->temp) ;
			    res[bin] += kernel*(gp->mass)*(gp->hsmooth) ;
			}
		    }
		}
	      }
	    }
	  }
	}
	for(i = 0; i < zbin; i++){
	    if(mass[i] != 0.0){
		vel[i] /= mass[i] ;
		temp[i] /= mass[i] ;
		res[i] /= mass[i] ;
	    }
	}
	msys = msolunit/(cosmof*kpcunit*cosmof*kpcunit)*
                (MSOLG/(KPCCM*KPCCM*MHYDR)) ;
	for(i = 0; i < zbin; i++){
	    mass[i] *= msys ;
	}
	if (strcmp(type,"allcol") == 0 || strcmp(type,"allcolumn") == 0 ||
		strcmp(type,"hneutralcolumn") == 0 ||
		strcmp(type,"hneutralcol") == 0 ||
		strcmp(type,"hneutcol") == 0){
	    for(i = 0; i < zbin; i++)
	      {
		fprintf(fp, "%g %g %g %g %g\n", rsys*(bin_size*(i+0.5) + zmin),
			mass[i], vel[i], temp[i],rsys*res[i]);
	      }
	    fclose(fp);
	    free(vbins);
	    free(vbins_t);
	    free(mass);
	    free(vel);
	    free(temp);
	    free(res);
	    return ;
	}
	bsys = sqrt(2.*KBOLTZ/MHYDR)/1.e5 ;
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
		    vmin = min(vmin,vel[i]) ;
		    vmax = max(vmax,vel[i]) ;
		}
	    }
	    else{
		voffset = vmin ;
		vmax = voffset + (zmax - zmin)*rsys*hubble_constant ;
	    }
	}
	vbin_size = (vmax - vmin)/(double)nvbin ;
	for(i = 0; i < nvbin; i++){
	    vbins[i] = 0.0;
	    vbins_t[i] = 0.0;
	}
	for(i = 0; i < zbin; i++){
	    if(mass[i] == 0.0)
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
		v_interp = (vel[k+1] - vel[k])*(i + ((double)(j))/
			((double)(SUBBIN)) - (k + 0.5)) + vel[k] ;
		t_interp = (temp[k+1] - temp[k])*(i + ((double)(j))/
			((double)(SUBBIN)) - (k + 0.5)) + temp[k] ;
		b = bsys*sqrt(t_interp) ;
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
		    vbins_t[ibin] += mass[i]/(double)SUBBIN ;
		}
		if((v_interp + 6.0*b <= vmax && v_interp - 6.0*b >= vmin) ||
			(autolim == YES && periodic == YES)){
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
				vbins[ibin] += mass[i]/(double)SUBBIN*0.5*
					(erf(abs_vlower) + erf(vupper)) ;
			    }
			    else{
				if(abs_vlower < abs_vupper){
				    vbins[ibin] += mass[i]/(double)SUBBIN*
					    0.5*(erf(abs_vupper) -
					    erf(abs_vlower)) ;
				}
				else{
				    vbins[ibin] += mass[i]/(double)SUBBIN*
					    0.5*(erf(abs_vlower) -
					    erf(abs_vupper)) ;
				}
			    }
			}
		    }
		}
	    }
	}
	for(i = 0; i < nvbin; i++){
	    tau=LYCS*vbins[i]/vbin_size ;
	    vbins[i] = exp(-tau);
	    vbins_t[i] = exp(-LYCS*vbins_t[i]/vbin_size);
	    fprintf(fp, "%g %g %g %g\n", vmin + (i+0.5)*vbin_size,
		    vbins[i],vbins_t[i],tau) ;
	}
	fclose(fp);
	free(vbins);
	free(vbins_t);
	free(mass);
	free(vel);
	free(temp);
	free(res);
      }
      else {
	input_error(command) ;
      }
    }
    else {
      printf("<plot does not represent the present internal state, %s>",title) ;
    }
}
