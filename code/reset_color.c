#include "defs.h"
#include <X11/Intrinsic.h>
#include <X11/Xaw/Scrollbar.h>

  extern Widget scrollbar;

void
reset_color(job)
    char job[MAXCOMM] ;
{
  XawScrollbarSetThumb(scrollbar, 0.0, -1.0);
}
