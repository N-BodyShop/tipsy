#include "defs.h"
#include "fdefs.h"

void
load_color_table()
{
    int i ;
    int size;

  if(mono != YES) 
    {
    if (array_plot) {
      for(i = 2; i < CMAPSIZE-1; i++)
	{
	    clist[i].red = arraymap_red[i-2] << 8 ;
	    clist[i].green = arraymap_green[i-2] << 8 ;
	    clist[i].blue = arraymap_blue[i-2] << 8 ;
	    size = 127;
        }
    }
    else if (radial_plot) {
      for(i = 2; i < CMAPSIZE-1; i++)
	{
	    clist[i].red = rainbow_red[i-2] << 8 ;
	    clist[i].green = rainbow_green[i-2] << 8 ;
	    clist[i].blue = rainbow_blue[i-2] << 8 ;
	    size = 127;
        }
    }
    else if(gas_plot || star_plot || potential_plot || magnitude_plot){
      for(i = 2; i < CMAPSIZE-1; i++)
	{
	    clist[i].red = wrbb_red[i-2] << 8 ;
	    clist[i].green = wrbb_green[i-2] << 8 ;
	    clist[i].blue = wrbb_blue[i-2] << 8 ;
	    size = 127;
	}
    }
    else{
      for(i = 0; i < CMAPSIZE-1; i++)
	{
	    clist[i].red = colormap_red[i] << 8;
	    clist[i].green = colormap_green[i] << 8;
	    clist[i].blue = colormap_blue[i] << 8;
	    size = CMAPSIZE;
	}
    }
    if(truecolor != YES) {
	XStoreColors(baseframe_dpy, default_cmap, clist, size);
	}
    else {
	extern Widget colorbar ;

	XFreeColors(baseframe_dpy, default_cmap, colors, CMAPSIZE, 0);
	for(i = 0; i < CMAPSIZE ; i++){
	    XAllocColor(baseframe_dpy, default_cmap,
			&clist[i]);
	    colors[i] = clist[i].pixel;
	    XSetForeground(baseframe_dpy,gc_color[i],colors[i]) ;
	}
	bar_resize_proc(colorbar, NULL, NULL, NULL);
    }
    reset_color(NULL);
  }
}
