/* $Header$
 * $Log$
 * Revision 1.3  1999/08/25 22:05:24  nsk
 * added center to boxstat, checks for periodic in smooth, prints out
 * cooling stuff, vista makes plots
 *
 * Revision 1.2  1995/11/21  00:02:49  trq
 * Changed output format to be a little more usable.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:23  trq
 * Import to CVS
 *
 * Revision 2.2  94/04/20  08:45:24  trq
 * Added title variable.
 * 
 */
#include "defs.h"

void input_error();

void
boxstat_sub(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    int box ;
    char type[MAXCOMM] ;
    double rho ;
    double mass ;
    double x_size,y_size,z_size ;

    if(sscanf(job,"%s %d %s",command,&box,type) == 3) {
	if (boxes_loaded[box]) {
	    x_size = sqrt((boxes[box].x4[0]-boxes[box].x2[0]) * 
		    (boxes[box].x4[0]-boxes[box].x2[0]) +
		    (boxes[box].x4[1]-boxes[box].x2[1]) *
		    (boxes[box].x4[1]-boxes[box].x2[1]) +
		    (boxes[box].x4[2]-boxes[box].x2[2]) *
		    (boxes[box].x4[2]-boxes[box].x2[2])) ;
	    y_size = sqrt((boxes[box].x2[0]-boxes[box].x1[0]) * 
		    (boxes[box].x2[0]-boxes[box].x1[0]) +
		    (boxes[box].x2[1]-boxes[box].x1[1]) *
		    (boxes[box].x2[1]-boxes[box].x1[1]) +
		    (boxes[box].x2[2]-boxes[box].x1[2]) *
		    (boxes[box].x2[2]-boxes[box].x1[2])) ;
	    z_size = sqrt((boxes[box].x5[0]-boxes[box].x3[0]) * 
		    (boxes[box].x5[0]-boxes[box].x3[0]) +
		    (boxes[box].x5[1]-boxes[box].x3[1]) *
		    (boxes[box].x5[1]-boxes[box].x3[1]) +
		    (boxes[box].x5[2]-boxes[box].x3[2]) *
		    (boxes[box].x5[2]-boxes[box].x3[2])) ;
	    if (strcmp(type,"gas") == 0 ) {
		rho = boxes[box].gas_mass / boxes[box].volume ;
		printf("<physical parameters of the gas in box %d>\n",box) ;
		printf("number of gas particles = %d\n",boxlist[box].ngas) ;
		printf("mass = %.6e, density = %.6e, volume = %.6e\n",
			boxes[box].gas_mass, rho, boxes[box].volume) ;
		printf("center coordinates = {%.6e %.6e %.6e}\n",
			boxes[box].center[0],boxes[box].center[1],
			boxes[box].center[2]) ;
		printf("size = (%.6e x %.6e x %.6e)\n",x_size,y_size,z_size) ;
		printf("center of mass coordinates = {%.6e %.6e %.6e}\n",
			boxes[box].gas_com[0],boxes[box].gas_com[1],
			boxes[box].gas_com[2]) ;
		printf("center of mass velocity = {%.6e %.6e %.6e}\n",
			boxes[box].gas_com_vel[0],boxes[box].gas_com_vel[1],
			boxes[box].gas_com_vel[2]) ;
		printf("angular momentum vector = {%.6e %.6e %.6e}\n",
			boxes[box].gas_angular_mom[0],
			boxes[box].gas_angular_mom[1],
			boxes[box].gas_angular_mom[2]) ;
	    }
	    else if (strcmp(type,"dark") == 0 ) {
		rho = boxes[box].dark_mass / boxes[box].volume ;
		printf("<physical parameters of the dark matter in box %d>\n",
			box) ;
		printf("number of dark particles = %d\n",boxlist[box].ndark) ;
		printf("mass = %.6e, density = %.6e, volume = %.6e\n",
			boxes[box].dark_mass, rho, boxes[box].volume) ;
		printf("center coordinates = {%.6e %.6e %.6e}\n",
			boxes[box].center[0],boxes[box].center[1],
			boxes[box].center[2]) ;
		printf("size = (%.6e x %.6e x %.6e)\n",x_size,y_size,z_size) ;
		printf("center of mass coordinates = {%.6e %.6e %.6e}\n",
			boxes[box].dark_com[0],boxes[box].dark_com[1],
			boxes[box].dark_com[2]) ;
		printf("center of mass velocity = {%.6e %.6e %.6e}\n",
			boxes[box].dark_com_vel[0],boxes[box].dark_com_vel[1],
			boxes[box].dark_com_vel[2]) ;
		printf("angular momentum vector = {%.6e %.6e %.6e}\n",
			boxes[box].dark_angular_mom[0],
			boxes[box].dark_angular_mom[1],
			boxes[box].dark_angular_mom[2]) ;
	    }
	    else if (strcmp(type,"star") == 0 ) {
		rho = boxes[box].star_mass / boxes[box].volume ;
		printf("<physical parameters of the stars in box %d>\n", box) ;
		printf("number of star particles = %d\n",boxlist[box].nstar) ;
		printf("mass = %.6e, density = %.6e, volume = %.6e\n",
			boxes[box].star_mass, rho, boxes[box].volume) ;
		printf("center coordinates = {%.6e %.6e %.6e}\n",
			boxes[box].center[0],boxes[box].center[1],
			boxes[box].center[2]) ;
		printf("size = (%.6e x %.6e x %.6e)\n",x_size,y_size,z_size) ;
		printf("center of mass coordinates = {%.6e %.6e %.6e}\n",
			boxes[box].star_com[0],boxes[box].star_com[1],
			boxes[box].star_com[2]) ;
		printf("center of mass velocity = {%.6e %.6e %.6e}\n",
			boxes[box].star_com_vel[0],boxes[box].star_com_vel[1],
			boxes[box].star_com_vel[2]) ;
		printf("angular momentum vector = {%.6e %.6e %.6e}\n",
			boxes[box].star_angular_mom[0],
			boxes[box].star_angular_mom[1],
			boxes[box].star_angular_mom[2]) ;
	    }
	    else if (strcmp(type,"baryon") == 0 ) {
		mass = (boxes[box].gas_mass + boxes[box].star_mass) ;
		rho = mass / boxes[box].volume ;
		printf("<physical parameters of baryons in box %d>\n", box) ;
		printf("number of gas and star particles = %d and %d\n",
			boxlist[box].ngas, boxlist[box].nstar) ;
		printf("mass = %.6e, density = %.6e, volume = %.6e\n",
			mass, rho,boxes[box].volume) ;
		printf("center coordinates = {%.6e %.6e %.6e}\n",
			boxes[box].center[0],boxes[box].center[1],
			boxes[box].center[2]) ;
		printf("size = (%.6e x %.6e x %.6e)\n",x_size,y_size,z_size) ;
		printf("center of mass coordinates = {%.6e %.6e %.6e}\n",
			(boxes[box].gas_com[0] * boxes[box].gas_mass +
			boxes[box].star_com[0] * boxes[box].star_mass)/mass,
			(boxes[box].gas_com[1] * boxes[box].gas_mass +
			boxes[box].star_com[1] * boxes[box].star_mass)/mass,
			(boxes[box].gas_com[2] * boxes[box].gas_mass +
			boxes[box].star_com[2] * boxes[box].star_mass)/mass);
		printf("center of mass velocity = {%.6e %.6e %.6e}\n",
			(boxes[box].gas_com_vel[0] * boxes[box].gas_mass +
			boxes[box].star_com_vel[0] * boxes[box].star_mass)/
			mass, (boxes[box].gas_com_vel[1] *
			boxes[box].gas_mass + boxes[box].star_com_vel[1] *
			boxes[box].star_mass)/mass,
			(boxes[box].gas_com_vel[2] * boxes[box].gas_mass +
			boxes[box].star_com_vel[2] *
			boxes[box].star_mass)/mass);
		printf("angular momentum vector = {%.6e %.6e %.6e}\n",
			(boxes[box].gas_angular_mom[0] *
			boxes[box].gas_mass +
			boxes[box].star_angular_mom[0] *
			boxes[box].star_mass)/ mass,
			(boxes[box].gas_angular_mom[1] *
			boxes[box].gas_mass +
			boxes[box].star_angular_mom[1] *
			boxes[box].star_mass)/mass,
			(boxes[box].gas_angular_mom[2] *
			boxes[box].gas_mass +
			boxes[box].star_angular_mom[2] *
			boxes[box].star_mass)/mass);
	    }
	    else if (strcmp(type,"all") == 0 ) {
		rho = boxes[box].total_mass / boxes[box].volume ;
		printf("<physical parameters of box %d>\n", box) ;
	     printf("number of dark, gas and star particles = %d, %d, and %d\n",
			boxlist[box].ndark,boxlist[box].ngas,
			boxlist[box].nstar) ;
		printf("mass = %.6e, density = %.6e, volume = %.6e\n",
			boxes[box].total_mass, rho, boxes[box].volume) ;
		printf("center coordinates = {%.6e %.6e %.6e}\n",
			boxes[box].center[0],boxes[box].center[1],
			boxes[box].center[2]) ;
		printf("size = (%.6e x %.6e x %.6e)\n",x_size,y_size,z_size) ;
		printf("center of mass coordinates = {%.6e %.6e %.6e}\n",
			boxes[box].total_com[0],boxes[box].total_com[1],
			boxes[box].total_com[2]) ;
		printf("center of mass velocity = {%.6e %.6e %.6e}\n",
			boxes[box].total_com_vel[0],boxes[box].total_com_vel[1],
			boxes[box].total_com_vel[2]) ;
		printf("angular momentum vector = {%.6e %.6e %.6e}\n",
			boxes[box].total_angular_mom[0],
			boxes[box].total_angular_mom[1],
			boxes[box].total_angular_mom[2]) ;
	    }
	    else {
		printf("<sorry, %s is not a proper data type, %s>\n",type,title) ;
	    }
	}
	else {
	    printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
	}
    }
    else {
	input_error(command) ;
    }
}
