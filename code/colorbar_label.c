/*
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:34  trq
 * Initial revision
 *
 * Revision 2.7  1994/10/07  16:55:15  trq
 * Corrected use of XtGetValues().
 *
 * Revision 2.6  1994/08/31  23:20:13  nsk
 * changed error message
 *
 * Revision 2.5  1994/04/20  08:49:40  trq
 * Made casts from Args array.
 *
 * Revision 2.4  94/03/08  16:08:22  trq
 * Neal: make "master" a variable.
 * 
 * Revision 2.2  94/02/16  14:29:01  trq
 * Added check for array_plot.
 * 
 */
#include "defs.h"
#include "fdefs.h"
#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

extern Widget colorbar;
extern XFontStruct *label_font;
extern char color_left_text[];
extern char color_middle_text[];
extern char color_right_text[];


void
colorbar_label(job)
    char *job;
{
    char command[MAXCOMM] ;
    char type[MAXCOMM] ;
    char label_text[MAXCOMM] ;
    char *find() ;

    if (gas_plot || radial_plot || star_plot || potential_plot || magnitude_plot
	|| array_plot) {
	if(sscanf(job,"%s %s",command,type) >= 2) {
	  if(find( find(job, " ") + 1, " ") != NULL)
	    strcpy( label_text, find( find(job, " ") + 1, " ") + 1) ;
	  else
	    label_text[0] = '\0';
	    if (strcmp(type,"left") == 0 || strcmp(type,"l") == 0) {
	      strcpy(color_left_text, label_text);
	    }
	    else if (strcmp(type,"middle") == 0 || strcmp(type,"m") == 0) {
	      strcpy(color_middle_text, label_text);
	    }
	    else if (strcmp(type,"right") == 0 || strcmp(type,"r") == 0) {
	      strcpy(color_right_text, label_text);
	    }
	    else {
		input_error(command) ;
		return ;
	    }
	    draw_label();
	}
	else {
	    input_error(command) ;
	}
    }
    else {
	printf("<sorry, not a colorbar plot, %s>\n",title) ;
    }
}

void
draw_color_label()
{
    if(*color_left_text)
      {
	int x, y;
	
	x = 10;
	y = label_font->ascent + label_font->descent;
	XDrawString(baseframe_dpy, colorbar_xid, gc_color[127], x, y,
		    color_left_text, strlen(color_left_text));
      }
    if(*color_middle_text)
      {
	int x, y;
	Arg args[1];
	Dimension width;
	int text_width;
	
	XtSetArg(args[0], XtNwidth, &width);
	XtGetValues(colorbar, args, 1);
	text_width = XTextWidth(label_font, color_middle_text,
				strlen(color_middle_text));
	x = (width - text_width)/2;
	y = label_font->ascent + label_font->descent;
	XDrawString(baseframe_dpy, colorbar_xid, gc_color[127], x, y,
		    color_middle_text, strlen(color_middle_text));
      }
    if(*color_right_text)
      {
	int x, y;
	Arg args[1];
	Dimension width;
	int text_width;
	
	XtSetArg(args[0], XtNwidth, &width);
	XtGetValues(colorbar, args, 1);
	text_width = XTextWidth(label_font, color_right_text,
				strlen(color_right_text));
	x = width - (text_width + 10);
	y = label_font->ascent + label_font->descent;
	XDrawString(baseframe_dpy, colorbar_xid, gc_color[127], x, y,
		    color_right_text, strlen(color_right_text));
      }
}
