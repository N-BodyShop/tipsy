/* $Header$
 * $Log$
 * Revision 1.15  1997/12/18 17:57:11  trq
 * Added VALL vista plot: total mass density.
 *
 * Revision 1.14  1997/12/06  22:00:54  trq
 * Updated xray stuff to use spline emmission files.
 *
 * Revision 1.13  1997/09/25  18:35:12  nsk
 * added uv command and discrete uv sources
 *
 * Revision 1.12  1997/09/19  13:41:27  nsk
 * fixed memeory troubles.
 *
 * Revision 1.11  1997/09/05  01:02:21  nsk
 * streamlined vista (I hope it works), added neutralize command, added SZ
 * effect to vista.
 *
 * Revision 1.10  1997/02/20  02:36:05  trq
 * Don't redefine PI.
 *
 * Revision 1.9  1996/12/19  18:03:24  trq
 * Fixed handling of arrays and vectors when a sub-box is loaded.
 *
 * Revision 1.8  1996/06/26  00:34:48  nsk
 * Added deuterium to absorb, HeI, HeII, and xray to vista
 *
 * Revision 1.7  1996/04/24  23:40:33  trq
 * balls_loaded variable to keep track of balls.
 * Fixed smoothing length bug.
 *
 * Revision 1.6  1995/12/11  20:00:43  nsk
 * added helium, dark absorb,  integral for elcetronic heating,
 * and read in 6 numbers for background
 *
 * Revision 1.5  1995/06/06  17:47:51  trq
 * dump_pixmap.c: Cleaned up declarations.
 *
 * Added kd.c and smooth.c for variable smoothing; NOW WITH LOSER TREES.
 *
 * Revision 1.4  1995/03/30  00:14:41  trq
 * Added "ruler" command.
 *
 * Revision 1.3  1995/03/24  18:49:06  trq
 * Added "revrain" colormap.
 *
 * divv.c: included malloc.h.
 *
 * Revision 1.2  1995/03/02  17:30:23  nsk
 * changed absorption cross section tb be done by integral
 * added optical depth output and fixed bug in absorb
 * added stellar mass plot to view_star
 *
 * Revision 1.1.1.1  1995/01/10  22:57:21  trq
 * Import to CVS
 *
 * Revision 2.17  1994/11/01  19:19:21  nsk
 * removed vistasize and xraysize
 *
 * Revision 2.16  1994/10/07  16:55:15  trq
 * Corrected use of XtGetValues().
 *
 * Revision 2.15  1994/09/20  22:11:39  nsk
 * added omega, periodic and period
 *
 * Revision 2.14  1994/09/12  20:46:59  nsk
 * added hubble_constant and comove
 *
 * Revision 2.13  1994/07/13  12:59:24  trq
 * Changed INTMAX to 2^29-1
 *
 * Revision 2.12  94/05/05  11:38:11  trq
 * Increased INTMAX to 2^30-1.
 * 
 * Revision 2.11  94/04/20  08:57:31  trq
 * ksr has unique definition of colors.
 * 
 * Revision 2.10  94/03/08  16:34:02  trq
 * Neal: added vector plot.
 * 
 * Revision 2.8  94/02/18  10:05:29  trq
 * Removed hard_replay and four_replay
 * 
 * Revision 2.7  94/02/16  10:11:40  trq
 * Changed macro structures.
 * 
 * Revision 2.6  93/10/18  15:42:29  trq
 * Added HNEUT plot type
 * 
 * Revision 2.5  1992/11/21  13:04:16  trq
 * New variables and defines for cooling, hneutral and meanmwt.
 *
 * Revision 2.4  1992/10/22  13:09:07  trq
 * Added xray variables and moved function declarations to fdefs.h
 *
 * Revision 2.3  1991/10/24  13:20:05  trq
 * Weight reduced.
 *
 * Revision 2.2  91/06/02  12:17:47  trq
 * Added struct comm;
 * 
 * Revision 2.1  1991/04/11  17:02:45  trq
 * deleted some redundant variables and made *smooth dynamically allocated.
 *
 * Revision 2.0  1991/01/31  18:09:26  trq
 * Athena Widget version
 *
 */
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <stdio.h>
#include <math.h>

