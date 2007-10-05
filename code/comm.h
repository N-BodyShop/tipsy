/* $Header$
 * $Log$
 * Revision 1.20  2007/10/05 19:11:08  trq
 * Adrienne Stilp:
 *
 * Added "cube" command to produce velocity cubes for HI. (cube.c, comm.h,
 * arguments.c, Makefile.in)
 *
 * fits.c:  added comments and fits3d subroutine.
 *
 * neutral.c: added option to read in ionization fractions from gasoline
 * outputs.
 *
 * vista.c: automatically add "fits" to file names if not present.
 *
 * Revision 1.19  2006/08/16 20:23:02  trq
 * New command from Ryan William Maas: arraymoments: finds shapes of shells
 * based on array values.
 *
 * Revision 1.18  2006/01/19 17:19:45  trq
 * Added "readpackedvector" command to get vectors ordered x1 y1 z1 ...
 *
 * Revision 1.17  2002/12/28 17:28:25  trq
 * Added "readbinvector" command for reading in binary vector file.
 *
 * Revision 1.16  2002/12/23 19:44:05  trq
 * Added "readbinarray" command to read binary arrays.  Only native formats
 * are supported.
 *
 * Revision 1.15  2001/10/04 17:58:36  nsk
 *
 *             Added back in two continue lines that were accidently deleted
 *
 * Revision 1.14  1999/08/25  22:05:25  nsk
 * added center to boxstat, checks for periodic in smooth, prints out
 * cooling stuff, vista makes plots
 *
 * Revision 1.13  1999/04/28  22:08:51  nsk
 * Added the writeiv command to output iv format.
 *
 * Revision 1.12  1998/07/24  17:41:00  trq
 * Added "closeb" alias for "closebinary.
 *
 * Revision 1.11  1998/03/31  00:26:53  trq
 * New command: scale.  N.B.  This command will mess up makebox and ruler.
 * This needs to be fixed.
 *
 * Revision 1.10  1998/02/27  19:51:33  nsk
 * Fixed star formation stuff, added starform routine
 *
 * Revision 1.9  1997/09/25  18:35:10  nsk
 * added uv command and discrete uv sources
 *
 * Revision 1.8  1997/09/24  17:35:04  trq
 * Added "loadstandard" command which reads tipsy standard (as opposed to native)
 * binaries.
 *
 * Revision 1.7  1997/09/05  01:02:20  nsk
 * streamlined vista (I hope it works), added neutralize command, added SZ
 * effect to vista.
 *
 * Revision 1.6  1997/08/28  20:29:18  nsk
 *      Added the gasify command, chnged redshift to to do lambda
 *
 * Revision 1.5  1996/12/20  01:25:09  trq
 * Added catbox command.
 *
 * Revision 1.4  1995/06/06  17:47:50  trq
 * dump_pixmap.c: Cleaned up declarations.
 *
 * Added kd.c and smooth.c for variable smoothing; NOW WITH LOSER TREES.
 *
 * Revision 1.3  1995/04/25  17:59:43  trq
 * New command: zspace
 *
 * Revision 1.2  1995/03/30  00:14:40  trq
 * Added "ruler" command.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:21  trq
 * Import to CVS
 *
 * Revision 2.12  1994/12/22  01:37:54  nsk
 * added drift
 *
 * Revision 2.11  1994/12/22  01:14:19  trq
 * Added markmass command.
 *
 * Revision 2.10  1994/09/20  21:57:52  trq
 * Added absorb command.
 *
 * Revision 2.9  1994/04/19  16:23:27  nsk
 * added pcenter and writeboxrot
 *
 * Revision 2.8  94/04/19  15:12:45  trq
 * Added "svec command and "varray" command.
 * 
 * Revision 2.7  94/03/22  11:50:42  trq
 * Added commands for vector, magnitude, and radial plots.
 * 
 * Revision 2.5  93/10/06  09:09:49  trq
 * Added shell command
 * 
 * Revision 2.4  93/10/05  17:09:44  trq
 * Added vcenter command
 * 
 * Revision 2.3  1993/05/07  12:46:40  trq
 * Added xray commands
 *
 * Revision 2.2  1992/10/22  13:08:24  trq
 * Added markgal command.
 *
 * Revision 2.1  1991/10/24  13:18:48  trq
 * Changed structure completely.
 *
 */
