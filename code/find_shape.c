#include "defs.h"
#include "fdefs.h"

void
find_shape(particle_type,box,center)
    int box ;
    char particle_type[MAXCOMM] ;
    Real center[MAXDIM] ;
{
    double radius ;
    double mass_r ;
    double distance() ;
    double ell_distance() ;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    int i,j,k ;
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

    mass_r = 0. ;
    for (j = 0 ; j < header.ndim ;j++) {
	cm_r[j]  = 0. ;
	for (k = 0 ; k < header.ndim ;k++) {
	    inertia_r[j][k]  = 0. ;
	}
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
	    radius = distance(center,dp->pos) ;
	    if (radius < fit_radius) {
		mass_r += dp->mass ;
		for (j = 0 ; j < header.ndim ;j++) {
		    cm_r[j]  += dp->mass * dp->pos[j] ;
		    for (k = 0 ; k <= j ;k++) {
			inertia_r[j][k]  += dp->mass * dp->pos[j] *
				dp->pos[k] ;
		    }
		}
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
	    radius = distance(center,sp->pos) ;
	    if (radius < fit_radius) {
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
	for (i = 0 ;i < boxlist[box].ngas ; i++) {
	    if(strcmp(particle_type,"mark") == 0 &&
	       mark_gas[boxlist[box].gpi[i]] == 0)
		continue;
	    if(strcmp(particle_type,"unmark") == 0 &&
	       mark_gas[boxlist[box].gpi[i]] != 0)
		continue;
	    gp = boxlist[box].gp[i] ;
	    radius = distance(center,gp->pos) ;
	    if (radius < fit_radius) {
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
    }
    jacobi(inertia_cm,MAXDIM,evalues,evectors,&nrot) ;

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
    }

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
    if(ca == 0.0 || ba == 0.0) /* give up */
	return;
    ba_old = ba ;
    ca_old = ca ;
    phi_old = phi ;
    theta_old = theta ;
    psi_old = psi ;
    niter = 0 ;
    forever{
	mass_r = 0. ;
	for (j = 0 ; j < header.ndim ;j++) {
	    cm_r[j]  = 0. ;
	    for (k = 0 ; k < header.ndim ;k++) {
		inertia_r[j][k]  = 0. ;
	    }
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
		radius = ell_distance(dp->pos) ;
		if (radius < fit_radius) {
		    mass_r += dp->mass ;
		    for (j = 0 ; j < header.ndim ;j++) {
			cm_r[j]  += dp->mass * dp->pos[j] ;
			for (k = 0 ; k <= j ;k++) {
			    inertia_r[j][k]  += dp->mass * dp->pos[j] *
				    dp->pos[k] ;
			}
		    }
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
		radius = ell_distance(sp->pos) ;
		if (radius < fit_radius) {
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
	    for (i = 0 ;i < boxlist[box].ngas ; i++) {
		if(strcmp(particle_type,"mark") == 0 &&
		   mark_gas[boxlist[box].gpi[i]] == 0)
		    continue;
		if(strcmp(particle_type,"unmark") == 0 &&
		   mark_gas[boxlist[box].gpi[i]] != 0)
		    continue;
		gp = boxlist[box].gp[i] ;
		radius = ell_distance(gp->pos) ;
		if (radius < fit_radius) {
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
	if(mass_r == 0.0) break;
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
	}
	jacobi(inertia_cm,MAXDIM,evalues,evectors,&nrot) ;

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
	}

	/* Check if Eigenvectors are righthanded in 3D : ev[ib] x ev[ic] = ev[ia] */
	
	VecProd[1] =  evectors[2][ib]*evectors[3][ic] - evectors[3][ib]*evectors[2][ic];
	VecProd[2] = -evectors[1][ib]*evectors[3][ic] + evectors[3][ib]*evectors[1][ic];
	VecProd[3] =  evectors[1][ib]*evectors[2][ic] - evectors[2][ib]*evectors[1][ic];
	ScalProd   =  evectors[1][ia]*VecProd[1] + evectors[2][ia]*VecProd[2] + evectors[3][ia]*VecProd[3];
	if (ScalProd < 0.0) {
	  for(i=0 ; i < header.ndim ; i++) evectors[i+1][ia] = -evectors[i+1][ia];
	}

	ba = sqrt((double)(evalues[ib]/evalues[ia])) ;
        ca = sqrt((double)(evalues[ic]/evalues[ia])) ;

        /* euler angles for a zyz rotation */
        theta = 180. / PI * acos((double) evectors[3][ic]);
	if(evectors[1][ic]*evectors[1][ic] + evectors[2][ic]*evectors[2][ic] > 0.0)
            phi =   180. / PI
		* acos(evectors[1][ic]/sqrt(evectors[1][ic]*evectors[1][ic]
					    + evectors[2][ic]*evectors[2][ic]));
	else
	    phi = 0.0;
	if(evectors[1][ic]*evectors[1][ic] + evectors[2][ic]*evectors[2][ic] > 0.0)
	    psi =   180. / PI
		* acos((-evectors[2][ic]*evectors[1][ib]
			+ evectors[1][ic]*evectors[2][ib])/
		       sqrt(evectors[1][ic]*evectors[1][ic]
			    + evectors[2][ic]*evectors[2][ic]));
	else
	    psi = 0.0;

        /* inverse acos is only defined between 0 and pi therefore we
	   must deal with pi to 2*pi */
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
	if((fabs(ba-ba_old)/ba_old <= 1.e-4 &&
		fabs(ca-ca_old)/ca_old <= 1.e-4) || niter > 20){
	    if(niter > 20){
		ba = (ba + ba_old) / 2. ;
		ca = (ca + ca_old) / 2. ;
		phi = (phi + phi_old) / 2. ;
		theta = (theta + theta_old) / 2. ;
		psi = (psi + psi_old) / 2. ;
	    }
	    break ;
	}
	ba_old = ba ;
	ca_old = ca ;
	phi_old = phi ;
	theta_old = theta ;
	psi_old = psi ;
	niter++ ;
	if(ca == 0.0 || ba == 0.0) /* give up */
	    return;
    }
}
