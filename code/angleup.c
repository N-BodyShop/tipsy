#include "defs.h"

void setvec();
void matrix_vector_mult();
void rotate();
void input_error();

void
angleup(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    int box ;
    char type[MAXCOMM] ;
    Real box_angle_mom[MAXDIM] ;
    Real angle_mom[MAXDIM] ;

    if(sscanf(job,"%s %d %s",command,&box,type) == 3) {
	if (boxes_loaded[box]) {
	    if (strcmp(type,"gas") == 0 ) {
		setvec(box_angle_mom,boxes[box].gas_angular_mom) ;
	    }
	    else if (strcmp(type,"dark") == 0 ) {
		setvec(box_angle_mom,boxes[box].dark_angular_mom) ;
	    }
	    else if (strcmp(type,"star") == 0 ) {
		setvec(box_angle_mom,boxes[box].star_angular_mom) ;
	    }
	    else if (strcmp(type,"all") == 0 ) {
		setvec(box_angle_mom,boxes[box].total_angular_mom) ;
	    }
	    else {
		printf("<sorry, %s is not a proper data type, %s>\n",type,title) ;
	    }
	    matrix_vector_mult(rot_matrix,box_angle_mom,angle_mom) ;
	    rotate(CLOCKWISE,-180.0 / PI * (atan2(angle_mom[0],angle_mom[1]))) ;
	    matrix_vector_mult(rot_matrix,box_angle_mom,angle_mom) ;
	    rotate(UP,180.0 / PI * (atan2(angle_mom[2],angle_mom[1]))) ;
	}
	else {
	    printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
