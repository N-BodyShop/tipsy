/* $Header$
 * $Log$
 * Revision 1.4  2003/08/28 16:00:29  trq
 * Set current_color to NO in case xxx_color() routine runs into trouble
 * (usually bad array loaded)
 *
 * Revision 1.3  1997/01/17 21:24:06  trq
 * Added more informative error messages.
 *
 * Revision 1.2  1995/03/24  18:49:10  trq
 * Added "revrain" colormap.
 *
 * divv.c: included malloc.h.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:36  trq
 * Import to CVS
 *
 * Revision 1.4  94/04/20  08:46:23  trq
 * Added title variable.
 * 
 * Revision 1.2  94/02/16  14:06:18  trq
 * Parse everything correctly before we do anything.
 * 
 */
#include "defs.h"
#include "fdefs.h"
void
view_array(job)
    char *job;
{
    char command[MAXCOMM] ;
    char type[MAXCOMM] ;
    char mapname[MAXCOMM] ;
    char clip_type[MAXCOMM] ;
    double low ;
    double high ;
    int i ;

    if (boxes_loaded[0]) {
      if(array_size == 0) 
	{
	  printf("<Sorry %s, there is no array loaded\n",title);
	  return;
	}
	if((i = sscanf(job,"%s %s %s %lf %lf %s",command, type, mapname,
		  &low, &high, clip_type)) >= 5) {
	    if(i == 6){
	        if(strcmp(clip_type,"clip") == 0 ||
			strcmp(clip_type,"c") == 0 ||
			strcmp(clip_type,"clipboth") == 0 ||
			strcmp(clip_type,"cb") == 0 ){
		    clip_flag = BCLIP ;
		}
	        else if(strcmp(clip_type,"clipup") == 0 ||
			strcmp(clip_type,"cup") == 0 ){
		    clip_flag = UPCLIP ;
		}
	        else if(strcmp(clip_type,"cliplo") == 0 ||
			strcmp(clip_type,"clo") == 0 ){
		    clip_flag = LOCLIP ;
		}
		else{
		    printf("<sorry, %s is not a proper option, %s>\n",
			    clip_type,title) ;
		    return ;
		}
	    }
	    else{
		clip_flag = NOCLIP ;
	    }
	    if((strcmp(mapname, "wrbb") == 0) || (strcmp(mapname, "w") == 0))
	      {
		arraymap_red = wrbb_red;
		arraymap_green = wrbb_green;
		arraymap_blue = wrbb_blue;
	      }
	    else if((strcmp(mapname, "rainbow") == 0)
		    || (strcmp(mapname, "r") == 0))
	      {
		arraymap_red = rainbow_red;
		arraymap_green = rainbow_green;
		arraymap_blue = rainbow_blue;
	      }
	    else if((strcmp(mapname, "reverse_rainbow") == 0)
		    || (strcmp(mapname, "revrain") == 0)
		    || (strcmp(mapname, "rr") == 0))
	      {
		arraymap_red = revrain_red;
		arraymap_green = revrain_green;
		arraymap_blue = revrain_blue;
	      }
	    else if((strcmp(mapname, "default") == 0)
		    || (strcmp(mapname, "d") == 0))
	      {
		arraymap_red = colormap_red+2;
		arraymap_green = colormap_green+2;
		arraymap_blue = colormap_blue+2;
	      }
	    else
	      {
		printf("Sorry %s, I don't recognize that colormap\n",title);
		return;
	      }
	
	    if ( strcmp(type,"gas") == 0 ) {
		color_type = GAS ;
                log_type = LIN ;
	    }
	    else if ( strcmp(type,"dark") == 0 ) {
		color_type = DARK ;
                log_type = LIN ;
	    }
	    else if ( strcmp(type,"star") == 0 ) {
		color_type = STAR ;
                log_type = LIN ;
	    }
	    else if ( strcmp(type,"all") == 0 ) {
		color_type = ALL ;
                log_type = LIN ;
	    }
	    else if ( strcmp(type,"loggas") == 0 || strcmp(type,"lgas") == 0 ) {
		color_type = GAS ;
                log_type = LOG ;
	    }
	    else if ( strcmp(type,"logdark") == 0 || strcmp(type,"ldark") == 0){
		color_type = DARK ;
                log_type = LOG ;
	    }
	    else if ( strcmp(type,"logstar") == 0 || strcmp(type,"lstar") == 0){
		color_type = STAR ;
                log_type = LOG ;
	    }
	    else if ( strcmp(type,"logall") == 0 || strcmp(type,"lall") == 0){
		color_type = ALL ;
                log_type = LOG ;
	    }
	    else {
		printf("<%s is an unknown particle type, %s>\n",type,title) ;
		return ;
	    }
	    if(!array_plot) {
		array_plot = YES ;
		potential_plot = NO ;
		magnitude_plot = NO ;
		radial_plot = NO ;
		gas_plot = NO ;
		star_plot = NO ;
	    }
	    load_color_table() ;
	    color_slope = 124.0 / (high - low) ;
	    color_offset = 2.0 - color_slope * low ;
	    project() ;
	    current_color = NO;
	    array_color() ;
	    plot_sub(job) ;
	}
	else {
	    input_error(command) ;
	}
    }
    else {
	printf("<sorry, no boxes are loaded, %s>\n",title) ;
    }
}
