#include "defs.h"
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

bar_resize_proc(colorbar, event, params, n_params)
    Widget colorbar ;
     XEvent *event;
     String *params;
     Cardinal *n_params;
{
    int i ;
    int p_color ;
    Arg args[2];

    XtSetArg(args[0], XtNwidth, &bar_width);
    XtSetArg(args[1], XtNheight, &bar_height);
    XtGetValues(colorbar, args, 2);

    for(i = 0 ; i < bar_width ; i++){
	    p_color = ((CMAPSIZE - 2) * i)/bar_width + 2 ;
	    p_color = max(2,p_color) ;
	    p_color = min(CMAPSIZE - 1,p_color) ;
	    XDrawLine(baseframe_dpy,colorbar_xid,gc_color[p_color],
		    i,0,i,bar_height-1) ;
    }
}
