/* $Header$
 * $Log$
 * Revision 1.2  1995/05/10 18:32:44  trq
 * Added "dump" parameter to hard command which dumps the backing pixmap in
 * XWD format.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:34  trq
 * Import to CVS
 *
 * Revision 2.3  94/04/20  08:21:58  trq
 * deleted hard_replay variable.
 * 
 * Revision 2.2  94/02/16  10:14:13  trq
 * Added a "movie" parameter to just dump current window.
 * 
 * Revision 2.1  93/10/28  10:16:25  trq
 * First X implementation
 * 
 */
#include "defs.h"
#include "fdefs.h"
#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

extern XtAppContext app_con;
extern Widget canvas;
extern Widget popup_hard, hard_canvas;

void
hard(job)
    char *job;
{
    char command[MAXCOMM] ;
    XEvent event;

	if (sscanf(job,"%s %s",command,hardfile.name) == 2) {
	  if (strcmp(hardfile.name, "movie") == 0) 
	    {
				/* just dump current window to a file */
	      if(sscanf(job, "%*s %*s %s", hardfile.name) != 1)
		{
		  input_error(command);
		  return;
		}
	      sprintf(command, "xwd -id %d > %s", currentview_xid,
		      hardfile.name);
	      system(command);
	      return;
	    }
	  if (strcmp(hardfile.name, "dump") == 0) 
	    {
				/* just dump current window to a file */
	      if(sscanf(job, "%*s %*s %s", hardfile.name) != 1)
		{
		  input_error(command);
		  return;
		}
	      dump_pixmap(hardfile.name);
	      return;
	    }
	    window("window 1");
	    plot_sub(NULL);
	    draw_label();
	    
	    while(1)
		{
		  XtAppNextEvent(app_con, &event);
		  if((event.type == ButtonPress)
		     || (event.type == KeyPress))
		    break;
		  XtDispatchEvent(&event);
		}
	    if(strcmp(hardfile.name,"print") == 0 ){
		sprintf(command,
			"xwd -root | xpr -device ps -rv | lpr") ;
		system(command) ;
	    }
	    else if(strcmp(hardfile.name, "photo") != 0) {
		sprintf(command,"xwd -root > %s",hardfile.name) ;
		system(command) ;
	    }
	    delete_window("deletewin 1");
	}
	else {
	    input_error(command) ;
	}
}
