/* $Header$
 * $Log$
 * Revision 1.3  1998/02/18 22:53:06  trq
 * Always call canvas_resize_proc() to get new window height and width.
 *
 * Revision 1.2  1996/02/16  17:19:25  trq
 * Fixed bug in window for non-8 bit displays.
 * Added optional size arguments to the window command.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:35  trq
 * Import to CVS
 *
 * Revision 2.7  1994/10/07  16:55:15  trq
 * Corrected use of XtGetValues().
 *
 * Revision 2.6  1994/07/13  11:47:10  trq
 * Look for ConfigureNotify event.
 *
 * Revision 2.5  94/04/20  08:56:52  trq
 * Added title variable
 * 
 * Revision 2.3  1991/07/16  16:11:15  trq
 * Added backing store code.
 *
 * Revision 2.2  1991/07/02  14:05:26  trq
 * Expanded functionallity and added delete_window().
 *
 * Revision 2.1  1991/04/12  17:21:30  trq
 * *** empty log message ***
 *
 */
#include "defs.h"
#include "fdefs.h"
#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/Xaw/Simple.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Viewport.h>

static char vpTranslations[] = 
  "<Expose>:	RedrawViewport() \n\
   <ConfigureNotify>:	canvas_resize_proc() \n\
   None <Btn1Down>:	ZoomIn() \n\
   None <Btn2Down>:	ZoomOut() \n\
   None <Btn3Down>:	Pan() \n\
   ! Shift <Btn1Down>:	SetBox() \n\
   <MotionNotify>:	DrawBox()";

extern XtAppContext app_con;
extern Widget toplevel;
extern Widget canvas;
static Widget *shells;
static Widget *views;
static Widget *canvases;
static int *popped;

void *malloc();
void *realloc();

void
window(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    static int maxwins = 0;
    int winnum;
    int i;
    String params;
    Cardinal nparams;
    int narg;
    int width, height;
    Visual *visual;
    int depth;
    
    if((narg = sscanf(job, "%s %d %d %d", command, &winnum, &width, &height)) == 2
	|| narg == 4)
      {
	XEvent event;
	Arg args[2];

	if(winnum > maxwins)
	  {
	    if(maxwins == 0) 
	      {
		shells = (Widget *)malloc(winnum*sizeof(Widget));
		views = (Widget *)malloc(winnum*sizeof(Widget));
		canvases = (Widget *)malloc(winnum*sizeof(Widget));
		popped = (int *)malloc(winnum*sizeof(int));
	      }
	    else 
	      {
		shells = (Widget *)realloc(shells, winnum*sizeof(Widget));
		views = (Widget *)realloc(views, winnum*sizeof(Widget));
		canvases = (Widget *)realloc(canvases, winnum*sizeof(Widget));
		popped = (int *)realloc(popped, winnum*sizeof(int));
	      }
	    for(i = maxwins; i < winnum; i++) 
	      {
		shells[i] = NULL;
		popped[i] = 0;
	      }
	    maxwins = winnum;
	  }

	if(winnum == 0)
	  {
	    currentview_xid = XtWindow(canvas);
	    canvas_resize_proc(canvas, &event, &params, &nparams);
	    XtSetArg( args[0], XtNbackground, &colors[0]);
	    XtGetValues(canvas, args, 1);
	  }
	else
	  {
	    if(shells[winnum-1] == NULL) 
	      {

		shells[winnum-1] = XtCreatePopupShell("tipsy_popup",
						   topLevelShellWidgetClass,
						   toplevel, NULL, 0);
		XtSetArg(args[0], XtNvisual, &visual);
		XtSetArg(args[1], XtNdepth, &depth);
		XtGetValues(toplevel, args, 2);
		XtSetArg(args[0], XtNvisual, visual);
		XtSetArg(args[1], XtNdepth, depth);
		XtSetValues(shells[winnum-1], args, 2);
		views[winnum-1] = XtCreateManagedWidget( "hardview",
					     viewportWidgetClass,
					     shells[winnum-1], NULL, 0);
		canvases[winnum-1] = XtCreateManagedWidget("hard",
						simpleWidgetClass,
						views[winnum-1], NULL, 0);
		XtOverrideTranslations(canvases[winnum-1],
				   XtParseTranslationTable(vpTranslations));
		if(winnum == 1)
		  {
		    hard_width = WidthOfScreen(XtScreen(canvas));
		    hard_height = HeightOfScreen(XtScreen(canvas));
		    XtSetArg(args[0], XtNwidth, hard_width);
		    XtSetArg(args[1], XtNheight, hard_height);
		  }
		else
		  {
		      Dimension xwidth, xheight;

		      if(narg == 2) {

			XtSetArg(args[0], XtNwidth, &xwidth);
			XtSetArg(args[1], XtNheight, &xheight);
			XtGetValues(canvas, args, 2);
		      } else {
			  xwidth = width;
			  xheight = height;
		      }
			XtSetArg(args[0], XtNwidth, xwidth);
			XtSetArg(args[1], XtNheight, xheight);
		  }
		XtSetValues( views[winnum-1], args, 2 );
		XtSetValues( canvases[winnum-1], args, 2 );
		XtSetValues( shells[winnum-1], args, 2 );
		XtRealizeWidget(shells[winnum-1]);
		XtPopup(shells[winnum-1], XtGrabNone);
		popped[winnum-1] = 1;
		if(DoesBackingStore(XtScreen(shells[winnum-1])) != NotUseful)
		  {
		    XSetWindowAttributes win_atts;
		    unsigned long mask = CWBackingStore;
		    win_atts.backing_store
		      = DoesBackingStore(XtScreen(shells[winnum-1]));
		    XChangeWindowAttributes(baseframe_dpy,
					    XtWindow(canvases[winnum-1]),
					    mask, &win_atts);
		  }
	      }
	    canvas_resize_proc(canvases[winnum-1], &event, &params,
				 &nparams);
	    if(!popped[winnum-1])
	      {
		XtPopup(shells[winnum-1], XtGrabNone);
		popped[winnum-1] = 1;
	      }
	    XClearWindow(baseframe_dpy,XtWindow(canvases[winnum-1])) ;
	    currentview_xid = XtWindow(canvases[winnum-1]);
	    XtSetArg( args[0], XtNbackground, &colors[0]);
	    XtGetValues(canvases[winnum-1], args, 1);
	  }
	while(XtAppPending(app_con))
	  {
	    XtAppNextEvent(app_con, &event);
	    XtDispatchEvent(&event);
	  }
      }
    else {
	printf("<bad arguments to window, %s>\n",title) ;
    }
}

void
delete_window(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    int winnum;
    String params;
    Cardinal nparams;
    
    if(sscanf(job, "%s %d", command, &winnum) == 2)
      {
	XEvent event;
      
	if(winnum == 0)
	  {
	    printf("<try quit, %s>\n",title);
	    return;
	  }
	XtPopdown(shells[winnum-1]);
	popped[winnum-1] = 0;
	currentview_xid = XtWindow(canvas);
	canvas_resize_proc(canvas, &event, &params, &nparams);
      
	while(XtAppPending(app_con))
	  {
	    XtAppNextEvent(app_con, &event);
	    XtDispatchEvent(&event);
	  }
      }
    else {
	printf("<bad arguments to deletewin, %s>\n",title) ;
    }
}
