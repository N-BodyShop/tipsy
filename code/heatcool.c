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
    g0 = gp0_H ;
    g1 = gp0_He ;
    g2 = gp0_Hep ;

    h0 = eps_H ;
    h1 = eps_He ;
    h2 = eps_Hep ;

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

void
calc_hneutral(temp, density, hneutral_p, heneutral_p, heII_p)
     double temp;
     double density;
     double *hneutral_p;
     double *heneutral_p;
     double *heII_p;
{
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
    g0 = gp0_H ;
    g1 = gp0_He ;
    g2 = gp0_Hep ;

    xion(temp, hneutral_p, heneutral_p, heII_p);

    return ;
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
    g0 = gp0_H ;
    g1 = gp0_He ;
    g2 = gp0_Hep ;

    xion(temp, &x, &x_1, &x_2);
    return (1.0 - y/4.0)/(2.0 - x) + y/(3.0 - x_1 - x_2);
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
    double midpnt() ;
    double qromo() ;

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
    gint = qromo(fac1,0.,1.,midpnt) ;
    gp0_H = a0*gint/planck ;
    gp0_Hep = gp0_H*pow((e0_H/e0_Hep),alphaj)/4. ;
    eint = qromo(fac2,0.,1.,midpnt) ;
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
#define EPS 1.0e-6
#define JMAX 14
#define JMAXP (JMAX+1)
#define K 5

double qromo(func,a,b,choose)
        double (*func)() ;
	double a,b ;
        double (*choose)() ;
{
	void polint();
	void nrerror();
	int j;
	double ss,dss,h[JMAXP+1],s[JMAXP+1];

	h[1]=1.0;
	for (j=1;j<=JMAX;j++) {
		s[j]=(*choose)(func,a,b,j);
		if (j >= K) {
			polint(&h[j-K],&s[j-K],K,0.0,&ss,&dss);
			if (fabs(dss) < EPS*fabs(ss)) return ss;
		}
		s[j+1]=s[j];
		h[j+1]=h[j]/9.0;
	}
	nrerror("Too many steps in routing qromo");
	return 0.0;
}
#undef EPS
#undef JMAX
#undef JMAXP
#undef K
#define FUNC(x) ((*func)(x))

double midpnt(func,a,b,n)
        double (*func)() ;
	double a,b ;
	int n ;
{
	double x,tnm,sum,del,ddel;
	static float s;
	static int it;
	int j;

	if (n == 1) {
		it=1;
		return (s=(b-a)*FUNC(0.5*(a+b)));
	} else {
		tnm=it;
		del=(b-a)/(3.0*tnm);
		ddel=del+del;
		x=a+0.5*del;
		sum=0.0;
		for (j=1;j<=it;j++) {
			sum += FUNC(x);
			x += ddel;
			sum += FUNC(x);
			x += del;
		}
		it *= 3;
		s=(s+(b-a)*sum/tnm)/3.0;
		return s;
	}
}
#undef FUNC

#include <math.h>
#define NRANSI

double *vector_nr();
double **matrix();
double **convert_matrix();
double *dvector_nr();
double **dmatrix();
int *ivector_nr();
int **imatrix();
double **submatrix();
void free_vector_nr();
void free_dvector_nr();
void free_ivector_nr();
void free_matrix();
void free_dmatrix();
void free_imatrix();
void free_submatrix();
void free_convert_matrix();
void nrerror();

void polint(xa,ya,n,x,y,dy)
        double xa[],ya[],x,*y,*dy ;
	int n ;
{
	int i,m,ns=1;
	double den,dif,dift,ho,hp,w;
	double *c,*d;

	dif=fabs(x-xa[1]);
	c=vector_nr(1,n);
	d=vector_nr(1,n);
	for (i=1;i<=n;i++) {
		if ( (dift=fabs(x-xa[i])) < dif) {
			ns=i;
			dif=dift;
		}
		c[i]=ya[i];
		d[i]=ya[i];
	}
	*y=ya[ns--];
	for (m=1;m<n;m++) {
		for (i=1;i<=n-m;i++) {
			ho=xa[i]-x;
			hp=xa[i+m]-x;
			w=c[i+1]-d[i];
			if ( (den=ho-hp) == 0.0) nrerror("Error in routine polint");
			den=w/den;
			d[i]=hp*den;
			c[i]=ho*den;
		}
		*y += (*dy=(2*ns < (n-m) ? c[ns+1] : d[ns--]));
	}
	free_vector_nr(d,1,n);
	free_vector_nr(c,1,n);
}
#undef NRANSI

#include <malloc.h>
#include <stdio.h>

void nrerror(error_text)
char error_text[];
{
	void exit();

	fprintf(stderr,"Numerical Recipes run-time error...\n");
	fprintf(stderr,"%s\n",error_text);
	fprintf(stderr,"...now exiting to system...\n");
	exit(1);
}



double *vector_nr(nl,nh)
int nl,nh;
{
	double *v;

	v=(double *)malloc((unsigned) (nh-nl+1)*sizeof(double));
	if (!v) nrerror("allocation failure in vector_nr()");
	return v-nl;
}

int *ivector_nr(nl,nh)
int nl,nh;
{
	int *v;

	v=(int *)malloc((unsigned) (nh-nl+1)*sizeof(int));
	if (!v) nrerror("allocation failure in ivector_nr()");
	return v-nl;
}

double *dvector_nr(nl,nh)
int nl,nh;
{
	double *v;

	v=(double *)malloc((unsigned) (nh-nl+1)*sizeof(double));
	if (!v) nrerror("allocation failure in dvector_nr()");
	return v-nl;
}



double **matrix(nrl,nrh,ncl,nch)
int nrl,nrh,ncl,nch;
{
	int i;
	double **m;

	m=(double **) malloc((unsigned) (nrh-nrl+1)*sizeof(double*));
	if (!m) nrerror("allocation failure 1 in matrix()");
	m -= nrl;

	for(i=nrl;i<=nrh;i++) {
		m[i]=(double *) malloc((unsigned) (nch-ncl+1)*sizeof(double));
		if (!m[i]) nrerror("allocation failure 2 in matrix()");
		m[i] -= ncl;
	}
	return m;
}

double **dmatrix(nrl,nrh,ncl,nch)
int nrl,nrh,ncl,nch;
{
	int i;
	double **m;

	m=(double **) malloc((unsigned) (nrh-nrl+1)*sizeof(double*));
	if (!m) nrerror("allocation failure 1 in dmatrix()");
	m -= nrl;

	for(i=nrl;i<=nrh;i++) {
		m[i]=(double *) malloc((unsigned) (nch-ncl+1)*sizeof(double));
		if (!m[i]) nrerror("allocation failure 2 in dmatrix()");
		m[i] -= ncl;
	}
	return m;
}

int **imatrix(nrl,nrh,ncl,nch)
int nrl,nrh,ncl,nch;
{
	int i,**m;

	m=(int **)malloc((unsigned) (nrh-nrl+1)*sizeof(int*));
	if (!m) nrerror("allocation failure 1 in imatrix()");
	m -= nrl;

	for(i=nrl;i<=nrh;i++) {
		m[i]=(int *)malloc((unsigned) (nch-ncl+1)*sizeof(int));
		if (!m[i]) nrerror("allocation failure 2 in imatrix()");
		m[i] -= ncl;
	}
	return m;
}



double **submatrix(a,oldrl,oldrh,oldcl,oldch,newrl,newcl)
double **a;
int oldrl,oldrh,oldcl,oldch,newrl,newcl;
{
	int i,j;
	double **m;

	m=(double **) malloc((unsigned) (oldrh-oldrl+1)*sizeof(double*));
	if (!m) nrerror("allocation failure in submatrix()");
	m -= newrl;

	for(i=oldrl,j=newrl;i<=oldrh;i++,j++) m[j]=a[i]+oldcl-newcl;

	return m;
}



void free_vector_nr(v,nl,nh)
double *v;
int nl,nh;
{
	free((char*) (v+nl));
}

void free_ivector_nr(v,nl,nh)
int *v,nl,nh;
{
	free((char*) (v+nl));
}

void free_dvector_nr(v,nl,nh)
double *v;
int nl,nh;
{
	free((char*) (v+nl));
}



void free_matrix(m,nrl,nrh,ncl,nch)
double **m;
int nrl,nrh,ncl,nch;
{
	int i;

	for(i=nrh;i>=nrl;i--) free((char*) (m[i]+ncl));
	free((char*) (m+nrl));
}

void free_dmatrix(m,nrl,nrh,ncl,nch)
double **m;
int nrl,nrh,ncl,nch;
{
	int i;

	for(i=nrh;i>=nrl;i--) free((char*) (m[i]+ncl));
	free((char*) (m+nrl));
}

void free_imatrix(m,nrl,nrh,ncl,nch)
int **m;
int nrl,nrh,ncl,nch;
{
	int i;

	for(i=nrh;i>=nrl;i--) free((char*) (m[i]+ncl));
	free((char*) (m+nrl));
}



void free_submatrix(b,nrl,nrh,ncl,nch)
double **b;
int nrl,nrh,ncl,nch;
{
	free((char*) (b+nrl));
}



double **convert_matrix(a,nrl,nrh,ncl,nch)
double *a;
int nrl,nrh,ncl,nch;
{
	int i,j,nrow,ncol;
	double **m;

	nrow=nrh-nrl+1;
	ncol=nch-ncl+1;
	m = (double **) malloc((unsigned) (nrow)*sizeof(double*));
	if (!m) nrerror("allocation failure in convert_matrix()");
	m -= nrl;
	for(i=0,j=nrl;i<=nrow-1;i++,j++) m[j]=a+ncol*i-ncl;
	return m;
}



void free_convert_matrix(b,nrl,nrh,ncl,nch)
double **b;
int nrl,nrh,ncl,nch;
{
	free((char*) (b+nrl));
}
