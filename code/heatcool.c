#include "defs.h"
#include "fdefs.h"

/* xion tolerance */
#define TOLERANCE 1e-14
#define SMALLNUM 1.e-30

static double n_h, g0, g1, g2, r;

/* ------------------------------------------------------ */
/* rate coefficient of ionization of H by electron impact in cc/s */
/* N.B. only accurate for 1e4 < T < 2e5 (Black 1981) */
/* Modified as in Cen 1992 */

static
double g_H(t)
     double t;
{
    if (t == 0.0) {
	return 0.0;
    }
    return 5.85e-11*sqrt(t)*exp(-157809.1 / t)/(1.0 + sqrt(t/1e5));
}

/* --------------------------------------------------------- */
/* rate coefficient of ionization of He by electron impact in cc/s */
/* N.B. only accurate for 1e4 < T < 2e5 */
/* Modified as in Cen 1992 */

static
double g_He(t)
     double t;
{
    if (t == 0.0) {
	return 0.0;
    }
    return 2.38e-11*sqrt(t)*exp(-285335.4 / t)/(1.0 + sqrt(t/1e5));
}

/* --------------------------------------------------------- */
/* rate coefficient of ionization of He+ by electron impact in cc/s */
/* N.B. only accurate for 1e4 < T < 2e5 */
/* Modified as in Cen 1992 */

static
double g_Hep(t)
     double t;
{
    if (t == 0.0) {
	return 0.0;
    }
    return 5.68e-12*sqrt(t)*exp(-631515.0 / t)/(1.0 + sqrt(t/1e5));
}

/* ---------------------------------------------------------- */
/*radiative recombination rate coefficient for H (XXX units? most likely cc/s)
*/
/* Modified as in Cen 1992 */

static
double a_Hp(t)
     double t;
{
    if (t > 0.0) {
	return (8.40e-11/sqrt(t))*pow(t/1e3, -0.2)/(1.0 + pow(t/1e6, 0.7));
    }
    return 0.0;
}

/* ----------------------------------------------------------- */
/*radiative recombination rate coefficient for He (XXX units? most likely cc/s
)*/

static
double a_Hep(t)
     double t;
{
    if (t > 0.0) {
	return 1.5e-10*pow(t, -0.6353);
    }
    return 0.0;
}

/* ------------------------------------------------------------ */
/*radiative recombination rate coefficient for He+ (XXX units? most likely*/
/* cc/s) */
/* Modified as in Cen 1992 */

static
double a_Hepp(t)
     double t;
{
    if (t > 0.0) {
	return (3.36e-10/sqrt(t))*pow(t/1e3, -0.2)/(1.0 + pow(t/1e6, 0.7));
    }
    return 0.0;
}

/* ------------------------------------------------------------- */
/*dielectronic radiative recombination rate coefficient for He+ (XXX units?*/
/* most likely cc/s) */

static
double a_p(t)
     double t;
{
    if (t == 0.0) {
	return 0.0;
    }
    return .0019*pow(t, -1.5)*exp(-4.7e5/t)*(exp(-9.4e4/t)*.3 + 1.0);
}

/* --------------------------------------------------------- */
/* cooling rate coefficient for recombination to H in erg*cc/s. */
/* The cooling will scale as n_e*n_H+. */
/* Modified as in Cen 1992 */

static
double rate_Hp(t)
     double t;
{
    if (t == 0.0) {
	return 0.0;
    }
    return 8.70e-27*sqrt(t)*pow(t/1e3, -0.2)/(1.0 + pow(t/1e6, 0.7));
}

/* --------------------------------------------------------- */
/*cooling rate coefficient for recombination to He, collisional ionization*/
/* of He+, collisional exitation of He+ and dielectronic recombination of */
/* He+ in erg*cc/s. */
/* The cooling will scale as n_e*n_He+. */
/* Modified as in Cen 1992 */

