/* $Header$
 * $Log$
 * Revision 1.2  1995/03/30 00:14:43  trq
 * Added "ruler" command.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:35  trq
 * Import to CVS
 *
 * Revision 2.20  1994/12/29  02:26:49  trq
 * changed can_size from min to max of height and width.
 *
 * Revision 2.19  1994/10/31  17:42:23  trq
 * Fixed off-by-one errors in RedrawColorbar().
 *
 * Revision 2.18  1994/10/07  16:56:10  trq
 * Corrected use of XtGetValues(), and changed "Counter" label to "Cntr".
 *
 * Revision 2.17  1994/07/13  11:45:34  trq
 * Ignore button presses in inactive windows, and look for ConfigureNotify
 * events.
 *
 * Revision 2.16  94/04/20  09:07:27  trq
 * Added unzoom button.
 * 
 * Revision 2.15  94/04/18  13:26:30  trq
 * Fix argument scanning problem.
 * 
 * Revision 2.14  94/04/06  13:46:01  trq
 * Workaround for ksr: use bg_pix to get background color instead of color[0]
 * directly.
 * 
 * Revision 2.13  94/03/11  12:11:42  trq
 * Add casts, and redeclare plane_masks for KSR.
 * 
 * Revision 2.12  94/03/08  16:06:57  trq
 * Neal: make "master" a variable.
 * 
 * Revision 2.10  94/02/18  08:43:26  trq
 * Attempt to find proper (8 bit Pseudocolor) visual.
 * 
 * Revision 2.9  93/10/25  16:59:20  trq
 * initialized backgrounds of graphics contexts.
 * 
 * Revision 2.8  93/10/21  08:55:41  trq
 * Added check for null pwuid
 * 
 * Revision 2.7  93/07/22  17:15:28  trq
 * Added disgusting picture.
 * 
 * Revision 2.6  1993/02/24  13:59:30  trq
 * Allowed for adustment of screen size and background colors via resources.
 *
 * Revision 2.5  1992/11/27  12:40:22  trq
 * Create icon from bitmap.
 *
 * Revision 2.4  1992/07/07  11:07:01  trq
 * Added orientation box stuff and current_plot checks.
 *
 * Revision 2.3  1991/07/02  13:50:14  trq
 * Added code for rotate bars.
 *
 * Revision 2.2  1991/05/09  14:31:15  trq
 * Added support for labels and alternate colormaps.
 *
 * Revision 2.1  1991/04/11  17:04:58  trq
 * changed to a common window id.
 *
 * Revision 2.0  1991/01/31  18:09:26  trq
 * Athena Widget version
 *
 */

#include "tipsy_icon.h"
#include "defs.h"
#include "fdefs.h"
#include "main.h"
#include <pwd.h>

#include <readline/readline.h>

#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <X11/Xaw/Box.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Viewport.h>

static String fallback_resources[] = { 
    "tipsy.Title:	      Tipsy: brought to you by Neal Katz and Tom Quinn",
    "*input:                  True",
    "*background:	      white",
    "*foreground:	      black",
    "*viewport*width:         800",
    "*viewport*height:        800",
    "*viewport*background:    black",
    "*hardview*background:    black",
    "*scrollbar*orientation:  horizontal",
    "*scrollbar*length:       180",
    "*scrollbar*background:   white",
    "*scrollbar*foreground:   black",
    "*color_label*background: white",
    "*color_label*foreground: black",
    "*colorview*width:	      320",
    "*colorview*height:	      30",
    "*colorview*background:   black",
    "*colorbar.font:          *-times-med*-r-*--*-240-*",
    "*orient*width:	      80",
    "*orient*height:	      80",
    "*orient*foreground:      white",
    "*orient*background:      black",
    "*colorbox*background:    white",
    "*colorbox*foreground:    black",
    "*unzoom*ShapeStyle:      oval",
    "*unzoom*highlightThickness:      3",
    NULL,
};

static void RedrawViewport();
static void ZoomIn();
static void ZoomOut();
static void Pan();
static void SetBox();
static void DrawBox();
static void RedrawColorbar();
void canvas_resize_proc();
void bar_resize_proc();
void zoom_scroll();
void unzoom();
void rotate_ud();
void rotate_lr();
void rotate_cc();
void zoom_window();

