#include "defs.h"
#include "fdefs.h"

void
profile(job)
    char *job ;
{
    char command[MAXCOMM] ;
    char particle_type[MAXCOMM] ;
    char projection_type[MAXCOMM] ;
    char bin_type[MAXCOMM] ;
    char cbox_type[MAXCOMM] ;
    int box ;
    int center_box ;
    int number_bins ;
    int bin ;
    double min_radius ;
    Real angular_mom[MAXBIN][MAXDIM] ;
    double vel_radial[MAXBIN] ;
    double vel_radial_sigma[MAXBIN] ;
    double vel_tang_sigma[MAXBIN] ;
    double mass[MAXBIN] ;
    double gas_mass[MAXBIN] ;
    double density[MAXBIN] ;
    double pressure[MAXBIN] ;
    double temp[MAXBIN] ;
    double entropy[MAXBIN] ;
    double lum[MAXBIN] ;
    double ar_mean[MAXBIN] ;
    int number[MAXBIN] ;
    Real center[MAXDIM] ;
    Real center_e[MAXDIM] ;
    Real center_vel[MAXDIM] ;
    Real center_angular_mom[MAXDIM] ;
    Real delta_x[MAXDIM] ;
    Real delta_v[MAXDIM] ;
    Real ang_mom[MAXDIM] ;
    double radius ;
    double radius_old ;
    double radius_mean ;
    double vel ;
    double rho ;
    double volume ;
    double lum_den ;
    double vel_circ ;
    double ang ;
    double ang_phi ;
    double ang_theta ;
    double mass_r ;
    Real vel_tang[MAXDIM] ;
    Real vel_tang_pec[MAXDIM] ;
    Real vel_shell[MAXDIM] ;
    double bin_size ;
    double dx2;
    double gasconst;		/* convert kboltz/mhydrogen to system
				   units assuming G == 1 */
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    int i,j ;

    if (sscanf(job,"%s %d %s %s %s %s %d %s %lf",command,&box,cbox_type,
	    particle_type,projection_type,bin_type,&number_bins,
	    hardfile.name,&min_radius) == 9) { 
      if(strcmp(cbox_type, "pot") == 0)
	center_box = -1;
      else if(sscanf(cbox_type,"%d", &center_box) != 1)
	{
	  input_error(command);
	  return;
	}
	if (boxes_loaded[box]
	    && (center_box == -1 || boxes_loaded[center_box])
	    && number_bins <= MAXBIN) {
	    for (i = 0 ; i < MAXBIN ;i++) {
		mass[i] = density[i] = pressure[i] = temp[i] =
		        entropy[i] = gas_mass[i] =
			vel_radial[i] = vel_radial_sigma[i] =
			vel_tang_sigma[i] = lum[i] = ar_mean[i] = 0.0 ;
		number[i] = 0 ;
		for (j = 0 ; j < header.ndim ;j++) {
		    angular_mom[i][j]  = 0.0 ;
		}
	    }
	    if (strcmp(bin_type,"lin") == 0) {
		bin_size = (boxes[box].size / 2.0  - min_radius) /
			(double)number_bins ;
	    } else if (strcmp(bin_type,"log") == 0) {
		min_radius = log10(min_radius) ;
		bin_size = (log10(boxes[box].size / 2.0 ) - min_radius) /
			(double)number_bins ;
	    } else {
		printf("<sorry, %s not a bin type, %s>\n",bin_type,title) ;
		return ;
	    }
	    if (strcmp(particle_type,"dark") != 0 && 
		    strcmp(particle_type,"gas") != 0 &&
		    strcmp(particle_type,"star") != 0 &&
		    strcmp(particle_type,"baryon") != 0 &&
		    strcmp(particle_type,"mark") != 0 &&
		    strcmp(particle_type,"unmark") != 0 &&
		    strcmp(particle_type,"all") !=0) {
		printf("<sorry, %s is not a particle type, %s>\n",
			particle_type,title) ;
		return ;
	    }
	    if(center_box == -1)
	      pot_center(center, box);
	    if (strcmp(particle_type,"dark") == 0){
		if(center_box != -1)
		    setvec(center,boxes[center_box].dark_com) ;
		setvec(center_vel,boxes[box].dark_com_vel) ;
		setvec(center_angular_mom,boxes[box].dark_angular_mom) ;
	    }
	    else if (strcmp(particle_type,"star") == 0){
		if(center_box != -1)
		    setvec(center,boxes[center_box].star_com) ;
		setvec(center_vel,boxes[box].star_com_vel) ;
		setvec(center_angular_mom,boxes[box].star_angular_mom) ;
		color_filter = BBAND ;
	    }
	    else if (strcmp(particle_type,"gas") == 0){
		if(center_box != -1)
		    setvec(center,boxes[center_box].gas_com) ;
		setvec(center_vel,boxes[box].gas_com_vel) ;
		setvec(center_angular_mom,boxes[box].gas_angular_mom) ;
	    }
	    else if (strcmp(particle_type,"baryon") == 0){
		if(center_box != -1)
		    mass_add_vec(center,boxes[center_box].gas_com,
				 boxes[center_box].gas_mass, 
				 boxes[center_box].star_com,boxes[center_box].star_mass) ;
		mass_add_vec(center_vel,boxes[box].gas_com_vel,
			boxes[box].gas_mass,
			boxes[box].star_com_vel, 
			boxes[box].star_mass) ;
		mass_add_vec(center_angular_mom,boxes[box].gas_angular_mom,
			boxes[box].gas_mass, 
			boxes[box].star_angular_mom, 
			boxes[box].star_mass) ;
		color_filter = VBAND ;
	    }
	    else{
		if(center_box != -1)
		    setvec(center,boxes[center_box].total_com) ;
		setvec(center_vel,boxes[box].total_com_vel) ;
		setvec(center_angular_mom,boxes[box].total_angular_mom) ;
	    }
	    if (strcmp(projection_type,"ellipse") == 0 ||
		    strcmp(projection_type,"ell") == 0) {
		printf("<enter the fitting radius, %s>\n",title) ;
		scanf("%lf",&fit_radius) ;
                setvec(center_e,center) ;
		find_shape(particle_type,box,center_e) ;
		find_vel(particle_type,box,center_vel,center_angular_mom) ;
		printf("b/a = %g, c/a = %g\n",ba,ca) ;
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
		    if (strcmp(projection_type,"sphere") == 0 ||
			    strcmp(projection_type,"sph") == 0) {
			radius = distance(center,dp->pos) ;
		    }
		    else if (strcmp(projection_type,"cylinder") == 0 ||
			    strcmp(projection_type,"cyl") == 0) {
			radius = perp_distance(center,
				center_angular_mom,dp->pos) ;
		    }
		    else if (strcmp(projection_type,"ellipse") == 0 ||
			    strcmp(projection_type,"ell") == 0) {
			radius = ell_distance(dp->pos) ;
		    }
		    else {
			printf("<sorry, %s not a projection type, %s>\n",
				projection_type,title) ;
			return ;
		    }
		    if (strcmp(bin_type,"log") == 0) {
			if(radius > 0.0)
			    radius = log10(radius) ;
			else
			    radius = min_radius;
		    }
		    bin = max((int)(floor((radius - min_radius) / bin_size)),0);
		    if (bin < number_bins) {
			number[bin]++ ;
			mass[bin] += dp->mass ;
			sub_vec(delta_x,dp->pos,center) ;
			sub_vec(delta_v,dp->vel,center_vel) ;
			cross_product(ang_mom,delta_x,delta_v) ;
			add_const_mult_vec(&angular_mom[bin][0],dp->mass,
				ang_mom) ;
			dx2 = dot_product(delta_x, delta_x);
			if(dx2 != 0.0)
			  vel = dot_product(delta_x,delta_v) / sqrt(dx2);
			else
			  vel = sqrt(dot_product(delta_v, delta_v));
			vel_radial[bin] += (dp->mass) * vel ;
			vel_radial_sigma[bin] += (dp->mass) * vel * vel ;
			if(array)
			  ar_mean[bin] += dp->mass
			      *array[boxlist[box].dpi[i]+header.nsph];
		    }
		}
	    }
	    if (strcmp(particle_type,"star") == 0 || 
		    strcmp(particle_type,"baryon") ==0 ||
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
		    if (strcmp(projection_type,"sphere") == 0 ||
			    strcmp(projection_type,"sph") == 0) {
			radius = distance(center,sp->pos) ;
		    }
		    else if (strcmp(projection_type,"cylinder") == 0 ||
			    strcmp(projection_type,"cyl") == 0) {
			radius = perp_distance(center,
				center_angular_mom,sp->pos) ;
		    }
		    else if (strcmp(projection_type,"ellipse") == 0 ||
			    strcmp(projection_type,"ell") == 0) {
			radius = ell_distance(sp->pos) ;
		    }
		    else {
			printf("<sorry, %s not a projection type, %s>\n",
				projection_type,title) ;
			return ;
		    }
		    if (strcmp(bin_type,"log") == 0) {
			if(radius > 0.0)
			    radius = log10(radius) ;
			else
			    radius = min_radius;
		    }
		    bin = max((int)(floor((radius - min_radius) / bin_size)),0);
		    if (bin < number_bins) {
			number[bin]++ ;
			mass[bin] += sp->mass ;
			lum[bin] += star_lum(sp->mass,sp->tform) ;
			sub_vec(delta_x,sp->pos,center) ;
			sub_vec(delta_v,sp->vel,center_vel) ;
			cross_product(ang_mom,delta_x,delta_v) ;
			add_const_mult_vec(&angular_mom[bin][0],sp->mass,
				ang_mom) ;
			dx2 = dot_product(delta_x, delta_x);
			if(dx2 != 0.0)
			  vel = dot_product(delta_x,delta_v) /
			    sqrt(dx2);
			else
			  vel = sqrt(dot_product(delta_v, delta_v));
			vel_radial[bin] += (sp->mass) * vel ;
			vel_radial_sigma[bin] += (sp->mass) * vel * vel ;
			if(array)
			  ar_mean[bin] += sp->mass
			      *array[boxlist[box].spi[i] + header.nsph + header.ndark];
		    }
		}
	    }
	    if (strcmp(particle_type,"gas") == 0 ||
		    strcmp(particle_type,"baryon") ==0 ||
		    strcmp(particle_type,"mark") == 0 ||
		    strcmp(particle_type,"unmark") == 0 ||
		    strcmp(particle_type,"all") ==0) {
		if(boxlist[box].ngas > 0 && !meanmwt_loaded) {
		    meanmwt_func();
		    gasconst = kpcunit*KPCCM*KBOLTZ/MHYDR/GCGS/msolunit/MSOLG;
		}
		
		for (i = 0 ;i < boxlist[box].ngas ; i++) {
		    if(strcmp(particle_type,"mark") == 0 &&
		       mark_gas[boxlist[box].gpi[i]] == 0)
			continue;
		    if(strcmp(particle_type,"unmark") == 0 &&
		       mark_gas[boxlist[box].gpi[i]] != 0)
			continue;
		    gp = boxlist[box].gp[i] ;
		    if (strcmp(projection_type,"sphere") == 0 ||
			    strcmp(projection_type,"sph") == 0) {
			radius = distance(center,gp->pos) ;
		    }
		    else if (strcmp(projection_type,"cylinder") == 0 ||
			    strcmp(projection_type,"cyl") == 0) {
			radius = perp_distance(center,
				center_angular_mom,gp->pos) ;
		    }
		    else if (strcmp(projection_type,"ellipse") == 0 ||
			    strcmp(projection_type,"ell") == 0) {
			radius = ell_distance(gp->pos) ;
		    }
		    else {
			printf("<sorry, %s not a projection type, %s>\n",
				projection_type,title) ;
			return ;
		    }
		    if (strcmp(bin_type,"log") == 0) {
			if(radius > 0.0)
			    radius = log10(radius) ;
			else
			    radius = min_radius;
		    }
		    bin = max((int)(floor((radius - min_radius) / bin_size)),0);
		    if (bin < number_bins) {
			number[bin]++ ;
			mass[bin] += gp->mass ;
			gas_mass[bin] += gp->mass ;
			sub_vec(delta_x,gp->pos,center) ;
			sub_vec(delta_v,gp->vel,center_vel) ;
			cross_product(ang_mom,delta_x,delta_v) ;
			add_const_mult_vec(&angular_mom[bin][0],gp->mass,
				ang_mom) ;
			dx2 = dot_product(delta_x, delta_x);
			if(dx2 != 0.0)
			  vel = dot_product(delta_x,delta_v) /
			    sqrt(dx2);
			else
			  vel = sqrt(dot_product(delta_v, delta_v));
			vel_radial[bin] += (gp->mass) * vel ;
			vel_radial_sigma[bin] += (gp->mass) * vel * vel ;
			density[bin] += (gp->mass) * (gp->rho) ;
			temp[bin] += (gp->mass) * (gp->temp) ;
			pressure[bin] += (gp->mass) * gasconst
			    * (gp->temp/meanmwt[gp-gas_particles]) * (gp->rho) ;
			entropy[bin] += (gp->mass) *
			    log10(pow(gp->temp, 1.5)/(gp->rho)) ;
			if(array)
			  ar_mean[bin] += gp->mass*array[boxlist[box].gpi[i]];
		    }
		}
	    }
	    for (i = 0 ;i < number_bins ;i++) {
		if(mass[i] == 0) 
		    continue;
		if (strcmp(particle_type,"gas") == 0 || 
			strcmp(particle_type,"baryon") ==0 ||
			strcmp(particle_type,"mark") == 0 ||
			strcmp(particle_type,"unmark") == 0 ||
			strcmp(particle_type,"all") == 0) {
		    if(gas_mass[i] > 0.0) {
			density[i] /= gas_mass[i] ;
			temp[i] /= gas_mass[i] ;
			pressure[i] /= gas_mass[i] ;
			entropy[i] /= gas_mass[i] ;
		    }
		    else {
			density[i] = 0.0 ;
			temp[i] = 0.0 ;
			pressure[i] = 0.0 ;
			entropy[i] = -HUGE;
		    }
		}
		vel_radial[i] /= mass[i] ;
		vel_radial_sigma[i] /= mass[i] ;
		if(vel_radial_sigma[i] > vel_radial[i]*vel_radial[i])
		    vel_radial_sigma[i] = sqrt(vel_radial_sigma[i]
					       - vel_radial[i]*vel_radial[i]) ;
		else
		    vel_radial_sigma[i] = 0.0;
		    
		for (j = 0 ;j < MAXDIM ; j++) {
		    angular_mom[i][j] /= mass[i] ;
		}
		if(array)
		  ar_mean[i] /= mass[i];
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
		    if (strcmp(projection_type,"sphere") == 0 ||
			    strcmp(projection_type,"sph") == 0) {
			radius = distance(center,dp->pos) ;
		    }
		    else if (strcmp(projection_type,"cylinder") == 0 ||
			    strcmp(projection_type,"cyl") == 0) {
			radius = perp_distance(center,
				center_angular_mom,dp->pos) ;
		    }
		    else if (strcmp(projection_type,"ellipse") == 0 ||
			    strcmp(projection_type,"ell") == 0) {
			radius = ell_distance(dp->pos) ;
		    }
		    if (strcmp(bin_type,"log") == 0) {
			if(radius > 0.0)
			    radius = log10(radius) ;
			else
			    radius = min_radius;
		    }
		    bin = max((int)(floor((radius - min_radius) / bin_size)),0);
		    if (bin < number_bins) {
			sub_vec(delta_x,dp->pos,center) ;
			sub_vec(delta_v,dp->vel,center_vel) ;
			dx2 = dot_product(delta_x, delta_x);
			if(dx2 == 0.0)
			  continue;
			vec_add_const_mult_vec(vel_tang,delta_v,
				-dot_product(delta_v,delta_x) / dx2, delta_x) ;
			cross_product(vel_shell,&angular_mom[bin][0],delta_x) ;
			vec_add_const_mult_vec(vel_tang_pec,vel_tang,
				-1.0 / dx2,vel_shell) ;
			vel_tang_sigma[bin] += (dp->mass) * 
				dot_product(vel_tang_pec,vel_tang_pec) ;
		    }
		}
	    }
	    if (strcmp(particle_type,"star") == 0 ||
		    strcmp(particle_type,"baryon") ==0 ||
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
		    if (strcmp(projection_type,"sphere") == 0 ||
			    strcmp(projection_type,"sph") == 0) {
			radius = distance(center,sp->pos) ;
		    }
		    else if (strcmp(projection_type,"cylinder") == 0 ||
			    strcmp(projection_type,"cyl") == 0) {
			radius = perp_distance(center,
				center_angular_mom,sp->pos) ;
		    }
		    else if (strcmp(projection_type,"ellipse") == 0 ||
			    strcmp(projection_type,"ell") == 0) {
			radius = ell_distance(sp->pos) ;
		    }
		    if (strcmp(bin_type,"log") == 0) {
			if(radius > 0.0)
			    radius = log10(radius) ;
			else
			    radius = min_radius;
		    }
		    bin = max((int)(floor((radius - min_radius) / bin_size)),0);
		    if ( bin >= 0 && bin < number_bins) {
			sub_vec(delta_x,sp->pos,center) ;
			sub_vec(delta_v,sp->vel,center_vel) ;
			dx2 = dot_product(delta_x, delta_x);
			if(dx2 == 0.0)
			  continue;
			vec_add_const_mult_vec(vel_tang,delta_v,
				-dot_product(delta_v,delta_x) / dx2 ,delta_x) ;
			cross_product(vel_shell,&angular_mom[bin][0],delta_x) ;
			vec_add_const_mult_vec(vel_tang_pec,vel_tang,
				-1.0 / dx2,vel_shell) ;
			vel_tang_sigma[bin] += (sp->mass) * 
				dot_product(vel_tang_pec,vel_tang_pec) ;
		    }
		}
	    }
	    if (strcmp(particle_type,"gas") == 0 ||
		    strcmp(particle_type,"baryon") ==0 ||
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
		    if (strcmp(projection_type,"sphere") == 0 ||
			    strcmp(projection_type,"sph") == 0) {
			radius = distance(center,gp->pos) ;
		    }
		    else if (strcmp(projection_type,"cylinder") == 0 ||
			    strcmp(projection_type,"cyl") == 0) {
			radius = perp_distance(center,
				center_angular_mom,gp->pos) ;
		    }
		    else if (strcmp(projection_type,"ellipse") == 0 ||
			    strcmp(projection_type,"ell") == 0) {
			radius = ell_distance(gp->pos) ;
		    }
		    if (strcmp(bin_type,"log") == 0) {
			if(radius > 0.0)
			    radius = log10(radius) ;
			else
			    radius = min_radius;
		    }
		    bin = max((int)(floor((radius - min_radius) / bin_size)),0);
		    if (bin < number_bins) {
			sub_vec(delta_x,gp->pos,center) ;
			sub_vec(delta_v,gp->vel,center_vel) ;
			dx2 = dot_product(delta_x, delta_x);
			if(dx2 == 0.0)
			  continue;
			vec_add_const_mult_vec(vel_tang,delta_v,
				-dot_product(delta_v,delta_x) / dx2,delta_x) ;
			cross_product(vel_shell,&angular_mom[bin][0],delta_x) ;
			vec_add_const_mult_vec(vel_tang_pec,vel_tang,
				-1.0 / dx2,vel_shell) ;
			vel_tang_sigma[bin] += (gp->mass) * 
				dot_product(vel_tang_pec,vel_tang_pec) ;
		    }
		}
	    }
	    for (i = 0 ;i < number_bins ;i++) {
		if(mass[i] == 0.0)
		    continue;
		vel_tang_sigma[i] = sqrt(vel_tang_sigma[i] / mass[i]) ;
	    }
	    hardfile.ptr = fopen(hardfile.name,"w") ;
	    if(hardfile.ptr == NULL)
	      {
		printf("<sorry, can not open %s, %s>\n",hardfile.name,title) ;
		return;
	      }
	    
	    radius_old = 0.0 ;
	    for (i = 0, mass_r = 0.0 ;i < number_bins ;i++) {
		radius = bin_size * (double)(i + 1) ;
		if (strcmp(bin_type,"lin") == 0) {
		    radius += min_radius ;
		}
		else if (strcmp(bin_type,"log") == 0) {
		    radius = pow(10.,radius + min_radius) ;
		}
		radius_mean = (radius + radius_old) / 2.0 ;
		mass_r += mass[i] ;
		if (strcmp(projection_type,"sphere") == 0 ||
			strcmp(projection_type,"sph") == 0) {
		    volume = (4. / 3. * PI * (radius * radius * radius -
			    radius_old * radius_old * radius_old)) ;
		}
		else if (strcmp(projection_type,"cylinder") == 0 ||
			strcmp(projection_type,"cyl") == 0) {
		    volume = ( PI * (radius * radius -
			    radius_old * radius_old)) ;
		}
		else if (strcmp(projection_type,"ellipse") == 0 ||
			strcmp(projection_type,"ell") == 0) {
		    volume = (4. * ba * ca / 3. * PI * (radius * radius *
			    radius - radius_old * radius_old * radius_old)) ;
		}
		rho = mass[i] / volume ;
		if (strcmp(particle_type,"star") == 0 ||
			strcmp(particle_type,"baryon") ==0 ||
			strcmp(particle_type,"mark") == 0 ||
			strcmp(particle_type,"unmark") == 0 ||
			strcmp(particle_type,"all") == 0) {
		    lum_den = lum[i] / volume ;
		}
		vel = sqrt(mass_r / radius) ;
		ang = sqrt(dot_product(&angular_mom[i][0],&angular_mom[i][0])) ;
		if(ang > 0.0)
		    ang_theta = 180.*acos(angular_mom[i][2]/ang)/PI ;
		else
		    ang_theta = 0.0;
		ang_phi = 180.*atan2(angular_mom[i][1],angular_mom[i][0])/PI ;
		vel_circ = ang / radius_mean ;
		fprintf(hardfile.ptr,"%g %d %g %g %g %g %g %g %g %g %g %g",
			radius,number[i],rho,mass_r,vel,vel_radial[i],
			vel_radial_sigma[i],vel_circ,vel_tang_sigma[i],
			ang,ang_theta,ang_phi) ;
		if ((strcmp(particle_type,"gas") == 0 || 
			strcmp(particle_type,"baryon") ==0 ||
			strcmp(particle_type,"mark") == 0 ||
			strcmp(particle_type,"unmark") == 0 ||
			strcmp(particle_type,"all") == 0)
			&& boxlist[box].ngas > 0) {
		    fprintf(hardfile.ptr," %g %g %g %g",density[i],temp[i],
			    pressure[i], entropy[i]) ;
		}
		if ((strcmp(particle_type,"star") == 0 ||
			strcmp(particle_type,"baryon") ==0 ||
			strcmp(particle_type,"mark") == 0 ||
			strcmp(particle_type,"unmark") == 0 ||
			strcmp(particle_type,"all") == 0)
			&& boxlist[box].nstar > 0) {
		    fprintf(hardfile.ptr," %g",lum_den) ;
		}
		if(array)
		  fprintf(hardfile.ptr," %g", ar_mean[i]);
		fprintf(hardfile.ptr,"\n") ;
		radius_old = radius ;
	    }
	    fclose(hardfile.ptr) ;
	}
	else if (!boxes_loaded[box]) {
	    printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
	}
	else if (!boxes_loaded[center_box]) {
	    printf("<sorry, box %d is not loaded, %s>\n",center_box,title) ;
	}
	else {
	    printf("<%d is too many bins, %s>\n",number_bins,title) ;
	}
    }
    else {
	input_error(command) ;
    }
}