static
double rate_Hep(t)
     double t;
{
    if (t == 0.0) {
	return 0.0;
    }
    return 1.55e-26*pow(t, .3647)	/* recombination */
      + 4.95e-22*sqrt(t)*exp(-631515.0/t)/(1.0 + sqrt(t/1e5)) /* ionization */
				/* exitation */
      + 5.54e-17*pow(t, -.397)*exp(-473638.0/t)/(1.0 + sqrt(t/1e5))
				/* dielectric recombination */
      + 1.24e-13*pow(t, -1.5)*exp(-4.7e5/t)*(exp(-9.4e4/t)*.3 + 1.0);
}

/* --------------------------------------------------------- */
/* cooling rate coefficient for recombination to He+ in erg*cc/s. */
/* The cooling will scale as n_e*n_He++. */
/* Modified as in Cen 1992 */
static
double rate_Hepp(t)
     double t;
{
    if (t == 0.0)
      return  0.0;
    return 3.48e-26*sqrt(t)*pow(t/1e3, -0.2)/(1.0 + pow(t/1e6, 0.7));
}

/* --------------------------------------------------------- */
/*cooling rate coefficient for collisional ionization and excitation of H in*/
/* erg*cc/s. */
/* The cooling will scale as n_e*n_H. */
/* Modified as in Cen 1992 */
static
double rate_H(t)
     double t;
{
    if (t == 0.0)
	return 0.0;
				/* ionization */
    return 1.27e-21*sqrt(t)*exp(-157809.1/t)/(1.0 + sqrt(t/1e5))
      + 7.5e-19*exp(-118348.0/t)/(1.0 + sqrt(t/1e5));	/* excitation */
}
/* --------------------------------------------------------- */
/*cooling rate coefficient for collisional excitation of H in*/
/* erg*cc/s. */
/* The cooling will scale as n_e*n_H. */
/* Modified as in Cen 1992 */
static
double rate_H_cex(t)
     double t;
{
    if (t == 0.0)
	return 0.0;
    return 7.5e-19*exp(-118348.0/t)/(1.0 + sqrt(t/1e5));	/* excitation */
}

/* --------------------------------------------------------- */
/* cooling rate coefficient for collisional ionization of He in erg*cc/s. */
/* The cooling will scale as n_e*n_He. */
/* Modified as in Cen 1992 */
static
double rate_He(t)
     double t;
{
    if (t == 0.0) {
	return 0.0;
    }
    return 9.38e-22*sqrt(t)*exp(-285335.4/t)/(1.0 + sqrt(t/1e5));
}

/* ----------------------------------------------------------- */
/* cooling rate coefficient for bremsstrahlung for all ions in erg*cc/s. */
/* The cooling will scale as n_e*(n_H+ + n_He+ + 4n_He++). */
static
double rate_br(t)
     double t;
{
    double gf = 1.1 + 0.34*exp(-pow(5.5-log10(t), 2.0)/3.0);
    return sqrt(t)*1.42e-27*gf;
}
/* --------------------------------------------------------- */
/* radiative emission coefficient for recombination to H in erg*cc/s. */
/* (ie rate_Hp but includes potential energy as well) */
/* The cooling will scale as n_e*n_H+. */
/* Modified as in Cen 1992 */

static
double radrate_Hp(t)
     double t;
{
    if (t == 0.0) {
	return 0.0;
    }
    return (8.70e-27 * t + 1.83e-21) / sqrt(t)
      * pow(t/1e3, -0.2)/(1.0 + pow(t/1e6, 0.7));
}


/* --------------------------------------------------------- */
/* radiative emission coefficient for recombination to He, */
/* collisional ionization of He+, */
/* collisional exitation of He+ and */
/* dielectronic recombination of He+ in erg*cc/s. */
/* The cooling will scale as n_e*n_He+. */
/* Modified as in Cen 1992 */

