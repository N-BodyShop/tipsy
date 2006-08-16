#include "defs.h"
#include "fdefs.h"

void
find_shape_array(particle_type,box,center,aindex,particlemax,particlemin,rbinsize)
    int box ;
    char particle_type[MAXCOMM] ;
    Real center[MAXDIM] ;
    int *aindex ;  /* points to index array created in moments */
    int particlemax;
    int particlemin;
    int rbinsize;
{
    double radius ;
    double mass_r ;
    double distance() ;
    double ell_distance() ;
    double ell_distance2();
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    int i,j,k,l ;
    double cm_r[MAXDIM] ;
    double inertia_r[MAXDIM][MAXDIM] ;
    double inertia_cm[MAXDIM+1][MAXDIM+1] ;
    double evalues[MAXDIM+1] ;
    double evectors[MAXDIM+1][MAXDIM+1] ;
    int ia,ib,ic ;
    int nrot ;
    double ba_old, ca_old ;
    double phi_old,theta_old,psi_old ;
    double VecProd[MAXDIM+1], ScalProd ;
    int niter ;

    double radius_sum;
    int count;
    int offset;
    int aoffset;

    radius_sum = 0.0;
    count = 0;

    mass_r = 0. ;
    for (j = 0 ; j < header.ndim ;j++) {
	cm_r[j]  = 0. ;
	for (k = 0 ; k < header.ndim ;k++) {
	    inertia_r[j][k]  = 0. ;
	}
    }

    offset = header.nsph; /* for use in array index computation */
    if (strcmp(particle_type,"dark") == 0 ||
	    strcmp(particle_type,"mark") == 0 ||
	    strcmp(particle_type,"unmark") == 0 ||
	    strcmp(particle_type,"all") ==0) {
        for (l = particlemin ; l < particlemax; l++) {
	    i = aindex[l]; /* gets the actual particle index from the ranked list */

	    if (strcmp(particle_type,"all") == 0 &&
                i >= boxlist[box].ngas && i < boxlist[box].ngas+boxlist[box].ndark) {
		i -= boxlist[box].ngas;
	    }
	    else if (strcmp(particle_type,"all") == 0)
		continue;

	    if(strcmp(particle_type,"mark") == 0 &&
	       mark_dark[boxlist[box].dpi[i]] == 0)
		continue;
	    if(strcmp(particle_type,"unmark") == 0 &&
	       mark_dark[boxlist[box].dpi[i]] != 0)
		continue;
	    dp = boxlist[box].dp[i] ;

        /*  aoffset = dp-dark_particles+offset;
            if (array[aoffset] > DEN_MIN && array[aoffset] < DEN_MAX) {
	        if (particlemin == 5000) {
                mark_dark[boxlist[box].dpi[i]]++;
            } */


	    radius = distance(center,dp->pos) ;


            if (1 > 0) {
		mass_r += dp->mass ;
		for (j = 0 ; j < header.ndim ;j++) {
		    cm_r[j]  += dp->mass * dp->pos[j] ;
		    for (k = 0 ; k <= j ;k++) {
			inertia_r[j][k]  += dp->mass * dp->pos[j] *
				dp->pos[k] ;
		    }
		}
	    } /* reads in M*R_(i) and M*R_(i)*R_(j)*/
	}
    }
    if (strcmp(particle_type,"star") == 0 ||
	    strcmp(particle_type,"mark") == 0 ||
	    strcmp(particle_type,"unmark") == 0 ||
	    strcmp(particle_type,"all") ==0) {
        for (l = particlemin ; l < particlemax; l++) {
	    i = aindex[l]; /* gets the actual particle index from the ranked list */

            if (strcmp(particle_type,"all") == 0 &&
                i >= boxlist[box].ngas+boxlist[box].ndark && i < boxlist[box].ngas+boxlist[box].ndark+boxlist[box].nstar) {
                i -= (boxlist[box].ngas+boxlist[box].ndark);
            }
            else if (strcmp(particle_type,"all") == 0)
                continue;

	    if(strcmp(particle_type,"mark") == 0 &&
	       mark_star[boxlist[box].spi[i]] == 0)
		continue;
	    if(strcmp(particle_type,"unmark") == 0 &&
	       mark_star[boxlist[box].spi[i]] != 0)
		continue;
	    sp = boxlist[box].sp[i] ;
	    radius = distance(center,sp->pos) ;
	    if (0 < 1) {
		mass_r += sp->mass ;
		for (j = 0 ; j < header.ndim ;j++) {
		    cm_r[j]  += sp->mass * sp->pos[j] ;
		    for (k = 0 ; k <= j ;k++) {
			inertia_r[j][k]  += sp->mass * sp->pos[j] *
				sp->pos[k] ;
		    }
		}
	    }
	}
    }
    if (strcmp(particle_type,"gas") == 0 ||
	    strcmp(particle_type,"mark") == 0 ||
	    strcmp(particle_type,"unmark") == 0 ||
	    strcmp(particle_type,"all") ==0) {
        for (l = particlemin ; l < particlemax; l++) {
	    i = aindex[l]; /* gets the actual particle index from the ranked list */

            if (strcmp(particle_type,"all") == 0 &&
                i >= 0 && i < boxlist[box].ngas) {
                i -= 0;
            }
            else if (strcmp(particle_type,"all") == 0)
                continue;

	    if(strcmp(particle_type,"mark") == 0 &&
	       mark_gas[boxlist[box].gpi[i]] == 0)
		continue;
	    if(strcmp(particle_type,"unmark") == 0 &&
	       mark_gas[boxlist[box].gpi[i]] != 0)
		continue;
	    gp = boxlist[box].gp[i] ;
	    radius = distance(center,gp->pos) ;
	    if (0 < 1) {
		mass_r += gp->mass ;
		for (j = 0 ; j < header.ndim ;j++) {
		    cm_r[j]  += gp->mass * gp->pos[j] ;
		    for (k = 0 ; k <= j ;k++) {
			inertia_r[j][k]  += gp->mass * gp->pos[j] *
				gp->pos[k] ;
		    }
		}
	    }
	}
    }
    if(mass_r == 0.0) return;
    for (j = 0 ; j < header.ndim ;j++) {
	for (k = 0 ; k < header.ndim ;k++) {
	    if(k <= j){
		inertia_cm[j+1][k+1]  = ((inertia_r[j][k] -
		    cm_r[j] * cm_r[k] / mass_r) / mass_r) ;
	    } else {
		inertia_cm[j+1][k+1]  = ((inertia_r[k][j] -
		    cm_r[k] * cm_r[j] / mass_r) / mass_r) ;
	    }
	}
	evalues[j+1] = inertia_cm[j+1][j+1] ;
    } /* gets the actual MI tensor */
    jacobi(inertia_cm,MAXDIM,evalues,evectors,&nrot) ;
       /* finds eigenvalues */
    if(evalues[1] < 0.0) {
	fprintf(stderr, "Warning %s, negative eigenvalues in find_shape()\n", 
		title);
	evalues[1] = 0.0;
    }
    if(evalues[2] < 0.0) {
	fprintf(stderr, "Warning %s, negative eigenvalues in find_shape()\n", 
		title);
	evalues[2] = 0.0;
    }
    if(evalues[3] < 0.0) {
	fprintf(stderr, "Warning %s, negative eigenvalues in find_shape()\n", 
		title);
	evalues[3] = 0.0;
    }
    if(evalues[1] == 0.0 && evalues[2] == 0.0 && evalues[3] == 0.0) {
	fprintf(stderr, "Warning %s, all eigenvalues 0 in find_shape()\n", 
		title);
	fprintf(stderr, "Giving up in find_shape()\n");
	return;
    }
    
    if(evalues[1] >= evalues[2] && evalues[1] >= evalues[3]){
	ia = 1 ;
	if(evalues[2] >= evalues[3]){
	    ib = 2 ;
	    ic = 3 ;
	}
	else{
	    ib = 3 ;
	    ic = 2 ;
	}
    }
    else if(evalues[2] > evalues[1] && evalues[2] >= evalues[3]){
	ia = 2 ;
	if(evalues[1] >= evalues[3]){
	    ib = 1 ;
	    ic = 3 ;
	}
	else{
	    ib = 3 ;
	    ic = 1 ;
	}
    }
    else{
	ia = 3 ;
	if(evalues[1] >= evalues[2]){
	    ib = 1 ;
	    ic = 2 ;
	}
	else{
	    ib = 2 ;
	    ic = 1 ;
	}
    } /* sorts eigenvalues decreasing */

    /* Check if Eigenvectors are righthanded in 3D :
       ev[ib] x ev[ic] = ev[ia] */

    VecProd[1] =  evectors[2][ib]*evectors[3][ic]
	- evectors[3][ib]*evectors[2][ic];
    VecProd[2] = -evectors[1][ib]*evectors[3][ic]
	+ evectors[3][ib]*evectors[1][ic];
    VecProd[3] =  evectors[1][ib]*evectors[2][ic]
	- evectors[2][ib]*evectors[1][ic];
    ScalProd   =  evectors[1][ia]*VecProd[1] + evectors[2][ia]*VecProd[2]
	+ evectors[3][ia]*VecProd[3];
    if (ScalProd < 0.0) {
      for(i=0; i < header.ndim ; i++) evectors[i+1][ia] = -evectors[i+1][ia];
    }

    ba = sqrt((double)(evalues[ib]/evalues[ia])) ;
    ca = sqrt((double)(evalues[ic]/evalues[ia])) ;

    /* euler angles for a zyz rotation */
    theta = 180. / PI * acos((double) evectors[3][ic]);
    if(evectors[1][ic]*evectors[1][ic] + evectors[2][ic]*evectors[2][ic] > 0.0)
    	phi = 180. / PI
	    * acos(evectors[1][ic]/sqrt(evectors[1][ic]*evectors[1][ic]
					+ evectors[2][ic]*evectors[2][ic]));
    else
	phi = 0.0;
    if(evectors[1][ic]*evectors[1][ic] + evectors[2][ic]*evectors[2][ic] > 0.0)
	psi =   180. / PI
	    * acos((-evectors[2][ic]*evectors[1][ib]
		    + evectors[1][ic]*evectors[2][ib])
		   /sqrt(evectors[1][ic]*evectors[1][ic]
			 + evectors[2][ic]*evectors[2][ic]));
    else
	psi = 0.0;

    /* inverse acos is only defined between 0 and pi therefore we must
       deal with pi to 2*pi */
    if(evectors[2][ic] < 0.0) phi = 360. - phi; /* phi always positive */
    if(evectors[3][ib] < 0.0) psi = 360. - psi; /* psi always positive */ 

    for(i = 0 ;i < header.ndim ;i++){
	center[i] = cm_r[i] / mass_r ;
    }
    for(i=0;i<3;i++){
      ell_matrix_inv[i][0] = evectors[i+1][ia];
      ell_matrix_inv[i][1] = evectors[i+1][ib];
      ell_matrix_inv[i][2] = evectors[i+1][ic];
    }
    transpose(ell_matrix_inv,ell_matrix);
    setvec(center_ell,center) ;

    radius_sum = 0.0;
    count = 0;
    for(l = particlemax - rbinsize; l < particlemax; l++) {
	i = aindex[l];
    	if (strcmp(particle_type,"dark") == 0 || (strcmp(particle_type,"all") == 0 && 
		i >= boxlist[box].ngas && i < boxlist[box].ngas+boxlist[box].ndark) ) {
   	    if (strcmp(particle_type,"all") == 0) i -= boxlist[box].ngas;
	    dp = boxlist[box].dp[i] ;
	    radius = ell_distance(dp->pos) ;
	} 
	else
    	if (strcmp(particle_type,"star") == 0 || (strcmp(particle_type,"all") == 0 && 
		i >= boxlist[box].ngas+boxlist[box].ndark && i < boxlist[box].ngas+boxlist[box].ndark+boxlist[box].nstar) ) {
   	    if (strcmp(particle_type,"all") == 0) i -= (boxlist[box].ngas+boxlist[box].ndark);
	    sp = boxlist[box].sp[i] ;
	    radius = ell_distance(sp->pos) ;
	}
	else
    	if (strcmp(particle_type,"gas") == 0 || (strcmp(particle_type,"all") == 0 && 
		i >= 0 && i < boxlist[box].ngas) ) {
	    gp = boxlist[box].gp[i] ;
	    radius = ell_distance(gp->pos) ;
	}
	else {
	    printf("<radius error, particle type unknown>\n");
	    return;
	} 
        radius_sum += radius;
        count++;
    }


    raverage = radius_sum/((double)count);

    if(ca == 0.0 || ba == 0.0) /* give up */ {
	printf("<ca and ba are both 0.0, giving up>\n");
	return;
    }

/* shape marking diagnotistic 
    if(particlemax == 8199) {
        for(l = 0; l <= 17000; l++) {
                i = aindex[l];
                dp = boxlist[box].dp[i] ;
                radius = ell_distance(dp->pos) ;

                if (radius < raverage +0.0003 && radius > raverage) {
                    mark_dark[boxlist[box].dpi[i]]++;
                }

        }
    }
*/

    /* shape diagnostic viewing 	
    if(fit_radius > .004 && shellcounter != 1) {
	for(l = 0; l < boxlist[box].ndark; l++) {
	    dp = boxlist[box].dp[l];
	    radius = ell_distance(dp->pos) ;
	    if (radius < .003) {
		mark_dark[boxlist[box].dpi[l]]++;
	    }	
	}
    } */
    
}

