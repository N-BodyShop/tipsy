/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:28  trq
 * Initial revision
 *
 * Revision 2.4  1994/12/05  17:49:20  trq
 * Fixed call to my_gets().
 *
 */

#include "defs.h"
#include "fdefs.h"

double star_lum_redshift(mass,time,wavelength)
    double mass ;
    double time ;
    double wavelength ;
{
    static float age[11] = {.01,.02,.05,.1,.2,.5,1.,2.,5.,10.,20.} ;
    static float lum[11] = {.0635,.0719,.0454,.153,.293,.436,.636,
	    .898,1.39,2.54,5.05} ;
    static float iv_dat[11] = {-.71,.18,-.1,-.29,-.15,-.31,-.15,-.29,-.4,-.89,
	    -1.01} ;
    static float vv_dat[11] = {-.01,-.02,-.01,0.,-.01,-.01,-.01,0.,0.,.01,.01} ;
    static float bv_dat[11] = {-.02,-.33,.0,.19,.04,.1,.18,.31,.42,.75,.84} ;
    static float uv_dat[11] = {.16,-.12,.84,.97,.83,1.08,1.24,1.39,1.51,
	    2.22,2.50} ;
    static float uuv_dat[11] = {-.32,-.21,1.68,1.67,1.75,2.62,3.50,4.70,6.30,
	    8.86,9.97} ;
    static float lumv_fit[11] ;
    static float iv_fit[11] ;
    static float vv_fit[11] ;
    static float bv_fit[11] ;
    static float uv_fit[11] ;
    static float uuv_fit[11] ;
    static int spline_loaded = 0 ;
    double spft() ;
    double iv ;
    double vv ;
    double bv ;
    double uv ;
    double uuv ;
    double color ;
    double lum_star ;
    double star_age ;
    char *units_line ;

    if(!spline_loaded){
	splinit(age,lum,lumv_fit,11,0.,0.) ;
	splinit(age,iv_dat,iv_fit,11,0.,0.) ;
	splinit(age,vv_dat,vv_fit,11,0.,0.) ;
	splinit(age,bv_dat,bv_fit,11,0.,0.) ;
	splinit(age,uv_dat,uv_fit,11,0.,0.) ;
	splinit(age,uuv_dat,uv_fit,11,0.,0.) ;
	spline_loaded = 1 ;
    }
    if(!lum_loaded && !cool_loaded){
	printf("<please enter: mass-unit(solar masses)") ;
	printf(" distance-unit(kiloparsecs)>\n") ;
	forever{
	    units_line = my_gets(" ") ;
	    if(sscanf(units_line,"%lf %lf",
		    &msolunit,&kpcunit) == 2){
	        time_unit = sqrt(pow(kpcunit*KPCCM,3.0)/(GCGS*msolunit*MSOLG));
		time_unit /= GYRSEC;
		break ;
	    }
	    printf("<sorry, inconsistent input, %s>\n",title) ;
	}
	lum_loaded = YES ;
    }
    star_age = max((header.time-time)*time_unit,.01) ;
    if(mass != 0.){
	lum_star = mass * msolunit / spft(star_age,age,lum,lumv_fit,11,0) ;
	if(wavelength <= 8800 && wavelength > 5500){
	    iv =  spft(star_age,age,iv_dat,iv_fit,11,0) ;
	    vv =  spft(star_age,age,vv_dat,vv_fit,11,0) ;
	    color = (iv-vv)/(8800-5500)*(wavelength-5500)+vv ;
	    lum_star *= pow(10.,-.4*color) ;
	}
	else if(wavelength <= 5500 && wavelength > 4400){
	    vv =  spft(star_age,age,vv_dat,vv_fit,11,0) ;
	    bv =  spft(star_age,age,bv_dat,bv_fit,11,0) ;
	    color = (vv-bv)/(5500-4400)*(wavelength-4400)+bv ;
	    lum_star *= pow(10.,-.4*color) ;
	}
	else if(wavelength <= 4400 && wavelength > 3360){
	    bv =  spft(star_age,age,bv_dat,bv_fit,11,0) ;
	    uv =  spft(star_age,age,uv_dat,uv_fit,11,0) ;
	    color = (bv-uv)/(4400-3360)*(wavelength-3360)+uv ;
	    lum_star *= pow(10.,-.4*color) ;
	}
	else if(wavelength <= 3360 && wavelength >= 1940){
	    uv =  spft(star_age,age,uv_dat,uv_fit,11,0) ;
	    uuv =  spft(star_age,age,uuv_dat,uuv_fit,11,0) ;
	    color = (uv-uuv)/(3360-1940)*(wavelength-1940)+uuv ;
	    lum_star *= pow(10.,-.4*color) ;
	}
    }
    else{
	lum_star = 0. ;
    }
    return (lum_star) ;
}
