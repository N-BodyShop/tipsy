#include "defs.h"
#include "fdefs.h"

void
find_vel(particle_type,box,center_vel,center_angular_mom)
    int box ;
    Real center_vel[MAXDIM] ;
    Real center_angular_mom[MAXDIM] ;
    char particle_type[MAXCOMM] ;
{
    double mass_r ;
    Real ang_mom[MAXDIM] ;
    double ell_distance() ;
    int i ;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    Real center[MAXDIM] ;

    mass_r = 0. ;
    for (i = 0 ; i < header.ndim ; i++) {
	center[i] = 0. ;
	center_vel[i] = 0. ;
	center_angular_mom[i] = 0. ;
    }
    if (strcmp(particle_type,"dark") == 0 ||
	    strcmp(particle_type,"mark") == 0 ||
	    strcmp(particle_type,"unmark") == 0 ||
	    strcmp(particle_type,"all") ==0) {
	for (i = 0 ;i < boxlist[box].ndark ; i++) {
	    if(strcmp(particle_type,"mark") == 0 &&
	       mark_dark[boxlist[box].dpi[i]] == 0)
		continue;
	    if(strcmp(particle_type,"unmark") == 0 &&
	       mark_dark[boxlist[box].dpi[i]] != 0)
		continue;
	    dp = boxlist[box].dp[i] ;
	    if(ell_distance(dp->pos) <= fit_radius){
		mass_r += dp->mass ;
		add_const_mult_vec(center, dp->mass, dp->pos) ;
		add_const_mult_vec(center_vel, dp->mass, dp->vel) ;
		cross_product(ang_mom, dp->pos, dp->vel) ;
		add_const_mult_vec(center_angular_mom, dp->mass, ang_mom) ;
	    }
	}
    }
    if (strcmp(particle_type,"star") == 0 ||
	    strcmp(particle_type,"mark") == 0 ||
	    strcmp(particle_type,"unmark") == 0 ||
	    strcmp(particle_type,"all") ==0) {
	for (i = 0 ;i < boxlist[box].nstar ; i++) {
	    if(strcmp(particle_type,"mark") == 0 &&
	       mark_star[boxlist[box].spi[i]] == 0)
		continue;
	    if(strcmp(particle_type,"unmark") == 0 &&
	       mark_star[boxlist[box].spi[i]] != 0)
		continue;
	    sp = boxlist[box].sp[i] ;
	    if(ell_distance(sp->pos) <= fit_radius){
		mass_r += sp->mass ;
		add_const_mult_vec(center, sp->mass, sp->pos) ;
		add_const_mult_vec(center_vel, sp->mass, sp->vel) ;
		cross_product(ang_mom, sp->pos, sp->vel) ;
		add_const_mult_vec(center_angular_mom, sp->mass, ang_mom) ;
	    }
	}
    }
    if (strcmp(particle_type,"gas") == 0 ||
	    strcmp(particle_type,"mark") == 0 ||
	    strcmp(particle_type,"unmark") == 0 ||
	    strcmp(particle_type,"all") ==0) {
	for (i = 0 ;i < boxlist[box].ngas ; i++) {
	    if(strcmp(particle_type,"mark") == 0 &&
	       mark_gas[boxlist[box].gpi[i]] == 0)
		continue;
	    if(strcmp(particle_type,"unmark") == 0 &&
	       mark_gas[boxlist[box].gpi[i]] != 0)
		continue;
	    gp = boxlist[box].gp[i] ;
	    if(ell_distance(gp->pos) <= fit_radius){
		mass_r += gp->mass ;
		add_const_mult_vec(center, gp->mass, gp->pos) ;
		add_const_mult_vec(center_vel, gp->mass, gp->vel) ;
		cross_product(ang_mom, gp->pos, gp->vel) ;
		add_const_mult_vec(center_angular_mom, gp->mass, ang_mom) ;
	    }
	}
    }
    for (i = 0 ; i < header.ndim ; i++) {
	center[i] /= mass_r ;
	center_vel[i] /= mass_r ;
    }
    cross_product(ang_mom, center, center_vel) ;
    add_const_mult_vec(center_angular_mom,
	    -mass_r, ang_mom) ;
    for (i = 0 ; i < header.ndim ; i++) {
	center_angular_mom[i] /= mass_r ;
    }
}
