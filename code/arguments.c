/* $Header$
 * $Log$
 * Revision 1.31  2010/10/20 21:56:06  adrienne
 * ams: Added HI thermal velocity dispersion to cube code as an option
 * (type cube gas <filename> <pixels> <min vel> <max vel> <delta vel> thermal
 * to use).
 *
 * Changed fits.c so python is happy with the fits headers - added ''s around
 * KPC in cunit.
 *
 * Revision 1.30  2007/10/05 19:11:08  trq
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
 * Revision 1.29  2006/08/16 20:23:02  trq
 * New command from Ryan William Maas: arraymoments: finds shapes of shells
 * based on array values.
 *
 * Revision 1.28  2006/01/19 17:19:45  trq
 * Added "readpackedvector" command to get vectors ordered x1 y1 z1 ...
 *
 * Revision 1.27  2002/12/28 17:28:25  trq
 * Added "readbinvector" command for reading in binary vector file.
 *
 * Revision 1.26  2002/12/23 19:44:03  trq
 * Added "readbinarray" command to read binary arrays.  Only native formats
 * are supported.
 *
 * Revision 1.25  2001/10/04 17:58:35  nsk
 *
 *             Added back in two continue lines that were accidently deleted
 *
 * Revision 1.24  2000/06/07  23:49:28  trq
 * Added optional "mark" argument to the writebox command.
 *
 * Revision 1.23  2000/01/12  22:55:00  nsk
 * Fixed bugs in cooling routines, added cooling damping,
 * fixed bugs in starformation,  regularized dependencies.
 *
 * Revision 1.22  1999/08/25  22:05:23  nsk
 * added center to boxstat, checks for periodic in smooth, prints out
 * cooling stuff, vista makes plots
 *
 * Revision 1.21  1999/04/28  22:08:50  nsk
 * Added the writeiv command to output iv format.
 *
 * Revision 1.20  1998/07/24  17:40:59  trq
 * Added "closeb" alias for "closebinary.
 *
 * Revision 1.19  1998/03/31  00:26:54  trq
 * New command: scale.  N.B.  This command will mess up makebox and ruler.
 * This needs to be fixed.
 *
 * Revision 1.18  1998/02/27  19:51:31  nsk
 * Fixed star formation stuff, added starform routine
 *
 * Revision 1.17  1997/09/30  17:32:43  nsk
 * generalized rotcur
 *
 * Revision 1.16  1997/09/25  18:35:08  nsk
 * added uv command and discrete uv sources
 *
 * Revision 1.15  1997/09/24  17:35:03  trq
 * Added "loadstandard" command which reads tipsy standard (as opposed to native)
 * binaries.
 *
 * Revision 1.14  1997/09/12  18:43:04  nsk
 * fixed arguments for redshift
 *
 * Revision 1.13  1997/09/05  01:02:19  nsk
 * streamlined vista (I hope it works), added neutralize command, added SZ
 * effect to vista.
 *
 * Revision 1.12  1997/08/28  20:29:17  nsk
 *      Added the gasify command, chnged redshift to to do lambda
 *
 * Revision 1.11  1997/01/17  21:22:55  trq
 * Added optional argument to resetcolor command.
 *
 * Revision 1.10  1996/12/20  01:25:08  trq
 * Added catbox command.
 *
 * Revision 1.9  1996/06/29  00:17:00  trq
 * Changed loadbinary to load subboxes.
 *
 * Revision 1.8  1996/03/05  00:04:23  nsk
 * *** empty log message ***
 *
 * Revision 1.7  1996/02/16  17:19:21  trq
 * Fixed bug in window for non-8 bit displays.
 * Added optional size arguments to the window command.
 *
 * Revision 1.6  1995/09/21  21:36:03  trq
 * Corrected label arguments.
 *
 * Revision 1.5  1995/06/06  17:47:49  trq
 * dump_pixmap.c: Cleaned up declarations.
 *
 * Added kd.c and smooth.c for variable smoothing; NOW WITH LOSER TREES.
 *
 * Revision 1.4  1995/04/25  17:59:42  trq
 * New command: zspace
 *
 * Revision 1.3  1995/03/30  00:14:39  trq
 * Added "ruler" command.
 *
 * Revision 1.2  1995/03/02  23:27:18  trq
 * Changed documentation for setbox.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:23  trq
 * Import to CVS
 *
 * Revision 2.25  1994/12/29  02:28:38  trq
 * Changed definition of setbox command.
 *
 * Revision 2.24  1994/12/22  01:36:29  nsk
 * added drift
 *
 * Revision 2.23  1994/12/22  01:14:37  trq
 * added markmass command.
 *
 * Revision 2.22  1994/11/01  19:21:17  nsk
 * added variable size to vista and xray
 *
 * Revision 2.21  1994/11/01  01:27:06  trq
 * Fixed typo for absorb.
 *
 * Revision 2.20  1994/09/22  21:12:51  nsk
 * changed aruments for absorb
 *
 * Revision 2.19  1994/09/22  16:17:57  trq
 * Fixed typo.
 *
 * Revision 2.18  1994/09/20  22:30:32  nsk
 * added omega, boundary, and period
 *
 * Revision 2.17  1994/09/20  21:34:57  trq
 * Added absorb command.
 *
 * Revision 2.16  1994/09/12  20:48:43  nsk
 * *** empty log message ***
 *
 * Revision 2.15  1994/09/12  20:47:47  nsk
 * added hubble_constant and comove to redshift
 *
 * Revision 2.15  1994/09/12  20:47:47  nsk
 * added hubble_constant and comove to redshift
 *
 * Revision 2.14  1994/09/06  22:36:59  nsk
 * added rmac
 *
 * Revision 2.13  1994/09/02  18:11:48  nsk
 * changed arguments for markstat
 *
 * Revision 2.12  1994/09/01  00:08:34  nsk
 * fixed lumconstants list
 *
 * Revision 2.11  1994/08/31  23:18:59  nsk
 * added hubble center
 *
 * Revision 2.10  1994/04/19  16:24:21  nsk
 * added pcenter and writeboxrot
 *
 * Revision 2.9  94/04/19  15:14:04  trq
 * Many changes to match comm.h file.
 * 
 * Revision 2.8  94/03/22  11:50:02  trq
 * Added commands for vector, radial, and magnitude plots
 * 
 */
