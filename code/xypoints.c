#include "defs.h"
#include "fdefs.h"

void
xypoints(job)
    char *job ;
{
    char command[MAXCOMM] ;
    int *xi,*yi,*vxi,*vyi ;
    int i ;
    int offset ;
    int center_x, center_y ;
    double x, y ;
    double vx, vy ;

    if (current_project){
	center_x = (dv2_x + dv1_x) / 2 ;
	center_y = (dv2_y + dv1_y) / 2 ;
	if (sscanf(job,"%s %s",command,hardfile.name) == 2) {
	    hardfile.ptr = fopen(hardfile.name,"w");
	    if (showdark && !gas_plot && !star_plot &&
		    ((!radial_plot && !potential_plot && !magnitude_plot) ||
		    ((radial_plot || potential_plot || magnitude_plot) &&
		    (color_type == DARK || color_type == ALL)))) {
		for (xi = dark_x, yi = dark_y, vxi = darkvel_x,
			vyi = darkvel_y,i = 0 ; i < boxlist[active_box].ndark ;
			xi++, yi++, vxi++,vyi++, i++) {
		    if ( *xi <= dv2_x && *xi >= dv1_x && *yi <= dv2_y &&
			    *yi >= dv1_y && ((!radial_plot &&
			    !potential_plot && !magnitude_plot) ||
			    particle_color[i])) {
			x = ((double)(*xi - center_x)) / scaling ;
			y = ((double)(*yi - center_y)) / scaling ;
			fprintf(hardfile.ptr,"%g %g",x,y) ;
			if (showvel) {
			    vx = ((double)(*vxi - center_x)) / scaling ;
			    vy = ((double)(*vyi - center_y)) / scaling ;
			    fprintf(hardfile.ptr," %g %g",vx,vy) ;
			}
			fprintf(hardfile.ptr,"\n") ;
		    }
		}
	    }
	    if ((showgas || gas_plot) && !star_plot &&
		    ((!radial_plot && !potential_plot && !magnitude_plot) ||
		    ((radial_plot || potential_plot || magnitude_plot) &&
		    (color_type == GAS || color_type == ALL)))) {
		if ((radial_plot || potential_plot || magnitude_plot) &&
		        color_type == ALL) {
		    offset = boxlist[active_box].ndark ;
		}
		else {
		    offset = 0 ;
		}
		for (xi = gas_x, yi = gas_y, vxi = gasvel_x, vyi = gasvel_y,
			i = 0 ; i < boxlist[active_box].ngas ; xi++, yi++,
			vxi++, vyi++, i++) {
		    if ( *xi <= dv2_x && *xi >= dv1_x && *yi <= dv2_y &&
			    *yi >= dv1_y && ((!gas_plot && !radial_plot &&
			    !potential_plot && !magnitude_plot) ||
			    particle_color[i+offset])) {
			x = ((double)(*xi - center_x)) / scaling ;
			y = ((double)(*yi - center_y)) / scaling ;
			fprintf(hardfile.ptr,"%g %g",x,y) ;
			if (showvel) {
			    vx = ((double)(*vxi - center_x)) / scaling ;
			    vy = ((double)(*vyi - center_y)) / scaling ;
			    fprintf(hardfile.ptr," %g %g",vx,vy) ;
			}
			fprintf(hardfile.ptr,"\n") ;
		    }
		}
	    }
	    if ((showstar || star_plot) && !gas_plot &&
		    ((!radial_plot && !potential_plot && !magnitude_plot) ||
		    ((radial_plot || potential_plot || magnitude_plot) &&
		    (color_type == STAR || color_type == ALL)))) {
		if ((radial_plot || potential_plot || magnitude_plot) &&
		       color_type == ALL) {
		    offset = boxlist[active_box].ndark +
			    boxlist[active_box].ngas ;
		}
		else {
		    offset = 0 ;
		}
		for (xi = star_x, yi = star_y, vxi = starvel_x, vyi = starvel_y,
			i = 0 ; i < boxlist[active_box].nstar ; xi++, yi++,
			vxi++, vyi++, i++) {
		    if ( *xi <= dv2_x && *xi >= dv1_x && *yi <= dv2_y &&
			    *yi >= dv1_y && ((!star_plot && !radial_plot &&
			    !potential_plot && !magnitude_plot) ||
			    particle_color[i+offset])) {
			x = ((double)(*xi - center_x)) / scaling ;
			y = ((double)(*yi - center_y)) / scaling ;
			fprintf(hardfile.ptr,"%g %g",x,y) ;
			if (showvel) {
			    vx = ((double)(*vxi - center_x)) / scaling ;
			    vy = ((double)(*vyi - center_y)) / scaling ;
			    fprintf(hardfile.ptr," %g %g",vx,vy) ;
			}
			fprintf(hardfile.ptr,"\n") ;
		    }
		}
	    }
	    fclose(hardfile.ptr) ;
	}
	else {
	    input_error(command) ;
	}
    }
    else {
	printf("<plot does not represent the present internal state, %s>",title) ;
    }
}
