/* $Header$
 * $Log$
 * Revision 1.14  2000/01/12 22:55:09  nsk
 * Fixed bugs in cooling routines, added cooling damping,
 * fixed bugs in starformation,  regularized dependencies.
 *
 * Revision 1.13  1999/08/25  22:05:27  nsk
 * added center to boxstat, checks for periodic in smooth, prints out
 * cooling stuff, vista makes plots
 *
 * Revision 1.12  1998/11/26  01:06:20  trq
 * Updated function declarations.
 *
 * Revision 1.11  1998/06/11  20:28:54  trq
 * Recover from read errors more gracefully.
 *
 * Revision 1.10  1997/12/06  22:00:55  trq
 * Updated xray stuff to use spline emmission files.
 *
 * Revision 1.9  1997/09/25  18:35:12  nsk
 * added uv command and discrete uv sources
 *
 * Revision 1.8  1997/01/17  21:22:58  trq
 * Added optional argument to resetcolor command.
 *
 * Revision 1.7  1996/12/20  01:25:10  trq
 * Added catbox command.
 *
 * Revision 1.6  1996/08/19  20:56:01  trq
 * main.c, plot_all.c: allow use of pseudocolor visuals with depth >= 8.
 * nsktrq.c: include fdefs.h.
 *
 * Revision 1.5  1996/06/29  00:17:02  trq
 * Changed loadbinary to load subboxes.
 *
 * Revision 1.4  1996/04/11  21:27:53  trq
 * view_star.c: fixed bug in abox vs. density calculation.
 * activatebox.c: hsmdivv is independent of abox.
 * divv.c, smooth.c, smooth.h, view_gas.c: Use tree to calculate hsmdivv[].
 *
 * Revision 1.3  1995/12/11  20:00:44  nsk
 * added helium, dark absorb,  integral for elcetronic heating,
 * and read in 6 numbers for background
 *
 * Revision 1.2  1995/06/06  17:47:53  trq
 * dump_pixmap.c: Cleaned up declarations.
 *
 * Added kd.c and smooth.c for variable smoothing; NOW WITH LOSER TREES.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:21  trq
 * Import to CVS
 *
 * Revision 2.4  1994/10/12  21:25:54  trq
 * Added prototype for wait_on_X.
 *
 * Revision 2.3  1994/09/20  21:58:16  trq
 * Added absorb()
 *
 * Revision 2.2  1994/04/20  09:06:53  trq
 * Added several prototypes
 *
 * Revision 2.0  93/10/19  13:10:18  trq
 * Initial revision
 * 
 */

/* Make it easier to declare prototypes (puts conditional here) */
#ifndef PROTO
#       if __STDC__
#               define PROTO(type, name, arglist) type name arglist
#       else
#               define PROTO(type, name, arglist) type name ()
#       endif
#endif

/* Function definitions */
PROTO(void, absorb, (char *job));
PROTO(void, add_const_mult_vec, (Real *a,double constant, Real *b));
PROTO(void, all_color, ());
PROTO(void, arguments, (char *job));
PROTO(void, array_color, ());
PROTO(int, boxlist_alloc, (int box, int ngas, int ndark, int nstar));
PROTO(void, boxlist_realloc, (int box));
PROTO(void, box_cumulate, (int box));
PROTO(void, calc_dudt, ());
PROTO(void, calc_hneutral, (double temp, double density, double *hneutral_p,
			     double *heneutral_p, double *heII_p));
PROTO(double, calc_meanmwt, (double temp, double density));
PROTO(void, calc_uv, (struct gas_particle *gp));
PROTO(double, calc_xemiss, (double temp, double density, int band));
PROTO(void, canvas_resize_proc, (Widget canvas, XEvent *event, String
				 *params, Cardinal *n_params));
PROTO(void, clear_rot, ());
PROTO(void, colorbar_label, (char *job));
PROTO(void, command_interp, (char *job));
PROTO(void, cool_func, ());
PROTO(void, cross_product, (Real *a, Real *b, Real *c));
PROTO(void, delete_macro_sub, (struct macro_list *macro));
PROTO(void, diff_add_vec, (Real a[MAXDIM], Real b[MAXDIM], Real
			 c[MAXDIM], Real d[MAXDIM]));
PROTO(double, distance, (Real *x1, Real *x2));
PROTO(void, divv, ());
PROTO(void, dkernel_load, ());
PROTO(double, dot_product, (Real *x1,Real *x2));
PROTO(void, draworient, ());
PROTO(void, draw_label, ());
PROTO(void, draw_color_label,());
PROTO(void, dump_pixmap, (char *name));
PROTO(double, ell_distance, (Real *x1));
PROTO(void, find_shape, (char *particle_type, int box, Real *center));
PROTO(void, find_vel, (char *particle_type, int box, Real *center_vel,
		       Real *center_angular_mom));
