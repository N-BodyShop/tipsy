#include "defs.h"
#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Scrollbar.h>

void
change_color(scroll, colorbar , top_ptr) 
    Widget scroll ;
     XtPointer  colorbar , top_ptr ;
{
  int base_color;
  int i;
  int size;
  
  if(mono != YES && truecolor != YES) 
    {
      
    base_color = ((*(float *) top_ptr)) * (CMAPSIZE-3);
    if (!cmap && array_plot) {
      for(i = 2; i < CMAPSIZE-1; i++)
	{
	    clist[i].red = arraymap_red[base_color + i-2] << 8 ;
	    clist[i].green = arraymap_green[base_color + i-2] << 8 ;
	    clist[i].blue = arraymap_blue[base_color + i-2] << 8 ;
	    size = 127;
        }
    } else if ( (cmap==CM_RAINBOW) || (!cmap && radial_plot) ) {
      for(i = 2; i < CMAPSIZE-1; i++)
	{
	    clist[i].red = rainbow_red[base_color + i-2] << 8 ;
	    clist[i].green = rainbow_green[base_color + i-2] << 8 ;
	    clist[i].blue = rainbow_blue[base_color + i-2] << 8 ;
	    size = 127;
        }
    } else if (cmap==CM_REVRAIN) {
      for(i = 2; i < CMAPSIZE-1; i++)
	{
	    clist[i].red = revrain_red[base_color + i-2] << 8 ;
	    clist[i].green = revrain_green[base_color + i-2] << 8 ;
	    clist[i].blue = revrain_blue[base_color + i-2] << 8 ;
	    size = 127;
	}
    } else if ( (cmap==CM_WRBB) || 
      (!cmap && (gas_plot || star_plot || potential_plot || magnitude_plot))) {
      for(i = 2; i < CMAPSIZE-1; i++)
	{
	    clist[i].red = wrbb_red[base_color + i-2] << 8 ;
	    clist[i].green = wrbb_green[base_color + i-2] << 8 ;
	    clist[i].blue = wrbb_blue[base_color + i-2] << 8 ;
	    size = 127;
	}
    } else if (cmap==CM_TEMP) {
      for(i = 2; i < CMAPSIZE-1; i++)
	{
	    clist[i].red = temp_red[base_color + i-2] << 8 ;
	    clist[i].green = temp_green[base_color + i-2] << 8 ;
	    clist[i].blue = temp_blue[base_color + i-2] << 8 ;
	    size = 127;
	}
    } else {
      for(i = 2; i < CMAPSIZE-1; i++)
	{
	    clist[i].red = colormap_red[base_color + i] << 8 ;
	    clist[i].green = colormap_green[base_color + i] << 8 ;
	    clist[i].blue = colormap_blue[base_color + i] << 8 ;
	    size = 127;
        }
    }
    
    XStoreColors(baseframe_dpy, default_cmap, clist, size);
  }
}
