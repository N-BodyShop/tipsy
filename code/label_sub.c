#include "defs.h"
#include "fdefs.h"

extern XFontStruct *label_font;
extern char label_text[];
void draw_label();
static int label_x;
static int label_y;
static int justify;

void
label_sub(job)
    char *job;
{
    char command[MAXCOMM];
    double fx, fy;

    if(!current_project || !current_color){
	printf("<sorry, plot does not represent internal state, %s>\n",title) ;
	return ;
    }
    if(sscanf(job, "%s %lf %lf %d %[ -~]", command, &fx, &fy, &justify,
	      label_text) == 5) {
	label_x = fx;
	label_y = can_height - fy;
	draw_label();
    }
    else {
	input_error(command);
    }
}

    
void
draw_label()    
{
    if(*label_text)
      {
	  int length;
	  int x, y;
	  
	  x = label_x;
	  y = label_y;
	  length = XTextWidth(label_font, label_text, strlen(label_text));
	  switch(justify) {
	  case 1:
	      x -= length;
	      y += label_font->ascent;
	      break;
	  case 2:
	      x -= length/2;
	      y += label_font->ascent;
	      break;
	  case 3:
	      y += label_font->ascent;
	      break;
	  case 4:
	      x -= length;
	      y += (label_font->ascent - label_font->descent)/2;
	      break;
	  case 5:
	      x -= length/2;
	      y += (label_font->ascent - label_font->descent)/2;
	      break;
	  case 6:
	      y += (label_font->ascent - label_font->descent)/2;
	      break;
	  case 7:
	      x -= length;
	      y -= label_font->descent;
	      break;
	  case 8:
	      x -= length/2;
	      y -= label_font->descent;
	      break;
	  case 9:
	      y -= label_font->descent;
	      break;
	  default:
	      printf("<sorry, %d is not a valid position, %s>\n",
		     justify, title) ;
	      return;
	  }
	      
	  XDrawString(baseframe_dpy, currentview_xid, gc_color[127], x, y,
		    label_text, strlen(label_text));
      }
    draw_color_label();
    XFlush(baseframe_dpy);
}
