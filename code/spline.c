int circe(x,t,n) /* returns index i of first element of t >= x */
double x ;
float *t ;
int n ;
{
	register int lo, hi, mid ;
	int dir;
	float tm ;
	lo = 0 ;
	hi = n-1 ;
	if ( x >= *t && x <= *(t+n-1))  {
		while ( hi - lo > 1 ) {
			mid = ( lo + hi )/2. ;
			tm = *(t + mid) ;
			if ( x < tm ) hi = mid ;
			else  lo = mid ;
			}
		return hi ;
	}
	if(x < *t) return 0 ;
	if(x > *(t+n-1)) return n-1 ;
}
tridi(a,b,c,f,x,n)  /* solves tridiagonal linear system .   
	diag elts mii=ai, subdiag mii-1=bi, superdiag mii+1=ci.
	it is understood that b0 and cn-1 are zero, but are not referenced.
	f is rhs, x soln, may be same array; all arrays are trashed */

float *a, *b, *c, *f, *x;
int n ;
{
	register int i;
	for ( i=1; i < n; i++) {
		b[i] /= a[i-1];
		a[i] -= b[i] * c[i-1];
	}
	for( i=1 ; i < n ; i++){ 
		f[i] -= b[i] * f[i-1];
	}
	x[n-1] = f[n-1]/a[n-1] ;
	for( i=n-2; i > -1; i-- ){
		x[i] = (f[i] - (c[i] * x[i+1])) / a[i];
	}
}
splinit(x,y,k,n,q2b,q2e)  /* sets up spline derivative array k for a
	given x and y array of length n POINTS, n-1 intervals, for
	given estimates for the second derivatives at the endpoints,
	q2b and q2e; "natural" boundary conditions for q2b=q2e=0 */

float *x, *y, *k ;
double q2b, q2e ;
int n ;
{
	float hio, hip, dio, dip ;
	float *a, *b, *c, *f;
	int i, ip ;
	char *calloc();   
	a = (float*)calloc(4*n, sizeof(float)) ; 
	b = a + n ;
	c = b + n ;
	f = c + n ;
	hio = 0. ;
	dio = 0. ;
	for( i=0 ; i < n ; i++ ) {
		hip = ((ip = i+1) < n ? *(x + ip) - *(x + i) : 0. ) ;
		dip = ( ip < n ? (*(y + ip) - *(y + i))/hip : 0. ) ;
		*(b+i) = ( ip < n ? hip : hio ) ;
		*(a+i) = 2.*( hip + hio ) ;
		*(c+i) = ( i > 0 ? hio : hip ) ;
		*(f+i) = 3.*(hip*dio + hio*dip ) ;
		if( i == 0 ) *f = 3.* hip * dip  - hip * hip * q2b * 0.5 ;
		else if ( i == n-1 )
			*(f+n-1) = 3.* hio* dio + hio* hio* q2e* 0.5  ;
		dio = dip ;
		hio = hip ;
		}
	tridi(a,b,c,f,k,n) ;
	free(a) ;
}
double spf(dx,x,y,k,m,key) /* general spline evaluator; xyk are the x,y, and 
	derivative arrays, dx the argument = (x-xi-1), m the index of
	the next GREATER abscissa. If key!=0, routine evaluates derivative. */

double dx ;
float *x, *y, *k ;
int m, key ;
{
	float h, t, d, tm, a, b ;
	double val ;
	h = *(x+m) - *(x+m-1) ;
	d = (*(y+m) - *(y+m-1))/h ;
	t = dx/h ;
	tm = 1.-t ;
	a = (*(k+m-1) - d) * tm ;
	b = (*(k+m) -d) *t ;
	if(!key){
		 val = t * *(y+m) + tm * *(y+m-1) + h*t*tm*(a-b) ;
	}
	else{
	         val = d + a*(1.-3.*t) - b*(2.-3.*t) ;
        }
	return  val ;
}
double spft(z,x,y,k,n,key) /* spline evaluator for general monotonic
	(increasing) abscissa tables  */
float *x, *y, *k ;
int key, n;
double z ;
{
	float dx ;
	int m ;
	m = circe( z, x, n) ;
	dx = z - *(x+m-1) ;
	return spf(dx,x,y,k,m,key) ;
}
