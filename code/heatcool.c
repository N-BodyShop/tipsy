#include "defs.h"

/*     Thomson cross section */
#define SIGMAES  6.665e-25
/*     electron mass */
#define ME  9.11e-28
/*     speed of light */
#define C 2.998e10
/* xion tolerance */
#define TOLERANCE 1e-14

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

/* ------------------------------------------------------- */
/* calculate ionization equilibrium */

static
void xion(t, x, x_1, x_2)
     double t;
     double *x;
     double *x_1;
     double *x_2;
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
	    zx3 = zx2*(g2 + zg_Hep)/za_Hepp;
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
    *x = zx;
    *x_1 = zx1;
    *x_2 = zx2;
}


double heatcool(temp, density)
     double temp;
     double density;
{
    double crate, hrate, compcrate;
    double h0, h1, h2;
    double x, x_1, x_2, f_e;
    double y;

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
    g0 = jnu21 * 1.26e-11 / (alphaj + 3.0);
    g1 = jnu21 * 1.52e-11 * pow(3.29/5.95, alphaj)/(alphaj+3);
    g2 = jnu21 * 2.65e-12 * pow(3.29/13.2, alphaj)/(alphaj+3);

    h0 = jnu21 * 2.6e-22 /(alphaj + 2.0)/(alphaj + 3.0);
    h1 = jnu21 * 5.98e-22 * pow(3.29/5.95, alphaj)/(alphaj + 2.0)/(alphaj + 3.0);
    h2 = jnu21 * 2.32e-22 * pow(3.29/13.2, alphaj)/(alphaj + 2.0)/(alphaj + 3.0);

    xion(temp, &x, &x_1, &x_2);
    f_e = 1.0 - x + x_2*r + (1.0 - x_1 - x_2)*2.0*r;
    crate = f_e*rate_Hp(temp)*(1.0 - x);
    crate += f_e*x_2*r*rate_Hep(temp);
    crate += f_e*(1.0 - x_1 - x_2)*r*rate_Hepp(temp);
    crate += f_e*x*rate_H(temp);
    crate += f_e*x_1*r*rate_He(temp);
    crate += f_e*(1.0 - x + x_2*r + (1.0 - x_1 - x_2)*4.0*r)*rate_br(temp);
    crate *= n_h*n_h;
    hrate = h0*x + h1*x_1*r + h2*x_2*r;
    hrate *= n_h;
    
/*    compcrate = 8.0*SIGMAES*f_e/(3.0*ME*C); */

/* see Ikeuchi and Ostriker 1986, Ap. J. 301, 522. */
    compcrate = 5.41e-36*pow(1.0 + redshift, 4.0)*f_e*temp*n_h;
    
    return hrate - (crate + compcrate);
}

double calc_hneutral(temp, density)
     double temp;
     double density;
{
    double x, x_1, x_2;
    double y;

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
/* c  h0 is photoionization heating rate coefficient? */
/* c  h1 is photoionization heating rate coefficient? */
/* c  h2 is photoionization heating rate coefficient? */
/*   x is fraction of H that is NEUTRAL */
/*   x_1 is fraction of He that is NEUTRAL */
/*   x_2 is fraction of He that is singly ionized */

    y = 1.0 - fhydrogen;
    r = y / 4.0 / (1.0 - y);
    g0 = jnu21 * 1.26e-11 / (alphaj + 3.0);
    g1 = jnu21 * 1.52e-11 * pow(3.29/5.95, alphaj)/(alphaj+3);
    g2 = jnu21 * 2.65e-12 * pow(3.29/13.2, alphaj)/(alphaj+3);

    xion(temp, &x, &x_1, &x_2);
    return x;
}

double calc_meanmwt(temp, density)
     double temp;
     double density;
{
    double x, x_1, x_2;
    double y;

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
    g0 = jnu21 * 1.26e-11 / (alphaj + 3.0);
    g1 = jnu21 * 1.52e-11 * pow(3.29/5.95, alphaj)/(alphaj+3);
    g2 = jnu21 * 2.65e-12 * pow(3.29/13.2, alphaj)/(alphaj+3);

    xion(temp, &x, &x_1, &x_2);
    return (1.0 - y/4.0)/(2.0 - x) + y/(3.0 - x_1 - x_2);
}
