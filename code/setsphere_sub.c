#include "defs.h"
#include "fdefs.h"

void
setsphere_sub(job)
    char *job;
{
    char command[MAXCOMM] ;
    char center_type[MAXCOMM] ;
    char particle_type[MAXCOMM] ;
    int center_box ;
    Real center[MAXDIM] ;
    Real radius ;
    int box ;

    if ((sscanf(job, "%s %d %s %d %f",command,&box,center_type,&center_box,
	 &radius) == 5) && (strcmp(center_type, "pot") == 0)) {
      if(boxes_loaded[center_box]){
	pot_center(center, center_box);
      }
      else {
	printf("<sorry, box %d is not loaded, %s>\n",center_box,title) ;
	return;
      }
    }
    else if ((sscanf(job, "%s %d %s %d %s %f",command,&box,center_type,
	 &center_box,particle_type,&radius) == 6) && 
	 (strcmp(center_type, "com") == 0)) {
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
    else if (sscanf(job, "%s %d %f %f %f %f",command,&box,&center[0],
		    &center[1],&center[2],&radius) != 6) {
	input_error(command) ;
	return;
    }
    if (!redshift_loaded ){
        if(load_redshift()==0){
            return ;
        }
    }
	if(boxes_loaded[0] || (box == 0 && binary_loaded)){
		if(box <= MAXBOX  && box >= 0){
		boxes[box].x1[0] = center[0]-radius ;
		boxes[box].x1[1] = center[1]+radius ;
		boxes[box].x1[2] = center[2]+radius ;
		boxes[box].x2[0] = center[0]-radius ;
		boxes[box].x2[1] = center[1]-radius ;
		boxes[box].x2[2] = center[2]+radius ;
		boxes[box].x3[0] = center[0]+radius ;
		boxes[box].x3[1] = center[1]+radius ;
		boxes[box].x3[2] = center[2]+radius ;
		boxes[box].x4[0] = center[0]+radius ;
		boxes[box].x4[1] = center[1]-radius ;
		boxes[box].x4[2] = center[2]+radius ;
		boxes[box].x5[0] = center[0]+radius ;
		boxes[box].x5[1] = center[1]+radius ;
		boxes[box].x5[2] = center[2]-radius ;
		boxes[box].x6[0] = center[0]+radius ;
		boxes[box].x6[1] = center[1]-radius ;
		boxes[box].x6[2] = center[2]-radius ;
		loadsphere(box,center,radius) ;
		if(box == 0){
		    unload_all() ;
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
