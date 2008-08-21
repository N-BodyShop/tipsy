#include "defs.h"
void rot_cur(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    char projection_type[MAXCOMM] ;
    char bin_type[MAXCOMM] ;
    int box ;
    int center_box ;
    int number_bins ;
    double min_radius ;
    double max_radius ;
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
    double rot_vel_tot ;
    double rot_vel_bar ;
    double rot_vel_dark ;
    double radius ;
    double bin_size ;
    Real test_particle[MAXDIM] ;
    Real acc_tot[MAXDIM] ;
    Real acc_gas[MAXDIM] ;
    Real acc_star[MAXDIM] ;
    Real acc_dark[MAXDIM] ;
    Real acc_bar[MAXDIM] ;
    double acc_rad_bar ;
    double acc_rad_dark ;
    double acc_rad_tot ;
    char cbox_type[MAXCOMM] ;
    char particle_type[MAXCOMM] ;

    if (sscanf(job,"%s %d %d %s %s %s %d %s %lf %lf",command,&box,&center_box,
	    cbox_type,particle_type,bin_type,&number_bins,
	    hardfile.name,&min_radius,&max_radius) == 10) { 
	if(strcmp(cbox_type, "com") != 0 && strcmp(cbox_type, "pot") != 0){
	    printf("<sorry, %s not a center type, %s>\n",cbox_type,title) ;
	    return;
	}
	if (strcmp(particle_type,"dark") != 0 && 
		strcmp(particle_type,"gas") != 0 &&
		strcmp(particle_type,"star") != 0 &&
		strcmp(particle_type,"baryon") != 0 &&
		strcmp(particle_type,"all") !=0) {
	    printf("<sorry, %s is not a particle type, %s>\n",
		    particle_type,title) ;
	    return ;
	}
	if (boxes_loaded[box] && boxes_loaded[center_box]) {
	    if (strcmp(bin_type,"lin") == 0) {
		bin_size = (max_radius - min_radius) /
			(double)(number_bins) ;
	    } else if (strcmp(bin_type,"log") == 0) {
		min_radius = log10(min_radius) ;
		max_radius = log10(max_radius) ;
		bin_size = (max_radius - min_radius) /
			(double)(number_bins) ;
	    } else {
		printf("<sorry, %s not a bin type, %s>\n",bin_type,title) ;
		return ;
	    }
	    if(strcmp(cbox_type, "pot") == 0)
	      pot_center(center, center_box);
	    if (strcmp(particle_type,"dark") == 0){
	      if(strcmp(cbox_type, "pot") != 0)
		setvec(center,boxes[center_box].dark_com) ;
		setvec(center_angular_mom,boxes[box].dark_angular_mom) ;
	    }
	    else if (strcmp(particle_type,"star") == 0){
	      if(strcmp(cbox_type, "pot") != 0)
		setvec(center,boxes[center_box].star_com) ;
		setvec(center_angular_mom,boxes[box].star_angular_mom) ;
	    }
	    else if (strcmp(particle_type,"gas") == 0){
	      if(strcmp(cbox_type, "pot") != 0)
		setvec(center,boxes[center_box].gas_com) ;
		setvec(center_angular_mom,boxes[box].gas_angular_mom) ;
	    }
	    else if (strcmp(particle_type,"baryon") == 0){
	      if(strcmp(cbox_type, "pot") != 0)
		mass_add_vec(center,boxes[center_box].gas_com,
			boxes[center_box].gas_mass, 
			boxes[center_box].star_com,boxes[center_box].star_mass);
		mass_add_vec(center_angular_mom,boxes[box].gas_angular_mom,
			boxes[box].gas_mass, 
			boxes[box].star_angular_mom, 
			boxes[box].star_mass) ;
	    }
	    else{
	      if(strcmp(cbox_type, "pot") != 0)
		setvec(center,boxes[center_box].total_com) ;
		setvec(center_angular_mom,boxes[box].total_angular_mom) ;
	    }
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
	    for(i = 0 ;i <= number_bins ; i++){
		rot_vel_bar = 0. ;
		rot_vel_dark = 0. ;
		rot_vel_tot = 0. ;
		radius = ((double)i)*bin_size + min_radius ;
	        if (strcmp(bin_type,"log") == 0) {
		    radius = pow(10.,radius) ;
		}
		vec_add_const_mult_vec(test_particle,center,radius,unit1) ;
		grav(test_particle,acc_gas,acc_star,acc_dark,box) ;
		add_vec(acc_bar,acc_gas,acc_star) ;
		add_vec(acc_tot,acc_bar,acc_dark) ;
		acc_rad_bar = dot_product(acc_bar,unit1) ;
		acc_rad_dark = dot_product(acc_dark,unit1) ;
		acc_rad_tot = dot_product(acc_tot,unit1) ;
		rot_vel_bar += sqrt(fabs(radius*acc_rad_bar)) ;
		rot_vel_dark += sqrt(fabs(radius*acc_rad_dark)) ;
		rot_vel_tot += sqrt(fabs(radius*acc_rad_tot)) ;
		vec_add_const_mult_vec(test_particle,center,radius,unit2) ;
		grav(test_particle,acc_gas,acc_star,acc_dark,box) ;
		add_vec(acc_bar,acc_gas,acc_star) ;
		add_vec(acc_tot,acc_bar,acc_dark) ;
		acc_rad_bar = dot_product(acc_bar,unit2) ;
		acc_rad_dark = dot_product(acc_dark,unit2) ;
		acc_rad_tot = dot_product(acc_tot,unit2) ;
		rot_vel_bar += sqrt(fabs(radius*acc_rad_bar)) ;
		rot_vel_dark += sqrt(fabs(radius*acc_rad_dark)) ;
		rot_vel_tot += sqrt(fabs(radius*acc_rad_tot)) ;
		vec_add_const_mult_vec(test_particle,center,radius,unit3) ;
		grav(test_particle,acc_gas,acc_star,acc_dark,box) ;
		add_vec(acc_bar,acc_gas,acc_star) ;
		add_vec(acc_tot,acc_bar,acc_dark) ;
		acc_rad_bar = dot_product(acc_bar,unit3) ;
		acc_rad_dark = dot_product(acc_dark,unit3) ;
		acc_rad_tot = dot_product(acc_tot,unit3) ;
		rot_vel_bar += sqrt(fabs(radius*acc_rad_bar)) ;
		rot_vel_dark += sqrt(fabs(radius*acc_rad_dark)) ;
		rot_vel_tot += sqrt(fabs(radius*acc_rad_tot)) ;
		vec_add_const_mult_vec(test_particle,center,radius,unit4) ;
		grav(test_particle,acc_gas,acc_star,acc_dark,box) ;
		add_vec(acc_bar,acc_gas,acc_star) ;
		add_vec(acc_tot,acc_bar,acc_dark) ;
		acc_rad_bar = dot_product(acc_bar,unit4) ;
		acc_rad_dark = dot_product(acc_dark,unit4) ;
		acc_rad_tot = dot_product(acc_tot,unit4) ;
		rot_vel_bar += sqrt(fabs(radius*acc_rad_bar)) ;
		rot_vel_dark += sqrt(fabs(radius*acc_rad_dark)) ;
		rot_vel_tot += sqrt(fabs(radius*acc_rad_tot)) ;
		rot_vel_bar /= 4. ;
		rot_vel_dark /= 4. ;
		rot_vel_tot /= 4. ;
		fprintf(hardfile.ptr,"%g %g %g %g\n",radius,rot_vel_tot,
			rot_vel_dark,rot_vel_bar) ;
	    }
	    fclose(hardfile.ptr) ;
	}
	else if (!boxes_loaded[box]) {
	    printf("<sorry, box %d not loaded, %s>\n",box,title) ;
	}
	else {
	    printf("<sorry, box %d not loaded, %s>\n",center_box,title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
