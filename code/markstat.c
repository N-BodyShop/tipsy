#include "defs.h"
markstat(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    int box ;
    char type[MAXCOMM] ;
    double mass ;
    double rho ;
    double x_size,y_size,z_size ;
    Real com[MAXDIM] ;
    Real com_vel[MAXDIM] ;
    Real angular_mom[MAXDIM] ;
    Real ang_mom[MAXDIM] ;
    int i ;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;

    if(sscanf(job,"%s %d %s",command,&box,type) == 3) {
	if (strcmp(type,"gas") != 0 && strcmp(type,"all") != 0 && 
		strcmp(type,"dark") != 0 && strcmp(type,"baryon") != 0 &&
		strcmp(type,"star") != 0) {
	    printf("<sorry, %s is not a proper data type, %s>\n",type,title) ;
	    return ;
	}
	if (boxes_loaded[box]) {
	    x_size = sqrt((boxes[box].x4[0]-boxes[box].x2[0]) * 
		    (boxes[box].x4[0]-boxes[box].x2[0]) +
		    (boxes[box].x4[1]-boxes[box].x2[1]) *
		    (boxes[box].x4[1]-boxes[box].x2[1]) +
		    (boxes[box].x4[2]-boxes[box].x2[2]) *
		    (boxes[box].x4[2]-boxes[box].x2[2])) ;
	    y_size = sqrt((boxes[box].x2[0]-boxes[box].x1[0]) * 
		    (boxes[box].x2[0]-boxes[box].x1[0]) +
		    (boxes[box].x2[1]-boxes[box].x1[1]) *
		    (boxes[box].x2[1]-boxes[box].x1[1]) +
		    (boxes[box].x2[2]-boxes[box].x1[2]) *
		    (boxes[box].x2[2]-boxes[box].x1[2])) ;
	    z_size = sqrt((boxes[box].x5[0]-boxes[box].x3[0]) * 
		    (boxes[box].x5[0]-boxes[box].x3[0]) +
		    (boxes[box].x5[1]-boxes[box].x3[1]) *
		    (boxes[box].x5[1]-boxes[box].x3[1]) +
		    (boxes[box].x5[2]-boxes[box].x3[2]) *
		    (boxes[box].x5[2]-boxes[box].x3[2])) ;
	    mass = 0.0 ;
	    for (i = 0 ; i < header.ndim ; i++) {
		com[i] = 0.0 ;
		com_vel[i] = 0.0 ;
		angular_mom[i] = 0.0 ;
	    }
	    if (strcmp(type,"gas") == 0 || strcmp(type,"all") == 0 || 
		    strcmp(type,"baryon") == 0 ) {
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    if(mark_gas[boxlist[box].gpi[i]]){
			gp = boxlist[box].gp[i] ;
			mass += gp->mass ;
			add_const_mult_vec(com, gp->mass, gp->pos) ;
			add_const_mult_vec(com_vel, gp->mass, gp->vel) ;
			cross_product(ang_mom, gp->pos, gp->vel) ;
			add_const_mult_vec(angular_mom, gp->mass, ang_mom) ;
		    }
		}
	    }
	    if (strcmp(type,"dark") == 0 || strcmp(type,"all") == 0) {
		for (i = 0 ;i < boxlist[box].ndark ;i++) {
		    if(mark_dark[boxlist[box].dpi[i]]){
			dp = boxlist[box].dp[i] ;
			mass += dp->mass ;
			add_const_mult_vec(com, dp->mass, dp->pos) ;
			add_const_mult_vec(com_vel, dp->mass, dp->vel) ;
			cross_product(ang_mom, dp->pos, dp->vel) ;
			add_const_mult_vec(angular_mom, dp->mass, ang_mom) ;
		    }
		}
	    }
	    if (strcmp(type,"star") == 0 || strcmp(type,"all") == 0 || 
		    strcmp(type,"baryon") == 0 ) {
		for (i = 0 ;i < boxlist[box].nstar ;i++) {
		    if(mark_star[boxlist[box].spi[i]]){
			sp = boxlist[box].sp[i] ;
			mass += sp->mass ;
			add_const_mult_vec(com, sp->mass, sp->pos) ;
			add_const_mult_vec(com_vel, sp->mass, sp->vel) ;
			cross_product(ang_mom, sp->pos, sp->vel) ;
			add_const_mult_vec(angular_mom, sp->mass, ang_mom) ;
		    }
		}
	    }
            if(mass == 0.){
                printf("<sorry, no particles are marked, %s>\n",title) ;
                return ;
            }
	    for (i = 0 ; i < header.ndim ; i++) {
		com[i] /= mass ;
		com_vel[i] /= mass ;
	    }
	    cross_product(ang_mom, com, com_vel) ;
	    add_const_mult_vec(angular_mom,-mass, ang_mom) ;
	    for (i = 0 ; i < header.ndim ; i++) {
		angular_mom[i] /= mass ;
	    }
	    printf("<physical parameters of the marked %s particles in box %d>\n"
		    ,type,box);
	    rho = mass / boxes[box].volume ;
	    printf("mass = %.6e, density = %.6e, volume = %.6e\n",
		    mass, rho, boxes[box].volume) ;
	    printf("size = (%.6e x %.6e x %.6e)\n",x_size,y_size,z_size) ;
	    printf("center of mass coordinates = {%.6e, %.6e, %.6e}\n",
		    com[0],com[1],com[2]) ;
	    printf("center of mass velocity = {%.6e, %.6e, %.6e}\n",
		    com_vel[0],com_vel[1],com_vel[2]) ;
	    printf("angular momentum vector = {%.6e, %.6e, %.6e}\n",
		    angular_mom[0],angular_mom[1],angular_mom[2]) ;
	}
	else {
	    printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
