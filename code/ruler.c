#include "defs.h"
#include "fdefs.h"
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
extern XtAppContext app_con;
extern Widget canvas;

void wait_on_X_ruler()
{
  XEvent event;

  while(XtAppPending(app_con) || ruler_flag)
    {
      XtAppNextEvent(app_con, &event);
      XtDispatchEvent(&event);
    }
}
  

void
ruler(job)
    char *job;
{
    Real x1,x2,y1,y2;
    double offset_x, offset_y ;
    double span_x, span_y ;

	if ( plotted_box != active_box ) {
	    printf("<the box that is plotted is not the ") ;
	    printf("active box, %s>\n",title) ;
	}
	else if (!current_project || !current_color) {
	    printf("<plot does not represent the present internal state") ;
	    printf(", %s>\n",title) ;
	}
	else {
	    setival(&ruler_flag, 1);
	    wait_on_X_ruler();
	    span_x = dv2_x - dv1_x ;
	    offset_x = can_width/2.
			- can_size * ((dv2_x + dv1_x)/ (double)span_x)/2. ;
	    span_y = dv1_y - dv2_y ;
	    offset_y = can_height/2.
			- can_size * ((dv2_y + dv1_y)/ (double)span_y)/2. ;
	    setival(&ruler_flag, 2) ;
	    ruler_x = (int)((ruler_x - offset_x) * (double) span_x /
		    (double)can_size) ;
	    x1 = ruler_x/scaling;
	    ruler_y = (int)((ruler_y - offset_y) * (double) span_y /
		    (double)can_size) ;
	    y1 = ruler_y/scaling;
	    wait_on_X_ruler();
	    ruler_x = (int)((ruler_x - offset_x) * (double) span_x /
		    (double)can_size) ;
	    x2 = ruler_x/scaling;
	    ruler_y = (int)((ruler_y - offset_y) * (double) span_y /
		    (double)can_size) ;
	    y2 = ruler_y/scaling;
	    printf("distance = %g\n",
		   sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)));
	}
}