void
pot_center(center, box)
Real *center;
int box;
{
  double max_pot = 1e+37;
  int nmax = -1;
  int i;
		
    for (i = 0 ;i < boxlist[box].ngas ;i++) {
      if(boxlist[box].gp[i]->phi < max_pot)
	{
	  max_pot = boxlist[box].gp[i]->phi;
	  nmax = i;
	}
    }
    for (i = 0 ;i < boxlist[box].ndark ;i++) {
      if(boxlist[box].dp[i]->phi < max_pot)
	{
	  max_pot = boxlist[box].dp[i]->phi;
	  nmax = i+boxlist[box].ngas;
	}
    }
    for (i = 0 ;i < boxlist[box].nstar ;i++) {
      if(boxlist[box].sp[i]->phi < max_pot)
	{
	  max_pot = boxlist[box].sp[i]->phi;
	  nmax = i+boxlist[box].ngas+boxlist[box].ndark;
	}
    }
  if(nmax < boxlist[box].ngas)
    {
      setvec(center, boxlist[box].gp[nmax]->pos);
    }
  else if(nmax < boxlist[box].ngas+boxlist[box].ndark)
    {
      setvec(center, boxlist[box].dp[nmax-boxlist[box].ngas]->pos);
    }
  else
    {
      setvec(center,
	   boxlist[box].sp[nmax-(boxlist[box].ngas+boxlist[box].ndark)]->pos);
    }
}