/* declarations file for all commands */

/* declare tipsy commands here */

extern void absorb() ;
extern void activatebox() ;
extern void angleup() ;
extern void arguments() ;
extern void arraymoments() ;
extern void arraystat() ;
extern void axesscale() ;
extern void boxstat_sub() ;

extern void catbox();
extern void colorbar_label();
extern void cconst_sub() ;
extern void clearall() ;
extern void clearrot();
extern void closeascii() ;
extern void closebinary() ;

extern void commands() ;
extern void cooling_sub() ;
extern void cube();
extern void delete_macro() ;
extern void delete_window() ;
extern void drift();
extern void eps_sub() ;
extern void epsgas_sub() ;
extern void fconst_sub() ;

extern void framepoints() ;
extern void gasify() ;
extern void hard() ;
extern void help() ;
extern void hubble() ;
extern void label_sub() ;
extern void lconst_sub() ;

extern void loadall_sub() ;
extern void loadbinary_sub() ;
extern void loadstandard_sub() ;
extern void make_box() ;
extern void make_macro() ;
extern void markarray() ;
extern void markbox() ;
extern void markgal() ;
extern void markmass() ;

extern void markstat() ;
extern void massflux() ;
extern void moments() ;
extern void neutralize() ;
extern void oldreadascii() ;
extern void openascii() ;

extern void openbinary() ;
extern void pcenter();
extern void plot_sub() ;
extern void point_size_sub() ;
extern void printhelp() ;
extern void profile() ;

extern void read_macro() ;
extern void readarray() ;
extern void readbinarray() ;
extern void readvector() ;
extern void readbinvector() ;
extern void readpackedvector() ;
extern void readascii() ;
extern void readoldbin() ;
extern void readmark();
extern void redshift_sub() ;
extern void reset_color() ;

extern void rhomap() ;
extern void rot_cur() ;
extern void rot_cur_grp() ;
extern void rotate_sub() ;
extern void ruler() ;
extern void run_macro() ;
extern void scale_sub() ;
extern void setbox_sub() ;
extern void setsphere_sub() ;

extern void shell_sub();
extern void showaxes_sub() ;
extern void showboxes_sub() ;
extern void showdark_sub() ;
extern void showgas_sub() ;
extern void showstar_sub() ;

extern void showvec_sub() ;
extern void showvel_sub() ;
extern void smooth_sub();
extern void starform_sub() ;
extern void star_history() ;
extern void unload_box() ;
extern void unmark_all() ;
extern void unmarkbox_sub() ;
extern void unzoom() ;
extern void uv() ;

extern void vcenter();
extern void vconst_sub() ;
extern void vel_dist() ;
extern void vel_sigma() ;
extern void velscale() ;
extern void version() ;

extern void view() ;
extern void view_all() ;
extern void view_array() ;
extern void view_gas() ;
extern void view_mag() ;
extern void view_pot() ;
extern void view_star() ;
extern void view_rad() ;

extern void vista() ;
extern void vortmap() ;
extern void whatis() ;
extern void window() ;
extern void write_macro() ;
extern void writebox() ;
extern void writebox_rot() ;
extern void writeiv() ;
extern void write_gas() ;
extern void writemark() ;

extern void xplot_all() ;
extern void xplot_array() ;
extern void xplot_gas() ;
extern void xplot_mag() ;
extern void xplot_pot() ;
extern void xplot_star() ;
extern void xplot_rad() ;
extern void xray() ;
extern void xray_load_sub() ;

extern void xypoints() ;
extern void xyzpoints() ;
extern void yplot_all() ;
extern void yplot_array() ;
extern void yplot_gas() ;
extern void yplot_mag() ;
extern void yplot_pot() ;

extern void yplot_star() ;
extern void yplot_rad() ;
extern void zplot_all() ;
extern void zplot_array() ;
extern void zplot_gas() ;
extern void zplot_mag() ;
extern void zplot_pot() ;

extern void zplot_rad() ;
extern void zplot_star() ;
extern void zoom_sub() ;
extern void zspace() ;

/* initialize command list here.  Create a new command by adding a new
entry to this list and declaring it above. Also, do not forget to
change the makefile and compose a man page!  */

