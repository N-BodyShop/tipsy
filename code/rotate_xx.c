#include "defs.h"
#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Scrollbar.h>

void rotate();
void view();

extern Widget lrscrollbar;
extern Widget udscrollbar;
extern Widget ccscrollbar;

static double lastangle_ud;
static double lastangle_lr;
static double lastangle_cc;

void reset_rotate_scroll()
{
    XawScrollbarSetThumb(udscrollbar, 0.5, -1.0);
    XawScrollbarSetThumb(lrscrollbar, 0.5, -1.0);
    XawScrollbarSetThumb(ccscrollbar, 0.5, -1.0);

  lastangle_ud = lastangle_lr = lastangle_cc = 0.0;
}

  

void
rotate_ud(scroll, ignore, top_ptr)
     Widget scroll;
     XtPointer ignore, top_ptr;
{
  double angle;
  
  angle = ((*(float *) top_ptr))*360.0 - 180.0;
  rotate(UP, angle-lastangle_ud);
  view(NULL);
  lastangle_ud = angle;
}

void
rotate_lr(scroll, ignore, top_ptr)
     Widget scroll;
     XtPointer ignore, top_ptr;
{
  double angle;
  
  angle = ((*(float *) top_ptr))*360.0 - 180.0;
  rotate(RIGHT, angle-lastangle_lr);
  view(NULL);
  lastangle_lr = angle;
}

void
rotate_cc(scroll, ignore, top_ptr)
     Widget scroll;
     XtPointer ignore, top_ptr;
{
  double angle;
  
  angle = ((*(float *) top_ptr))*360.0 - 180.0;
  rotate(CLOCKWISE, angle-lastangle_cc);
  view(NULL);
  lastangle_cc = angle;
}
