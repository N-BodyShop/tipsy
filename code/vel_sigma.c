#include "defs.h"
#include "fdefs.h"

void
vel_sigma(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    char particle_type[MAXCOMM] ;
    char bin_type[MAXCOMM] ;
    int box ;
    int center_box ;
    int number_bins ;
    int bin ;
    double min_radius ;
    double max_radius ;
    Real angular_mom[MAXBIN][MAXDIM] ;
    double mass[MAXBIN] ;
    Real sigma[MAXDIM] ;
    Real center_vel[MAXDIM] ;
    Real x_axis_ellipse[MAXDIM] ;
    Real y_axis_ellipse[MAXDIM] ;
    Real z_axis_ellipse[MAXDIM] ;
    Real delta_x[MAXDIM] ;
    Real delta_v[MAXDIM] ;
    Real ang_mom[MAXDIM] ;
    Real center[MAXDIM] ;
    double radius ;
    Real vel_circ[MAXDIM] ;
    double mass_r ;
    Real vel_pec[MAXDIM] ;
    Real vel_shell[MAXDIM] ;
    double bin_size ;
    double perp_distance() ;
    double ell_distance() ;
    double dot_product() ;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    int i,j ;
    Real center_angular_mom[MAXDIM] ;

    if (sscanf(job,"%s %d %d %s %s %d %lf %lf",command,&box,&center_box,
	    particle_type,bin_type,&number_bins,&min_radius,&max_radius) == 8){
	if (boxes_loaded[box] && boxes_loaded[center_box] &&
		number_bins <= MAXBIN) {
	    for (i = 0 ; i < MAXBIN ;i++) {
		mass[i] = 0. ;
		for (j = 0 ; j < header.ndim ;j++) {
		    angular_mom[i][j]  = 0.0 ;
		}
	    }
	    for (j = 0 ; j < header.ndim ;j++) {
		vel_circ[j] = 0.0 ;
		sigma[j] = 0.0 ;
	    }
	    if (strcmp(bin_type,"lin") == 0) {
		bin_size = (max_radius - min_radius) /
			(double)number_bins ;
	    }
	    else if (strcmp(bin_type,"log") == 0) {
		min_radius = log10(min_radius) ;
		bin_size = (log10(max_radius) - min_radius) /
			(double)number_bins ;
	    }
	    else {
		printf("<sorry, %s not a bin type, %s>\n",bin_type,title) ;
		return ;
	    }
	    if (strcmp(particle_type,"dark") == 0){
		setvec(center,boxes[center_box].dark_com) ;
		setvec(center_vel,boxes[box].dark_com_vel) ;
		setvec(center_angular_mom,boxes[box].dark_angular_mom) ;
	    }
	    else if (strcmp(particle_type,"star") == 0){
		setvec(center,boxes[center_box].star_com) ;
		setvec(center_vel,boxes[box].star_com_vel) ;
		setvec(center_angular_mom,boxes[box].star_angular_mom) ;
	    }
	    else if (strcmp(particle_type,"gas") == 0){
		setvec(center,boxes[center_box].gas_com) ;
		setvec(center_vel,boxes[box].gas_com_vel) ;
		setvec(center_angular_mom,boxes[box].gas_angular_mom) ;
	    }
	    else{
		setvec(center,boxes[center_box].total_com) ;
		setvec(center_vel,boxes[box].total_com_vel) ;
		setvec(center_angular_mom,boxes[box].total_angular_mom) ;
	    }
	    fit_radius = max_radius ;
	    find_shape(particle_type,box,center) ;
	    find_vel(particle_type,box,center_vel,center_angular_mom) ;
	    setvec(center,center_ell) ;
	    delta_x[0] = 1. ;
	    delta_x[1] = 0. ;
	    delta_x[2] = 0. ;
	    matrix_vector_mult(ell_matrix_inv,delta_x,x_axis_ellipse) ;
	    delta_x[0] = 0. ;
	    delta_x[1] = 1. ;
	    delta_x[2] = 0. ;
	    matrix_vector_mult(ell_matrix_inv,delta_x,y_axis_ellipse) ;
	    delta_x[0] = 0. ;
	    delta_x[1] = 0. ;
	    delta_x[2] = 1. ;
	    matrix_vector_mult(ell_matrix_inv,delta_x,z_axis_ellipse) ;
	    matrix_vector_mult(ell_matrix,center_angular_mom,delta_x) ;
	    printf("angle between J and z = %g\n",
		    acos(delta_x[2]/sqrt(delta_x[0]*delta_x[0] +
		    delta_x[2]*delta_x[2])) * 180. / PI) ;
	    if (strcmp(particle_type,"dark") != 0 && 
		    strcmp(particle_type,"gas") != 0 &&
		    strcmp(particle_type,"star") != 0 &&
		    strcmp(particle_type,"all") !=0) {
		printf("<sorry, %s is not a particle type, %s>\n",
			particle_type,title) ;
		return ;
	    }
	    if (strcmp(particle_type,"dark") == 0 ||
		    strcmp(particle_type,"all") ==0) {
		for (i = 0 ;i < boxlist[box].ndark ; i++) {
		    dp = boxlist[box].dp[i] ;
		    if(ell_distance(dp->pos) <= max_radius){
			radius = perp_distance(center,center_angular_mom,
				dp->pos) ;
			if (strcmp(bin_type,"log") == 0) {
			    radius = log10(radius) ;
			}
			bin = max((int)(floor((radius - min_radius) /
				bin_size)),0);
			if (bin < number_bins) {
			    mass[bin] += dp->mass ;
			    sub_vec(delta_x,dp->pos,center) ;
			    sub_vec(delta_v,dp->vel,center_vel) ;
			    cross_product(ang_mom,delta_x,delta_v) ;
			    vel_circ[0] += dp->mass * dot_product(ang_mom,
				    x_axis_ellipse) / perp_distance(center,
				    x_axis_ellipse,dp->pos) ;
			    vel_circ[1] += dp->mass * dot_product(ang_mom,
				    y_axis_ellipse) / perp_distance(center,
				    y_axis_ellipse,dp->pos) ;
			    vel_circ[2] += dp->mass * dot_product(ang_mom,
				    z_axis_ellipse) / perp_distance(center,
				    z_axis_ellipse,dp->pos) ;
			    add_const_mult_vec(&angular_mom[bin][0],dp->mass, 
				    ang_mom) ;
			}
		    }
		}
	    }
	    if (strcmp(particle_type,"star") == 0 ||
		    strcmp(particle_type,"all") ==0) {
		for (i = 0 ;i < boxlist[box].nstar ; i++) {
		    sp = boxlist[box].sp[i] ;
		    if(ell_distance(sp->pos) <= max_radius){
			radius = perp_distance(center,center_angular_mom,
				sp->pos) ;
			if (strcmp(bin_type,"log") == 0) {
			    radius = log10(radius) ;
			}
			bin = max((int)(floor((radius - min_radius) /
				bin_size)),0);
			if (bin < number_bins) {
			    mass[bin] += sp->mass ;
			    sub_vec(delta_x,sp->pos,center) ;
			    sub_vec(delta_v,sp->vel,center_vel) ;
			    cross_product(ang_mom,delta_x,delta_v) ;
			    vel_circ[0] += sp->mass * dot_product(ang_mom,
				    x_axis_ellipse) / perp_distance(center,
				    x_axis_ellipse,sp->pos) ;
			    vel_circ[1] += sp->mass * dot_product(ang_mom,
				    y_axis_ellipse) / perp_distance(center,
				    y_axis_ellipse,sp->pos) ;
			    vel_circ[2] += sp->mass * dot_product(ang_mom,
				    z_axis_ellipse) / perp_distance(center,
				    z_axis_ellipse,sp->pos) ;
			    add_const_mult_vec(&angular_mom[bin][0],sp->mass, 
				    ang_mom) ;
			}
		    }
		}
	    }
	    if (strcmp(particle_type,"gas") == 0 ||
		    strcmp(particle_type,"all") ==0) {
		for (i = 0 ;i < boxlist[box].ngas ; i++) {
		    gp = boxlist[box].gp[i] ;
		    if(ell_distance(gp->pos) <= max_radius){
			radius = perp_distance(center,center_angular_mom,
				gp->pos) ;
			if (strcmp(bin_type,"log") == 0) {
			    radius = log10(radius) ;
			}
			bin = max((int)(floor((radius - min_radius) /
				bin_size)),0);
			if (bin < number_bins) {
			    mass[bin] += gp->mass ;
			    sub_vec(delta_x,gp->pos,center) ;
			    sub_vec(delta_v,gp->vel,center_vel) ;
			    cross_product(ang_mom,delta_x,delta_v) ;
			    vel_circ[0] += gp->mass * dot_product(ang_mom,
				    x_axis_ellipse) / perp_distance(center,
				    x_axis_ellipse,gp->pos) ;
			    vel_circ[1] += gp->mass * dot_product(ang_mom,
				    y_axis_ellipse) / perp_distance(center,
				    y_axis_ellipse,gp->pos) ;
			    vel_circ[2] += gp->mass * dot_product(ang_mom,
				    z_axis_ellipse) / perp_distance(center,
				    z_axis_ellipse,gp->pos) ;
			    add_const_mult_vec(&angular_mom[bin][0],gp->mass, 
				    ang_mom) ;
			}
		    }
		}
	    }
	    for (i = 0 ;i < number_bins ;i++) {
		for (j = 0 ;j < header.ndim ; j++) {
		    angular_mom[i][j] /= mass[i] ;
		}
	    }
	    if (strcmp(particle_type,"dark") == 0 ||
		    strcmp(particle_type,"all") ==0) {
		for (i = 0 ;i < boxlist[box].ndark ; i++) {
		    dp = boxlist[box].dp[i] ;
		    if(ell_distance(dp->pos) <= max_radius){
			radius = perp_distance(center,center_angular_mom,
				dp->pos) ;
			if (strcmp(bin_type,"log") == 0) {
			    radius = log10(radius) ;
			}
			bin = max((int)(floor((radius - min_radius) /
				bin_size)),0);
			if (bin < number_bins) {
			    sub_vec(delta_x,dp->pos,center) ;
			    sub_vec(delta_v,dp->vel,center_vel) ;
			    cross_product(vel_shell,&angular_mom[bin][0],
				    delta_x) ;
			    vec_add_const_mult_vec(vel_pec,delta_v,
				    -1.0 / dot_product(delta_x,delta_x),
				    vel_shell) ;
			    matrix_vector_mult(ell_matrix,vel_pec,delta_v) ;
			    for(j = 0 ;j < header.ndim ;j++){
				sigma[j] += (dp->mass) * delta_v[j] *
					delta_v[j] ;
			    }
			}
		    }
		}
	    }
	    if (strcmp(particle_type,"star") == 0 ||
		    strcmp(particle_type,"all") ==0) {
		for (i = 0 ;i < boxlist[box].nstar ; i++) {
		    sp = boxlist[box].sp[i] ;
		    if(ell_distance(sp->pos) <= max_radius){
			radius = perp_distance(center,center_angular_mom,
				sp->pos) ;
			if (strcmp(bin_type,"log") == 0) {
			    radius = log10(radius) ;
			}
			bin = max((int)(floor((radius - min_radius) /
				bin_size)),0);
			if (bin < number_bins) {
			    sub_vec(delta_x,sp->pos,center) ;
			    sub_vec(delta_v,sp->vel,center_vel) ;
			    cross_product(vel_shell,&angular_mom[bin][0],
				    delta_x) ;
			    vec_add_const_mult_vec(vel_pec,delta_v,
				    -1.0 / dot_product(delta_x,delta_x),
				    vel_shell) ;
			    matrix_vector_mult(ell_matrix,vel_pec,delta_v) ;
			    for(j = 0 ;j < header.ndim ;j++){
				sigma[j] += (sp->mass) * delta_v[j] *
					delta_v[j] ;
			    }
			}
		    }
		}
	    }
	    if (strcmp(particle_type,"gas") == 0 ||
		    strcmp(particle_type,"all") ==0) {
		for (i = 0 ;i < boxlist[box].ngas ; i++) {
		    gp = boxlist[box].gp[i] ;
		    if(ell_distance(gp->pos) <= max_radius){
			radius = perp_distance(center,center_angular_mom,
				gp->pos) ;
			if (strcmp(bin_type,"log") == 0) {
			    radius = log10(radius) ;
			}
			bin = max((int)(floor((radius - min_radius) /
				bin_size)),0);
			if (bin < number_bins) {
			    sub_vec(delta_x,gp->pos,center) ;
			    sub_vec(delta_v,gp->vel,center_vel) ;
			    cross_product(vel_shell,&angular_mom[bin][0],
				    delta_x) ;
			    vec_add_const_mult_vec(vel_pec,delta_v,
				    -1.0 / dot_product(delta_x,delta_x),
				    vel_shell) ;
			    matrix_vector_mult(ell_matrix,vel_pec,delta_v) ;
			    for(j = 0 ;j < header.ndim ;j++){
				sigma[j] += (gp->mass) * delta_v[j] *
					delta_v[j] ;
			    }
			}
		    }
		}
	    }
	    for (i = 0, mass_r = 0.0 ;i < number_bins ;i++) {
		mass_r += mass[i] ;
	    }
	    for(j = 0 ;j < header.ndim ;j++){
		vel_circ[j] /= mass_r ;
		sigma[j] /= mass_r ;
		sigma[j] = sqrt(sigma[j]) ;
	    }
	    printf("b/a = %g, c/a = %g\n",ba,ca) ;
	    printf("vel = (%g, %g, %g)\nsigma = (%g, %g, %g)\n",vel_circ[0],
		    vel_circ[1],vel_circ[2],sigma[0],sigma[1],sigma[2]) ;
	}
	else if (!boxes_loaded[box]) {
	    printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
	}
	else if (!boxes_loaded[center_box]) {
	    printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
	}
	else {
	    printf("<%d is too many bins, %s>\n",number_bins,title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
