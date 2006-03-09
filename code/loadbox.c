/*
 * $Header$
 * $Log$
 * Revision 1.5  2006/03/09 17:25:54  trq
 * Avoid divisions by 0.
 *
 * Revision 1.4  2003/08/04 23:51:03  trq
 * Reset current_plot if we have reloaded an active box.
 *
 * Revision 1.3  1996/12/20 01:25:10  trq
 * Added catbox command.
 *
 * Revision 1.2  1996/04/24  23:40:35  trq
 * balls_loaded variable to keep track of balls.
 * Fixed smoothing length bug.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:26  trq
 * Import to CVS
 *
 * Revision 2.4  94/04/20  09:00:38  trq
 * Check for no particles in the box.
 * 
 * Revision 2.3  94/03/14  15:12:14  trq
 * Check for bad malloc returns.
 * 
 */
#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>

void
loadbox(box)
    int box ;
{ 
    int i,j ;
    double constant[5] ;
    double d ;
    double upper,lower ;
    struct gas_particle *gp, *lastgp ;
    struct dark_particle *dp, *lastdp ;
    struct star_particle *sp, *lastsp ;

    plane(boxes[box].x1,boxes[box].x2,boxes[box].x3,boxes[box].x5,constant) ;
    lastgp = gas_particles + header.nsph ;
    lastdp = dark_particles + header.ndark ;
    lastsp = star_particles + header.nstar ;
    if(boxlist_alloc(box, header.nsph, header.ndark, header.nstar) == 0)
      return;
    if(box == 0) {
	lower = constant[3] ;
	upper = constant[4] ;

	boxlist[box].ngas = 0 ;
	for (gp = gas_particles, j = 0 ;gp < lastgp ;gp++, j++) {
	    d = 0.0 ;
	    for(i = 0 ;i < header.ndim ;i++) {
		d += constant[i] * gp->pos[i] ;
	    }
	    if (d >= lower && d <= upper) {
		boxlist[box].gpi[boxlist[box].ngas] = j ;
		boxlist[box].gp[boxlist[box].ngas++] = gp ;
	    }
	}

	boxlist[box].ndark = 0 ;
	for (dp = dark_particles, j = 0 ;dp < lastdp ;dp++, j++) {
	    d = 0.0 ;
	    for(i = 0 ;i < header.ndim ;i++) {
		d += constant[i] * dp->pos[i] ;
	    }
	    if (d >= lower && d <= upper) {
		boxlist[box].dpi[boxlist[box].ndark] = j ;
		boxlist[box].dp[boxlist[box].ndark++] = dp ;
	    }
	}

	boxlist[box].nstar = 0 ;
	for (sp = star_particles, j = 0 ;sp < lastsp ;sp++, j++) {
	    d = 0.0 ;
	    for(i = 0 ;i < header.ndim ;i++) {
		d += constant[i] * sp->pos[i] ;
	    }
	    if (d >= lower && d <= upper) {
		boxlist[box].spi[boxlist[box].nstar] = j ;
		boxlist[box].sp[boxlist[box].nstar++] = sp ;
	    }
	}
	balls_loaded = NO;
    }
    else {
	boxlist[box].ngas = boxlist[0].ngas ;
	for (i = 0 ;i < boxlist[box].ngas ;i++) {
	    boxlist[box].gp[i] = boxlist[0].gp[i] ;
	    boxlist[box].gpi[i] = boxlist[0].gpi[i] ;
	}
	boxlist[box].ndark = boxlist[0].ndark ;
	for (i = 0 ;i < boxlist[box].ndark ;i++) {
	    boxlist[box].dp[i] = boxlist[0].dp[i] ;
	    boxlist[box].dpi[i] = boxlist[0].dpi[i] ;
	}
	boxlist[box].nstar = boxlist[0].nstar ;
	for (i = 0 ;i < boxlist[box].nstar ;i++) {
	    boxlist[box].sp[i] = boxlist[0].sp[i] ;
	    boxlist[box].spi[i] = boxlist[0].spi[i] ;
	}
	sift(constant,box) ;
    }
    plane(boxes[box].x1,boxes[box].x3,boxes[box].x5,boxes[box].x6,constant) ;
    sift(constant,box) ;
    plane(boxes[box].x6,boxes[box].x5,boxes[box].x3,boxes[box].x1,constant) ;
    sift(constant,box) ;
    boxlist_realloc(box);
    boxes_loaded[box] = LOADED ;

    boxes[box].size = 0.0 ;
    d = distance(boxes[box].x1, boxes[box].x2) ;
    boxes[box].volume = d ;
    boxes[box].size = max(boxes[box].size, d) ;
    d = distance(boxes[box].x1, boxes[box].x3) ;
    boxes[box].volume *= d ;
    boxes[box].size = max(boxes[box].size, d) ;
    d = distance(boxes[box].x5, boxes[box].x3) ;
    boxes[box].volume *= d ;
    boxes[box].size = max(boxes[box].size, d) ;
    for (i = 0 ; i < header.ndim ; i++ ) {
	boxes[box].center[i] = (boxes[box].x1[i] +boxes[box].x6[i]) / 2.0 ;
    }
    box_cumulate(box);
    if(box == active_box) {
	current_project = NO;
	}
}

