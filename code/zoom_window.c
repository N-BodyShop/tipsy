/*
 * $Header$
 * $Log$
 * Revision 1.1.1.1.6.1  2000/07/13 01:19:59  nsk
 * Added 1-param calling mode to zoom from center of current box.  (maf)
 *
 * Revision 1.1.1.1  1995/01/10  22:57:32  trq
 * Import to CVS
 *
 * Revision 2.3  1994/12/23  01:10:36  nsk
 * added ability to zoom to center of mass of a box
 *
 * Revision 2.2  1994/04/20  08:24:36  trq
 * Added zoom scrollbar.
 *
 * Revision 2.1  94/02/18  09:38:55  trq
 * Added zoom_sub to support zoom command
 * 
 */
#include "defs.h"
#include "fdefs.h"
#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Scrollbar.h>

void
zoom_window(x,y)
    int x ;
    int y ;
{
    double offset_x, offset_y ;
    double span_x, span_y ;

    span_x = dv2_x - dv1_x ;
    offset_x = can_width/2. - can_size * ((dv2_x + dv1_x)/ (double)span_x)/2. ;
    span_y = dv1_y - dv2_y ;
    offset_y = can_height/2. - can_size * ((dv2_y + dv1_y)/ (double)span_y)/2. ;
    x = (int)((x - offset_x) * (double)span_x / (double)can_size) ;
    y = (int)((y - offset_y) * (double)span_y / (double)can_size) ;
    dv1_x = x - view_size ;
    dv1_y = y - view_size ;
/*    dv1_x  = max(dv1_x,-INTMAX) ;
    dv1_y  = max(dv1_y,-INTMAX) ;
 */
    dv2_x = dv1_x + 2 * view_size ;
    dv2_y = dv1_y + 2 * view_size ;
/*    dv2_x  = min(dv2_x,INTMAX) ;
    dv2_y  = min(dv2_y,INTMAX) ;
 */
    dv1_x = dv2_x - 2 * view_size ;
    dv1_y = dv2_y - 2 * view_size ;
    pixel_size = ((double)(dv2_y - dv1_y)) / can_size ;
}

void
zoom_sub(job)
    char *job;
{
    char command[MAXCOMM];
    char center_type[MAXCOMM];
    char particle_type[MAXCOMM] ;
    int center_box ;
    Real center[MAXDIM];
    double factor;
    double x, y;
    int ix, iy, j;
    double offset_x, offset_y ;
    double span_x, span_y ;
  
    if ((sscanf(job, "%s %s %d %lf",command,center_type,&center_box,&factor)
	    == 4) && (strcmp(center_type, "pot") == 0)) {
      if(boxes_loaded[center_box]){
	pot_center(center, center_box);
      }
      else {
	printf("<sorry, box %d is not loaded, %s>\n",center_box,title) ;
	return;
      }
    }
    else if ((sscanf(job, "%s %s %d %s %lf",command,center_type,
	 &center_box,particle_type,&factor) == 5) && 
	 (strcmp(center_type, "com") == 0)) {
      if(boxes_loaded[center_box]){
	if (strcmp(particle_type,"dark") != 0 && 
		strcmp(particle_type,"gas") != 0 &&
		strcmp(particle_type,"star") != 0 &&
		strcmp(particle_type,"baryon") != 0 &&
		strcmp(particle_type,"all") !=0) {
	    printf("<sorry, %s is not a particle type, %s>\n",
		    particle_type,title) ;
	    return ;
	}
	if (strcmp(particle_type,"dark") == 0){
	    setvec(center,boxes[center_box].dark_com) ;
	}
	else if (strcmp(particle_type,"star") == 0){
	    setvec(center,boxes[center_box].star_com) ;
	}
	else if (strcmp(particle_type,"gas") == 0){
	    setvec(center,boxes[center_box].gas_com) ;
	}
	else if (strcmp(particle_type,"baryon") == 0){
	    mass_add_vec(center,boxes[center_box].gas_com,
		    boxes[center_box].gas_mass, 
		    boxes[center_box].star_com,boxes[center_box].star_mass) ;
	}
	else{
	    setvec(center,boxes[center_box].total_com) ;
	}
      }
      else {
	printf("<sorry, box %d is not loaded, %s>\n",center_box,title) ;
	return;
      }
    }
    else if (sscanf(job, "%s %f %f %f %lf",command,&center[0],
		    &center[1],&center[2],&factor) == 5) {
      /* got what we needed */
    }
    else if (sscanf(job, "%s %lf",command,&factor) == 2) {
	setvec(center, boxes[active_box].center);
    }
    else {
	input_error(command) ;
	return;
    }

    if (view_size == INTMAX ) {
	view_size = (view_size + 1) / factor ;
    }
    else {
	view_size /= factor ;
    }
    for (x = y = 0.0,j = 0 ;j < header.ndim ;j++) {
	x += rot_matrix[0][j] * (center[j] - boxes[active_box].center[j]) ;
	y += rot_matrix[1][j] * (center[j] - boxes[active_box].center[j]) ;
    }
    ix = (int)(x * scaling +0.5) ;
    iy = (int)(y * scaling +0.5) ;

    span_x = dv2_x - dv1_x ;
    offset_x = can_width/2.
      - can_size * ((dv2_x + dv1_x)/ (double)span_x)/2. ;
    span_y = dv1_y - dv2_y ;
    offset_y = can_height/2.
      - can_size * ((dv2_y + dv1_y)/ (double)span_y)/2. ;
    ix = can_size * ((ix)/(double)span_x) + offset_x ;
    iy = can_size * ((iy)/(double)span_y) + offset_y ;
    
    zoom_factor = (double) INTMAX/(double) view_size;
    reset_zoom_scroll();
    zoom_window(ix, iy);
    plot_sub(NULL);
}

extern Widget zoomscrollbar;

void
reset_zoom_scroll()
{
    XawScrollbarSetThumb(zoomscrollbar, 0.5, -1.0);
}


static double lastzoomfactor = 1.0;

void
zoom_scroll(scroll, ignore, top_ptr)
     Widget scroll;
     XtPointer ignore, top_ptr;
{
  double factor;
  
  factor = pow(2.0, 2.0*((*(float *) top_ptr)) - 1.0);
  lastzoomfactor = factor;
  view_size = INTMAX/(zoom_factor*factor);
  zoom_window(can_width/2, can_height/2);
  plot_sub(NULL);
}

/* test */
void
zoom_report_down(Widget w,
	    XtPointer calldata, 
	    XtPointer ev) {
  scrollval_save = lastzoomfactor;
}


/* test */
void
zoom_report_up(Widget w,
	    XtPointer calldata, 
	    XtPointer ev) {
  if (xverbose) printf("zoom by %f\n", lastzoomfactor / scrollval_save);
}
