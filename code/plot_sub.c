/*
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:30  trq
 * Initial revision
 *
 * Revision 2.6  94/04/20  08:46:01  trq
 * Added title variable.
 * 
 * Revision 2.4  94/02/22  16:22:09  trq
 * Got rid of call to plot_vel().
 * 
 * Revision 2.3  94/02/16  13:19:10  trq
 * Check for binary_loaded and boxes_loaded.
 * 
 */
#include "defs.h"
#include "fdefs.h"
void
plot_sub(job)
    char *job;
{
    if(binary_loaded == UNLOADED || boxes_loaded[0] == NO)
      return;
    if(!current_project || !current_color)
      {
	printf("<plot does not represent the present internal state") ;
	printf(", %s>\n",title) ;
	return;
      }
    plot_all(job) ;
    draworient();
    XFlush(baseframe_dpy);
    plotted_box = active_box ;
}