static char vpTranslations[] = 
  "<Expose>:	RedrawViewport() \n\
   <ConfigureNotify>:	canvas_resize_proc() \n\
   None <Btn1Down>:	ZoomIn() \n\
   None <Btn2Down>:	ZoomOut() \n\
   None <Btn3Down>:	Pan() \n\
   ! Shift <Btn1Down>:	SetBox() \n\
   <MotionNotify>:	DrawBox()";

static char cbTranslations[] = 
  "<Expose>:	RedrawColorbar() \n\
   <ConfigureNotify>:	bar_resize_proc()";

static char obTranslations[] = 
  "<Expose>:	draworient()";

static int actionTsize = 10;
static XtActionsRec actionTable[] =
{
  {"RedrawViewport", RedrawViewport},
  {"canvas_resize_proc", canvas_resize_proc},
  {"ZoomIn", ZoomIn},
  {"ZoomOut", ZoomOut},
  {"Pan", Pan},
  {"SetBox", SetBox},
  {"DrawBox", DrawBox},
  {"RedrawColorbar", RedrawColorbar},
  {"bar_resize_proc", bar_resize_proc},
  {"draworient", draworient},
};
  
char label_text[MAXCOMM] = "\0";
char color_left_text[MAXCOMM] = "\0";
char color_middle_text[MAXCOMM] = "\0";
char color_right_text[MAXCOMM] = "\0";
XtAppContext app_con;
Widget scrollbar;
Widget zoomscrollbar;
Widget lrscrollbar;
Widget udscrollbar;
Widget ccscrollbar;
Widget canvas;
Widget colorbar;
Widget orient;
XFontStruct *label_font;
void setbuf();
void initialize_color_table();
void unload_all();
int fclose();
void command_interp();

