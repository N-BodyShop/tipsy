#include "defs.h"
#include "fdefs.h"

void
loadbinary_sub(job)
    char *job;
{
    char command[MAXCOMM] ;
    double time ;
    int k;
    Real center[MAXDIM];
    Real xoffset[MAXDIM];
    Real xmax[MAXDIM];
    Real xmin[MAXDIM];

    if (!binaryopen){
	printf("<sorry, binary file not open, %s>\n",title) ;
    }
    else {
	if (sscanf(job,"%s %lf %f %f %f %f %f %f",command,&time,
		   &center[0], &center[1], &center[2], &xoffset[0],
		   &xoffset[1], &xoffset[2]) == 8) {
	    for(k = 0; k < MAXDIM; k++){
		xmax[k] = center[k] + xoffset[k] ;
		xmin[k] = center[k] - xoffset[k] ;
	    }
	    if(loadbin_box(binaryfile.ptr, time, xmin, xmax) == FALSE)
		return;
	    boxes_loaded[0] = NO ;
	    unload_all() ;
	    active_box = 0 ;
	    binary_loaded = LOADED ;
	    current_project = NO ;
	    current_color = NO ;
	    redshift_loaded = NO ;
	    divv_loaded = NO ;
	    cooling_loaded = NO ;
	    starform_loaded = NO ;
	    dudt_loaded = NO ;
	    hneutral_loaded = NO ;
	    meanmwt_loaded = NO ;
	    xray_loaded = NO ;
	}
	else if (sscanf(job,"%s %lf",command,&time) == 2) {
	    if(loadbinary(binaryfile.ptr,time) == FALSE)
		return;
	    boxes_loaded[0] = NO ;
	    unload_all() ;
	    active_box = 0 ;
	    binary_loaded = LOADED ;
	    current_project = NO ;
	    current_color = NO ;
	    redshift_loaded = NO ;
	    divv_loaded = NO ;
	    cooling_loaded = NO ;
	    starform_loaded = NO ;
	    dudt_loaded = NO ;
	    hneutral_loaded = NO ;
	    meanmwt_loaded = NO ;
	    xray_loaded = NO ;
	}
	else {
	    input_error(command) ;
	}
    }
}

void
loadstandard_sub(job)
    char *job;
{
    char command[MAXCOMM] ;
    double time ;
    int k;
    Real center[MAXDIM];
    Real xoffset[MAXDIM];
    Real xmax[MAXDIM];
    Real xmin[MAXDIM];

    if (!binaryopen){
	printf("<sorry, binary file not open, %s>\n",title) ;
    }
    else {
	if (sscanf(job,"%s %lf %f %f %f %f %f %f",command,&time,
		   &center[0], &center[1], &center[2], &xoffset[0],
		   &xoffset[1], &xoffset[2]) == 8) {
	    for(k = 0; k < MAXDIM; k++){
		xmax[k] = center[k] + xoffset[k] ;
		xmin[k] = center[k] - xoffset[k] ;
	    }
	    if(loadstd_box(binaryfile.ptr, time, xmin, xmax) == FALSE)
		return;
	    boxes_loaded[0] = NO ;
	    unload_all() ;
	    active_box = 0 ;
	    binary_loaded = LOADED ;
	    current_project = NO ;
	    current_color = NO ;
	    redshift_loaded = NO ;
	    divv_loaded = NO ;
	    cooling_loaded = NO ;
	    starform_loaded = NO ;
	    dudt_loaded = NO ;
	    hneutral_loaded = NO ;
	    meanmwt_loaded = NO ;
	    xray_loaded = NO ;
	}
	else if (sscanf(job,"%s %lf",command,&time) == 2) {
	    if(loadstandard(binaryfile.ptr,time) == FALSE)
		return;
	    boxes_loaded[0] = NO ;
	    unload_all() ;
	    active_box = 0 ;
	    binary_loaded = LOADED ;
	    current_project = NO ;
	    current_color = NO ;
	    redshift_loaded = NO ;
	    divv_loaded = NO ;
	    cooling_loaded = NO ;
	    starform_loaded = NO ;
	    dudt_loaded = NO ;
	    hneutral_loaded = NO ;
	    meanmwt_loaded = NO ;
	    xray_loaded = NO ;
	}
	else {
	    input_error(command) ;
	}
    }
}
