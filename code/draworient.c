/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:34  trq
 * Initial revision
 *
 * Revision 2.6  1994/11/01  01:26:05  trq
 * Fixed bug in drawing zoom label.
 *
 * Revision 2.5  1994/10/07  16:55:15  trq
 * Corrected use of XtGetValues().
 *
 * Revision 2.4  1994/04/20  08:49:41  trq
 * Made casts from Args array.
 *
 * Revision 2.3  94/03/08  16:09:49  trq
 * Added a zoom label.
 * 
 * Revision 2.2  93/10/25  18:56:30  trq
 * Added cast to satisfy machines with pointers > longs
 * 
 * Revision 2.1  93/10/18  14:59:38  trq
 * Fixed font color problem.
 * 
 */

#include "defs.h"
#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
extern Widget orient;

static double orient_coord[3][3] = 
{
  {0.8, 0.0 , 0.0},
  {0.0, 0.8 , 0.0},
  {0.0, 0.0 , 0.8}
} ;

static char *axeslab[3] = 
{
  "x", "y", "z"
};

static GC label_gc = NULL;

void
draworient()
{
  Arg args[2];
  double x, y;
  int j, k, x_int, y_int;
  Dimension zero_x, zero_y;
  char zoomlab[MAXCOMM];

  if(!label_gc)
    {
      XFontStruct *font;
      Pixel fg;

      label_gc = XCreateGC(baseframe_dpy,currentview_xid,0,NULL) ;
      XtSetArg( args[0], XtNfont, &font);
      XtSetArg( args[1], XtNforeground, &fg);
      XtGetValues(orient, args, 2);
      XSetForeground(baseframe_dpy,label_gc, fg);
			     /* WhitePixelOfScreen(XtScreen(orient))) ; */
      XSetFont(baseframe_dpy, label_gc, font->fid);
    }
  XtSetArg(args[0], XtNwidth, &zero_x);
  XtSetArg(args[1], XtNheight, &zero_y);
  XtGetValues(orient, args, 2);
  zero_x /= 2;
  zero_y /= 2;
    XClearWindow(baseframe_dpy,XtWindow(orient)) ;
    for (j = 0 ;j < 3 ;j++) {
	for (x = y = 0.0,k = 0 ;k < 3 ;k++) {
	    x += rot_matrix[0][k] * orient_coord[j][k];
	    y += rot_matrix[1][k] * orient_coord[j][k];
	}
	x_int = (int)(x * zero_x +0.5)  + zero_x;
	y_int = -(int)(y * zero_x +0.5)  + zero_y;
	XDrawLine(baseframe_dpy,XtWindow(orient),gc_color[1],zero_x,zero_y,
		  x_int,y_int) ;
	XDrawString(baseframe_dpy, XtWindow(orient), label_gc,
		    x_int, y_int+2, axeslab[j], strlen(axeslab[j]));
    }
  if(view_size != 0)
    sprintf(zoomlab, "%.2fx", (double) INTMAX/(double) view_size);
  else
    sprintf(zoomlab, "0x");
    
  XDrawString(baseframe_dpy, XtWindow(orient), label_gc,
		    0, (int) zero_y*2, zoomlab, strlen(zoomlab));
}

  
