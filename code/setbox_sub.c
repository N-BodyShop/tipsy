/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:27  trq
 * Initial revision
 *
 * Revision 2.4  1994/12/23  19:26:39  nsk
 * made setbox take offsets
 *
 * Revision 2.3  1994/04/20  09:03:46  trq
 * Return if !binary_loaded.
 *
 * Revision 2.2  94/03/29  10:18:19  trq
 * Neal: added title.
 * 
 */
#include "defs.h"
#include "fdefs.h"

void
setbox_sub(job)
    char *job ;
{
    char command[MAXCOMM] ;
    int box ;
    char center_type[MAXCOMM] ;
    char particle_type[MAXCOMM] ;
    int center_box ;
    Real center[MAXDIM] ;
    Real xoffset[MAXDIM] ;
    Real xmax[MAXDIM] ;
    Real xmin[MAXDIM] ;
    int k ;

    if ((sscanf(job, "%s %d %s %d %f %f %f",command,&box,center_type,
	    &center_box,&xoffset[0],&xoffset[1],&xoffset[2]) == 7) &&
	    (strcmp(center_type, "pot") == 0)) {
      if(boxes_loaded[center_box]){
	pot_center(center, center_box);
      }
      else {
	printf("<sorry, box %d is not loaded, %s>\n",center_box,title) ;
	return;
      }
    }
    else if ((sscanf(job, "%s %d %s %d %s %f %f %f",command,&box,center_type,
	 &center_box,particle_type,&xoffset[0],&xoffset[1],&xoffset[2]) == 8)
	 && (strcmp(center_type, "com") == 0)) {
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
    else if (sscanf(job, "%s %d %f %f %f %f %f %f",command,&box,&center[0],
		    &center[1],&center[2],&xoffset[0],&xoffset[1],&xoffset[2])
		    != 8) {
	input_error(command) ;
	return;
    }
    if(!binary_loaded) {
	printf("<sorry, no binary is loaded, %s>\n",title) ;
	return;
    }
    for(k = 0; k < header.ndim; k++){
	xmax[k] = center[k] + xoffset[k] ;
	xmin[k] = center[k] - xoffset[k] ;
    }
    if(boxes_loaded[0] || box == 0){
	if(box <= MAXBOX  && box >= 0){
	    if(xmax[0] > xmin[0] && xmax[1] > xmin[1] && xmax[2] > xmin[2]){
		setsize(box,xmax,xmin) ;
		loadbox(box) ;
		if(box == 0){
		    unload_all() ;
		}
	    }
	    else{
		printf("<sorry, min is greater than max, %s>\n",title) ;
	    }
	}
	else{
	    printf("<sorry, box number %d is not allowed, %s>\n",box,title) ;
	}
    }
    else if(box){
	printf("<sorry, box 0 not loaded, %s>\n",title) ;
    }
    else{
	printf("<sorry, no binary is loaded, %s>\n",title) ;
    }
}