/*
 * Allocate memory for boxlists.
 */
int
boxlist_alloc(box, ngas, ndark, nstar)
     int box;
     int ngas;
     int ndark;
     int nstar;
{
    if(boxlist[box].gpi != NULL) free(boxlist[box].gpi);
    if(boxlist[box].gp != NULL) free(boxlist[box].gp);
    if(ngas != 0) {
	boxlist[box].gpi =
	  (int *)malloc(ngas*sizeof(*boxlist[box].gpi));
	if(boxlist[box].gpi == NULL) {
	    printf("<sorry, no memory for gas particles in box, %s>\n",title) ;
	    return 0;
	}
	boxlist[box].gp = (struct gas_particle **)
			    malloc(ngas*sizeof(*boxlist[box].gp));
	if(boxlist[box].gp == NULL) {
	    printf("<sorry, no memory for gas particles in box, %s>\n",title) ;
	    return 0;
	}
    }
    else {
	boxlist[box].gpi = NULL;
	boxlist[box].gp = NULL;
    }
    
    if(boxlist[box].dpi != NULL) free(boxlist[box].dpi);
    if(boxlist[box].dp != NULL) free(boxlist[box].dp);
    if(ndark != 0) {
	boxlist[box].dpi = (int *)
	                     malloc(ndark*sizeof(*boxlist[box].dpi));
	if(boxlist[box].dpi == NULL) {
	    printf("<sorry, no memory for dark particles in box, %s>\n",title) ;
	    return 0;
	}
	boxlist[box].dp = (struct dark_particle **)
			    malloc(ndark*sizeof(*boxlist[box].dp));
	if(boxlist[box].dp == NULL) {
	    printf("<sorry, no memory for dark particles in box, %s>\n",title) ;
	    return 0;
	}
    }
    else {
	boxlist[box].dpi = NULL;
	boxlist[box].dp = NULL;
    }
    if(boxlist[box].spi != NULL) free(boxlist[box].spi);
    if(boxlist[box].sp != NULL) free(boxlist[box].sp);
    if(nstar != 0) {
	boxlist[box].spi =
	  (int *)malloc(nstar*sizeof(*boxlist[box].spi));
	if(boxlist[box].spi == NULL) {
	    printf("<sorry, no memory for star particles in box, %s>\n",title) ;
	    return 0;
	}
	boxlist[box].sp = (struct star_particle **)
			    malloc(nstar*sizeof(*boxlist[box].sp));
	if(boxlist[box].sp == NULL) {
	    printf("<sorry, no memory for star particles in box, %s>\n",title) ;
	    return 0;
	}
    }
    else {
	boxlist[box].spi = NULL;
	boxlist[box].sp = NULL;
    }
    return 1;
}

