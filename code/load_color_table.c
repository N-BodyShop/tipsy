#include "defs.h"
void reset_color();

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
  XStoreColors(baseframe_dpy, default_cmap, clist, size);
    reset_color(NULL);
  }
}
