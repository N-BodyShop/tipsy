/*
 * $Header$
 * $Log$
 * Revision 1.1  1999/08/25 22:05:31  nsk
 * added center to boxstat, checks for periodic in smooth, prints out
 * cooling stuff, vista makes plots
 *
 * Revision 1.3  1999/02/09  19:17:31  trq
 * Added "entropy" plot type.
 *
 * Revision 1.2  1996/04/11  21:27:59  trq
 * view_star.c: fixed bug in abox vs. density calculation.
 * activatebox.c: hsmdivv is independent of abox.
 * divv.c, smooth.c, smooth.h, view_gas.c: Use tree to calculate hsmdivv[].
 *
 * Revision 1.1.1.1  1995/01/10  22:57:32  trq
 * Import to CVS
 *
 * Revision 2.9  1994/09/26  23:05:26  trq
 * Added hneutral fraction plots.
 *
 * Revision 2.8  1994/09/22  16:22:39  trq
 * Changed indexing of hneutral.
 *
 * Revision 2.7  1994/04/20  08:47:33  trq
 * fixed meanmwt bug.
 *
 * Revision 2.6  94/03/04  18:13:12  trq
 * Make "master" variable.
 * 
 * Revision 2.4  94/02/16  10:10:11  trq
 * Added calls to ionization balance calculation.
 * 
 */
#include <stdlib.h>
#include "defs.h"
#include "fdefs.h"