int
main(argc,argv)
char *argv[];
int argc;
{    
    struct passwd *pwuid ;
    char name[MAXCOMM] ;
    char prompt[MAXCOMM];
    char job[MAXCOMM] ;
    char command[MAXCOMM] ;
    void change_color() ;
    int i ;
    char *njob;
    Widget toplevel, outer, viewport, box, label;
    Widget colorview;
    Widget unzoom_button;
    Widget zoom_label;
    Widget first_label;
    Widget down_label;
    Widget left_label;
    XTextProperty canvas_prop;
    char *canvas_name = "tipsy_canvas";

#ifdef __ksr__
    unsigned int plane_masks[1];
#else
    unsigned long plane_masks[1];
#endif
    Arg args[3];
    unsigned long mask;
    XSetWindowAttributes win_atts;
    Pixmap nsktrq_pix;
    int nsktrq_h, nsktrq_w;
    char *nsktrq_bit;
    Pixel bg_pix;

    Visual *visual;
    XVisualInfo vinfo;

    sprintf(title,"master") ;
    for(i=1;i<argc;i++){
	sscanf(argv[i],"%s",job) ;
	if( strcmp(job,"-nodisplay") == 0 || strcmp(job,"-nodisp") == 0 ) {
	    display = NO ;
	}
	else if( strcmp(job,"-add") == 0 || strcmp(job,"-address") == 0 ) {
	    sscanf(argv[++i],"%s",title) ;
	}
    }
    if(display){
	setbuf(stdout,NULL) ;
	/* Initialize Xaw */
	toplevel = XtAppInitialize(&app_con, "tipsy", NULL, ZERO,
			       &argc, argv, fallback_resources, NULL, ZERO);

	XtAppAddActions(app_con, actionTable, actionTsize);
	
	baseframe_dpy = XtDisplay(toplevel);
	/* for debugging: XSynchronize(baseframe_dpy, True); */

	XtSetArg(args[0], XtNiconPixmap,
		 XCreateBitmapFromData(baseframe_dpy,
				       XtScreen(toplevel)->root,
				       (char *) tipsy_bits,
				       tipsy_width, tipsy_height));
	XtSetValues(toplevel, args, ONE);

	if(DefaultDepthOfScreen(XtScreen(toplevel)) == 1)
	  mono = YES;
	
	default_cmap = DefaultColormapOfScreen(XtScreen(toplevel));
	if(mono != YES) 
	  {
	    
	    if(XMatchVisualInfo(baseframe_dpy, DefaultScreen(baseframe_dpy), 8,
				PseudoColor, &vinfo) == 0)
	      {
		printf("<sorry %s, I need an 8 bit Pseudocolor display\n",title);
		return(-1);
	      }
	    visual = vinfo.visual;
	    if(visual != DefaultVisualOfScreen(XtScreen(toplevel)))
	      {
		XtSetArg(args[0], XtNvisual, visual);
		XtSetArg(args[1], XtNdepth, vinfo.depth);
	        default_cmap = XCreateColormap(baseframe_dpy,
				RootWindowOfScreen(XtScreen(toplevel)),
				visual, AllocNone);
		XtSetArg(args[2], XtNcolormap, default_cmap );
		XtSetValues( toplevel, args, 3 );
	      }
	    if(XAllocColorCells(baseframe_dpy, default_cmap, False,
			     plane_masks, 0, colors, CMAPSIZE) == 0) {
	        default_cmap = XCreateColormap(baseframe_dpy,
				RootWindowOfScreen(XtScreen(toplevel)),
				visual, AllocNone);
		XAllocColorCells(baseframe_dpy, default_cmap, False,
				 plane_masks, 0, colors, CMAPSIZE);
		XtSetArg( args[0], XtNcolormap, default_cmap );
		XtSetValues( toplevel, args, ONE );
	    }
	  }
	
	    
	/* Create windows */
	outer = XtCreateManagedWidget( "paned", panedWidgetClass, toplevel,
				  NULL, ZERO);
	viewport = XtCreateManagedWidget( "viewport", viewportWidgetClass,
					 outer, NULL, ZERO);

	canvas = XtCreateManagedWidget("canvas", simpleWidgetClass,
				       viewport, NULL, ZERO);
	XtOverrideTranslations(canvas,
			       XtParseTranslationTable(vpTranslations));

	box = XtCreateManagedWidget("controlbox", formWidgetClass, outer,
				    NULL, ZERO);

	colorview = XtCreateManagedWidget( "colorview", viewportWidgetClass,
					 box, NULL, ZERO);
	colorbar = XtCreateManagedWidget( "colorbar", labelWidgetClass,
					 colorview, NULL, ZERO);
	XtOverrideTranslations(colorbar,
			       XtParseTranslationTable(cbTranslations));
	XtSetArg( args[0], XtNfont, &label_font);
	XtGetValues(colorbar, args, ONE);

	XtSetArg( args[0], XtNlabel, "Color" );
	XtSetArg( args[1], XtNfromVert, colorview);
	label = XtCreateManagedWidget("color_label", labelWidgetClass, box, 
				  args, 2);

	XtSetArg( args[0], XtNfromHoriz, label);
	XtSetArg( args[1], XtNfromVert, colorview);
	scrollbar = XtCreateManagedWidget("scrollbar", scrollbarWidgetClass,
					  box, args, 2); 
	XtAddCallback(scrollbar, XtNjumpProc, change_color,
		      (XtPointer) colorbar);

	XtSetArg( args[0], XtNlabel, "zoom" );
	XtSetArg( args[1], XtNfromVert, label);
	zoom_label = XtCreateManagedWidget("zoom_label",
					   labelWidgetClass,box, args, 2);

	XtSetArg( args[0], XtNfromHoriz, zoom_label);
	XtSetArg( args[1], XtNfromVert, label);
	zoomscrollbar = XtCreateManagedWidget("scrollbar",
					      scrollbarWidgetClass,
					      box, args, 2);
	XtAddCallback(zoomscrollbar, XtNjumpProc, zoom_scroll,
		      (XtPointer) zoomscrollbar);

	XtSetArg( args[0], XtNlabel, "unzoom" );
	XtSetArg( args[1], XtNfromHoriz, zoomscrollbar);
	XtSetArg( args[2], XtNfromVert, label);
	unzoom_button = XtCreateManagedWidget("unzoom",
					      commandWidgetClass, box,
					      args, 3);
	XtAddCallback(unzoom_button, XtNcallback, unzoom, NULL);

	XtSetArg( args[0], XtNfromHoriz, colorview);
	XtSetArg( args[1], XtNlabel, "" );
	orient = XtCreateManagedWidget( "orient", labelWidgetClass,
					 box, args, 2);
	XtOverrideTranslations(orient,
			       XtParseTranslationTable(obTranslations));

	XtSetArg( args[0], XtNfromHoriz, orient);
	XtSetArg( args[1], XtNlabel, "Down" );
	down_label = XtCreateManagedWidget( "label", labelWidgetClass,
					 box, args, 2);
	XtSetArg( args[0], XtNfromHoriz, down_label);
	udscrollbar = XtCreateManagedWidget("scrollbar", scrollbarWidgetClass,
					  box, args, ONE); 
	XtAddCallback(udscrollbar, XtNjumpProc, rotate_ud,
		      (XtPointer) NULL);
	XtSetArg( args[0], XtNfromHoriz, udscrollbar);
	XtSetArg( args[1], XtNlabel, "Up" );
	label = XtCreateManagedWidget( "label", labelWidgetClass,
					 box, args, 2);

	XtSetArg( args[0], XtNfromVert, down_label);
	XtSetArg( args[1], XtNlabel, "Left" );
	XtSetArg( args[2], XtNfromHoriz, orient);
	left_label = XtCreateManagedWidget( "label", labelWidgetClass,
					 box, args, 3);
	XtSetArg( args[0], XtNfromHoriz, left_label);
	XtSetArg( args[1], XtNfromVert, down_label);
	lrscrollbar = XtCreateManagedWidget("scrollbar", scrollbarWidgetClass,
					  box, args, 2); 
	XtAddCallback(lrscrollbar, XtNjumpProc, rotate_lr,
		      (XtPointer) NULL);
	XtSetArg( args[0], XtNfromHoriz, lrscrollbar);
	XtSetArg( args[1], XtNlabel, "Right" );
	XtSetArg( args[2], XtNfromVert, down_label);
	label = XtCreateManagedWidget( "label", labelWidgetClass,
					 box, args, 3);

	XtSetArg( args[0], XtNfromVert, left_label);
	XtSetArg( args[1], XtNlabel, "Cntr" );
	XtSetArg( args[2], XtNfromHoriz, orient);
	first_label = XtCreateManagedWidget( "label", labelWidgetClass,
					 box, args, 3);
	XtSetArg( args[0], XtNfromHoriz, first_label);
	XtSetArg( args[1], XtNfromVert, left_label);
	ccscrollbar = XtCreateManagedWidget("scrollbar", scrollbarWidgetClass,
					  box, args, 2); 
	XtAddCallback(ccscrollbar, XtNjumpProc, rotate_cc,
		      (XtPointer) NULL);
	XtSetArg( args[0], XtNfromHoriz, ccscrollbar);
	XtSetArg( args[1], XtNlabel, "Clock" );
	XtSetArg( args[2], XtNfromVert, left_label);
	label = XtCreateManagedWidget( "label", labelWidgetClass,
					 box, args, 3);

	XtRealizeWidget(toplevel);
	currentview_xid = XtWindow(canvas);
	colorbar_xid = XtWindow(colorbar);
	XStringListToTextProperty(&canvas_name, 1, &canvas_prop);
	XSetWMName(baseframe_dpy, currentview_xid, &canvas_prop);
	
	if(DoesBackingStore(XtScreen(toplevel)) != NotUseful)
	  {
	    win_atts.backing_store = DoesBackingStore(XtScreen(toplevel));
	    mask = CWBackingStore;
	    XChangeWindowAttributes(baseframe_dpy, currentview_xid,
				    mask, &win_atts);
	  }
	
	if(mono != YES) 
	  {
	    initialize_color_table() ;
	    XStoreColors(baseframe_dpy, default_cmap, clist, CMAPSIZE);
	    for(i = 0; i < CMAPSIZE ; i++){
	      gc_color[i] = XCreateGC(baseframe_dpy,currentview_xid,0,NULL) ;
	      XSetForeground(baseframe_dpy,gc_color[i],colors[i]) ;
	      XSetBackground(baseframe_dpy,gc_color[i],
			     BlackPixelOfScreen(XtScreen(toplevel))) ;
	    }
	  }
	else 
	  {
	    for(i = 0; i < CMAPSIZE ; i++){
	      gc_color[i] = XCreateGC(baseframe_dpy,currentview_xid,0,NULL) ;
	      XSetForeground(baseframe_dpy,gc_color[i],
			     WhitePixelOfScreen(XtScreen(toplevel))) ;
	    }
	  }
	XtSetArg( args[0], XtNbackground, &bg_pix);
	XtGetValues(canvas, args, ONE);
	colors[0] = bg_pix;
	XSetFont(baseframe_dpy, gc_color[127], label_font->fid);
	get_nsktrq(&nsktrq_bit, &nsktrq_w, &nsktrq_h);
	nsktrq_pix
	  = XCreateBitmapFromData(baseframe_dpy,
				 RootWindowOfScreen(XtScreen(toplevel)),
				 nsktrq_bit, nsktrq_w, nsktrq_h);
	XCopyPlane(baseframe_dpy, nsktrq_pix, currentview_xid, gc_color[2],
		   0, 0, nsktrq_w, nsktrq_h, 0, 0, 1);
    }
    unload_all() ;

    pwuid = getpwuid(getuid()) ;
    if(pwuid == NULL)
       strcpy(name,title) ;
    else {
	for(i = 0;  pwuid->pw_gecos[i] && pwuid->pw_gecos[i] != ' '; i++){
	    name[i] = pwuid->pw_gecos[i] ;
	    if(name[i] == '.'){
		strcpy(name,title) ;
		break ;
	    }
            name[i+1] = '\0' ;
	}
    }
    printf("<welcome, %s, your every wish is my command>\n",title) ;

    if(display){
      XEvent event;
      
      while(XtAppPending(app_con))
	{
	  XtAppNextEvent(app_con, &event);
	  XtDispatchEvent(&event);
	}
    }
    initialize_readline();
    sprintf(prompt, "<yes, %s>",name);
    forever {
	njob = my_gets(prompt) ;
        if(njob) sscanf(njob,"%s",command) ;
	if ( !njob || strcmp(command,"quit") == 0 ) {
	    if (asciiopen) {
		fclose(bodfile.ptr) ;
	    }
	    if (binaryopen) {
		fclose(binaryfile.ptr) ;
	    }
	    stifle_history(50);
	    write_history(".tipsy_history");
	    printf("<I will miss you, %s>\n",title) ;
	    break ;
	}
	else {
	    strcpy(job,njob) ;
	    command_interp(job) ;
	}
    }

    if(display){
	if(default_cmap !=
	   DefaultColormapOfScreen(XtScreen(toplevel)))
	  XFreeColormap(baseframe_dpy, default_cmap);
	XtDestroyWidget(toplevel) ;
    }
    return(0);
}

