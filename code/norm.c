#define NPOINTSGAUSS 100
#define EPS 3.0e-11
#include "defs.h"

double norm()
{
    double quad3d() ;
    double jaffe_norm() ;

    return 8.*quad3d(jaffe_norm,center_fit[0],fit_radius+center_fit[0]) ;
}
static double xsav,ysav;
static double (*nrfunc)();

double quad3d(func,x1,x2)
double x1,x2,(*func)();
{
	double qgaus(),f1();

	nrfunc=func;
	return qgaus(f1,x1,x2);
}
double f1(x)
double x;
{
	double qgaus(),f2();
	double yy1(),yy2();

	xsav=x;
	return qgaus(f2,yy1(x),yy2(x));
}
double f2(y)
double y;
{
	double qgaus(),f3();
	double z1(),z2();

	ysav=y;
	return qgaus(f3,z1(xsav,y),z2(xsav,y));
}
double f3(z)
double z;
{
	return (*nrfunc)(xsav,ysav,z);
}
double qgaus(func,a,b)
double a,b;
double (*func)();
{
	int j;
	double xr,xm,dx,s;
	static double x[NPOINTSGAUSS+1] ;
	static double w[NPOINTSGAUSS+1] ;
	static int firstcall = 1 ;

	if(firstcall){
	    firstcall = 0 ;
	    gauleg(-1.,1.,x,w,NPOINTSGAUSS) ;
	}
	xm=0.5*(b+a);
	xr=0.5*(b-a);
	s=0;
	for (j=1;j<=NPOINTSGAUSS/2;j++) {
		dx=xr*x[j];
		s += w[j]*((*func)(xm+dx)+(*func)(xm-dx));
	}
	return s *= xr;
}
double jaffe_norm(x,y,z)
    double x,y,z ;
{
    double rho ;
    double zeta ;
    double zeta2 ;
    Real pos[MAXDIM] ;
    double ell_distance() ;

    pos[0] = x  ;
    pos[1] = y  ;
    pos[2] = z  ;
    zeta = ell_distance(pos) ;
    zeta2 = zeta*zeta ;
    rho = 1./(zeta2+fit_r1*fit_r1)/(zeta+fit_r2)/(zeta+fit_r2) ;
    return rho ;
}
double yy1(x)
    double x ;
{
    return center_fit[1] ;
}
double yy2(x)
    double x ;
{
    return sqrt((fit_radius*fit_radius-(x-center_fit[0])*
	    (x-center_fit[0]))) + center_fit[1] ;
}
double z1(x,y)
    double x ;
    double y ;
{
    return center_fit[2] ;
}
double z2(x,y)
    double x ;
    double y ;
{
    return sqrt((fit_radius*fit_radius-(x-center_fit[0])*(x-center_fit[0])-
	    (y-center_fit[1])*(y-center_fit[1]))) + center_fit[2] ;
}
gauleg(x1,x2,x,w,n)
double x1,x2,x[],w[];
int n;
{
	int m,j,i;
	double z1,z,xm,xl,pp,p3,p2,p1;

	m=(n+1)/2;
	xm=0.5*(x2+x1);
	xl=0.5*(x2-x1);
	for (i=1;i<=m;i++)  {
		z=cos(3.141592654*(i-0.25)/(n+0.5));
		do {
			p1=1.0;
			p2=0.0;
			for (j=1;j<=n;j++) {
				p3=p2;
				p2=p1;
				p1=((2.0*j-1.0)*z*p2-(j-1.0)*p3)/j;
			}
			pp=n*(z*p1-p2)/(z*z-1.0);
			z1=z;
			z=z1-p1/pp;
		} while (fabs(z-z1) > EPS);
		x[i]=xm-xl*z;
		x[n+1-i]=xm+xl*z;
		w[i]=2.0*xl/((1.0-z*z)*pp*pp);
		w[n+1-i]=w[i];
	}
}
