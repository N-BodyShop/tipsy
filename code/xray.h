/*
 * External variables for X-ray calculation.
 */
extern struct xray_table
{
    double *yspl;		/* y values for splines */
    double *y2spl;		/* y2 values for splines */
    } *xray_lums;		/* table of spline coefficients for
				   each X-ray band */
extern double *temp_spline;	/* Array of temperatures for the above
				 splines. */
extern int number_bands;	/* Number of X-Ray bands */
extern int xray_spline_n;	/* Number of spline points */
