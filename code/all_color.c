/*
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:36  trq
 * Initial revision
 *
 * Revision 1.2  94/04/20  08:45:09  trq
 * Added title variable.
 * 
 */
#include <stdlib.h>
#include "defs.h"
#include "fdefs.h"

void
all_color() 
{
    int i,k ;
    
    k = 0 ;
    if(particle_color != NULL) free(particle_color);
    if(boxlist[active_box].ngas + boxlist[active_box].ndark
       + boxlist[active_box].nstar != 0) {
	particle_color = (int *)malloc((boxlist[active_box].ngas
	    + boxlist[active_box].ndark + boxlist[active_box].nstar)
	    *sizeof(*particle_color));
	if(particle_color == NULL) {
	    printf("<sorry, no memory for particle colors, %s>\n",title) ;
	    return ;
	}
    }
    else
      particle_color = NULL;

    if ( color_type == DARK || color_type == ALL ) {
	for (i = 0 ;i < boxlist[active_box].ndark ;i++, k++) {
	  particle_color[k] = 2;
	}
    }
    if ( color_type == GAS || color_type == ALL ) {
	for (i = 0 ;i < boxlist[active_box].ngas ;i++, k++) {
	  particle_color[k] = 3;
	}
    }
    if ( color_type == STAR || color_type == ALL ) {
	for (i = 0 ;i < boxlist[active_box].nstar ;i++, k++) {
	  particle_color[k] = 4;
	}
    }
    particle_color = (int *)realloc(particle_color, k*sizeof(*particle_color));
    current_color = YES;
}