static
double radrate_Hep(t)
     double t;
{
    if (t == 0.0) {
	return 0.0;
    }
    return (1.55e-26 * t + 5.91e-21) * pow(t, -0.6353)	/* recombination */
      + 4.95e-22*sqrt(t)*exp(-631515.0/t)/(1.0 + sqrt(t/1e5)) /* ionization */
				/* exitation */
      + 5.54e-17*pow(t, -.397)*exp(-473638.0/t)/(1.0 + sqrt(t/1e5))
				/* dielectric recombination */
      + 1.99e-13*pow(t, -1.5)*exp(-4.7e5/t)*(exp(-9.4e4/t)*.3 + 1.0);
}

/* --------------------------------------------------------- */
/* radiative emission coefficient for recombination to He+ in erg*cc/s. */
/* The cooling will scale as n_e*n_He++. */
/* Modified as in Cen 1992 */
static
double radrate_Hepp(t)
     double t;
{
    if (t == 0.0)
      return  0.0;
    return (3.48e-26 * t + 2.93e-20) / sqrt(t)
      * pow(t/1e3, -0.2)/(1.0 + pow(t/1e6, 0.7));
}

/* ------------------------------------------------------- */
/* calculate ionization equilibrium */

static
void xion(t, x, x_1, x_2, x_3, p_ne)
     double t;
     double *x;
     double *x_1;
     double *x_2;
     double *x_3;
     double *p_ne;
{
    double a;
    int i;
    double zx;
    double zx1, zx2;
    double zx3;
    double za_Hp, za_Hep, za_Hepp; /* recombination */
    double zg_H, zg_He, zg_Hep;	/* collisional ionization */
    double n_e;
    double old_n_e;

    zx = .05;
    zx1 = 1.0;
    zx2 = 0.0;
    n_e = (1.0 - zx)*n_h;
    old_n_e = n_e;
    
    za_Hp = a_Hp(t);
    zg_H = g_H(t);
    za_Hep = a_Hep(t) + a_p(t);
    zg_He = g_He(t);
    za_Hepp = a_Hepp(t);
    zg_Hep = g_Hep(t);
    for (i = 1; i <= 50; ++i) {
      if(i == 50)
	{
	  fprintf(stderr, "<too many iterations in xion>\n");
	  fprintf(stderr, "t: %g, n_h: %g, n_e: %g\n", t, n_h, n_e);
	}
/* solve for neutral H fraction given constant n_e */
	if (g0 + (zg_H + za_Hp)*n_e != 0.0) {
	    zx = za_Hp*n_e/(g0 + (zg_H + za_Hp)*n_e);
	} else {
	    zx = 1.0;
	}
	if (g1 + zg_He*n_e > 1e-50) {
	    a = za_Hep*n_e/(g1 + zg_He*n_e);
	} else {
	    zx1 = 1.0;
	    zx2 = 0.0;
	    zx3 = 0.0;
	    n_e = (1.0 - zx)*n_h;
	    if(old_n_e == 0.0 || fabs(n_e - old_n_e)/n_h < TOLERANCE)
	      break;
	    n_e = .5*(n_e + old_n_e);
	    old_n_e = n_e;
	    continue;
	}
	if (g2 + (zg_Hep + (a + 1.0)*za_Hepp)*n_e != 0.0) 
	  {
	    zx2 = za_Hepp*n_e/(g2 + (zg_Hep + (a + 1.0)*za_Hepp)*n_e);
	    zx1 = zx2 * a;
	    zx3 = zx2*(g2 + zg_Hep*n_e)/za_Hepp/n_e;
	} else {
	  zx3 = 0.0;
	    zx2 = 0.0;
	    zx1 = 1.0;
	}
	n_e = (1.0 - zx + zx2*r + zx3*2.0*r)*n_h;
      if(fabs(n_e - old_n_e)/n_h < TOLERANCE)
	break;
      n_e = .5*(n_e + old_n_e);
      old_n_e = n_e;
    }
    zx = min(1.,zx) ;
    *x = zx;
    *x_1 = zx1;
    *x_2 = zx2;
    *x_3 = zx3;
    *p_ne = n_e;
}


