/*
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:31  trq
 * Initial revision
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
view_gas(job)
    char *job;
{
    char command[MAXCOMM] ;
    char type[MAXCOMM] ;
    char clip_type[MAXCOMM] ;
    double low ;
    double high ;
    double c1 ;
    double c2 ;
    struct gas_particle *gp ;
    int i,num_read ;
    double min_temp ;
    double lum_xray ;
    double temp ;
    double drw ;
    int iwsm ;

    if (boxes_loaded[0]) {
	if((num_read = sscanf(job,"%s %s %lf %lf %s",command,type,&low,&high,
		clip_type)) >= 4) {
	    if(particle_color != NULL) free(particle_color);
	    if(boxlist[active_box].ngas != 0) {
		particle_color = (int *)malloc(boxlist[active_box].ngas
		    *sizeof(*particle_color));
		if(particle_color == NULL) {
		  printf("<sorry, no memory for particle colors, %s>\n",title) ;
		    return ;
		}
	    }
	    else
	      particle_color = NULL;
	    if(num_read == 5){
	        if(strcmp(clip_type,"clip") == 0 ||
			strcmp(clip_type,"c") == 0 ||
			strcmp(clip_type,"clipboth") == 0 ||
			strcmp(clip_type,"cb") == 0 ){
		    clip_flag = BCLIP ;
		}
	        else if(strcmp(clip_type,"clipup") == 0 ||
			strcmp(clip_type,"cup") == 0 ){
		    clip_flag = UPCLIP ;
		}
	        else if(strcmp(clip_type,"cliplo") == 0 ||
			strcmp(clip_type,"clo") == 0 ){
		    clip_flag = LOCLIP ;
		}
		else{
		    printf("<sorry, %s is not a proper option, %s>\n",
			    clip_type,title) ;
		    return ;
		}
	    }
	    else{
		clip_flag = NOCLIP ;
	    }
	    if(!gas_plot) {
		gas_plot = YES ;
		color_type = GAS;
		array_plot = NO;
		radial_plot = NO ;
		star_plot = NO ;
		potential_plot = NO ;
		magnitude_plot = NO ;
		load_color_table() ;
	    }
	    project() ;
	    color_slope = 124.0 / (high - low) ;
	    color_offset = 2.0 - color_slope * low ;
	    if ( strcmp(type,"rho") == 0 || strcmp(type,"density") == 0 ) {
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope * gp->rho + 
			    color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"logrho") == 0 ||
		    strcmp(type,"logdensity") == 0 ) {
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope * log10(gp->rho) +
			    color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"temp") == 0 ||
		    strcmp(type,"temperature") == 0 ) {
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope * gp->temp +
			    color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"logtemp") == 0 ||
		    strcmp(type,"logtemperature") == 0 ) {
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope * log10(gp->temp) +
			    color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"pressure") == 0 ||
		    strcmp(type,"press") == 0 ) {
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope * gp->rho * gp->temp +
			    color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"loggalaxies") == 0 ||
		    strcmp(type,"loggal") == 0 ) {
		printf("<enter minumum temperature, %s>\n",title) ;
		scanf("%lf",&min_temp) ;
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    if(gp->temp <= min_temp){
			particle_color[i] = (int)(color_slope *
				    log10(gp->rho) + color_offset +0.5) ;
		    }
		    else{
			particle_color[i] = 0 ;
		    }
		}
	    }
	    else if ( strcmp(type,"galaxies") == 0 ||
		    strcmp(type,"gal") == 0 ) {
		printf("<enter minumum temperature, %s>\n",title) ;
		scanf("%lf",&min_temp) ;
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    if(gp->temp <= min_temp){
			particle_color[i] = (int)(color_slope * gp->rho +
				    color_offset +0.5) ;
		    }
		    else{
			particle_color[i] = 0 ;
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
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
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
			particle_color[i] = (int)(color_slope * lum_xray +
				    color_offset +0.5) ;
		    }
		    else{
			particle_color[i] = 0 ;
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
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
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
			particle_color[i] = (int)(color_slope * log10(lum_xray)+
				    color_offset +0.5) ;
		    }
		    else{
			particle_color[i] = 0 ;
		    }
		}
	    }
	    else if ( strcmp(type,"logpressure") == 0 ||
		    strcmp(type,"logpress") == 0 ) {
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope *
			    log10(gp->rho * gp->temp) + color_offset +0.5) ;
		}
	    }
	    else if (strcmp(type,"divv") == 0 ) {
		if(!divv_loaded){
		    divv() ;
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    particle_color[i] = (int)(color_slope * hsmdivv[i] +
			    color_offset +0.5) ;
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
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    double tconst = meanmwt[i]*MHYDR*GCGS/KBOLTZ
		      *msolunit*MSOLG/kpcunit/KPCCM;
		    c1 = sqrt(GAMMA / tconst) ;
		    gp = boxlist[active_box].gp[i] ;
		    if(hsmdivv[i] < 0.){
			c2 = hsmdivv[i] * gp->hsmooth ;
		    }
		    else{
			c2 = 0. ;
		    }
		    particle_color[i] = (int)(color_slope * 
			    ((alpha * c1 * sqrt(gp->temp) * c2 + beta * c2 * c2)
			    / gp->rho) + color_offset +0.5) ;
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
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    double tconst = meanmwt[i]*MHYDR*GCGS/KBOLTZ
		      *msolunit*MSOLG/kpcunit/KPCCM;
		    c1 = sqrt(GAMMA / tconst) ;
		    gp = boxlist[active_box].gp[i] ;
		    if(hsmdivv[i] < 0.){
			c2 = -hsmdivv[i] * gp->hsmooth ;
			particle_color[i] = (int)(color_slope * log10(((alpha *
				c1 * sqrt(gp->temp) * c2 + beta * c2 * c2)
				/ gp->rho)) + color_offset +0.5) ;
		    }
		    else{
			particle_color[i] = 2 ;
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
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    double tconst = meanmwt[i]*MHYDR*GCGS/KBOLTZ
		      *msolunit*MSOLG/kpcunit/KPCCM;
		    c1 =  1. / .89553 / tconst  ;
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope * (pow(c1 * 
			    gp->temp / pow(gp->rho/cosmof3,1./3.) + c2 *
			    pow(gp->rho/cosmof3,2./3.),1.5)/gp->mass) +
			    color_offset +0.5) ;
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
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		  double tconst = meanmwt[i]*MHYDR*GCGS/KBOLTZ
		    *msolunit*MSOLG/kpcunit/KPCCM;
		  c1 =  1. / .89553 / tconst  ;
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope * log10(pow(c1 * 
			    gp->temp / pow(gp->rho/cosmof3,1./3.) + c2 *
			    pow(gp->rho/cosmof3,2./3.),1.5)/gp->mass) +
			    color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"smooth") == 0 ) {
		if(!epsgas_loaded){
		    load_epsgas() ;
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope * (gp->hsmooth /
			    epsgas_grav) + color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"logsmooth") == 0 ) {
		if(!epsgas_loaded){
		    load_epsgas() ;
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope * log10(gp->hsmooth /
			    epsgas_grav) + color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"soft") == 0 ) {
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		if(!epsgas_loaded){
		    load_epsgas() ;
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		  double tconst = meanmwt[i]*MHYDR*GCGS/KBOLTZ
		    *msolunit*MSOLG/kpcunit/KPCCM;
		  c1 = tconst * .89553 * (PI * epsgas_grav*cosmof) *
			(PI * epsgas_grav*cosmof) / 3. ;
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope
					      * (c1 * gp->rho /cosmof3
					 / gp->temp) + color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"logsoft") == 0 ) {
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		if(!epsgas_loaded){
		    load_epsgas() ;
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		  double tconst = meanmwt[i]*MHYDR*GCGS/KBOLTZ
		    *msolunit*MSOLG/kpcunit/KPCCM;
		  c1 = tconst * .89553 * (PI * epsgas_grav*cosmof) *
			(PI * epsgas_grav*cosmof) / 3. ;
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope * log10(c1 * 
			    gp->rho/cosmof3 / gp->temp) + color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"tsound") == 0) {
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		c1 = kpcunit*KPCCM*cosmof
		      *sqrt(4*PI/(cosmof3*GAMMA*KBOLTZ/(MHYDR)))
			/(time_unit*GYRSEC);
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope * (gp->hsmooth *
			    c1 * sqrt(meanmwt[i] * gp->rho / gp->temp)) +
			    color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"logtsound") == 0) {
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		c1 = kpcunit*KPCCM*cosmof
		      *sqrt(4*PI/(cosmof3*GAMMA*KBOLTZ/(MHYDR)))
			/(time_unit*GYRSEC);
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope *
			    log10(gp->hsmooth * c1 *
			    sqrt(meanmwt[i] * gp->rho / gp->temp)) +
					      color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"cooling") == 0 ||
		    strcmp(type,"cool") == 0 ) {
		if(!cooling_loaded){
		    cool_func() ;
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope*cooling[i]
					      + color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"logcooling") == 0 ||
		    strcmp(type,"logcool") == 0 ) {
		if(!cooling_loaded){
		    cool_func() ;
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    if(cooling[i] < 0.){
			particle_color[i] = (int)(color_slope
						  *log10(-cooling[i])
						  + color_offset +0.5) ;
		    }
		    else{
			particle_color[i] = 0 ;
		    }
		}
	    }
	    else if ( strcmp(type,"logheating") == 0 ||
		    strcmp(type,"logheat") == 0 ) {
		if(!cooling_loaded){
		    cool_func() ;
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    if(cooling[i] > 0.){
			particle_color[i] = (int)(color_slope
						  *log10(cooling[i])
						  + color_offset +0.5) ;
		    }
		    else{
			particle_color[i] = 0 ;
		    }
		}
	    }
	    else if ( strcmp(type,"meanmwt") == 0) {
		if(!meanmwt_loaded){
		    meanmwt_func() ;
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] = (int)(color_slope*meanmwt[i]
					      + color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"hneutral") == 0) {
		if(!hneutral_loaded){
		    hneutral_func() ;
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] =
		      (int)(color_slope*hneutral[gp-gas_particles]
			    *gp->rho*fhydrogen + color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"loghneutral") == 0) {
		if(!hneutral_loaded){
		    hneutral_func() ;
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    if(hneutral[gp-gas_particles] > 0.){
			particle_color[i] =
			  (int)(color_slope*log10(hneutral[gp-gas_particles]
						  *gp->rho*fhydrogen)
						  + color_offset +0.5) ;
		    }
		    else{
			particle_color[i] = 0 ;
		    }
		}
	    }
	    else if ( strcmp(type,"hfrac") == 0) {
		if(!hneutral_loaded){
		    hneutral_func() ;
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    particle_color[i] =
		      (int)(color_slope*hneutral[gp-gas_particles]
					      + color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"loghfrac") == 0) {
		if(!hneutral_loaded){
		    hneutral_func() ;
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    gp = boxlist[active_box].gp[i] ;
		    if(hneutral[gp-gas_particles] > 0.){
			particle_color[i] =
			  (int)(color_slope*log10(hneutral[gp-gas_particles])
						  + color_offset +0.5) ;
		    }
		    else{
			particle_color[i] = 0 ;
		    }
		}
	    }
	    else if ( strcmp(type,"formstar") == 0 ||
		    strcmp(type,"fstar") == 0 ) {
		if(!starform_loaded){
		    starform_func() ;
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    particle_color[i] = (int)(color_slope *
			    starform[i] + color_offset +0.5) ;
		}
	    }
	    else if ( strcmp(type,"logformstar") == 0 ||
		    strcmp(type,"logfstar") == 0 ) {
		if(!starform_loaded){
		    starform_func() ;
		}
		for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		    if(starform[i] > 0.){
			particle_color[i] = (int)(color_slope *
				log10(starform[i]) + color_offset +0.5) ;
		    }
		    else if(clip_flag){
			particle_color[i] = 0 ;
		    }
		    else{
			particle_color[i] = 2 ;
		    }
		}
	    }
	    else {
		printf("<%s is an unknown option, %s>\n",type,title) ;
		return ;
	    }
	    for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
		if(particle_color[i] < 2 && (clip_flag == LOCLIP ||
			clip_flag == BCLIP)){
		    particle_color[i] = 0 ;
		}
		else if(particle_color[i] > 126 && (clip_flag == UPCLIP ||
			clip_flag == BCLIP)){
		    particle_color[i] = 0 ;
		}
		else{
		    particle_color[i] = min(particle_color[i],126) ;
		    particle_color[i] = max(particle_color[i],2) ;
		}
	    }
	    current_color = YES;
	    plot_sub(job) ;
	}
	else {
	    input_error(command) ;
	}
    }
    else {
	printf("<sorry, no boxes are loaded, %s>\n",title) ;
    }
}
