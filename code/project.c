/*
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:30  trq
 * Initial revision
 *
 * Revision 2.6  1994/09/01  23:58:43  trq
 * Ensure that the axes projections do not overflow.
 *
 * Revision 2.5  94/05/05  11:38:36  trq
 * Changed velocity clipping to -2*INTMAX and 2*INTMAX.
 * 
 * Revision 2.4  94/04/20  08:45:56  trq
 * Added title variable.
 * 
 * Revision 2.2  94/02/22  16:25:00  trq
 * Added zoom_factor reset.
 * 
 * Revision 2.1  94/02/18  10:16:22  trq
 * Corrected array allocation.
 * 
 */
#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>

void
project()
{
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    struct vec *v ;
    double x,y ;
    Real box_coord[BOXPTS][MAXDIM] ;
    int i,j,k ;
    int offset;
    int aoffset;

    dv1_x = -INTMAX ;
    dv1_y = -INTMAX ;
    dv2_x = INTMAX ;
    dv2_y = INTMAX ;
    view_size = INTMAX ;
    zoom_factor = 1.0;
    reset_zoom_scroll();
    scaling = 2. * (double)INTMAX / boxes[active_box].size ;
	
    if ((showgas || gas_plot) && !star_plot &&
	    ((!radial_plot && !potential_plot && !magnitude_plot)
	    || ((potential_plot  || radial_plot || magnitude_plot) &&
	    (color_type == GAS || color_type == ALL)))) {
	if(gas_x != NULL) free(gas_x);
	if(gas_y != NULL) free(gas_y);
	if(boxlist[active_box].ngas != 0) {
	    gas_x = (int *)malloc(boxlist[active_box].ngas*sizeof(*gas_x));
	    if(gas_x == NULL) {
		printf("<sorry, no memory for gas points, %s>\n",title) ;
		return ;
	    }
	    gas_y = (int *)malloc(boxlist[active_box].ngas*sizeof(*gas_y));
	    if(gas_y == NULL) {
		printf("<sorry, no memory for gas points, %s>\n",title) ;
		return ;
	    }
	}
	else {
	  gas_x = NULL;
	  gas_y = NULL;
	}  
	for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
	    gp = boxlist[active_box].gp[i] ;
	    for (x = y = 0.0,j = 0 ;j < header.ndim ;j++) {
		x += rot_matrix[0][j] * (gp->pos[j] -
			boxes[active_box].center[j]) ;
		y += rot_matrix[1][j] * (gp->pos[j] -
			boxes[active_box].center[j]) ;
	    }
	    gas_x[i] = (int)(x * scaling + 0.5) ;
	    gas_y[i] = (int)(y * scaling + 0.5) ;
	}
    }
    if (showdark && !gas_plot && !star_plot &&
	    ((!radial_plot && !potential_plot && !magnitude_plot)
	    || ((potential_plot  || radial_plot || magnitude_plot) &&
	    (color_type == DARK || color_type == ALL)))) {
	if(dark_x != NULL) free(dark_x);
	if(dark_y != NULL) free(dark_y);
	if(boxlist[active_box].ndark != 0) {
	    dark_x = (int *)malloc(boxlist[active_box].ndark*sizeof(*dark_x));
	    if(dark_x == NULL) {
		printf("<sorry, no memory for dark points, %s>\n",title) ;
		return ;
	    }
	    dark_y = (int *)malloc(boxlist[active_box].ndark*sizeof(*dark_y));
	    if(dark_y == NULL) {
		printf("<sorry, no memory for dark points, %s>\n",title) ;
		return ;
	    }
	}
	else {
	  dark_x = NULL;
	  dark_y = NULL;
	}  
	for (i = 0 ;i < boxlist[active_box].ndark ;i++) {
	    dp = boxlist[active_box].dp[i] ;
	    for (x = y = 0.0,j = 0 ;j < header.ndim ;j++) {
		x += rot_matrix[0][j] * (dp->pos[j] -
			boxes[active_box].center[j]) ;
		y += rot_matrix[1][j] * (dp->pos[j] -
			boxes[active_box].center[j]) ;
	    }
	    dark_x[i] = (int)(x * scaling +0.5) ;
	    dark_y[i] = (int)(y * scaling +0.5) ;
	}
    }
    if ((showstar || star_plot)
	    && !gas_plot &&((!radial_plot && !potential_plot && !magnitude_plot)
	    || ((potential_plot  || radial_plot || magnitude_plot) &&
	    (color_type == STAR || color_type == ALL)))) {
	if(star_x != NULL) free(star_x);
	if(star_y != NULL) free(star_y);
	if(boxlist[active_box].nstar != 0) {
	    star_x = (int *)malloc(boxlist[active_box].nstar*sizeof(*star_x));
	    if(star_x == NULL) {
		printf("<sorry, no memory for star points, %s>\n",title) ;
		return ;
	    }
	    star_y = (int *)malloc(boxlist[active_box].nstar*sizeof(*star_y));
	    if(star_y == NULL) {
		printf("<sorry, no memory for star points, %s>\n",title) ;
		return ;
	    }
	}  
	else {
	  star_x = NULL;
	  star_y = NULL;
	}  
	for (i = 0 ;i < boxlist[active_box].nstar ;i++) {
	    sp = boxlist[active_box].sp[i] ;
	    for (x = y = 0.0,j = 0 ;j < header.ndim ;j++) {
		x += rot_matrix[0][j] * (sp->pos[j] -
			boxes[active_box].center[j]) ;
		y += rot_matrix[1][j] * (sp->pos[j] -
			boxes[active_box].center[j]) ;
	    }
	    star_x[i] = (int)(x * scaling +0.5) ;
	    star_y[i] = (int)(y * scaling +0.5) ;
	}
    }
    if (showvel) {
	if ((showgas || gas_plot) && !star_plot &&
		((!radial_plot && !potential_plot && !magnitude_plot)
		|| ((potential_plot  || radial_plot || magnitude_plot) &&
		(color_type == GAS || color_type == ALL)))) {
	    if(gasvel_x != NULL) free(gasvel_x);
	    if(gasvel_y != NULL) free(gasvel_y);
	    if(boxlist[active_box].ngas != 0) {
		gasvel_x = (int *)
		    malloc(boxlist[active_box].ngas*sizeof(*gasvel_x));
		if(gasvel_x == NULL) {
		    printf("<sorry, no memory for gas velocity, %s>\n",title) ;
		    return ;
		}
		gasvel_y = (int *)
			    malloc(boxlist[active_box].ngas*sizeof(*gasvel_y));
		if(gasvel_y == NULL) {
		    printf("<sorry, no memory for gas velocity, %s>\n",title) ;
		    return ;
		}
	    }  
	    else {
	      gasvel_x = NULL;
	      gasvel_y = NULL;
	    }  
	    for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		gp = boxlist[active_box].gp[i] ;
		for (x = y = 0.0,j = 0 ;j < header.ndim ;j++) {
		    x += rot_matrix[0][j] * (gp->pos[j] -
			    boxes[active_box].center[j] +
			    (vel_scale * gp->vel[j])) ;
		    y += rot_matrix[1][j] * (gp->pos[j] -
			    boxes[active_box].center[j] +
			    (vel_scale * gp->vel[j])) ;
		}
		gasvel_x[i] = (int)max(-2*INTMAX,min(2*INTMAX,
			(x * scaling +0.5))) ;
		gasvel_y[i] = (int)max(-2*INTMAX,min(2*INTMAX,
			(y * scaling +0.5))) ;
	    }
	}
	if (showdark && !gas_plot && !star_plot &&
		((!radial_plot && !potential_plot && !magnitude_plot)
		|| ((potential_plot  || radial_plot || magnitude_plot) &&
		(color_type == DARK || color_type == ALL)))) {
	    if(darkvel_x != NULL) free(darkvel_x);
	    if(darkvel_y != NULL) free(darkvel_y);
	    if(boxlist[active_box].ndark != 0) {
		darkvel_x = (int *)
		  malloc(boxlist[active_box].ndark*sizeof(*darkvel_x));
		if(darkvel_x == NULL) {
		    printf("<sorry, no memory for dark velocity, %s>\n",title) ;
		    return ;
		}
		darkvel_y = (int *)
		  malloc(boxlist[active_box].ndark*sizeof(*darkvel_y));
		if(darkvel_y == NULL) {
		    printf("<sorry, no memory for dark velocity, %s>\n",title) ;
		    return ;
		}
	    }  
	    else {
	      darkvel_x = NULL;
	      darkvel_y = NULL;
	    }  
	    for (i = 0 ;i < boxlist[active_box].ndark ;i++) {
		dp = boxlist[active_box].dp[i] ;
		for (x = y = 0.0,j = 0 ;j < header.ndim ;j++) {
		    x += rot_matrix[0][j] * (dp->pos[j] -
			    boxes[active_box].center[j] +
			    (vel_scale * dp->vel[j])) ;
		    y += rot_matrix[1][j] * (dp->pos[j] -
			    boxes[active_box].center[j] +
			    (vel_scale * dp->vel[j])) ;
		}
		darkvel_x[i] = (int)max(-2*INTMAX,min(2*INTMAX,
			(x * scaling +0.5))) ;
		darkvel_y[i] = (int)max(-2*INTMAX,min(2*INTMAX,
			(y * scaling +0.5))) ;
	    }
	}
	if ((showstar || star_plot)
		&& !gas_plot &&((!radial_plot && !potential_plot &&
		!magnitude_plot) || ((potential_plot  || radial_plot ||
		magnitude_plot) &&
		(color_type == STAR || color_type == ALL)))) {
	    if(starvel_x != NULL) free(starvel_x);
	    if(starvel_y != NULL) free(starvel_y);
	    if(boxlist[active_box].nstar != 0) {
		starvel_x = (int *)
		    malloc(boxlist[active_box].nstar*sizeof(*starvel_x));
		if(starvel_x == NULL) {
		    printf("<sorry, no memory for star velocity, %s>\n",title) ;
		    return ;
		}
		starvel_y = (int *)
		    malloc(boxlist[active_box].nstar*sizeof(*starvel_y));
		if(starvel_y == NULL) {
		    printf("<sorry, no memory for star velocity, %s>\n",title) ;
		    return ;
		}
	    }  
	    else {
	      starvel_x = NULL;
	      starvel_y = NULL;
	    }  
	    for (i = 0 ;i < boxlist[active_box].nstar ;i++) {
		sp = boxlist[active_box].sp[i] ;
		for (x = y = 0.0,j = 0 ;j < header.ndim ;j++) {
		    x += rot_matrix[0][j] * (sp->pos[j] -
			    boxes[active_box].center[j] +
			    (vel_scale * sp->vel[j])) ;
		    y += rot_matrix[1][j] * (sp->pos[j] -
			    boxes[active_box].center[j] +
			    (vel_scale * sp->vel[j])) ;
		}
		starvel_x[i] = (int)max(-2*INTMAX,min(2*INTMAX,
			(x * scaling +0.5))) ;
		starvel_y[i] = (int)max(-2*INTMAX,min(2*INTMAX,
			(y * scaling +0.5))) ;
	    }
	}
    }
    else if (showvec) {
	if ((showgas || gas_plot) && !star_plot &&
		((!radial_plot && !potential_plot && !magnitude_plot)
		|| ((potential_plot  || radial_plot || magnitude_plot) &&
		(color_type == GAS || color_type == ALL)))) {
	    if(gasvel_x != NULL) free(gasvel_x);
	    if(gasvel_y != NULL) free(gasvel_y);
	    if(boxlist[active_box].ngas != 0) {
		gasvel_x = (int *)
		    malloc(boxlist[active_box].ngas*sizeof(*gasvel_x));
		if(gasvel_x == NULL) {
		    printf("<sorry, no memory for gas vector, %s>\n",title) ;
		    return ;
		}
		gasvel_y = (int *)
			    malloc(boxlist[active_box].ngas*sizeof(*gasvel_y));
		if(gasvel_y == NULL) {
		    printf("<sorry, no memory for gas vector, %s>\n",title) ;
		    return ;
		}
	    }  
	    else {
	      gasvel_x = NULL;
	      gasvel_y = NULL;
	    }  
	    offset = 0;
	    for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		gp = boxlist[active_box].gp[i] ;
		aoffset = gp-gas_particles+offset;
		if(aoffset >= vector_size)
		  {
		    printf("<sorry %s, the loaded vector is too small>\n",title);
		    return;
		  }
		v = &vector[aoffset] ;
		for (x = y = 0.0,j = 0 ;j < header.ndim ;j++) {
		    x += rot_matrix[0][j] * (gp->pos[j] -
			    boxes[active_box].center[j] +
			    (vel_scale * v->v[j])) ;
		    y += rot_matrix[1][j] * (gp->pos[j] -
			    boxes[active_box].center[j] +
			    (vel_scale * v->v[j])) ;
		}
		gasvel_x[i] = (int)max(-2*INTMAX,min(2*INTMAX,
			(x * scaling +0.5))) ;
		gasvel_y[i] = (int)max(-2*INTMAX,min(2*INTMAX,
			(y * scaling +0.5))) ;
	    }
	}
	if (showdark && !gas_plot && !star_plot &&
		((!radial_plot && !potential_plot && !magnitude_plot)
		|| ((potential_plot  || radial_plot || magnitude_plot) &&
		(color_type == DARK || color_type == ALL)))) {
	    if(darkvel_x != NULL) free(darkvel_x);
	    if(darkvel_y != NULL) free(darkvel_y);
	    if(boxlist[active_box].ndark != 0) {
		darkvel_x = (int *)
		  malloc(boxlist[active_box].ndark*sizeof(*darkvel_x));
		if(darkvel_x == NULL) {
		    printf("<sorry, no memory for dark vector, %s>\n",title) ;
		    return ;
		}
		darkvel_y = (int *)
		  malloc(boxlist[active_box].ndark*sizeof(*darkvel_y));
		if(darkvel_y == NULL) {
		    printf("<sorry, no memory for dark vector, %s>\n",title) ;
		    return ;
		}
	    }  
	    else {
	      darkvel_x = NULL;
	      darkvel_y = NULL;
	    }  
	    offset = header.nsph;
	    for (i = 0 ;i < boxlist[active_box].ndark ;i++) {
		dp = boxlist[active_box].dp[i] ;
		aoffset = dp-dark_particles+offset;
		if(aoffset >= vector_size)
		  {
		    printf("<sorry %s, the loaded vector is too small>\n",title);
		    return;
		  }
		v = &vector[aoffset] ;
		for (x = y = 0.0,j = 0 ;j < header.ndim ;j++) {
		    x += rot_matrix[0][j] * (dp->pos[j] -
			    boxes[active_box].center[j] +
			    (vel_scale * v->v[j])) ;
		    y += rot_matrix[1][j] * (dp->pos[j] -
			    boxes[active_box].center[j] +
			    (vel_scale * v->v[j])) ;
		}
		darkvel_x[i] = (int)max(-2*INTMAX,min(2*INTMAX,
			(x * scaling +0.5))) ;
		darkvel_y[i] = (int)max(-2*INTMAX,min(2*INTMAX,
			(y * scaling +0.5))) ;
	    }
	}
	if ((showstar || star_plot)
		&& !gas_plot &&((!radial_plot && !potential_plot && 
		!magnitude_plot) || ((potential_plot  || radial_plot 
		|| magnitude_plot) &&
		(color_type == STAR || color_type == ALL)))) {
	    if(starvel_x != NULL) free(starvel_x);
	    if(starvel_y != NULL) free(starvel_y);
	    if(boxlist[active_box].nstar != 0) {
		starvel_x = (int *)
		    malloc(boxlist[active_box].nstar*sizeof(*starvel_x));
		if(starvel_x == NULL) {
		    printf("<sorry, no memory for star vector, %s>\n",title) ;
		    return ;
		}
		starvel_y = (int *)
		    malloc(boxlist[active_box].nstar*sizeof(*starvel_y));
		if(starvel_y == NULL) {
		    printf("<sorry, no memory for star vector, %s>\n",title) ;
		    return ;
		}
	    }  
	    else {
	      starvel_x = NULL;
	      starvel_y = NULL;
	    }  
	    offset = header.ndark + header.nsph;
	    for (i = 0 ;i < boxlist[active_box].nstar ;i++) {
		sp = boxlist[active_box].sp[i] ;
		aoffset = sp-star_particles+offset;
		if(aoffset >= vector_size)
		  {
		    printf("<sorry %s, the loaded vector is too small>\n",title);
		    return;
		  }
		v = &vector[aoffset] ;
		for (x = y = 0.0,j = 0 ;j < header.ndim ;j++) {
		    x += rot_matrix[0][j] * (sp->pos[j] -
			    boxes[active_box].center[j] +
			    (vel_scale * v->v[j])) ;
		    y += rot_matrix[1][j] * (sp->pos[j] -
			    boxes[active_box].center[j] +
			    (vel_scale * v->v[j])) ;
		}
		starvel_x[i] = (int)max(-2*INTMAX,min(2*INTMAX,
			(x * scaling +0.5))) ;
		starvel_y[i] = (int)max(-2*INTMAX,min(2*INTMAX,
			(y * scaling +0.5))) ;
	    }
	}
    }
    for (i=1 ;i <= MAXBOX ;i++) {
	if (boxes_loaded[i]) {
	    setvec(&box_coord[0][0],boxes[i].x1) ;
	    setvec(&box_coord[1][0],boxes[i].x2) ;
	    setvec(&box_coord[2][0],boxes[i].x3) ;
	    setvec(&box_coord[3][0],boxes[i].x4) ;
	    setvec(&box_coord[4][0],boxes[i].x5) ;
	    setvec(&box_coord[5][0],boxes[i].x6) ;
	    diff_add_vec(&box_coord[6][0],boxes[i].x5,boxes[i].x3,
		    boxes[i].x1) ;
	    diff_add_vec(&box_coord[7][0],boxes[i].x6,boxes[i].x4,
		    boxes[i].x2) ;
	    for (j = 0 ;j < BOXPTS ;j++) {
		for (x = y = 0.0,k = 0 ;k < header.ndim ;k++) {
		    x += rot_matrix[0][k] * (box_coord[j][k] -
			    boxes[active_box].center[k]) ;
		    y += rot_matrix[1][k] * (box_coord[j][k] -
			    boxes[active_box].center[k]) ;
		}
		plotbox[i][j].x = (int)(x * scaling +0.5) ;
		plotbox[i][j].y = (int)(y * scaling +0.5) ;
	    }
	    k = plotbox[i][2].x ;
	    plotbox[i][2].x = plotbox[i][3].x ;
	    plotbox[i][3].x = k ;
	    k = plotbox[i][2].y ;
	    plotbox[i][2].y = plotbox[i][3].y ;
	    plotbox[i][3].y = k ;
	    k = plotbox[i][6].x ;
	    plotbox[i][6].x = plotbox[i][7].x ;
	    plotbox[i][7].x = k ;
	    k = plotbox[i][6].y ;
	    plotbox[i][6].y = plotbox[i][7].y ;
	    plotbox[i][7].y = k ;
	    plotbox[i][8].x = plotbox[i][0].x ;
	    plotbox[i][8].y = plotbox[i][0].y ;
	    plotbox[i][9].x = plotbox[i][3].x ;
	    plotbox[i][9].y = plotbox[i][3].y ;
	    plotbox[i][10].x = plotbox[i][4].x ;
	    plotbox[i][10].y = plotbox[i][4].y ;
	    plotbox[i][11].x = plotbox[i][7].x ;
	    plotbox[i][11].y = plotbox[i][7].y ;
	    plotbox[i][12].x = plotbox[i][6].x ;
	    plotbox[i][12].y = plotbox[i][6].y ;
	    plotbox[i][13].x = plotbox[i][1].x ;
	    plotbox[i][13].y = plotbox[i][1].y ;
	    plotbox[i][14].x = plotbox[i][2].x ;
	    plotbox[i][14].y = plotbox[i][2].y ;
	    plotbox[i][15].x = plotbox[i][5].x ;
	    plotbox[i][15].y = plotbox[i][5].y ;
	}
    }
    for (j = 0 ;j < 6 ;j++) {
	for (x = y = 0.0,k = 0 ;k < header.ndim ;k++) {
	    x += rot_matrix[0][k] * (axes_coord[j][k] -
		    boxes[active_box].center[k]) ;
	    y += rot_matrix[1][k] * (axes_coord[j][k] -
		    boxes[active_box].center[k]) ;
	}
	plotaxes[j].x = (int)max(-2*INTMAX,min(2*INTMAX,
			(x * scaling +0.5))) ;
	plotaxes[j].y = (int)max(-2*INTMAX,min(2*INTMAX,
			(y * scaling +0.5))) ;
    }
    current_project = YES;
}