double heatcool(gp)
    struct gas_particle *gp;
{
    double temp;
    double density;
    double crate, hrate, compcrate;
    double h0, h1, h2;
    double x, x_1, x_2, x_3, f_e, n_e;
    double y;
    double totcool, rhoproper, acool, ethermal, coolfac, dudtmech;

    density = gp->rho;
    temp = gp->temp;

      n_h = (density/cosmof3)*MSOLG*msolunit* 
	    fhydrogen / ((kpcunit*kpcunit*kpcunit) * MHYDR * 
	    KPCCM*KPCCM*KPCCM);   

/* c------------------------------------------------------- */
/* c  crate is cooling rate in units of (erg cm^3/s) */
/* c  hrate is heating rate */
/* c */
/* c  alphaj is spectral index J \propto \nu^-\alpha */
/* c */
/* c  t  is temperature in K */
/* c */
/* c  j_nu is photoionizing flux at Lyman limit */
/* c  in units of 10^{-21} has to be passed in common block */
/* c  (per steradian) */
/* c */
/* c */
/* c  n_H is density of hydrogen ions (Not helium) */
/* c  passed in common */
/* c */
/* c  y is helium abundance by mass */
/* c  r is ratio of helium ions to hydrogen ions (all ionizations) */
/* c  g0 is photoionization rate coefficient? */
/* c  g1 is photoionization rate coefficient? */
/* c  g2 is photoionization rate coefficient? */
/* c */
/* c  h0 is photoionization heating rate coefficient? */
/* c  h1 is photoionization heating rate coefficient? */
/* c  h2 is photoionization heating rate coefficient? */
/*   x is fraction of H that is NEUTRAL */
/*   x_1 is fraction of He that is NEUTRAL */
/*   x_2 is fraction of He that is singly ionized */

    y = 1.0 - fhydrogen;
    r = y / 4.0 / (1.0 - y);
    g0 = gp0_H ;
    g1 = gp0_He ;
    g2 = gp0_Hep ;

    h0 = eps_H ;
    h1 = eps_He ;
    h2 = eps_Hep ;

    xion(temp, &x, &x_1, &x_2, &x_3, &n_e);
    f_e = 1.0 - x + x_2*r + (x_3)*2.0*r;
    crate = f_e*rate_Hp(temp)*(1.0 - x);
    crate += f_e*x_2*r*rate_Hep(temp);
    crate += f_e*(x_3)*r*rate_Hepp(temp);
    crate += f_e*x*rate_H(temp);
    crate += f_e*x_1*r*rate_He(temp);
    crate += f_e*(1.0 - x + x_2*r + (x_3)*4.0*r)*rate_br(temp);
    crate *= n_h*n_h;
    hrate = h0*x + h1*x_1*r + h2*x_2*r;
    hrate *= n_h;
    
/*    compcrate = 8.0*SIGMAES*f_e/(3.0*ME*C); */

/* see Ikeuchi and Ostriker 1986, Ap. J. 301, 522. */
    compcrate = 5.41e-36*pow(1.0 + redshift, 4.0)*f_e*temp*n_h;
    totcool = crate + compcrate - hrate ;
    if(slowcool != 1.0){
	/* limit cooling by fraction of thermal energy */
	rhoproper = (density/cosmof3) * msolunit / (kpcunit*kpcunit*kpcunit)
		 * MSOLG / (KPCCM*KPCCM*KPCCM);
	totcool /= rhoproper;  /* cooling per s per g */
	if (!meanmwt_loaded) {
	  meanmwt_func();
	}
	ethermal = KBOLTZ / (GAMMA-1.) * temp / meanmwt[gp-gas_particles] /
		   MHYDR; /* specific thermal energy */
	if(!divv_loaded){
	    divv() ;
	}
	if(!dudt_loaded){
	    calc_dudt() ;
	}
	dudtmech = (kpcunit*KPCCM) / (time_unit*GYRSEC)
		 * (kpcunit*KPCCM) / (time_unit*GYRSEC)
		 * dudt[gp-gas_particles] / (time_unit*GYRSEC) ;
	acool = (slowcool) * ethermal / (dtcool * time_unit * GYRSEC);
	acool = acool + dudtmech;
	if (acool < SMALLNUM) acool = SMALLNUM;
	coolfac = acool / sqrt( acool*acool + totcool*totcool );
	totcool *= coolfac;
	totcool *= rhoproper; 
    }
    