static Boolean CompareEvents( oldEvent, newEvent )
    XEvent *oldEvent, *newEvent;
{
#define Check(field) if (newEvent->field != oldEvent->field) return False;

    Check(xany.display);
    Check(xany.type);
    Check(xany.window);

    switch( newEvent->type ) {
      case MotionNotify:
		Check(xmotion.state); break;
      case ButtonPress:
      case ButtonRelease:
		Check(xbutton.state);
		Check(xbutton.button); break;
      case KeyPress:
      case KeyRelease:
		Check(xkey.state);
		Check(xkey.keycode); break;
      case EnterNotify:
      case LeaveNotify:
		Check(xcrossing.mode);
		Check(xcrossing.detail);
		Check(xcrossing.state); break;
    }
#undef Check

    return True;
}

struct EventData {
	XEvent *oldEvent;
	int count;
};

static Bool PeekNotifyEvent( dpy, event, args )
    Display *dpy;
    XEvent *event;
    char *args;
{
    struct EventData *eventData = (struct EventData*)args;

    return ((++eventData->count == QLength(dpy)) /* since PeekIf blocks */
	    || CompareEvents(event, eventData->oldEvent));
}

static Boolean LookAhead( w, event )
    Widget w;
    XEvent *event;
{
    XEvent newEvent;
    struct EventData eventData;

    if (QLength(XtDisplay(w)) == 0) return False;

    eventData.count = 0;
    eventData.oldEvent = event;

    XPeekIfEvent(XtDisplay(w), &newEvent, PeekNotifyEvent, (char*)&eventData);

    if (CompareEvents(event, &newEvent))
        return True;
    else
        return False;
}

