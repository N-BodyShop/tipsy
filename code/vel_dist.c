#include "defs.h"
vel_dist(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    int box ;
    int center_box ;
    int number_bins ;
    double min_vel ;
    double max_vel ;
    int bin ;
    double mass[MAXBIN] ;
    Real center_vel[MAXDIM] ;
    Real delta_x[MAXDIM] ;
    Real delta_v[MAXDIM] ;
    Real ang_mom[MAXDIM] ;
    Real center[MAXDIM] ;
    double radius ;
    double vel_circ ;
    double bin_size ;
    double perp_distance() ;
    double dot_product() ;
    struct star_particle *sp ;
    struct gas_particle *gp ;
    int i,j ;
    Real center_angular_mom[MAXDIM] ;
    double mag_center_ang ;

    if (sscanf(job,"%s %d %d %d %s %lf %lf",command,&box,&center_box,
	    &number_bins,hardfile.name,&min_vel,&max_vel) == 7){
	if (boxes_loaded[box] && number_bins <= MAXBIN) {
	    for (i = 0 ; i < MAXBIN ;i++) {
		mass[i] = 0. ;
	    }
	    setvec(center,boxes[center_box].total_com) ;
	    add_vec(center_vel,boxes[center_box].star_com_vel,
		    boxes[center_box].gas_com_vel) ;
	    setvec(center_angular_mom,boxes[center_box].gas_angular_mom) ;
	    mag_center_ang = sqrt(dot_product(center_angular_mom,
		    center_angular_mom)) ;
	    for (i = 0 ;i < MAXDIM ;i++){
		center_angular_mom[i] /= mag_center_ang ;
	    }
	    bin_size = (max_vel - min_vel) / (double)number_bins ;
	    for (i = 0 ;i < boxlist[box].nstar ; i++) {
		sp = boxlist[box].sp[i] ;
		radius = perp_distance(center,center_angular_mom,sp->pos) ;
		sub_vec(delta_x,sp->pos,center) ;
		sub_vec(delta_v,sp->vel,center_vel) ;
		cross_product(ang_mom,delta_x,delta_v) ;
		vel_circ = dot_product(ang_mom,center_angular_mom)/radius ;
		bin = (int)floor((vel_circ-min_vel)/bin_size) ;
		if(bin < number_bins && bin >= 0){
		    mass[bin] += sp->mass ;
		}
	    }
	    for (i = 0 ;i < boxlist[box].ngas ; i++) {
		gp = boxlist[box].gp[i] ;
		radius = perp_distance(center,center_angular_mom,gp->pos) ;
		sub_vec(delta_x,gp->pos,center) ;
		sub_vec(delta_v,gp->vel,center_vel) ;
		cross_product(ang_mom,delta_x,delta_v) ;
		vel_circ = dot_product(ang_mom,center_angular_mom)/radius ;
		bin = (int)floor((vel_circ-min_vel)/bin_size) ;
		if(bin < number_bins && bin >= 0){
		    mass[bin] += gp->mass ;
		}
	    }
	    hardfile.ptr = fopen(hardfile.name,"w") ;
	    for(i = 0 ; i < number_bins ; i++){
		vel_circ = ((double)i + .5)*bin_size + min_vel ;
		fprintf(hardfile.ptr,"%g %g\n",vel_circ,mass[i]) ;
	    } 
	    fclose(hardfile.ptr) ;
	}
	else if (!boxes_loaded[box]) {
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