#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

#include <fcntl.h>

#define INTMAX 536870911
#define CMAPSIZE 128
#define MAXCOMM 150
#define MAXDIM 3
#define ON 1
#define OFF 0
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4
#define CLOCKWISE 5
#define LOADED 1
#define UNLOADED 0
#define MAXBOX 100
#define BOXPTS 8
#define OPEN 1
#define CLOSED 0
#define SET 1
#define UNSET 0
#define TRUE 1
#define FALSE 0
#define NO 0
#define YES 1
#ifndef PI
#define PI 3.141592653589793
#endif
#define LN10 2.302585093
#define ZOOMSCALE 2
#define BARHEIGHT 50
#define LABELHEIGHT 20.0
#define BARLABELHEIGHT 10.0
#define MAXBIN 200
#define GAS 1
#define DARK 2
#define STAR 3
#define ALL 4
#define LIN 1
#define LOG 2
#define VELOCITY 1
#define VECTOR 2
#define NINTERP 20000
/* vista types */
#define RHO 1
#define TEMP 2
#define PRESS 3
#define TCOOL 4
#define JEANS 5
#define FSTAR 6
#define VSTAR 7
#define LUMSTAR 8
#define VDARK 9
#define TDRHO 10
#define XRAY 11
#define HNEUT 12
#define HEI 13
#define HEII 14
#define SZ 15
#define VALL 16
#define NOSPH 0
#define SPH 1
#define PREC_JACOBI 1.e-8 ;
#define IBAND 0
#define VBAND 1
#define BBAND 2
#define UBAND 3
#define UUBAND 4
#define I0 .45
#define V0 -.02
#define B0 -.22
#define U0 .85
#define UU0 0.
#define NOCLIP 0
#define UPCLIP 1
#define LOCLIP 2
#define BCLIP 3
#define forever for(;;)
#define max(A,B) ((A) > (B) ? (A) : (B))
#define min(A,B) ((A) < (B) ? (A) : (B))

/* G in cgs */
#define GCGS 6.67e-8
/* solar mass in grams */
#define MSOLG 1.99e33
/* kiloparsec in centimeters */
#define KPCCM 3.085678e+21
/* gigayear in seconds */
#define GYRSEC 3.155693e+16
/* mass of hydrogen atom in grams */
#define MHYDR 1.67e-24
/* bolzman constant in cgs */
#define KBOLTZ 1.381e-16
/* gamma of ideal gas */
#define GAMMA (5.0/3.0)
/*     Thomson cross section */
#define SIGMAES  6.665e-25
/*     electron mass */
#define ME  9.11e-28
/*     speed of light */
#define C 2.998e10


extern char title[MAXCOMM] ;
extern Window currentview_xid ;
extern Window colorbar_xid ;
extern Display *baseframe_dpy ;
extern Colormap default_cmap;
extern XColor clist[CMAPSIZE];
#ifdef __ksr__
extern unsigned int colors[CMAPSIZE];
#else
extern unsigned long colors[CMAPSIZE];
#endif
extern GC gc_color[CMAPSIZE] ;
extern int *gas_x;
extern int *gas_y;
extern int *dark_x;
extern int *dark_y;
extern int *star_x;
extern int *star_y;
extern int *gasvel_x;
extern int *gasvel_y;
extern int *darkvel_x;
extern int *darkvel_y;
extern int *starvel_x;
extern int *starvel_y;
struct xypoint {
	int x, y;
};
extern struct xypoint plotbox[MAXBOX + 1][BOXPTS + 8] ;
extern struct xypoint plotaxes[6] ;
extern int dv1_x, dv1_y ;
extern int dv2_x, dv2_y ;
extern Dimension can_height, can_width ;
extern int hard_height, hard_width ;
extern Dimension bar_height, bar_width ;
extern int can_size ;
extern int ruler_flag ;
extern int ruler_x ;
extern int ruler_y ;
extern int make_box_flag ;
extern int make_box_x1 ;
extern int make_box_y1 ;
extern int make_box_x2 ;
extern int make_box_y2 ;