static void
RedrawViewport(viewport, event, params, n_params)
     Widget viewport;
     XEvent *event;
     String *params;
     Cardinal *n_params;
{
    Arg args[2];

  if(LookAhead(viewport, event)) return;
  if(XtWindow(viewport) != currentview_xid) return;
    
    XtSetArg(args[0], XtNwidth, &can_width);
    XtSetArg(args[1], XtNheight, &can_height);
    XtGetValues(viewport, args, 2);
    can_size = max(can_height,can_width) ;
    plot_sub("redraw");
    draw_label();
}
  
static void
RedrawColorbar(colorbar, event, params, n_params)
     Widget colorbar;
     XEvent *event;
     String *params;
     Cardinal *n_params;
{
  int i, p_color;
  Arg args[2];
  
  XtSetArg(args[0], XtNwidth, &bar_width);
  XtSetArg(args[1], XtNheight, &bar_height);
  XtGetValues(colorbar, args, 2);

  if(LookAhead(colorbar, event)) return;
	for(i = 0 ; i < bar_width ; i++){
	    p_color = ((CMAPSIZE - 2) * i)/bar_width + 2 ;
	    p_color = max(2,p_color) ;
	    p_color = min(CMAPSIZE - 2,p_color) ;
	    XDrawLine(baseframe_dpy,colorbar_xid,gc_color[p_color],
		    i,0,i,bar_height-1) ;
	}
    draw_label();
}

