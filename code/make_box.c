#include "defs.h"
#include "fdefs.h"
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
extern XtAppContext app_con;
extern Widget canvas;

void wait_on_X()
{
  XEvent event;

  while(XtAppPending(app_con) || make_box_flag)
    {
      XtAppNextEvent(app_con, &event);
      XtDispatchEvent(&event);
    }
}
  

void
make_box(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    int box ;
    Real x1[MAXDIM],x2[MAXDIM],x3[MAXDIM],x4[MAXDIM],x5[MAXDIM],x6[MAXDIM] ;
    double inverse_matrix[MAXDIM][MAXDIM] ;
    double offset_x, offset_y ;
    double span_x, span_y ;

    if (sscanf(job,"%s %d",command,&box) == 2) {
	if (box < 1 || box > MAXBOX) {
	    printf("<box number must be between 1 and MAXBOX, %s>\n",title) ;
	}
	else if ( plotted_box != active_box ) {
	    printf("<the box that is plotted is not the ") ;
	    printf("active box, %s>\n",title) ;
	}
	else if (!current_project || !current_color) {
	    printf("<plot does not represent the present internal state") ;
	    printf(", %s>\n",title) ;
	}
	else {
	    setival(&make_box_flag,1) ;
	    wait_on_X();
	    if(mono == YES)
	      XSetForeground(baseframe_dpy,gc_color[0],
			     WhitePixelOfScreen(XtScreen(canvas))
			     ^BlackPixelOfScreen(XtScreen(canvas))) ;
	    else
	      XSetForeground(baseframe_dpy,gc_color[0],
			     colors[1]^colors[0]) ;
  	    XSetFunction(baseframe_dpy,gc_color[0],GXxor) ;
	    setival(&make_box_flag,2) ;
	    wait_on_X();
	    span_x = dv2_x - dv1_x ;
	    offset_x = can_width/2.
			- can_size * ((dv2_x + dv1_x)/ (double)span_x)/2. ;
	    span_y = dv1_y - dv2_y ;
	    offset_y = can_height/2.
			- can_size * ((dv2_y + dv1_y)/ (double)span_y)/2. ;
	    make_box_x1 = (int)((make_box_x1 - offset_x) * (double) span_x /
		    (double)can_size) ;
	    make_box_y1 = (int)((make_box_y1 - offset_y) * (double) span_y /
		    (double)can_size) ;
	    make_box_x2 = (int)((make_box_x2 - offset_x) * (double) span_x /
		    (double)can_size) ;
	    make_box_y2 = (int)((make_box_y2 - offset_y) * (double) span_y /
		    (double)can_size) ;
	    x1[0] = x2[0] = (double)make_box_x1 / scaling ;
	    x2[1] = x4[1] = x6[1] = (double)make_box_y1 / scaling ;
	    x3[0] = x4[0] = x5[0] = x6[0] = (double)make_box_x2 / scaling ;
	    x1[1] = x3[1] = x5[1] = (double)make_box_y2 / scaling ;
	    rotate(DOWN,90.) ;
	    project() ;
	    if (radial_plot) {
		radial_color() ;
	    }
	    plot_sub(job) ;
	    setival(&make_box_flag,4) ;
	    wait_on_X();
	    span_x = dv2_x - dv1_x ;
	    offset_x = can_width/2.
			- can_size * ((dv2_x + dv1_x)/ (double)span_x)/2. ;
	    make_box_x1 = can_size * (make_box_x1/(double)span_x) + offset_x ;
	    make_box_x2 = can_size * (make_box_x2/(double)span_x) + offset_x ;
	    setival(&make_box_flag,6) ;
	    wait_on_X();
  	    XSetForeground(baseframe_dpy,gc_color[0], colors[0]) ;
	    XSetFunction(baseframe_dpy,gc_color[0],GXcopy) ;
	    span_y = dv1_y - dv2_y ;
	    offset_y = can_height/2.
			- can_size * ((dv2_y + dv1_y)/ (double)span_y)/2. ;
	    make_box_y1 = (int)((make_box_y1 - offset_y) * (double)span_y /
		    (double)can_size) ;
	    make_box_y2 = (int)((make_box_y2 - offset_y) * (double)span_y /
		    (double)can_size) ;
	    x1[2] = x2[2] = x3[2] = x4[2] = -(double)make_box_y1 / scaling ;
	    x5[2] = x6[2] =  -(double)make_box_y2 / scaling ;
	    rotate(UP,90.) ;
	    transpose(rot_matrix,inverse_matrix) ;
	    rotate(DOWN,90.) ;
	    matrix_vector_mult(inverse_matrix,x1,boxes[box].x1) ;
	    matrix_vector_mult(inverse_matrix,x2,boxes[box].x2) ;
	    matrix_vector_mult(inverse_matrix,x3,boxes[box].x3) ;
	    matrix_vector_mult(inverse_matrix,x4,boxes[box].x4) ;
	    matrix_vector_mult(inverse_matrix,x5,boxes[box].x5) ;
	    matrix_vector_mult(inverse_matrix,x6,boxes[box].x6) ;
	    add_vec(boxes[box].x1, boxes[box].x1, boxes[active_box].center) ;
	    add_vec(boxes[box].x2, boxes[box].x2, boxes[active_box].center) ;
	    add_vec(boxes[box].x3, boxes[box].x3, boxes[active_box].center) ;
	    add_vec(boxes[box].x4, boxes[box].x4, boxes[active_box].center) ;
	    add_vec(boxes[box].x5, boxes[box].x5, boxes[active_box].center) ;
	    add_vec(boxes[box].x6, boxes[box].x6, boxes[active_box].center) ;
	    loadbox(box) ;
	}
    }
    else {
	input_error(command) ;
    }
}