    return  -(totcool);
}

void lycool(gp,cool_vec)
    struct gas_particle *gp;
    double cool_vec[COOLVECSIZE] ;
{
    double temp;
    double density;
    double crate, hrate, compcrate;
    double h0, h1, h2;
    double x, x_1, x_2, x_3, f_e, n_e;
    double totcool, rhoproper, acool, ethermal, coolfac, dudtmech;
    double y;
    int i ;

    density = gp->rho;
    temp = gp->temp;

      n_h = (density/cosmof3)*MSOLG*msolunit* 
	    fhydrogen / ((kpcunit*kpcunit*kpcunit) * MHYDR * 
	    KPCCM*KPCCM*KPCCM);   

/* c------------------------------------------------------- */
/* c  crate is cooling rate in units of (erg cm^3/s) */
/* c  hrate is heating rate */
/* c */
/* c  alphaj is spectral index J \propto \nu^-\alpha */
/* c */
/* c  t  is temperature in K */
/* c */
/* c  j_nu is photoionizing flux at Lyman limit */
/* c  in units of 10^{-21} has to be passed in common block */
/* c  (per steradian) */
/* c */
/* c */
/* c  n_H is density of hydrogen ions (Not helium) */
/* c  passed in common */
/* c */
/* c  y is helium abundance by mass */
/* c  r is ratio of helium ions to hydrogen ions (all ionizations) */
/* c  g0 is photoionization rate coefficient? */
/* c  g1 is photoionization rate coefficient? */
/* c  g2 is photoionization rate coefficient? */
/* c */
/* c  h0 is photoionization heating rate coefficient? */
/* c  h1 is photoionization heating rate coefficient? */
/* c  h2 is photoionization heating rate coefficient? */
/*   x is fraction of H that is NEUTRAL */
/*   x_1 is fraction of He that is NEUTRAL */
/*   x_2 is fraction of He that is singly ionized */

    y = 1.0 - fhydrogen;
    r = y / 4.0 / (1.0 - y);
    g0 = gp0_H ;
    g1 = gp0_He ;
    g2 = gp0_Hep ;

    h0 = eps_H ;
    h1 = eps_He ;
    h2 = eps_Hep ;

    xion(temp, &x, &x_1, &x_2, &x_3, &n_e);
    f_e = 1.0 - x + x_2*r + (x_3)*2.0*r;
    if(slowcool != 1.0){
	crate = f_e*rate_Hp(temp)*(1.0 - x);
	crate += f_e*x_2*r*rate_Hep(temp);
	crate += f_e*(x_3)*r*rate_Hepp(temp);
	crate += f_e*x*rate_H(temp);
	crate += f_e*x_1*r*rate_He(temp);
	crate += f_e*(1.0 - x + x_2*r + (x_3)*4.0*r)*rate_br(temp);
	crate *= n_h*n_h;
	hrate = h0*x + h1*x_1*r + h2*x_2*r;
	hrate *= n_h;
	
    /* see Ikeuchi and Ostriker 1986, Ap. J. 301, 522. */
	compcrate = 5.41e-36*pow(1.0 + redshift, 4.0)*f_e*temp*n_h;
    }
    cool_vec[0] = f_e*radrate_Hp(temp)*(1.0 - x);
    cool_vec[1] = f_e*x*rate_H_cex(temp);
    cool_vec[2] = f_e*x*rate_H(temp) - cool_vec[1];
    cool_vec[3] = f_e*x_2*r*radrate_Hep(temp);
    cool_vec[4] = f_e*(x_3)*r*radrate_Hepp(temp);
    cool_vec[5] = f_e*x_1*r*rate_He(temp);
    cool_vec[6] = f_e*(1.0 - x + x_2*r + (x_3)*4.0*r)*rate_br(temp);

    for(i = 0; i < COOLVECSIZE; i++){
	cool_vec[i] *= n_h*n_h;
    }
    if(slowcool != 1.0){
	/* limit cooling by fraction of thermal energy */
	rhoproper = (density/cosmof3) * msolunit / (kpcunit*kpcunit*kpcunit)
		 * MSOLG / (KPCCM*KPCCM*KPCCM);
	totcool = crate + compcrate - hrate ;
	totcool /= rhoproper;  /* cooling per s per g */
	if (!meanmwt_loaded) {
	  meanmwt_func();
	}
	ethermal = KBOLTZ / (GAMMA-1.) * temp / meanmwt[gp-gas_particles] /
		   MHYDR; /* specific thermal energy */
	if(!divv_loaded){
	    divv() ;
	}
	if(!dudt_loaded){
	    calc_dudt() ;
	}
	dudtmech = (kpcunit*KPCCM) / (time_unit*GYRSEC)
		 * (kpcunit*KPCCM) / (time_unit*GYRSEC)
		 * dudt[gp-gas_particles] / (time_unit*GYRSEC) ;
	acool = (slowcool) * ethermal / (dtcool * time_unit * GYRSEC);
	acool = acool + dudtmech;
	if (acool < SMALLNUM) acool = SMALLNUM;
	coolfac = acool / sqrt( acool*acool + totcool*totcool );
	for(i = 0; i < COOLVECSIZE; i++){
	    cool_vec[i] *= coolfac;
	}
    }

    return ;
}
void
calc_hneutral(temp, density, hneutral_p, heneutral_p, heII_p)
     double temp;
     double density;
     double *hneutral_p;
     double *heneutral_p;
     double *heII_p;
{
    double y;
    double n_e;
    double heIII;

    n_h = (density/cosmof3)*MSOLG*msolunit* 
	    fhydrogen / ((kpcunit*kpcunit*kpcunit) * MHYDR * 
	    KPCCM*KPCCM*KPCCM);

/* c  n_H is density of hydrogen ions (Not helium) */
/* c */
/* c  y is helium abundance by mass */
/* c  r is ratio of helium ions to hydrogen ions (all ionizations) */
/* c  g0 is photoionization rate coefficient? */
/* c  g1 is photoionization rate coefficient? */
/* c  g2 is photoionization rate coefficient? */
/*   x is fraction of H that is NEUTRAL */
/*   x_1 is fraction of He that is NEUTRAL */
/*   x_2 is fraction of He that is singly ionized */

    y = 1.0 - fhydrogen;
    r = y / 4.0 / (1.0 - y);
    g0 = gp0_H ;
    g1 = gp0_He ;
    g2 = gp0_Hep ;

    xion(temp, hneutral_p, heneutral_p, heII_p, &heIII, &n_e);

    return ;
}