struct comm c_list[] = {
     /* name  ---    function   - display */
     {"abox",        activatebox,	0},
     {"absorb",      absorb,		1},
     {"activatebox", activatebox,	0},
     {"angleup",     angleup,		0},
     {"arguments",   arguments,		0},
     {"arraymoments",arraymoments,	0},
     {"arraystat",   arraystat,		0},
     {"ascale",      axesscale,		0},

     {"aup",         angleup,		0},
     {"axesscale",   axesscale,		0},
     {"barlabel",    colorbar_label,	1},
     {"boxstat",     boxstat_sub,	0},
     {"catbox",      catbox,		0},
     {"cconst",      cconst_sub,	0},
     {"clearall",    clearall,		1},

     {"clearrot",    clearrot,		0},
     {"closeascii",  closeascii,	0},
     {"closeb",      closebinary,       0},
     {"closebinary", closebinary,	0},
     {"colorbarlabel",colorbar_label,	1},
     {"colorlabel",  colorbar_label,	1},
     {"commands",    commands,		0},
     {"coolconstants",cconst_sub,	0},
     {"coolout",     cooling_sub,       0},

     {"crot",        clearrot,		0},
     {"cube",        cube,              0},
     {"deletemacro", delete_macro,	0},
     {"deletewin",   delete_window,	1},
     {"dmac",        delete_macro,	0},
     {"drift",       drift,		0},
     {"eps",         eps_sub,		0},
     {"epsgas",      epsgas_sub,	0},

     {"fconst",      fconst_sub,	0},
     {"formconstants",fconst_sub,	0},
     {"framepoints", framepoints,	0},
     {"gasify",      gasify,            0},
     {"frp",         framepoints,	0},
     {"gravsoft",    eps_sub,		0},

     {"gravsoftgas", epsgas_sub,	0},
     {"hard",        hard,		1},
     {"help",        help,		0},
     {"hubble",      hubble,		0},
     {"label",	     label_sub,		1},
     
     {"lconst",      lconst_sub,	0},

     {"loadall",     loadall_sub,	0},
     {"loadb",       loadbinary_sub,	0},
     {"loadbinary",  loadbinary_sub,	0},
     {"loads",       loadstandard_sub,	0},
     {"loadstandard",loadstandard_sub,	0},
     {"lumconstants",lconst_sub,	0},
     {"macro",       run_macro,		0},
     {"makebox",     make_box,		1},

     {"makemacro",   make_macro,	0},
     {"man",         help,		0},
     {"markarray",   markarray,		0},
     {"markbox",     markbox,		0},
     {"markgal",     markgal,		0},
     {"markmass",    markmass,		0},
     {"markstat",    markstat,		0},

     {"mbox",        make_box,		1},
     {"mmac",        make_macro,	0},
     {"moments",     moments,		0},
     {"mrbox",       markbox,		0},
     {"mstat",       markstat,		0},
     {"neutral",     neutralize,	0},
     {"neutralize",  neutralize,	0},

     {"oldreadascii",oldreadascii,	0},
     {"openascii",   openascii,		0},
     {"openb",       openbinary,	0},
     {"openbinary",  openbinary,	0},
     {"pcenter",     pcenter,		0},
     {"phelp",       printhelp,		0},
     {"plot",        plot_sub,		1},

     {"pointsize",   point_size_sub,	1},
     {"printhelp",   printhelp,		0},
     {"profile",     profile,		0},
     {"psize",       point_size_sub,	1},

     {"rcol",        reset_color,	1},
     {"readarray",   readarray,		0},
     {"readbinarray", readbinarray,	0},
     {"readvector",  readvector,	0},
     {"readbinvector",  readbinvector,	0},
     {"readpackedvector",  readpackedvector,	0},
     {"readascii",   readascii,		0},
     {"readoldbin",  readoldbin,	0},
     {"readmacro",   read_macro,	0},
     {"readmark",    readmark,		0},
     {"redshift",    redshift_sub,	0},
     {"resetcolor",  reset_color,	1},

     {"rmac",        read_macro,	0},
     {"rotate",      rotate_sub,	0},
     {"rotationcurve",rot_cur,		0},
     {"rotcur",      rot_cur,		0},
     {"grouprotationcurve",rot_cur_grp,	0},
     {"rotcurgrp",     rot_cur_grp,	0},
     {"grprotcur",     rot_cur_grp,	0},
     {"ruler",       ruler,		1},

