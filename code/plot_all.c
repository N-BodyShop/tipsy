/* $Header$
 * $Log$
 * Revision 1.6  2003/06/13 17:37:37  trq
 * Replaced "include <malloc.h>" with "include <stdlib.h>".  This will allow
 * compilation on MAC OSX.  Also replaced "values.h" with "float.h".
 *
 * Revision 1.5  2002/07/02 21:36:17  trq
 * plot_all.c: fixed bug in plotting marked gas particles.
 *
 * Revision 1.4  1999/08/27 19:01:50  trq
 * Fix placement of psize > 0 points.
 *
 * Revision 1.3  1996/08/19  20:56:03  trq
 * main.c, plot_all.c: allow use of pseudocolor visuals with depth >= 8.
 * nsktrq.c: include fdefs.h.
 *
 * Revision 1.2  1995/05/10  18:32:45  trq
 * Added "dump" parameter to hard command which dumps the backing pixmap in
 * XWD format.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:31  trq
 * Import to CVS
 *
 * Revision 2.14  1994/10/12  21:26:39  trq
 * Cleaned up mallocs.
 *
 * Revision 2.13  1994/04/20  09:06:16  trq
 * deleted extra conversion.
 *
 * Revision 2.12  94/04/15  16:03:04  trq
 * Fix bug in color of marked particles.
 * 
 * Revision 2.11  94/04/08  17:42:22  trq
 * Always fill points.
 * 
 * Revision 2.10  94/03/31  13:57:08  trq
 * Simplified the showvel and showvec logic
 * 
 * Revision 2.9  94/03/07  10:53:13  trq
 * Neal: added logic for showvec display.
 * 
 * Revision 2.7  94/02/16  17:02:39  trq
 * Merged plot_vel and plot_all.
 * 
 * Revision 2.6  94/02/16  09:25:39  trq
 * Made stab at double-buffering the display
 * 
 * Revision 2.5  94/02/09  16:19:42  trq
 * Undid revision 2.3
 * 
 * Revision 2.4  93/11/03  14:34:40  trq
 * Check for span_x == 0
 * 
 * Revision 2.3  1993/05/07  13:26:24  trq
 * Deleted first XClearWindow so picture stays up.
 *
 * Revision 2.2  1993/02/08  10:17:59  trq
 * fixed up point size of marked particles
 *
 * Revision 2.1  91/06/04  15:35:08  trq
 * Changed to use currentview_xid
 * 
 */
#include "defs.h"
#include <stdlib.h>

Pixmap back_xid = 0;
extern Widget canvas;