void
write_gas(job)
    char *job;
{
    char command[MAXCOMM] ;
    char type[MAXCOMM] ;
    char filename[MAXCOMM] ;
    int box ;
    FILE *outfile;
    double c1 ;
    double c2 ;
    struct gas_particle *gp ;
    int i ;
    int nbodies ;
    double min_temp ;
    double lum_xray ;
    double temp ;
    double drw ;
    int iwsm ;

    if((sscanf(job,"%s %d %s %s",command,&box,type,filename)) == 4) {
	if (boxes_loaded[box]) {
	    outfile = fopen(filename, "w");
	    if(outfile == NULL)
		{
		  printf("<Sorry %s, can't open file>\n",title);
		  return;
		}
	    nbodies=boxlist[box].ngas+boxlist[box].ndark+boxlist[box].nstar ;
	    fprintf(outfile,"%d %d %d\n",nbodies,boxlist[box].ngas,
					    boxlist[box].nstar) ;
	    if ( strcmp(type,"rho") == 0 || strcmp(type,"density") == 0 ) {
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",gp->rho) ;
		}
	    }
	    else if ( strcmp(type,"logrho") == 0 ||
		    strcmp(type,"logdensity") == 0 ) {
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",log10(gp->rho)) ;
		}
	    }
	    else if ( strcmp(type,"temp") == 0 ||
		    strcmp(type,"temperature") == 0 ) {
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",gp->temp);
		}
	    }
	    else if ( strcmp(type,"logtemp") == 0 ||
		    strcmp(type,"logtemperature") == 0 ) {
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",log10(gp->temp));
		}
	    }
	    else if ( strcmp(type,"pressure") == 0 ||
		    strcmp(type,"press") == 0 ) {
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",gp->rho * gp->temp) ;
		}
	    }
	    else if ( strcmp(type,"loggalaxies") == 0 ||
		    strcmp(type,"loggal") == 0 ) {
		printf("<enter minumum temperature, %s>\n",title) ;
		scanf("%lf",&min_temp) ;
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    if(gp->temp <= min_temp){
			fprintf(outfile,"%g\n",log10(gp->rho));
		    }
		    else{
			fprintf(outfile,"%g\n",-HUGE) ;
		    }
		}
	    }
	    else if ( strcmp(type,"galaxies") == 0 ||
		    strcmp(type,"gal") == 0 ) {
		printf("<enter minumum temperature, %s>\n",title) ;
		scanf("%lf",&min_temp) ;
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    if(gp->temp <= min_temp){
			fprintf(outfile,"%g\n",gp->rho);
		    }
		    else{
			fprintf(outfile,"%g\n",-HUGE) ;
		    }
		}
	    }
	    else if ( strcmp(type,"xraylum") == 0 ||
		    strcmp(type,"xlum") == 0 ) {
		if(!xray_loaded){
		    if(xray_lum_load() != 0){
			return ;
		    }
		}
		if(number_bands != 1){
		    printf("<warning more than one band, %s>\n",title) ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    if(log10(gp->temp) > xray_min_temp){
			temp = log10(gp->temp) - xray_min_temp ;
			temp /= deldtempi ;
			iwsm = (int)temp ;
			if(iwsm >= number_xray_data){
			    printf("<sorry, gas temperature too") ;
			    printf(" big, %s>",title) ;
			    return ;
			}
			drw = temp - (double)iwsm ;
			lum_xray = ((1. - drw) * xray_lum[iwsm] +
				drw * xray_lum[1 + iwsm]) ;
			lum_xray *= 100./118.1/0.6/0.6*(gp->rho)*(gp->mass) ;
			fprintf(outfile,"%g\n",lum_xray);
		    }
		    else{
			fprintf(outfile,"%g\n",-HUGE) ;
		    }
		}
	    }
	    else if ( strcmp(type,"logxraylum") == 0 ||
		    strcmp(type,"lxlum") == 0 ) {
		if(!xray_loaded){
		    if(xray_lum_load() != 0){
			return ;
		    }
		}
		if(number_bands != 1){
		    printf("<warning more than one band, %s>\n",title) ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    if(log10(gp->temp) > xray_min_temp){
			temp = log10(gp->temp) - xray_min_temp ;
			temp /= deldtempi ;
			iwsm = (int)temp ;
			if(iwsm >= number_xray_data){
			    printf("<sorry, gas temperature too") ;
			    printf(" big, %s>",title) ;
			    return ;
			}
			drw = temp - (double)iwsm ;
			lum_xray = ((1. - drw) * xray_lum[iwsm] +
				drw * xray_lum[1 + iwsm]) ;
			lum_xray *= 100./118.1/0.6/0.6*(gp->rho)*(gp->mass) ;
			fprintf(outfile,"%g\n",log10(lum_xray));
		    }
		    else{
			fprintf(outfile,"%g\n",-HUGE) ;
		    }
		}
	    }
	    else if ( strcmp(type,"logpressure") == 0 ||
		    strcmp(type,"logpress") == 0 ) {
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
			    fprintf(outfile,"%g\n",log10(gp->rho * gp->temp));
		}
	    }
	    else if ( strcmp(type,"entropy") == 0 ){
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
			fprintf(outfile,"%g\n",
				log10(pow(gp->temp, 1.5)/gp->rho));
		}
	    }
	    else if (strcmp(type,"divv") == 0 ) {
		if(!divv_loaded){
		    divv() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
			  fprintf(outfile,"%g\n",hsmdivv[gp-gas_particles]) ;
		}
	    }
	    else if (strcmp(type,"bvisc") == 0 || 
		    strcmp(type,"bulkvisc") == 0 ) {
		if(!visc_loaded){
		    load_visc() ;
		}
		if(!divv_loaded){
		    divv() ;
		}
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    double tconst = meanmwt[i]*MHYDR*GCGS/KBOLTZ
		      *msolunit*MSOLG/kpcunit/KPCCM;
		    c1 = sqrt(GAMMA / tconst) ;
		    gp = boxlist[box].gp[i] ;
		    if(hsmdivv[gp-gas_particles] < 0.){
			c2 = hsmdivv[gp-gas_particles] * gp->hsmooth ;
		    }
		    else{
			c2 = 0. ;
		    }
			    fprintf(outfile,"%g\n",((alpha * c1 *
				    sqrt(gp->temp) * c2 + beta * c2 * c2)
				    / gp->rho)) ;
		}
	    }
	    else if (strcmp(type,"lbvisc") == 0 || 
		    strcmp(type,"logbulkvisc") == 0 ) {
		if(!visc_loaded){
		    load_visc() ;
		}
		if(!divv_loaded){
		    divv() ;
		}
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    double tconst = meanmwt[i]*MHYDR*GCGS/KBOLTZ
		      *msolunit*MSOLG/kpcunit/KPCCM;
		    c1 = sqrt(GAMMA / tconst) ;
		    gp = boxlist[box].gp[i] ;
		    if(hsmdivv[gp-gas_particles] < 0.){
			c2 = -hsmdivv[gp-gas_particles] * gp->hsmooth ;
			fprintf(outfile,"%g\n",log10(((alpha *
				c1 * sqrt(gp->temp) * c2 + beta * c2 * c2)
				/ gp->rho)));
		    }
		    else{
			fprintf(outfile,"%g\n",-HUGE) ;
		    }
		}
	    }
	    else if ( strcmp(type,"jeans") == 0 ) {
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		if(!epsgas_loaded){
		    load_epsgas() ;
		}
				/* See Katz and Gunn, 1991 */
		c2 = (PI*epsgas_grav*cosmof) * (PI*epsgas_grav*cosmof) / 3. ;
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    double tconst = meanmwt[i]*MHYDR*GCGS/KBOLTZ
		      *msolunit*MSOLG/kpcunit/KPCCM;
		    c1 =  1. / .89553 / tconst  ;
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",(pow(c1 * 
			    gp->temp / pow(gp->rho/cosmof3,1./3.) + c2 *
			    pow(gp->rho/cosmof3,2./3.),1.5)/gp->mass)) ;
		}
	    }
	    else if ( strcmp(type,"logjeans") == 0 ){
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		if(!epsgas_loaded){ 
		    load_epsgas() ;
		}
				/* See Katz and Gunn, 1991 */
		c2 = (PI * epsgas_grav) * (PI * epsgas_grav) / 3. ;
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		  double tconst = meanmwt[i]*MHYDR*GCGS/KBOLTZ
		    *msolunit*MSOLG/kpcunit/KPCCM;
		  c1 =  1. / .89553 / tconst  ;
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",log10(pow(c1 * 
			    gp->temp / pow(gp->rho/cosmof3,1./3.) + c2 *
			    pow(gp->rho/cosmof3,2./3.),1.5)/gp->mass)) ;
		}
	    }
	    else if ( strcmp(type,"smooth") == 0 ) {
		if(!epsgas_loaded){
		    load_epsgas() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",(gp->hsmooth /
			    epsgas_grav)) ;
		}
	    }
	    else if ( strcmp(type,"logsmooth") == 0 ) {
		if(!epsgas_loaded){
		    load_epsgas() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",log10(gp->hsmooth /
			    epsgas_grav)) ;
		}
	    }
	    else if ( strcmp(type,"soft") == 0 ) {
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		if(!epsgas_loaded){
		    load_epsgas() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		  double tconst = meanmwt[i]*MHYDR*GCGS/KBOLTZ
		    *msolunit*MSOLG/kpcunit/KPCCM;
		  c1 = tconst * .89553 * (PI * epsgas_grav*cosmof) *
			(PI * epsgas_grav*cosmof) / 3. ;
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",(c1 * gp->rho /cosmof3
					 / gp->temp)) ;
		}
	    }
	    else if ( strcmp(type,"logsoft") == 0 ) {
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		if(!epsgas_loaded){
		    load_epsgas() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		  double tconst = meanmwt[i]*MHYDR*GCGS/KBOLTZ
		    *msolunit*MSOLG/kpcunit/KPCCM;
		  c1 = tconst * .89553 * (PI * epsgas_grav*cosmof) *
			(PI * epsgas_grav*cosmof) / 3. ;
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",log10(c1 * 
			    gp->rho/cosmof3 / gp->temp)) ;
		}
	    }
	    else if ( strcmp(type,"tsound") == 0) {
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		c1 = kpcunit*KPCCM*cosmof
		      *sqrt(4*PI/(cosmof3*GAMMA*KBOLTZ/(MHYDR)))
			/(time_unit*GYRSEC);
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",(gp->hsmooth *
			    c1 * sqrt(meanmwt[i] * gp->rho / gp->temp))) ;
		}
	    }
	    else if ( strcmp(type,"logtsound") == 0) {
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		c1 = kpcunit*KPCCM*cosmof
		      *sqrt(4*PI/(cosmof3*GAMMA*KBOLTZ/(MHYDR)))
			/(time_unit*GYRSEC);
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		     fprintf(outfile,"%g\n",log10(gp->hsmooth * c1 *
			    sqrt(meanmwt[i] * gp->rho / gp->temp))) ;
		}
	    }
	    else if ( strcmp(type,"cooling") == 0 ||
		    strcmp(type,"cool") == 0 ) {
		if(!cooling_loaded){
		    cool_func() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",cooling[i]) ;
		}
	    }
	    else if ( strcmp(type,"logcooling") == 0 ||
		    strcmp(type,"logcool") == 0 ) {
		if(!cooling_loaded){
		    cool_func() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    if(cooling[i] < 0.){
			fprintf(outfile,"%g\n",log10(-cooling[i])) ;
		    }
		    else{
			fprintf(outfile,"%g\n",-HUGE);
		    }
		}
	    }
	    else if ( strcmp(type,"logheating") == 0 ||
		    strcmp(type,"logheat") == 0 ) {
		if(!cooling_loaded){
		    cool_func() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    if(cooling[i] > 0.){
			fprintf(outfile,"%g\n",log10(cooling[i])) ;
		    }
		    else{
			fprintf(outfile,"%g\n",-HUGE);
		    }
		}
	    }
	    else if ( strcmp(type,"meanmwt") == 0) {
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",meanmwt[i]) ;
		}
	    }
	    else if ( strcmp(type,"hneutral") == 0) {
		if(!hneutral_loaded){
		    hneutral_func() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",hneutral[gp-gas_particles]
			    *gp->rho*fhydrogen) ;
		}
	    }
	    else if ( strcmp(type,"loghneutral") == 0) {
		if(!hneutral_loaded){
		    hneutral_func() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    if(hneutral[gp-gas_particles] > 0.){
			fprintf(outfile,"%g\n",log10(hneutral[gp-gas_particles]
						  *gp->rho*fhydrogen)) ;
		    }
		    else{
			fprintf(outfile,"%g\n",-HUGE) ;
		    }
		}
	    }
	    else if ( strcmp(type,"hfrac") == 0) {
		if(!hneutral_loaded){
		    hneutral_func() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    fprintf(outfile,"%g\n",hneutral[gp-gas_particles]) ;
		}
	    }
	    else if ( strcmp(type,"loghfrac") == 0) {
		if(!hneutral_loaded){
		    hneutral_func() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    gp = boxlist[box].gp[i] ;
		    if(hneutral[gp-gas_particles] > 0.){
			fprintf(outfile,"%g\n",
				log10(hneutral[gp-gas_particles])) ;
		    }
		    else{
			fprintf(outfile,"%g\n",-HUGE) ;
		    }
		}
	    }
	    else if ( strcmp(type,"formstar") == 0 ||
		    strcmp(type,"fstar") == 0 ) {
		if(!starform_loaded){
		    starform_func() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    fprintf(outfile,"%g\n",starform[i]) ;
		}
	    }
	    else if ( strcmp(type,"logformstar") == 0 ||
		    strcmp(type,"logfstar") == 0 ) {
		if(!starform_loaded){
		    starform_func() ;
		}
		for (i = 0 ;i < boxlist[box].ngas ;i++) {
		    if(starform[i] > 0.){
			fprintf(outfile,"%g\n",log10(starform[i])) ;
		    }
		    else{
			fprintf(outfile,"%g\n",-HUGE) ;
		    }
		}
	    }
	    else {
		printf("<%s is an unknown option, %s>\n",type,title) ;
		fclose(outfile);
		return ;
	    }
	    fclose(outfile);
	}
	else {
	    printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