static void ExtractPosition( event, x, y )
    XEvent *event;
    Position *x, *y;            /* RETURN */
{
    switch( event->type ) {
      case MotionNotify:
                *x = event->xmotion.x;   *y = event->xmotion.y;   break;
      case ButtonPress:
      case ButtonRelease:
                *x = event->xbutton.x;   *y = event->xbutton.y;   break;
      case KeyPress:
      case KeyRelease:
                *x = event->xkey.x;      *y = event->xkey.y;      break;
      case EnterNotify:
      case LeaveNotify:
                *x = event->xcrossing.x; *y = event->xcrossing.y; break;
      default:
                *x = 0; *y = 0;
    }
}

static void
ZoomIn(viewport, event, params, n_params)
     Widget viewport;
     XEvent *event;
     String *params;
     Cardinal *n_params;
{
  Position x, y;
  
  if(XtWindow(viewport) != currentview_xid) return;
  
  if(!current_project || !current_color) 
    {
      printf("<plot does not represent the present internal state") ;
      printf(", %s>\n",title) ;
      return;
    }
   if(ruler_flag == 1 || ruler_flag == 2) {
      ExtractPosition(event, &x, &y);
      ruler_x = x;
      ruler_y = y;
      ruler_flag = 0;
      return;
   }

    if(make_box_flag != 2 && make_box_flag != 3 &&
	    make_box_flag != 6 && make_box_flag != 7){
      ExtractPosition(event, &x, &y);

	if (view_size == INTMAX ) {
	    view_size = (view_size + 1) / ZOOMSCALE ;
	}
	else {
	    view_size /= ZOOMSCALE ;
	}
        zoom_factor = (double) INTMAX / (double) view_size;
        reset_zoom_scroll();
	zoom_window((int)x,(int)y) ;
	plot_sub(NULL) ;
        draw_label();
	if(make_box_flag == 5){
	    make_box_flag = 4 ;
	}
   }
  
}

static void
ZoomOut(viewport, event, params, n_params)
     Widget viewport;
     XEvent *event;
     String *params;
     Cardinal *n_params;
{
  Position x, y;

  if(XtWindow(viewport) != currentview_xid) return;

  if(!current_project || !current_color) 
    {
      printf("<plot does not represent the present internal state") ;
      printf(", %s>\n",title) ;
      return;
    }
   if(ruler_flag == 1 || ruler_flag == 2)
       return;
    if (view_size < INTMAX && make_box_flag != 2 &&
	    make_box_flag != 3 && make_box_flag != 6 &&
	    make_box_flag != 7) {
      ExtractPosition(event, &x, &y);
	view_size = min((ZOOMSCALE * view_size),INTMAX) ;
        zoom_factor = (double) INTMAX / (double) view_size;
        reset_zoom_scroll();
	zoom_window((int) x,(int) y) ;
	plot_sub(NULL) ;
        draw_label();
	if(make_box_flag == 5){
	    make_box_flag = 4 ;
	}
    }
}