void
plot_all(job)
     char *job;
{
    int x,y,vx,vy ;
    int *xi,*yi,*vxi,*vyi ;
    int offset_x, offset_y ;
    int span_x, span_y ;
    XPoint box[BOXPTS + 8] ;
    int i,j ;
    int point_size ;
    int draw_point_size ;
    int point_size_m ;
    int offset ;
    int tmp_color;
    static GC back_gc = 0;
    static int h_back, w_back;
    static unsigned char **c_array;

    if(!back_gc)
      {
	unsigned long mask;
	XGCValues values;

	mask = GCFunction | GCForeground;
	values.function = GXcopy;
	values.foreground = colors[0];
	back_gc = XCreateGC(baseframe_dpy, currentview_xid, mask, &values);
      }
    if(back_xid)
      {
	if(can_width != w_back || can_height != h_back)
	  {
	    XFreePixmap(baseframe_dpy, back_xid);
	    back_xid = 0;
	    free(*c_array);
	    free(c_array);
	  }
	else if(job && !strcmp(job, "redraw"))
	  {
	    XCopyArea(baseframe_dpy, back_xid, currentview_xid, back_gc,
		      0, 0, w_back, h_back, 0, 0);
	    XFlush(baseframe_dpy) ;
	    return;
	  }
      }
    if(!back_xid)
      {
	Arg args[1];
	int depth;

	w_back = can_width;
	h_back = can_height;
	c_array = (unsigned char **)malloc(w_back*sizeof(*c_array));
	*c_array = (unsigned char *)malloc(w_back*h_back*sizeof(**c_array));
	for(i = 1; i < w_back; i++)
	  c_array[i] = &(c_array[0][i*h_back]);
	
	XtSetArg(args[0], XtNdepth, &depth);
	XtGetValues(canvas, args, 1);
	back_xid = XCreatePixmap(baseframe_dpy, currentview_xid,
				     w_back, h_back, depth);
	/* XXX - check for bad return? */
      }
    XFillRectangle(baseframe_dpy, back_xid, back_gc, 0, 0, w_back,
		   h_back) ;
    memset(*c_array, 0, w_back*h_back);
    span_x = dv2_x - dv1_x ;
  if(span_x != 0) {
    offset_x = can_width/2 - can_size * ((dv2_x + dv1_x)/ (double)span_x)/2 ;
    span_y = dv1_y - dv2_y ;
    offset_y = can_height/2 - can_size * ((dv2_y + dv1_y)/ (double)span_y)/2 ;
    point_size_m = point_size_mark ;
    point_size_m = max(point_size_m,0) ;
    if(color_type == DARK || (color_type == ALL && showdark)) {
	point_size = point_size_dark ;
	point_size = max(point_size,0) ;
	if (showvel || showvec) {
	    for (xi = dark_x, yi = dark_y, vxi = darkvel_x, vyi = darkvel_y,
		    i = 0 ; i < boxlist[active_box].ndark ; xi++, yi++, vxi++,
		    vyi++, i++) {
		if(mark_dark[boxlist[active_box].dpi[i]]){
		    tmp_color = particle_color[i];
		    particle_color[i] = 255 ; /* give precedence to marks */
		    draw_point_size = point_size_m ;
		}
		else {
		    draw_point_size = point_size ;
		}
		x = can_size * ((*xi)/(double)span_x) + offset_x ;
		y = can_size * ((*yi)/(double)span_y) + offset_y ;
		if(x >=0 && x < w_back && y >= 0 && y < h_back
		   && particle_color[i] > c_array[x][y]){
		    c_array[x][y] = particle_color[i];
		    if(particle_color[i] == 255)
		      particle_color[i] = 1;
		    vx = can_size * ((*vxi)/(double)span_x) + offset_x ;
		    vy = can_size * ((*vyi)/(double)span_y) + offset_y ;
		    XDrawLine(baseframe_dpy,back_xid,
			      gc_color[particle_color[i]],x,y,vx,vy) ;
		    if(draw_point_size) {
		      XFillArc(baseframe_dpy,back_xid,
			       gc_color[particle_color[i]],
			       x-draw_point_size,y-draw_point_size,
			       2*draw_point_size,2*draw_point_size,0,23040) ;
		    }
		}
		if(mark_dark[boxlist[active_box].dpi[i]])
		  particle_color[i] = tmp_color;
	    }
	}
	else {
	    for (xi = dark_x, yi = dark_y,i = 0 ; i < boxlist[active_box].ndark;
		    xi++, yi++, i++) {
		if(mark_dark[boxlist[active_box].dpi[i]]){
		    tmp_color = particle_color[i];
		    particle_color[i] = 255 ;
		    draw_point_size = point_size_m ;
		}
		else{
		    draw_point_size = point_size ;
		}
		x = can_size * ((*xi)/(double)span_x) + offset_x ;
		y = can_size * ((*yi)/(double)span_y) + offset_y ;
		if(x >=0 && x < w_back && y >= 0 && y < h_back
		   && particle_color[i] > c_array[x][y]){
		    c_array[x][y] = particle_color[i];
		    if(particle_color[i] == 255)
		      particle_color[i] = 1;
		    if(draw_point_size) {
		      XFillArc(baseframe_dpy,back_xid,
			       gc_color[particle_color[i]],
			       x-draw_point_size,y-draw_point_size,
			       2*draw_point_size,2*draw_point_size,0,23040) ;
		    }
		    else{
		      XDrawPoint(baseframe_dpy,back_xid,
				 gc_color[particle_color[i]],x,y); 
		    }
		}
		if(mark_dark[boxlist[active_box].dpi[i]])
		  particle_color[i] = tmp_color;
	    }
	}
    }
    if(color_type == GAS || (color_type == ALL && showgas)) {
	if (color_type == ALL) {
	    offset = boxlist[active_box].ndark ;
	}
	else {
	    offset = 0 ;
	}
	point_size = point_size_gas ;
	point_size = max(point_size,0) ;
	if (showvel || showvec) {
	    for (xi = gas_x, yi = gas_y, vxi = gasvel_x, vyi = gasvel_y,
		    i = 0 ; i < boxlist[active_box].ngas ; xi++, yi++, vxi++,
		    vyi++, i++) {
		if(mark_gas[boxlist[active_box].gpi[i]]){
		    tmp_color = particle_color[i+offset];
		    particle_color[i+offset] = 255 ;
		    draw_point_size = point_size_m ;
		}
		else{
		  draw_point_size = point_size ;
		}
		x = can_size * ((*xi)/(double)span_x) + offset_x ;
		y = can_size * ((*yi)/(double)span_y) + offset_y ;
		if(x >=0 && x < w_back && y >= 0 && y < h_back
		   && particle_color[i+offset] > c_array[x][y]){
		    c_array[x][y] = particle_color[i+offset];
		    if(particle_color[i+offset] == 255)
		      particle_color[i+offset] = 1;
		    vx = can_size * ((*vxi)/(double)span_x) + offset_x ;
		    vy = can_size * ((*vyi)/(double)span_y) + offset_y ;
		    XDrawLine(baseframe_dpy,back_xid,
			      gc_color[particle_color[i+offset]],x,y,vx,vy) ;
		    if(draw_point_size) {
		      XFillArc(baseframe_dpy,back_xid,
			       gc_color[particle_color[i+offset]],
			       x-draw_point_size,y-draw_point_size,
			       2*draw_point_size,2*draw_point_size,0,23040) ;
		    }
		}
		if(mark_gas[boxlist[active_box].gpi[i]])
		  particle_color[i+offset] = tmp_color;
	    }
	}
	else {
	    for (xi = gas_x, yi = gas_y,i = 0 ; i < boxlist[active_box].ngas;
		    xi++, yi++, i++) {
		if(mark_gas[boxlist[active_box].gpi[i]]){
		    tmp_color = particle_color[i+offset];
		    particle_color[i+offset] = 255 ;
		    draw_point_size = point_size_m ;
		}
		else{
		  draw_point_size = point_size ;
		}
		x = can_size * ((*xi)/(double)span_x) + offset_x ;
		y = can_size * ((*yi)/(double)span_y) + offset_y ;
		if(x >=0 && x < w_back && y >= 0 && y < h_back
		   && particle_color[i+offset] > c_array[x][y]){
		    c_array[x][y] = particle_color[i+offset];
		    if(particle_color[i+offset] == 255)
		      particle_color[i+offset] = 1;
		    if(draw_point_size) {
		      XFillArc(baseframe_dpy,back_xid,
			       gc_color[particle_color[i+offset]],
			       x-draw_point_size,y-draw_point_size,
			       2*draw_point_size,2*draw_point_size,0,23040) ;
		    }
		    else{
		      XDrawPoint(baseframe_dpy,back_xid,
				 gc_color[particle_color[i+offset]],x,y); 
		    }
		}
		if(mark_gas[boxlist[active_box].gpi[i]])
		  particle_color[i+offset] = tmp_color;
	    }
	}
    }
    if(color_type == STAR || (color_type == ALL && showstar)) {
	if (color_type == ALL) {
	    offset = boxlist[active_box].ndark + boxlist[active_box].ngas ;
	}
	else {
	    offset = 0 ;
	}
	point_size = point_size_star ;
	point_size = max(point_size,0) ;
	if (showvel || showvec) {
	    for (xi = star_x, yi = star_y, vxi = starvel_x, vyi = starvel_y,
		    i = 0 ; i < boxlist[active_box].nstar ; xi++, yi++, vxi++,
		    vyi++, i++) {
		if(mark_star[boxlist[active_box].spi[i]]){
		    tmp_color = particle_color[i+offset];
		    particle_color[i+offset] = 255 ;
		    draw_point_size = point_size_m ;
		}
		else{
		  draw_point_size = point_size ;
		}
		x = can_size * ((*xi)/(double)span_x) + offset_x ;
		y = can_size * ((*yi)/(double)span_y) + offset_y ;
		if(x >=0 && x < w_back && y >= 0 && y < h_back
		   && particle_color[i+offset] > c_array[x][y]){
		    c_array[x][y] = particle_color[i+offset];
		    if(particle_color[i+offset] == 255)
		      particle_color[i+offset] = 1;
		    vx = can_size * ((*vxi)/(double)span_x) + offset_x ;
		    vy = can_size * ((*vyi)/(double)span_y) + offset_y ;
		    XDrawLine(baseframe_dpy,back_xid,
			      gc_color[particle_color[i+offset]],x,y,vx,vy) ;
		    if(draw_point_size) {
		      XFillArc(baseframe_dpy,back_xid,
			       gc_color[particle_color[i+offset]],
			       x-draw_point_size,y-draw_point_size,
			       2*draw_point_size,2*draw_point_size,0,23040) ;
		    }
		}
		if(mark_star[boxlist[active_box].spi[i]])
		  particle_color[i+offset] = tmp_color;
	    }
	}
	else {
	    for (xi = star_x, yi = star_y,i = 0 ; i < boxlist[active_box].nstar;
		    xi++, yi++, i++) {
		if(mark_star[boxlist[active_box].spi[i]]){
		    tmp_color = particle_color[i+offset];
		    particle_color[i+offset] = 255 ;
		    draw_point_size = point_size_m ;
		}
		else{
		  draw_point_size = point_size ;
		}
		x = can_size * ((*xi)/(double)span_x) + offset_x ;
		y = can_size * ((*yi)/(double)span_y) + offset_y ;
		if(x >=0 && x < w_back && y >= 0 && y < h_back
		   && particle_color[i+offset] > c_array[x][y]){
		    c_array[x][y] = particle_color[i+offset];
		    if(particle_color[i+offset] == 255)
		      particle_color[i+offset] = 1;
		    if(draw_point_size) {
		      XFillArc(baseframe_dpy,back_xid,
			       gc_color[particle_color[i+offset]],
			       x-draw_point_size,y-draw_point_size,
			       2*draw_point_size,2*draw_point_size,0,23040) ;
		    }
		    else{
		      XDrawPoint(baseframe_dpy,back_xid,
				 gc_color[particle_color[i+offset]],x,y); 
		    }
		}
		if(mark_star[boxlist[active_box].spi[i]])
		  particle_color[i+offset] = tmp_color;
	    }
	}
    }
    if (showboxes) {
	for (i=1 ;i <= MAXBOX ;i++) {
	    if (boxes_loaded[i]) {
		for (j=0 ;j < BOXPTS + 8 ;j++) {
		    box[j].x = can_size * (plotbox[i][j].x/(double)span_x)
				 + offset_x ;
		    box[j].y = can_size * (plotbox[i][j].y/(double)span_y)
				 + offset_y ;
		}
		    XDrawLines(baseframe_dpy,back_xid,gc_color[1],box,
			    BOXPTS+8,CoordModeOrigin) ;
	    }
	}
    }
    if (showaxes) {
	for (j=0 ;j < 6 ;j++) {
	    box[j].x = can_size * (plotaxes[j].x/(double)span_x) + offset_x ;
	    box[j].y = can_size * (plotaxes[j].y/(double)span_y) + offset_y ;
	}
	    XDrawLines(baseframe_dpy,back_xid,gc_color[1],box,6,
		    CoordModeOrigin) ;
    }
  }
    XCopyArea(baseframe_dpy, back_xid, currentview_xid, back_gc,
	      0, 0, w_back, h_back, 0, 0);
    XFlush(baseframe_dpy) ;
}
