#include "defs.h"
void rot_cur_grp(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    int box ;
    int center_box ;
    int number_bins ;
    double min_radius ;
    Real center[MAXDIM] ;
    Real center_angular_mom[MAXDIM] ;
    Real unit1[MAXDIM] ;
    Real unit2[MAXDIM] ;
    Real unit3[MAXDIM] ;
    Real unit4[MAXDIM] ;
    int i,j ;
    static Real xaxis[MAXDIM] = {1,0,0} ;
    static Real yaxis[MAXDIM] = {0,1,0} ;
    double dot_product() ;
    double norm ;
    double radius ;
    Real test_particle[MAXDIM] ;
    Real acc_tot[MAXDIM] ;
    Real acc_gas[MAXDIM] ;
    Real acc_star[MAXDIM] ;
    Real acc_dark[MAXDIM] ;
    Real acc_bar[MAXDIM] ;
    double rot_vel_tot ;
    double acc_rad_tot ;
    double vsys ;

    if (sscanf(job,"%s %d %d %f %f %f %d %s %lf",command,&box,&center_box,
	    &center[0],&center[1],&center[2],&number_bins, hardfile.name,
	    &min_radius) == 9) { 
	if (boxes_loaded[box]) {
	    if (!cool_loaded ){
		load_cool() ;
	    }
	    mass_add_vec(center_angular_mom,boxes[center_box].gas_angular_mom,
		    boxes[center_box].gas_mass, 
		    boxes[center_box].star_angular_mom, 
		    boxes[center_box].star_mass) ;
	    norm = sqrt(dot_product(center_angular_mom,center_angular_mom)) ;
	    for(i = 0; i < MAXDIM ; i++){
		center_angular_mom[i] /= norm ;
	    }
	    if(fabs(dot_product(center_angular_mom,xaxis)) < 0.95){
		cross_product(unit1,center_angular_mom,xaxis) ;
	    }
	    else{
		cross_product(unit1,center_angular_mom,yaxis) ;
	    }
	    norm = sqrt(dot_product(unit1,unit1)) ;
	    for(i = 0; i < MAXDIM ; i++){
		unit1[i] /= norm ;
		unit2[i] = -unit1[i] ;
	    }
	    cross_product(unit3,center_angular_mom,unit1) ;
	    norm = sqrt(dot_product(unit3,unit3)) ;
	    for(i = 0; i < MAXDIM ; i++){
		unit3[i] /= norm ;
		unit4[i] = -unit3[i] ;
	    }
	    hardfile.ptr = fopen(hardfile.name,"w") ;
	    radius = min_radius ;
	    vsys = sqrt(msolunit/cosmof/kpcunit*(GCGS*MSOLG/KPCCM))/1.e5 ;
	    for(i = 0 ;i < number_bins ; i++){
		rot_vel_tot = 0. ;
		vec_add_const_mult_vec(test_particle,center,radius,unit1) ;
		grav(test_particle,acc_gas,acc_star,acc_dark,box) ;
		add_vec(acc_bar,acc_gas,acc_star) ;
		add_vec(acc_tot,acc_bar,acc_dark) ;
		acc_rad_tot = dot_product(acc_tot,unit1) ;
		rot_vel_tot += sqrt(fabs(radius*acc_rad_tot)) ;
		vec_add_const_mult_vec(test_particle,center,radius,unit2) ;
		grav(test_particle,acc_gas,acc_star,acc_dark,box) ;
		add_vec(acc_bar,acc_gas,acc_star) ;
		add_vec(acc_tot,acc_bar,acc_dark) ;
		acc_rad_tot = dot_product(acc_tot,unit2) ;
		rot_vel_tot += sqrt(fabs(radius*acc_rad_tot)) ;
		vec_add_const_mult_vec(test_particle,center,radius,unit3) ;
		grav(test_particle,acc_gas,acc_star,acc_dark,box) ;
		add_vec(acc_bar,acc_gas,acc_star) ;
		add_vec(acc_tot,acc_bar,acc_dark) ;
		acc_rad_tot = dot_product(acc_tot,unit3) ;
		rot_vel_tot += sqrt(fabs(radius*acc_rad_tot)) ;
		vec_add_const_mult_vec(test_particle,center,radius,unit4) ;
		grav(test_particle,acc_gas,acc_star,acc_dark,box) ;
		add_vec(acc_bar,acc_gas,acc_star) ;
		add_vec(acc_tot,acc_bar,acc_dark) ;
		acc_rad_tot = dot_product(acc_tot,unit4) ;
		rot_vel_tot += sqrt(fabs(radius*acc_rad_tot)) ;
		rot_vel_tot /= 4. ;
		radius *= 2.0 ;
	        fprintf(hardfile.ptr,"%g ",vsys*rot_vel_tot) ;
	    }
	    fprintf(hardfile.ptr,"\n") ;
	    fclose(hardfile.ptr) ;
	}
	else {
	    printf("<sorry, box %d not loaded, %s>\n",box,title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