double calc_meanmwt(temp, density)
     double temp;
     double density;
{
    double x, x_1, x_2, x_3;
    double y;
    double n_e;
    double f_e;

    n_h = (density/cosmof3)*MSOLG*msolunit* 
	    fhydrogen / ((kpcunit*kpcunit*kpcunit) * MHYDR * 
	    KPCCM*KPCCM*KPCCM);

/* c------------------------------------------------------- */
/* c  crate is cooling rate in units of (erg cm^3/s) */
/* c  hrate is heating rate */
/* c */
/* c  alphaj is spectral index J \propto \nu^-\alpha */
/* c */
/* c  t  is temperature in K */
/* c */
/* c  j_nu is photoionizing flux at Lyman limit */
/* c  in units of 10^{-21} has to be passed in common block */
/* c  (per steradian) */
/* c */
/* c */
/* c  n_H is density of hydrogen ions (Not helium) */
/* c */
/* c  y is helium abundance by mass */
/* c  r is ratio of helium ions to hydrogen ions (all ionizations) */
/* c  g0 is photoionization rate coefficient? */
/* c  g1 is photoionization rate coefficient? */
/* c  g2 is photoionization rate coefficient? */
/* c */
/*   x is fraction of H that is NEUTRAL */
/*   x_1 is fraction of He that is NEUTRAL */
/*   x_2 is fraction of He that is singly ionized */

    y = 1.0 - fhydrogen;
    r = y / 4.0 / (1.0 - y);
    g0 = gp0_H ;
    g1 = gp0_He ;
    g2 = gp0_Hep ;

    xion(temp, &x, &x_1, &x_2, &x_3, &n_e);
    f_e = 1.0 - x + x_2*r + (x_3)*2.0*r;
    return ((1. + 4.*r) / (1. + r + f_e));
}