typedef float Real;

struct gas_particle {
    Real mass;
    Real pos[MAXDIM];
    Real vel[MAXDIM];
    Real rho;
    Real temp;
    Real hsmooth;
    Real metals ;
    Real phi ;
} ;
extern struct gas_particle *gas_particles;

struct dark_particle {
    Real mass;
    Real pos[MAXDIM];
    Real vel[MAXDIM];
    Real eps;
    Real phi ;
} ;
extern struct dark_particle *dark_particles;

struct star_particle {
    Real mass;
    Real pos[MAXDIM];
    Real vel[MAXDIM];
    Real metals ;
    Real tform ;
    Real eps;
    Real phi ;
} ;
extern struct star_particle *star_particles;

struct dump {
    double time ;
    int nbodies ;
    int ndim ;
    int nsph ;
    int ndark ;
    int nstar ;
} ;
extern struct dump header ;

struct box {
    Real x1[MAXDIM];
    Real x2[MAXDIM];
    Real x3[MAXDIM];
    Real x4[MAXDIM];
    Real x5[MAXDIM];
    Real x6[MAXDIM];
    double size ;
    Real center[MAXDIM] ;
    double volume ;
    double gas_mass ;
    double dark_mass ;
    double star_mass ;
    double total_mass ;
    Real gas_com[MAXDIM] ;
    Real dark_com[MAXDIM] ;
    Real star_com[MAXDIM] ;
    Real total_com[MAXDIM] ;
    Real gas_com_vel[MAXDIM] ;
    Real dark_com_vel[MAXDIM] ;
    Real star_com_vel[MAXDIM] ;
    Real total_com_vel[MAXDIM] ;
    Real gas_angular_mom[MAXDIM] ;
    Real dark_angular_mom[MAXDIM] ;
    Real star_angular_mom[MAXDIM] ;
    Real total_angular_mom[MAXDIM] ;
} ;
extern struct box boxes[MAXBOX+1] ;    

struct list {
    int ngas ;
    int ndark ;
    int nstar ;
    struct gas_particle **gp;
    struct dark_particle **dp;
    struct star_particle **sp;
    int *gpi;
    int *dpi;
    int *spi;
} ;
extern struct list boxlist[MAXBOX+1] ;
extern int *box0_pi;

struct uv_source {
    Real pos[MAXDIM];
    double gp0_H ;
    double gp0_He ;
    double gp0_Hep ;
    double eps_H ;
    double eps_He ;
    double eps_Hep ;
} ;
extern struct uv_source *uv_sources;

#include "smooth.h"
extern SMX box0_smx;
extern Real ball_size;
extern int ball_size_loaded;
extern int balls_loaded;
extern int n_smooth;

struct command_list {
  char *command;
  struct command_list *next;
} ;

struct macro_list {
    struct command_list *start ;
    char *name ;
    struct macro_list *next;
} ;
extern struct macro_list *macros;
extern int showgas ;
extern int showdark ;
extern int showstar ;
extern int showboxes ;
extern int showaxes ;
extern int showvel ;
extern int showvec ;

extern int boxes_loaded[MAXBOX+1] ;
extern int mark_box[MAXBOX+1] ;
extern short *mark_gas ;
extern short *mark_dark ;
extern short *mark_star ;
extern Real *array;
extern int array_size;
struct vec {
    Real v[MAXDIM];
};
extern struct vec *vector;
extern int vector_size;
extern int active_box ;
extern int asciiopen ;
extern int binaryopen ;
extern int binary_loaded ;
extern int current_project;
extern int current_color;
extern int clip_flag ;
extern int plotted_box ;
extern int gas_plot ;
extern int star_plot ;
extern int radial_plot ;
extern int potential_plot ;
extern int magnitude_plot ;
extern int array_plot ;
extern int cool_loaded ;
extern int visc_loaded ;
extern int form_loaded ;
extern int acc_loaded ;
extern int ikernel_loaded ;
extern int dkernel_loaded ;
extern int xray_loaded ;
extern int divv_loaded ;
extern int cooling_loaded ;
extern int hneutral_loaded ;
extern int meanmwt_loaded ;
extern int lum_loaded ;
extern double max_temp_old ;
extern double min_rho_old ;
extern int starform_loaded ;
extern int redshift_loaded ;
extern int uv_loaded ;
extern int uniform ;
extern int source_num ;
extern int comove ;
extern int eps_loaded ;
extern int epsgas_loaded ;
extern int color_type ;
extern int vector_plot_type ;
extern int log_type ;
extern int file_type ;

