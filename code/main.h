/*
 * $Header$ 
 * $Log$
 * Revision 1.7  1996/04/24 23:40:36  trq
 * balls_loaded variable to keep track of balls.
 * Fixed smoothing length bug.
 *
 * Revision 1.6  1995/12/11  20:00:47  nsk
 * added helium, dark absorb,  integral for elcetronic heating,
 * and read in 6 numbers for background
 *
 * Revision 1.5  1995/06/06  17:47:57  trq
 * dump_pixmap.c: Cleaned up declarations.
 *
 * Added kd.c and smooth.c for variable smoothing; NOW WITH LOSER TREES.
 *
 * Revision 1.4  1995/03/30  00:14:44  trq
 * Added "ruler" command.
 *
 * Revision 1.3  1995/03/24  18:49:09  trq
 * Added "revrain" colormap.
 *
 * divv.c: included malloc.h.
 *
 * Revision 1.2  1995/03/02  17:30:25  nsk
 * changed absorption cross section tb be done by integral
 * added optical depth output and fixed bug in absorb
 * added stellar mass plot to view_star
 *
 * Revision 1.1.1.1  1995/01/10  22:57:21  trq
 * Import to CVS
 *
 * Revision 2.12  1994/10/07  16:55:15  trq
 * Corrected use of XtGetValues().
 *
 * Revision 2.11  1994/09/20  22:12:21  nsk
 * added omega, periodic and period
 *
 * Revision 2.10  1994/09/12  20:47:37  nsk
 * added hubble_constant and comove
 *
 * Revision 2.9  1994/04/20  08:50:04  trq
 * ksr defines colors differently from other X libraries.
 *
 * Revision 2.8  94/03/08  16:34:35  trq
 * Neal: added vector plot.
 * 
 * Revision 2.6  94/02/18  10:08:11  trq
 * *** empty log message ***
 * 
 */
char title[MAXCOMM] ;
Window currentview_xid ;
Window colorbar_xid ;
Display *baseframe_dpy ;
Colormap default_cmap;
#ifdef __ksr__
unsigned int colors [CMAPSIZE];
#else
unsigned long colors [CMAPSIZE];
#endif
XColor clist[CMAPSIZE];
GC gc_color[CMAPSIZE] ;
int *gas_x = NULL;
int *gas_y = NULL;
int *dark_x = NULL;
int *dark_y = NULL;
int *star_x = NULL;
int *star_y = NULL;
int *gasvel_x = NULL;
int *gasvel_y = NULL;
int *darkvel_x = NULL;
int *darkvel_y = NULL;
int *starvel_x = NULL;
int *starvel_y = NULL;
struct xypoint plotbox[MAXBOX + 1][BOXPTS + 8] ;
struct xypoint plotaxes[6] ;
int dv1_x, dv1_y ;
int dv2_x, dv2_y ;
Dimension can_height, can_width ;
int hard_height, hard_width ;
Dimension bar_height, bar_width ;
int can_size ;
int ruler_flag = 0;
int ruler_x ;
int ruler_y ;
int make_box_flag = 0 ;
int make_box_x1 ;
int make_box_y1 ;
int make_box_x2 ;
int make_box_y2 ;

struct gas_particle *gas_particles = NULL;
struct dark_particle *dark_particles = NULL;
struct star_particle *star_particles = NULL;
struct dump header  = { 0.0, 0, 0 , 0 , 0, 0};
struct box boxes[MAXBOX+1] ;    
struct list boxlist[MAXBOX+1] ;
struct macro_list *macros = NULL;

SMX box0_smx = NULL;
Real ball_size = 0.0;
int ball_size_loaded = 0;
int balls_loaded = 0;
int n_smooth = 32;

int showgas = ON ;
int showdark = ON ;
int showstar = ON ;
int showboxes = OFF ;
int showaxes = OFF ;
int showvel = OFF ;
int showvec = OFF ;