#include "xray.h"

PROTO(double, spline_int, (double *xa,double *ya,double *y2a,int n,double
		     x));

/*
 * Return X-Ray emmissivity in ergs/s/MSYS
 */
double calc_xemiss(temp, density, band)
     double temp;
     double density;
     int band;
{
    double x, x_1, x_2, x_3;
    double y;
    double xemiss;
    double n_e;

    n_h = (density/cosmof3)*MSOLG*msolunit* 
	    fhydrogen / ((kpcunit*kpcunit*kpcunit) * MHYDR * 
	    KPCCM*KPCCM*KPCCM);

    /* See previous comments. */

    y = 1.0 - fhydrogen;
    r = y / 4.0 / (1.0 - y);
    g0 = gp0_H ;
    g1 = gp0_He ;
    g2 = gp0_Hep ;

    xion(temp, &x, &x_1, &x_2, &x_3, &n_e);
    if(temp < 3e4)
	return 0.0;
    else
	xemiss = spline_int(temp_spline,
	       xray_lums[band].yspl,
	       xray_lums[band].y2spl,xray_spline_n,
	       log10(temp));
    
    /*
     * The interpolation returns 10^{-23} ergs/s/n_e/n_h per cc of gas
     * We need to convert to ergs/s/MSYS of gas.
     */
    return n_e*n_h*pow(10.0, xemiss)*1.0e-23/density
	*cosmof3*kpcunit*kpcunit*kpcunit*KPCCM*KPCCM*KPCCM;
}