extern double rot_matrix[MAXDIM][MAXDIM] ;
extern double ell_matrix[MAXDIM][MAXDIM] ;
extern double ell_matrix_inv[MAXDIM][MAXDIM] ;
extern double axes_coord[6][MAXDIM] ;

extern double point_size_gas ;
extern double point_size_dark ;
extern double point_size_star ;
extern double point_size_mark ;
extern double pixel_size ;
extern double vel_scale ;
extern double scaling ;
extern double color_slope ;
extern double color_offset ;
extern double window_pixels ;
extern double kpcunit ;
extern double msolunit ;
extern double fhydrogen ;
extern double jnu21 ;
extern double alphaj ;
extern double alpha ;
extern double beta ;
extern double cstar ;
extern double dtime ;
extern double temppar ;
extern double softpar ;
extern double redshift ;
extern double cosmof ;
extern double cosmof3 ;
extern double hubble_constant ;
extern double omega ;
extern int periodic ;
extern double period_size ;
extern double comptc ;
extern double eps_grav ;
extern double epsgas_grav ;
extern double time_unit ;
extern double lum_smooth ;
extern double xray_min_temp ;
extern int color_filter ;
extern int view_size ;
extern double zoom_factor;
extern int *particle_color;
extern double phi,psi,theta ;
extern double ba,ca ;
extern Real center_ell[MAXDIM] ;
extern Real center_fit[MAXDIM] ;
extern double fit_r1,fit_r2 ;
extern double fit_radius ;
extern double deldrg ;
extern double *acsmooth;
extern double deldr2 ;
extern double deldr2i ;
extern double *iwsmooth;
extern double *dwsmooth;
extern double *hsmdivv ;
extern double *cooling ;
extern double *hneutral ;
extern double *heneutral ;
extern double *heII ;
extern double *meanmwt ;
extern double *starform ;
extern double *xray_lum ;
extern int number_bands ;
extern double deldtempi ;
extern int number_xray_data ;
extern double gp0_H ;
extern double gp0_He ;
extern double gp0_Hep ;
extern double eps_H ;
extern double eps_He ;
extern double eps_Hep ;

extern unsigned char wrbb_red[256] ;
extern unsigned char wrbb_green[256] ;
extern unsigned char wrbb_blue[256] ;
extern unsigned char revrain_red[256] ;
extern unsigned char revrain_green[256] ;
extern unsigned char revrain_blue[256] ;
extern unsigned char rainbow_red[256] ;
extern unsigned char rainbow_green[256] ;
extern unsigned char rainbow_blue[256] ;
extern unsigned char colormap_red[256] ;
extern unsigned char colormap_green[256] ;
extern unsigned char colormap_blue[256] ;
extern unsigned char *arraymap_red;
extern unsigned char *arraymap_green;
extern unsigned char *arraymap_blue;

extern int base_color ;
extern int display ;
extern int mono;

struct file {
    char name[50];
    FILE *ptr ;
    int pipe;
} ;
extern struct file bodfile ;
extern struct file binaryfile ;
extern struct file hardfile ;
extern char *my_gets();
void    event_proc() ;
void    repaint_proc();
extern int ncomm;
/* command list structure */
struct comm {
     char name[MAXCOMM] ;  /* name of command */
     void (*funk)() ;        /* pointer to command function */
     int display;		/* is this a display function */
} ;
extern struct comm c_list[];
