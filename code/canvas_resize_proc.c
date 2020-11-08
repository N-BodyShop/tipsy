/* $Header$
 * $Log$
 * Revision 1.2  1998/02/18 22:51:21  trq
 * Avoid extraneous redraw messages from plotsub().
 *
 * Revision 1.1.1.1  1995/01/10  22:57:31  trq
 * Import to CVS
 *
 * Revision 2.4  1994/12/29  02:26:49  trq
 * changed can_size from min to max of height and width.
 *
 * Revision 2.3  1994/10/07  16:55:15  trq
 * Corrected use of XtGetValues().
 *
 * Revision 2.2  1994/07/13  11:47:46  trq
 * Call plot_sub() for servers with backing store.
 *
 */

#include "defs.h"
#include "fdefs.h"

void
canvas_resize_proc(canvas,event, params, n_params)
    Widget canvas ;
     XEvent *event;
     String *params;
     Cardinal *n_params;
{
    Arg args[2];

    XtSetArg(args[0], XtNwidth, &can_width);
    XtSetArg(args[1], XtNheight, &can_height);
    XtGetValues(canvas, args, 2);
    can_size = max(can_height,can_width) ;
    if(!current_project || !current_color)
	return;
    
    plot_sub("redraw");
    draw_label();
}