void
ionize()
{
    double a0 ;
    double planck ;
    double ev ;
    double e0_H ;
    double e0_He ;
    double e0_Hep ;
    double gint ;
    double eint ;
    double at ;
    double beta ;
    double s ;
    double fac1() ;
    double fac2() ;
    double romberg_o();

 /*  ionize -- compute photo-ionization rates and photo-ionization heating
     ionize(alphaj,gp0_H,gp0_He,gp0_Hep,eps_H,eps_He,eps_Hep)
     input:
       * alphaj = index of UV background, J \propto (nu_L/nu)**alphaj
     output:
       * gp0_H, gp0_He, gp0_Hep = photo-ionization rates for H, He, Hep */

    a0 = 6.30e-18 ;
    planck = 6.63e-27 ;
    ev = 1.60e-12 ;
    e0_H = 13.60*ev ;
    e0_He = 24.59*ev ;
    e0_Hep = 54.42*ev ;

 /*  evaluate dimensionless integrals needed for H and He+ rates (see notes) */
    gint = romberg_o(fac1,0.,1.,1e-6) ;
    gp0_H = a0*gint/planck ;
    gp0_Hep = gp0_H*pow((e0_H/e0_Hep),alphaj)/4. ;
    eint = romberg_o(fac2,0.,1.,1e-6) ;
    eps_H = a0*eint*(e0_H/planck) ;
    eps_Hep = eps_H*pow((e0_H/e0_Hep),(alphaj-1.))/4. ;

    at = 7.83e-18 ;
    beta = 1.66 ;
    s = 2.05 ;

    gp0_He = (at/planck)*pow((e0_H/e0_He),alphaj)*(beta/(alphaj+s)+(1.-beta)/
	    (alphaj+s+1)) ;
    eps_He = (e0_He/planck)*at*pow((e0_H/e0_He),alphaj)*(beta/(alphaj+s-1)+
	    (1-2*beta)/(alphaj+s)-(1-beta)/(alphaj+s+1)) ;
    return ;
}
double fac1(t)
    double t;
{
    double tinv ;
    double eps ;
    double fac ;

    tinv = 1./t ;
    eps = sqrt(tinv-1.) ;
    fac = exp(4.-4.*atan(eps)/eps)/(1.-exp(-2.*PI/eps))*pow(t,(alphaj+3.));
    return fac ;
}
double fac2(t)
    double t;
{
    double tinv ;
    double eps ;
    double fac ;

    tinv = 1./t ;
    eps = sqrt(tinv-1.) ;
    fac = exp(4.-4.*atan(eps)/eps)/(1.-exp(-2.*PI/eps))*pow(t,(alphaj+3.));
    fac = fac*(tinv-1.) ;
    return fac ;
}

#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#define MAXLEV 15

double romberg_o(func,a,b,eps)
        double (*func)() ;	/* function to integrate */
	double a,b ;		/* endpoints */
	double eps;		/* relative accuracy */
{
    double tllnew;
    double tll;
    double tlk[MAXLEV+1];
    int n = 1;
    int nsamples = 1;
    
    tlk[0] = tllnew = (b-a)*(*func)((b-a)/2.0);
    tll = 0.5*HUGE;
    
    while((fabs(tllnew-tll) > fabs(tllnew)*eps) && (n < MAXLEV)) {
	/*
	 * midpoint rule.
	 */
	double deltax;
	double tlktmp;
	int i;

	nsamples *= 3;
	deltax = (b-a)/nsamples;
	tlktmp = tlk[0];
	tlk[0] = tlk[0]/3.0;
	
	for(i = 0; i < nsamples/3; i++) {
	    tlk[0] += deltax*(*func)(a + (3*i + 0.5)*deltax);
	    tlk[0] += deltax*(*func)(a + (3*i + 2.5)*deltax);
	}
    
	/*
	 * Romberg extrapolation.
	 */

	for(i = 0; i < n; i++) {
	    double tlknew = (pow(9.0, i+1)*tlk[i] - tlktmp)
		/(pow(9.0, i+1) - 1.0);
	    
	    tlktmp = tlk[i+1];
	    tlk[i+1] = tlknew;
	}
	tll = tllnew;
	tllnew = tlk[n];
	n++;
    }
    
    assert(n < MAXLEV);
    return tllnew;
}

double spline_int(xa,ya,y2a,n,x)
     double *xa;		/* x table */
     double *ya;		/* y table */
     double *y2a;		/* y'' table */
     int n;			/* number of entries */
     double x;			/* evaluation point */
{
    int lo,hi,k;
    double h,b,a;
    double result;

    lo = 0;
    hi = n-1;
    while (hi-lo > 1) {
	k=(hi + lo) >> 1;	/* bisect interval */
	if (xa[k] > x) hi = k;
	else lo = k;
    }
    h = xa[hi] - xa[lo];
    assert(h > 0.0);

    a = (xa[hi] - x)/h;
    b = (x - xa[lo])/h;
    result = a*ya[lo] + b*ya[hi] /* linear interpolation */
	+ ((a*a*a - a)*y2a[lo] + (b*b*b - b)*y2a[hi])*(h*h)/6.0;
    return result;
}