static void
Pan(viewport, event, params, n_params)
     Widget viewport;
     XEvent *event;
     String *params;
     Cardinal *n_params;
{
  Position x, y;

  if(XtWindow(viewport) != currentview_xid) return;

  if(!current_project || !current_color) 
    {
      printf("<plot does not represent the present internal state") ;
      printf(", %s>\n",title) ;
      return;
    }
   if(ruler_flag == 1 || ruler_flag == 2)
       return;
  if (make_box_flag != 2 &&
      make_box_flag != 3 && make_box_flag != 6 &&
      make_box_flag != 7) {
    ExtractPosition(event, &x, &y);
    zoom_window(x,y) ;
    plot_sub(NULL) ;
    draw_label();
    if(make_box_flag == 5){
      make_box_flag = 4 ;
    }
  }
}


static void
SetBox(viewport, event, params, n_params)
     Widget viewport;
     XEvent *event;
     String *params;
     Cardinal *n_params;
{
  int span_x, offset_x;
  Position x, y;
  int x1;

  if(XtWindow(viewport) != currentview_xid) return;

  ExtractPosition(event, &x, &y);
    if(make_box_flag == 1){
	make_box_x1 = x ;
	make_box_y1 = y ;
	make_box_flag = 0;
    }
    else if(make_box_flag == 3){
	make_box_x2 = x ;
	make_box_y2 = y ;
	make_box_flag = 0;
    }
    else if(make_box_flag == 5){
	span_x = dv2_x - dv1_x ;
	offset_x = can_width/2. - can_size * ((dv2_x + dv1_x)/
		(double)span_x)/2. ;
	x = can_size * (make_box_x1/(double)span_x) + offset_x ;
	x1 = can_size * (make_box_x2/(double)span_x) + offset_x ;
	XDrawLine(baseframe_dpy,currentview_xid,gc_color[0],x,y,x1,y) ;
	make_box_y1 = y ;
	make_box_flag = 0;
    }
    else if(make_box_flag == 7){
	make_box_y2 = y ;
	make_box_flag = 0 ;
    }
}

static void
DrawBox(viewport, event, params, n_params)
     Widget viewport;
     XEvent *event;
     String *params;
     Cardinal *n_params;
{
  static Position x, y;
  int span_x;
  int offset_x;
  static int x1;

    if(make_box_flag == 2 || make_box_flag == 3){
        if(make_box_flag == 3){
	    XDrawRectangle(baseframe_dpy,currentview_xid,gc_color[0],
		    make_box_x1,y,x-make_box_x1,make_box_y1-y);
	}
	ExtractPosition(event, &x, &y);
	XDrawRectangle(baseframe_dpy,currentview_xid,gc_color[0],
		make_box_x1,y,x-make_box_x1,make_box_y1-y);
	make_box_flag = 3 ;
    }
    else if(make_box_flag == 4 || make_box_flag == 5){
	if(make_box_flag == 5){
	    XDrawLine(baseframe_dpy,currentview_xid,gc_color[0],x,y,
		    x1,y) ;
	}
	ExtractPosition(event, &x, &y);
	span_x = dv2_x - dv1_x ;
	offset_x = can_width/2. - can_size * ((dv2_x + dv1_x)/
		(double)span_x)/2. ;
	x = can_size * (make_box_x1/(double)span_x) + offset_x ;
	x1 = can_size * (make_box_x2/(double)span_x) + offset_x ;
	XDrawLine(baseframe_dpy,currentview_xid,gc_color[0],x,y,x1,y) ;
	make_box_flag = 5 ;
    }
    else if(make_box_flag == 6 || make_box_flag == 7){
	if(make_box_flag == 7){
	    XDrawRectangle(baseframe_dpy,currentview_xid,gc_color[0],
		    make_box_x1,y,x-make_box_x1,make_box_y1-y);
	}
	ExtractPosition(event, &x, &y);
	x = make_box_x2 ;
	XDrawRectangle(baseframe_dpy,currentview_xid,gc_color[0],
		make_box_x1,y,x-make_box_x1,make_box_y1-y);
	make_box_flag = 7 ;
    }
}