void
 boxlist_realloc(box)
     int box;
{
    if(boxlist[box].gpi) {
	boxlist[box].gpi = (int *)realloc(boxlist[box].gpi,
			    boxlist[box].ngas*sizeof(*boxlist[box].gpi));
	boxlist[box].gp = (struct gas_particle **)realloc(boxlist[box].gp,
			    boxlist[box].ngas*sizeof(*boxlist[box].gp));
    }
    if(boxlist[box].dpi) {
	boxlist[box].dpi = (int *)realloc(boxlist[box].dpi,
			    boxlist[box].ndark*sizeof(*boxlist[box].dpi));
	boxlist[box].dp = (struct dark_particle **)realloc(boxlist[box].dp,
			    boxlist[box].ndark*sizeof(*boxlist[box].dp));
    }
    if(boxlist[box].spi) {
	boxlist[box].spi = (int *)realloc(boxlist[box].spi,
			    boxlist[box].nstar*sizeof(*boxlist[box].spi));
	boxlist[box].sp = (struct star_particle **)realloc(boxlist[box].sp,
			    boxlist[box].nstar*sizeof(*boxlist[box].sp));
    }
}
/*
 * Find cumulative quantities for a box.
 */
void
box_cumulate(box)
     int box;
{
  int i;
  struct gas_particle *gp;
  struct dark_particle *dp;
  struct star_particle *sp;
    Real ang_mom[MAXDIM] ;

    boxes[box].gas_mass = boxes[box].dark_mass = boxes[box].star_mass =
	    boxes[box].total_mass = 0.0 ;
    for (i = 0 ; i < header.ndim ; i++) {
	boxes[box].gas_com[i] = boxes[box].dark_com[i] =
		boxes[box].star_com[i] = boxes[box].total_com[i] = 0.0 ;
	boxes[box].gas_com_vel[i] = boxes[box].dark_com_vel[i] =
		boxes[box].star_com_vel[i] = boxes[box].total_com_vel[i] = 0.0 ;
	boxes[box].gas_angular_mom[i] = boxes[box].dark_angular_mom[i] =
		boxes[box].star_angular_mom[i] =
		boxes[box].total_angular_mom[i] = 0.0 ;
    }
    for (i = 0 ;i < boxlist[box].ngas ;i++) {
	gp = boxlist[box].gp[i] ;
	boxes[box].gas_mass += gp->mass ;
	add_const_mult_vec(boxes[box].gas_com, gp->mass, gp->pos) ;
	add_const_mult_vec(boxes[box].gas_com_vel, gp->mass, gp->vel) ;
	cross_product(ang_mom, gp->pos, gp->vel) ;
	add_const_mult_vec(boxes[box].gas_angular_mom, gp->mass, ang_mom) ;
    }
    for (i = 0 ;i < boxlist[box].ndark ;i++) {
	dp = boxlist[box].dp[i] ;
	boxes[box].dark_mass += dp->mass ;
	add_const_mult_vec(boxes[box].dark_com, dp->mass, dp->pos) ;
	add_const_mult_vec(boxes[box].dark_com_vel, dp->mass, dp->vel) ;
	cross_product(ang_mom, dp->pos, dp->vel) ;
	add_const_mult_vec(boxes[box].dark_angular_mom, dp->mass, ang_mom) ;
    }
    for (i = 0 ;i < boxlist[box].nstar ;i++) {
	sp = boxlist[box].sp[i] ;
	boxes[box].star_mass += sp->mass ;
	add_const_mult_vec(boxes[box].star_com, sp->mass, sp->pos) ;
	add_const_mult_vec(boxes[box].star_com_vel, sp->mass, sp->vel) ;
	cross_product(ang_mom, sp->pos, sp->vel) ;
	add_const_mult_vec(boxes[box].star_angular_mom, sp->mass, ang_mom) ;
    }
    boxes[box].total_mass = boxes[box].gas_mass + boxes[box].dark_mass +
	    boxes[box].star_mass ;
    for (i = 0 ; i < header.ndim ; i++) {
	boxes[box].total_com[i] = boxes[box].gas_com[i] +
		boxes[box].dark_com[i] + boxes[box].star_com[i] ;
	boxes[box].total_com_vel[i] = boxes[box].gas_com_vel[i] +
		boxes[box].dark_com_vel[i] + boxes[box].star_com_vel[i] ;
	boxes[box].total_angular_mom[i] = boxes[box].gas_angular_mom[i] +
		boxes[box].dark_angular_mom[i] +
		boxes[box].star_angular_mom[i] ;
	if(boxlist[box].ngas && boxes[box].gas_mass != 0.0)
	  boxes[box].gas_com[i] /= boxes[box].gas_mass ;

	if(boxlist[box].ndark && boxes[box].dark_mass != 0.0)
	  boxes[box].dark_com[i] /= boxes[box].dark_mass ;

	if(boxlist[box].nstar && boxes[box].star_mass != 0.0)
	  boxes[box].star_com[i] /= boxes[box].star_mass ;

	if((boxlist[box].ngas || boxlist[box].ndark || boxlist[box].nstar)
	   && boxes[box].total_mass != 0.0)
	  boxes[box].total_com[i] /= boxes[box].total_mass ;

	if(boxlist[box].ngas && boxes[box].gas_mass != 0.0)
	  boxes[box].gas_com_vel[i] /= boxes[box].gas_mass ;

	if(boxlist[box].ndark && boxes[box].dark_mass != 0.0)
	  boxes[box].dark_com_vel[i] /= boxes[box].dark_mass ;

	if(boxlist[box].nstar && boxes[box].star_mass != 0.0)
	  boxes[box].star_com_vel[i] /= boxes[box].star_mass ;

	if((boxlist[box].ngas || boxlist[box].ndark || boxlist[box].nstar)
	   && boxes[box].total_mass != 0.0)
	  boxes[box].total_com_vel[i] /= boxes[box].total_mass ;
    }
    cross_product(ang_mom, boxes[box].gas_com, boxes[box].gas_com_vel) ;
    add_const_mult_vec(boxes[box].gas_angular_mom,
	    -boxes[box].gas_mass, ang_mom) ;
    cross_product(ang_mom, boxes[box].dark_com, boxes[box].dark_com_vel) ;
    add_const_mult_vec(boxes[box].dark_angular_mom,
	    -boxes[box].dark_mass, ang_mom) ;
    cross_product(ang_mom, boxes[box].star_com, boxes[box].star_com_vel) ;
    add_const_mult_vec(boxes[box].star_angular_mom,
	    -boxes[box].star_mass, ang_mom) ;
    cross_product(ang_mom, boxes[box].total_com, boxes[box].total_com_vel) ;
    add_const_mult_vec(boxes[box].total_angular_mom,
	    -boxes[box].total_mass, ang_mom) ;
    for (i = 0 ; i < header.ndim ; i++) {
	if(boxlist[box].ngas && boxes[box].gas_mass != 0.0)
	  boxes[box].gas_angular_mom[i] /= boxes[box].gas_mass ;

	if(boxlist[box].ndark && boxes[box].dark_mass != 0.0)
	  boxes[box].dark_angular_mom[i] /= boxes[box].dark_mass ;

	if(boxlist[box].nstar && boxes[box].star_mass != 0.0)
	  boxes[box].star_angular_mom[i] /= boxes[box].star_mass ;

	if((boxlist[box].ngas || boxlist[box].ndark || boxlist[box].nstar)
	   && boxes[box].total_mass != 0.0)
	  boxes[box].total_angular_mom[i] /= boxes[box].total_mass ;
    }
}