     {"saxes",       showaxes_sub,	0},
     {"sbox",	     showboxes_sub,	0},
     {"scale",	     scale_sub,		0},
     {"sdark",       showdark_sub,	0},
     {"setbox",      setbox_sub,	0},
     {"setsphere",   setsphere_sub,	0},
     {"sgas",        showgas_sub,	0},

     {"shell",	     shell_sub,		0},
     {"showaxes",    showaxes_sub,	0},
     {"showboxes",   showboxes_sub,	0},
     {"showdark",    showdark_sub,	0},
     {"showgas",     showgas_sub,	0},
     {"showstar",    showstar_sub,	0},

     {"showvec",     showvec_sub,	0},
     {"showvel",     showvel_sub,	0},
     {"smooth",      smooth_sub,	0},
     {"sstar",       showstar_sub,	0},
     {"starform",    starform_sub,      0},
     {"starhistory", star_history,	0},
     {"svec",        showvec_sub,	0},
     {"svel",        showvel_sub,	0},
     {"umall",       unmark_all,	0},
     {"umbox",       unmarkbox_sub,	0},

     {"unload",      unload_box,	0},
     {"unmarkall",   unmark_all,	0},
     {"unmarkbox",   unmarkbox_sub,	0},
     {"unzoom",      unzoom,		1},
     {"uv",          uv,		0},
     {"uvfield",     uv,		0},
     {"vall",        view_all,		1},
     {"varray",      view_array,	1},

     {"vcenter",     vcenter,		0},
     {"vconst",      vconst_sub,	0},
     {"veldist",     vel_dist,		0},
     {"velocitydist",vel_dist,		0},
     {"velscale",    velscale,		0},
     {"velsigma",    vel_sigma,		0},

     {"version",     version,		0},
     {"vgas",        view_gas,		1},
     {"view",        view,		1},
     {"viewall",     view_all,		1},
     {"viewarray",   view_array,	1},
     {"viewgas",     view_gas,		1},
     {"viewmag",     view_mag,		1},
     {"viewpot",     view_pot,		1},

     {"viewstar",    view_star,		1},
     {"viewrad",     view_rad,		1},
     {"viscconstants",vconst_sub,	0},
     {"vista",       vista,		0},

     {"vmag",        view_mag,		1},
     {"vpot",        view_pot,		1},
     {"vscale",      velscale,		0},
     {"vstar",       view_star,		1},
     {"vrad",        view_rad,		1},
     {"wbox",        writebox,		0},
     {"wboxr",       writebox_rot,	1},
     {"wiv",         writeiv,	        1},
     {"wgas",        write_gas,	        0},

     {"whatis",      whatis,		0},
     {"window",	     window,		1},
     {"wmac",        write_macro,	0},
     {"writebox",    writebox,		0},
     {"writeboxrot", writebox_rot,	1},
     {"writeiv",     writeiv,	        1},
     {"writegas",    write_gas,	        0},
     {"writemacro",  write_macro,	0},
     {"writemark",   writemark,		0},
     {"xall",        xplot_all,		1},

     {"xarray",      xplot_array,	1},
     {"xgas",        xplot_gas,		1},
     {"xmag",        xplot_mag,		1},
     {"xpot",        xplot_pot,		1},
     {"xray",        xray,		0},
     {"xrayload",    xray_load_sub,	0},
     {"xstar",       xplot_star,	1},
     {"xrad",        xplot_rad,		1},
     {"xypoints",    xypoints,		1},

     {"xyzpoints",   xyzpoints,		0},
     {"yall",        yplot_all,		1},
     {"yarray",      yplot_array,	1},
     {"ygas",        yplot_gas,		1},
     {"ymag",        yplot_mag,		1},
     {"ypot",        yplot_pot,		1},
     {"ystar",       yplot_star,	1},

     {"yrad",        yplot_rad,		1},
     {"zall",        zplot_all,		1},
     {"zarray",      zplot_array,	1},
     {"zgas",        zplot_gas,		1},
     {"zoom",        zoom_sub,		1},
     {"zmag",        zplot_mag,		1},
     {"zpot",        zplot_pot,		1},
     {"zstar",       zplot_star,	1},

     {"zspace",      zspace,		0},
     {"zrad",        zplot_rad,		1} } ;

int ncomm = (sizeof(c_list)/sizeof(struct comm));