#include "defs.h"

void input_error();

void
arguments(job)
    char *job ;
{
    char arg[MAXCOMM] ;
    char command[MAXCOMM] ;

    if(sscanf(job,"%s %s",arg,command) != 2) {
	input_error(arg) ;
	return ;
    }

    if ( strcmp(command,"absorb") == 0 ) {
	printf("absorb type spatial_bin_number vel_min vel_max ");
	printf("vel_bin_number filename x y\n") ;
    }
    else if ( strcmp(command,"activatebox") == 0 ||
	    strcmp(command,"abox") == 0 ) {
	printf("activatebox box-number\n") ;
    }
    else if ( strcmp(command,"angleup") == 0  ||
	    strcmp(command,"aup") == 0) {
	printf("angleup box-number particle\n") ;
    }
    else if ( strcmp(command,"arguments") == 0 ) {
	printf("arguments command\n") ;
    }
    else if ( strcmp(command,"arraystat") == 0) {
	printf("arraystat box-number particle\n") ;
    }
    else if ( strcmp(command,"axesscale") == 0 ||
	    strcmp(command,"ascale") == 0 ) {
	printf("axescale scale\n") ;
    }
    else if ( strcmp(command,"boxstat") == 0 ) {
	printf("boxstat box-number particle\n") ;
    }
    else if ( strcmp(command,"catbox") == 0 ) {
	printf("catbox box1 box2 box3\n") ;
    }
    else if ( strcmp(command,"clearall") == 0 ) {
	printf("clearall\n") ;
    }
    else if ( strcmp(command,"clearrot") == 0 || strcmp(command,"crot") == 0 ) {
	printf("clearrot\n") ;
    }
    else if ( strcmp(command,"closeascii") == 0 ) {
	printf("closeascii\n") ;
    }
    else if ( strcmp(command,"closebinary") == 0 ||
	      strcmp(command,"closeb") == 0) {
	printf("closebinary\n") ;
    }
    else if ( strcmp(command,"colorbarlabel") == 0  ||
	    strcmp(command,"colorlabel") == 0 ||
	    strcmp(command,"barlabel") == 0 ) {
	printf("colorlabel position text\n") ;
    }
    else if ( strcmp(command,"commands") == 0 ) {
	printf("commands [print]\n") ;
    }
    else if ( strcmp(command,"coolconstants") == 0 ||
	    strcmp(command,"cconst") == 0 ) {
	printf("coolconstants kpcunit msolunit fhydrogen (slowcool dtcool)\n") ;
    }
    else if ( strcmp(command,"coolout") == 0 ) {
	printf("coolout box filename\n") ;
    }
    else if (strcmp(command, "cube") == 0 ){
      printf("cube type filename size velmin velmax deltavel (thermal)\n");
    }
    else if ( strcmp(command,"deletemacro") == 0 ||
	    strcmp(command,"dmac") == 0 ) {
	printf("deletemacro macro-name\n") ;
    }
    else if ( strcmp(command,"deletewin") == 0) {
	printf("deletewin window-number\n") ;
    }
    else if ( strcmp(command,"drift") == 0) {
	printf("drift box-number type delta-t\n") ;
    }
    else if ( strcmp(command,"formconstants") == 0 ||
	    strcmp(command,"fconst") == 0 ) {
	printf("formconstants cstar dtime temppar softpar stareff rhomin overmin\n") ;
    }
    else if ( strcmp(command,"framepoints") == 0 ||
	    strcmp(command,"frp") == 0 ) {
	printf("framepoints box-number particle filename-root\n") ;
    }
    else if ( strcmp(command,"gasify") == 0) {
	printf("gasify gas_frac rhobar Tbar index RhoShock TShock metals\n");
    }
    else if ( strcmp(command,"gravsoft") == 0 ||
	    strcmp(command,"eps") == 0) {
	printf("gravsoft eps\n") ;
    }
    else if ( strcmp(command,"gravsoftgas") == 0 ||
	    strcmp(command,"epsgas") == 0) {
	printf("gravsoftgas epsgas\n") ;
    }
    else if ( strcmp(command,"hard") == 0 ) {
	printf("hard filename\n") ;
    }
    else if ( strcmp(command,"help") == 0 || strcmp(command,"man") == 0 ) {
	printf("help command\n") ;
    }
    else if ( strcmp(command,"hubble") == 0 ) {
	printf("hubble box-number hubble-constant x-center y-center z-center\n") ;
    }
    else if ( strcmp(command,"label") == 0 ) {
	printf("label x-position y-position orientation-integer text\n") ;
    }
    else if ( strcmp(command,"loadall") == 0 ) {
	printf("loadall\n") ;
    }
    else if ( strcmp(command,"loadbinary") == 0 ||
	    strcmp(command,"loadb") == 0 ) {
	printf("loadbinary time [x-center y-center z-center x-hwidth y-hwidth z-hwidth]\n") ;
    }
    else if ( strcmp(command,"loadstandard") == 0 ||
	    strcmp(command,"loads") == 0 ) {
	printf("loadstandard time [x-center y-center z-center x-hwidth y-hwidth z-hwidth]\n") ;
    }
    else if ( strcmp(command,"lumconstants") == 0 ||
	    strcmp(command,"lconst") == 0 ) {
	printf("lumconstants msolunit kpcunit\n") ;
    }
    else if ( strcmp(command,"macro") == 0 ) {
	printf("macro macro-name\n") ;
    }
    else if ( strcmp(command,"makebox") == 0 || strcmp(command,"mbox") == 0 ) {
	printf("makebox box-number\n") ;
    }
    else if ( strcmp(command,"makemacro") == 0 ||
	    strcmp(command,"mmac") == 0 ) {
	printf("makemacro macro-name\n") ;
    }
    else if ( strcmp(command,"markarray") == 0) {
	printf("markarray box-number minimum maximum\n") ;
    }
    else if ( strcmp(command,"markbox") == 0 ||
	    strcmp(command,"mrbox") == 0 ) {
	printf("markbox box-number\n") ;
    }
    else if ( strcmp(command,"markgal") == 0) {
	printf("markgal box-number max-temp min-rho\n") ;
    }
    else if ( strcmp(command,"markmass") == 0) {
	printf("markmass box-number minimum maximum\n") ;
    }
    else if ( strcmp(command,"markstat") == 0 ||
	    strcmp(command,"mstat") == 0 ) {
	printf("markstat box-number particle\n") ;
    }
    else if ( strcmp(command,"moments") == 0 ) {
	printf("moments box-number center-box-number particle binning") ;
	printf(" bin_number filename minimum_radius\n") ; 
    }
    else if ( strcmp(command,"arraymoments") == 0 ) {
	printf("arraymoments box-number center-box-number particle binning") ;
	printf(" bin_number filename bin_count increment radius_bin\n") ;	
    }
    else if ( strcmp(command,"neutral") == 0 ||
	     strcmp(command,"neutralize") == 0) {
	printf("neutralize box-number max-temp min-rho\n") ;
    }
    else if ( strcmp(command,"oldreadascii") == 0 ) {
	printf("oldreadascii filename type\n") ;
    }
    else if ( strcmp(command,"openascii") == 0 ) {
	printf("openascii filename\n") ;
    }
    else if ( strcmp(command,"openbinary") == 0 || 
	    strcmp(command,"openb") == 0 ) {
	printf("openbinary filename\n") ;
    }
    else if ( strcmp(command,"pcenter") == 0 ) {
	printf("pcenter box-number center_x center_y center_z period_x period_y period_z\n") ;
    }
    else if ( strcmp(command,"plot") == 0 ) {
	printf("plot\n") ;
    }
    else if ( strcmp(command,"pointsize") == 0  ||
	    strcmp(command,"psize") == 0) {
	printf("pointsize particle size\n") ;
    }
    else if ( strcmp(command,"printhelp") == 0 ||
	    strcmp(command,"phelp") == 0) {
	printf("printhelp command\n") ;
    }
    else if ( strcmp(command,"profile") == 0 ) {
	printf("profile box-number center-box-number particle projection ") ;
	printf("binning bin_number filename minimum_radius\n") ; 
    }
    else if ( strcmp(command,"readarray") == 0) {
	printf("readarray filename\n") ;
    }
    else if ( strcmp(command,"readbinarray") == 0) {
	printf("readbinarray filename type\n") ;
    }
    else if ( strcmp(command,"readvector") == 0) {
	printf("readvector filename\n") ;
    }
    else if ( strcmp(command,"readbinvector") == 0) {
	printf("readbinvector filename type\n") ;
    }
    else if ( strcmp(command,"readpackedvector") == 0) {
	printf("readbinvector filename\n") ;
    }
    else if ( strcmp(command,"readascii") == 0 ) {
	printf("readascii filename\n") ;
    }
    else if ( strcmp(command,"readmacro") == 0 ||
	    strcmp(command,"rmac") == 0 ) {
	printf("readmacro macro-filename\n") ;
    }
    else if ( strcmp(command,"readmark") == 0) {
	printf("readmark filename\n") ;
    }
    else if ( strcmp(command,"readoldbin") == 0) {
	printf("readoldbin filename\n") ;
    }
    else if ( strcmp(command,"redshift") == 0 ) {
	printf("redshift z coordinates hubble-constant omega lambda") ;
	printf(" boundary period\n") ;
    }
    else if ( strcmp(command,"resetcolor") ==0 ||
	    strcmp(command,"rcol") == 0 ) {
	printf("resetcolor [fraction]\n") ;
    }
    else if ( strcmp(command,"rotate") == 0 ) {
	printf("rotate direction degrees\n") ;
    }
    else if ( strcmp(command,"rotationcurve") == 0 ||
	    strcmp(command,"rotcur") == 0 ) {
	printf("rotationcurve box-number center-box-number center-type") ;
	printf(" particle-type binning bin_number filename minimum_radius") ;
	printf(" maximum_radius\n") ; 
    }
    else if ( strcmp(command,"grouprotationcurve") == 0 ||
	    strcmp(command,"grprotcur") == 0 ||
	    strcmp(command,"rotcurgrp") == 0) {
	printf("grouprotationcurve box-number center-box-number center_x") ;
	printf(" center_y center_z bin_number filename minimum_radius\n") ;
    }
    else if ( strcmp(command,"ruler") ==0 ) {
	printf("ruler\n") ;
    }
    else if ( strcmp(command,"scale") == 0 ) {
	printf("scale scale-factor\n") ;
    }
    else if ( strcmp(command,"setbox") == 0 ) {
      printf("setbox box xcenter ycenter zcenter xradius yradius zradius\n");
    }
    else if ( strcmp(command,"setsphere") == 0 ) {
	printf("setsphere box xcenter ycenter zcenter radius\n") ;
    }
    else if ( strcmp(command,"shell") == 0 ) {
	printf("shell <system command>\n") ;
    }
    else if ( strcmp(command,"showaxes") == 0 ||
	    strcmp(command,"saxes") == 0 ) {
	printf("showaxes yes/no\n") ;
    }
    else if ( strcmp(command,"showboxes") == 0 ||
	    strcmp(command,"sbox") == 0 ) {
	printf("showboxes yes/no\n") ;
    }
    else if ( strcmp(command,"showdark") == 0 ||
	    strcmp(command,"sdark") == 0 ) {
	printf("showdark yes/no\n") ;
    }
    else if ( strcmp(command,"showgas") == 0 || strcmp(command,"sgas") == 0 ) {
	printf("showgas yes/no\n") ;
    }
    else if ( strcmp(command,"showstar") == 0 ||
	    strcmp(command,"sstar") == 0 ) {
	printf("showstar yes/no\n") ;
    }
    else if ( strcmp(command,"showvec") == 0 || strcmp(command,"svec") == 0 ) {
	printf("showvec yes/no\n") ;
    }
    else if ( strcmp(command,"showvel") == 0 || strcmp(command,"svel") == 0 ) {
	printf("showvel yes/no\n") ;
    }
    else if ( strcmp(command,"smooth") == 0 ) {
	printf("smooth ball_size n_smooth\n") ;
    }
    else if ( strcmp(command,"starform") == 0 ) {
	printf("starform filename \n") ;
    }
    else if ( strcmp(command,"starhistory") == 0 ) {
	printf("starhistory box-number") ;
	printf(" bin_number filename minimum_time maximum_time\n") ; 
    }
    else if ( strcmp(command,"unload") == 0 ) {
	printf("unload box-number\n") ;
    }
    else if ( strcmp(command,"unmarkall") == 0 ||
	    strcmp(command,"umall") == 0 ) {
	printf("unmarkall\n") ;
    }
    else if ( strcmp(command,"unmarkbox") == 0 ||
	    strcmp(command,"umbox") == 0 ) {
	printf("unmarkbox box-number\n") ;
    }
    else if ( strcmp(command,"unzoom") == 0 ) {
	printf("unzoom\n") ;
    }
    else if ( strcmp(command,"uvfield") == 0 ||
	    strcmp(command,"uv") == 0 ) {
	printf("uvfield uv_type jnu21 alphaj\n") ;
    }
    else if ( strcmp(command,"vcenter") == 0 ) {
	printf("vcenter box-number center-box-number type\n") ;
    }
    else if ( strcmp(command,"velocitydist") == 0 ||
	    strcmp(command,"veldist") == 0 ) {
	printf("velocitydist box-number center-box-number") ;
	printf(" bin_number filename minimum_vel maximum_vel\n") ; 
    }
    else if ( strcmp(command,"velscale") == 0 ||
	    strcmp(command,"vscale") == 0 ) {
	printf("velscale scale\n") ;
    }
    else if ( strcmp(command,"velsigma") == 0 ) {
	printf("velsigma box-number center-box-number particle binning ") ;
	printf("bin_number minimum_radius maximum_radius\n") ;
    }
    else if ( strcmp(command,"version") == 0 ) {
	printf("version\n") ;
    }
    else if ( strcmp(command,"view") == 0 ) {
	printf("view\n") ;
    }
    else if ( strcmp(command,"viewall") == 0 || strcmp(command,"vall") == 0 ) {
	printf("viewall\n") ;
    }
    else if ( strcmp(command,"viewarray") == 0 || strcmp(command,"varray") == 0 ) {
	printf("viewarray plot_type mapname min max (clip)\n") ;
    }
    else if ( strcmp(command,"viewgas") == 0 || strcmp(command,"vgas") == 0 ) {
	printf("viewgas plot_type min max (clip)\n") ;
    }
    else if ( strcmp(command,"viewmag") == 0 || strcmp(command,"vmag") == 0 ) {
	printf("viewmag particle min max plot_type (clip)\n") ;
    }
    else if ( strcmp(command,"viewpot") == 0 || strcmp(command,"vpot") == 0 ) {
	printf("viewpot particle min max (clip)\n") ;
    }
    else if ( strcmp(command,"viewstar") == 0
	     || strcmp(command,"vstar") == 0 ) {
	printf("viewstar plot_type min max (clip)\n") ;
    }
    else if ( strcmp(command,"viewrad") == 0 || strcmp(command,"vrad") == 0 ) {
	printf("viewrad particle min max plot_type (clip)\n") ;
    }
    else if ( strcmp(command,"viscconstants") == 0 ||
	    strcmp(command,"vconst") == 0 ) {
	printf("viscconstants alpha beta\n") ;
    }
    else if ( strcmp(command,"vista") == 0 ) {
	printf("vista type min max filename size\n") ;
    }
    else if ( strcmp(command,"whatis") == 0 ) {
	printf("whatis command\n") ;
    }
    else if ( strcmp(command,"window") == 0) {
	printf("window window-number (width height)\n") ;
    }
    else if ( strcmp(command,"writebox") == 0 || strcmp(command,"wbox") == 0 ) {
	printf("writebox box-number filename\n") ;
    }
    else if ( strcmp(command,"writeboxrot") == 0 ||
	    strcmp(command,"wboxR") == 0 ) {
	printf("writeboxrot box-number filename\n") ;
    }
    else if ( strcmp(command,"writegas") == 0 || strcmp(command,"wgas") == 0 ) {
	printf("writegas box-number type filename\n") ;
    }
    else if ( strcmp(command,"writeiv") == 0 || strcmp(command,"wiv") == 0 ) {
	printf("writeiv fraction filename\n") ;
    }
    else if ( strcmp(command,"writemacro") == 0 ||
	    strcmp(command,"wmac") == 0 ) {
	printf("writemacro macro-name macro-filename\n") ;
    }
    else if ( strcmp(command,"writemark") == 0 ) {
	printf("writemark box filename [mark]\n") ;
    }
    else if ( strcmp(command,"xall") == 0 ) {
	printf("xall\n") ;
    }
    else if ( strcmp(command,"xarray") == 0 ) {
	printf("xarray plot_type mapname min max (clip)\n") ;
    }
    else if ( strcmp(command,"xgas") == 0 ) {
	printf("xgas plot_type min max (clip)\n") ;
    }
    else if (strcmp(command,"xmag") == 0 ) {
	printf("xmag particle min max plot_type (clip)\n") ;
    }
    else if ( strcmp(command,"xpot") == 0 ) {
	printf("xpot particle min max (clip)\n") ;
    }
    else if ( strcmp(command,"xray") == 0 ) {
	printf("xray min max filename size\n") ;
    }
    else if ( strcmp(command,"xrayload") == 0 ) {
	printf("xrayload\n") ;
    }
    else if ( strcmp(command,"xstar") == 0 ) {
	printf("xstar plot_type min max (clip)\n") ;
    }
    else if ( strcmp(command,"xrad") == 0 ) {
	printf("xrad particle min max plot_type (clip)\n") ;
    }
    else if ( strcmp(command,"xypoints") == 0 ) {
	printf("xypoints filename\n") ;
    }
    else if ( strcmp(command,"xyzpoints") == 0 ) {
	printf("xyzpoints particle filename\n") ;
    }
    else if ( strcmp(command,"yall") == 0 ) {
	printf("yall\n") ;
    }
    else if ( strcmp(command,"yarray") == 0 ) {
	printf("yarray plot_type mapname min max (clip)\n") ;
    }
    else if ( strcmp(command,"ygas") == 0 ) {
	printf("ygas plot_type min max (clip)\n") ;
    }
    else if (strcmp(command,"ymag") == 0 ) {
	printf("ymag particle min max plot_type (clip)\n") ;
    }
    else if ( strcmp(command,"ypot") == 0 ) {
	printf("ypot particle min max (clip)\n") ;
    }
    else if ( strcmp(command,"ystar") == 0 ) {
	printf("ystar plot_type min max (clip)\n") ;
    }
    else if ( strcmp(command,"yrad") == 0 ) {
	printf("yrad particle min max plot_type (clip)\n") ;
    }
    else if ( strcmp(command,"zall") == 0 ) {
	printf("zall\n") ;
    }
    else if ( strcmp(command,"zarray") == 0 ) {
	printf("zarray plot_type mapname min max (clip)\n") ;
    }
    else if ( strcmp(command,"zgas") == 0 ) {
	printf("zgas plot_type min max (clip)\n") ;
    }
    else if (strcmp(command,"zmag") == 0 ) {
	printf("zmag particle min max plot_type (clip)\n") ;
    }
    else if ( strcmp(command,"zpot") == 0 ) {
	printf("zpot particle min max (clip)\n") ;
    }
    else if ( strcmp(command,"zoom") == 0 ) {
	printf("zoom x y z factor\n") ;
    }
    else if ( strcmp(command,"zstar") == 0 ) {
	printf("zstar plot_type min max (clip)\n") ;
    }
    else if ( strcmp(command,"zspace") == 0 ) {
	printf("zspace box hubble-constant xcenter ycenter zcenter\n") ;
    }
    else if ( strcmp(command,"zrad") == 0 ) {
	printf("zrad particle min max plot_type (clip)\n") ;
    }
    else {
	printf("<sorry %s is not a command, %s>\n",command,title) ;
    }
}