int boxes_loaded[MAXBOX+1] ;
int mark_box[MAXBOX+1] ;
short *mark_gas = NULL;
short *mark_dark = NULL;
short *mark_star = NULL;
Real *array = NULL;
int array_size = 0;
struct vec *vector = NULL;
int vector_size = 0;
int active_box = 0 ;
int asciiopen = CLOSED ;
int binaryopen = CLOSED ;
int binary_loaded = UNLOADED ;
int current_project = NO;
int current_color = NO;
int clip_flag = NOCLIP ;
int plotted_box = -1 ;
int gas_plot = NO ;
int star_plot = NO ;
int radial_plot = NO ;
int potential_plot = NO ;
int magnitude_plot = NO ;
int array_plot  = NO ;
int cool_loaded = NO ;
int visc_loaded = NO ;
int form_loaded = NO ;
int acc_loaded = NO ;
int ikernel_loaded = NO ;
int dkernel_loaded = NO ;
int xray_loaded = NO ;
int divv_loaded = NO ;
int cooling_loaded = NO ;
int hneutral_loaded = NO ;
int meanmwt_loaded = NO ;
int starform_loaded = NO ;
int redshift_loaded = NO ;
int comove = NO ;
int eps_loaded = NO ;
int epsgas_loaded = NO ;
int lum_loaded = NO ;
int color_type = ALL ;
int vector_plot_type = VELOCITY ;
int log_type = LIN ;
int file_type = SPH ;

double rot_matrix[MAXDIM][MAXDIM] = {
    {1.0,0.0,0.0},
    {0.0,1.0,0.0},
    {0.0,0.0,1.0}
} ;
double ell_matrix[MAXDIM][MAXDIM] ;
double ell_matrix_inv[MAXDIM][MAXDIM] ;
double axes_coord[6][MAXDIM] = {
    {0.,0.,0.},
    {1.,0.,0.},
    {0.,0.,0.},
    {0.,1.,0.},
    {0.,0.,0.},
    {0.,0.,1.}
} ;

double point_size_gas = 0.0 ;
double point_size_dark = 0.0 ;
double point_size_star = 0.0 ;
double point_size_mark  = 0.0 ;
double pixel_size ;
double vel_scale = .05 ;
double scaling ;
double color_slope ;
double color_offset ;
double window_pixels = 800.0 ;
double alpha ;
double alphaj ;
double beta ;
double cstar ;
double dtime ;
double temppar ;
double softpar ;
double redshift ;
double omega ;
int periodic = NO ;
double period_size ;
double cosmof ;
double cosmof3 ;
double hubble_constant ;
double comptc ;
double eps_grav ;
double epsgas_grav ;
double kpcunit ;
double msolunit;
double fhydrogen ;
double jnu21 ;
double alphaj ;
double time_unit ;
double lum_smooth ;
double xray_min_temp ;
int color_filter = VBAND ;
int view_size ;
double zoom_factor;
int *particle_color = NULL;
double phi,psi,theta ;
double ba,ca ;
Real center_ell[MAXDIM] ;
Real center_fit[MAXDIM] ;
double fit_r1,fit_r2 ;
double fit_radius ;
double deldrg ;
double *acsmooth = NULL;
double deldr2 ;
double deldr2i ;
double *iwsmooth = NULL;
double *dwsmooth = NULL;
double *hsmdivv = NULL;
double *cooling = NULL;
double *hneutral = NULL;
double *heneutral = NULL;
double *heII = NULL;
double *meanmwt = NULL;
double *starform = NULL;
double *xray_lum = NULL;
int number_bands ;
double deldtempi ;
int number_xray_data ;
double gp0_H ;
double gp0_He ;
double gp0_Hep ;
double eps_H ;
double eps_He ;
double eps_Hep ;

unsigned char wrbb_red[256] ;
unsigned char wrbb_green[256] ;
unsigned char wrbb_blue[256] ;
unsigned char revrain_red[256] ;
unsigned char revrain_green[256] ;
unsigned char revrain_blue[256] ;
unsigned char rainbow_red[256] ;
unsigned char rainbow_green[256] ;
unsigned char rainbow_blue[256] ;
unsigned char colormap_red[256] ;
unsigned char colormap_green[256] ;
unsigned char colormap_blue[256] ;
unsigned char *arraymap_red;
unsigned char *arraymap_green;
unsigned char *arraymap_blue;
int base_color ;
int display = YES ;
int mono = NO ;

struct file hardfile ;
struct file binaryfile ;
struct file bodfile ;