PROTO(void, fits, (float **data, int xsize, int ysize,
			double xmin, double ymin, double deltax,
			double deltay, double zmin, double zmax, char
			*outfile));
PROTO(void, fits_xray, (float **data, int xsize, int ysize,
			double xmin, double ymin, double deltax,
			double deltay, double zmin, double zmax, char
			*outfile));
PROTO(void, get_nsktrq, (char **bitP, int *widthP, int *heightP));
PROTO(double, heatcool, (struct gas_particle *gp));
PROTO(void, hneutral_func, ());
PROTO(void, ikernel_load, ());
PROTO(void, input_error, (char *command));
PROTO(void, jacobi, (double (*a)[4], int n, double *d, double (*v)[4],
		     int *nrot));
PROTO(void, load_color_table, ());
PROTO(void, loadall, ());
PROTO(int, loadbinary, (FILE *infile, double time));
PROTO(int, loadbin_box, (FILE *infile,double time, Real *xmin, Real *xmax));
PROTO(void, loadbox, (int box));
PROTO(int, load_redshift, ());
PROTO(void, loadsphere, (int box, Real *center, double radius));
PROTO(int, loadstandard, (FILE *infile, double time));
PROTO(int, loadstd_box, (FILE *infile,double time, Real *xmin, Real *xmax));
PROTO(void, load_cool, ());
PROTO(void, load_uv, ());
PROTO(void, load_eps, ());
PROTO(void, load_epsgas, ());
PROTO(void, load_form, () );
PROTO(void, load_visc, ());
PROTO(void, lycool, (struct gas_particle *gp,double cool_vec[COOLVECSIZE]));

     
PROTO(void, magnitude_color, ());
PROTO(void, make_ell_matrix, ());
PROTO(void, mass_add_vec, (Real *a, Real *b, double mass_b, Real *c,
			   double mass_c));
PROTO(void, matrix_matrix_mult, (double a[MAXDIM][MAXDIM],
				 double b[MAXDIM][MAXDIM],
				 double c[MAXDIM][MAXDIM])) ;
PROTO(void, matrix_vector_mult, (double mat[MAXDIM][MAXDIM],
				 Real a[MAXDIM],
				 Real b[MAXDIM])) ;
PROTO(void, maxpos, (Real *xmax, Real *xmin));
PROTO(void, meanmwt_func, ());
PROTO(void, old2binary, (FILE *infile, FILE *outfile));
PROTO(double, perp_distance, (Real *x1,Real *x2,Real *x3));
PROTO(void, plane, (Real x1[MAXDIM], Real x2[MAXDIM], Real x3[MAXDIM],
		    Real x4[MAXDIM], double constant[5])) ;
PROTO(void, plot_all, (char *job));
PROTO(void, plot_sub, (char *job));
PROTO(void, pot_center, (Real *center, int box));
PROTO(void, potential_color, ());
PROTO(void, project, ());
PROTO(void, radial_color, ());
PROTO(void, readarray, (char *job));
PROTO(void, readoldbin, (char *job));
PROTO(void, reset_color, (char *job));
PROTO(void, reset_zoom_scroll, ());
PROTO(void, rotate, (int direction,double angle));
PROTO(void, setival, (int *a, int b));
PROTO(void, setsize, (int box, Real *xmax, Real *xmin));
PROTO(void, setvec, (Real *a, Real *b));
PROTO(void, sift, (double constant[5], int box));
PROTO(int, splinit, (float *x,float *y,float *k,int n,double q2b,double q2e));
PROTO(void, starform_func, ());
PROTO(void, star_history, (char *job));
PROTO(double, star_lum, (double mass, double time));
PROTO(void, sub_vec, (Real *a,Real *b,Real *c));
PROTO(void, transpose, (double mat[MAXDIM][MAXDIM], double
		      trans_mat[MAXDIM][MAXDIM]));
PROTO(void, unload_all, ());
PROTO(void, vec_add_const_mult_vec, (Real *a, Real *b, double
				     constant, Real *c));
PROTO(void, view, (char *job));
PROTO(void, view_array, (char *job));
PROTO(void, view_mag, (char *job));
PROTO(void, view_pot, (char *job)) ;
PROTO(void, view_rad, (char *job)) ;
PROTO(void, wait_on_X, ());
PROTO(void, window, (char *job));
PROTO(void, delete_window, (char *job));
PROTO(void, xplot_array, (char *job));
PROTO(int, xray_lum_load, ());
PROTO(void, yplot_array, (char *job));
PROTO(void, zplot_array, (char *job));
     
