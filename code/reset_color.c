#include "defs.h"
#include "fdefs.h"
#include <X11/Intrinsic.h>
#include <X11/Xaw/Scrollbar.h>

extern Widget scrollbar;
PROTO(void, change_color, (Widget scroll, XtPointer colorbar,
			   XtPointer top_ptr)) ;

void
reset_color(job)
    char *job;
{
  char command[MAXCOMM];
  float where;
  int count;

  if(job != NULL)
      count = sscanf(job, "%s %f", command, &where);
  else
      count = 1;

  if(count == 1) {
      where = 0.0;
  }
  else if(count != 2) {
	input_error(command) ;
  }
  if(where < 0.0 || where > 1.0) {
    printf("<sorry, argument %f is out of range, %s>\n", where,
	   title);
    return;
  }
  XawScrollbarSetThumb(scrollbar, where, -1.0);
  change_color(scrollbar, NULL, (XtPointer) &where);
}
