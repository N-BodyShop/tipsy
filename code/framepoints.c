#include "defs.h"
framepoints(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    char file_root[MAXCOMM] ;
    char particle_type[MAXCOMM] ;
    int box ;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
    double part_pos[2] ;
    int i,j,k ;
    char projection_type[MAXCOMM] ;

    if (sscanf(job,"%s %d %s %s",command,&box,
	    particle_type,file_root) == 4) { 
	if(strcmp(particle_type,"star") != 0 &&
		strcmp(particle_type,"gas") != 0 &&
		strcmp(particle_type,"dark") != 0 &&
		strcmp(particle_type,"gasandstar") != 0 &&
		strcmp(particle_type,"darkandstar") != 0 &&
		strcmp(particle_type,"darkandgas") != 0 &&
		strcmp(particle_type,"all") != 0){
	    printf("<sorry, %s is not a particle option, %s>\n",
		    particle_type,title) ;
	    return ;
	}
	if (boxes_loaded[box]) {
	    for(k = 0 ; k < MAXDIM ; k++){
		if(k == 0){
		    clear_rot() ;
		    rotate(LEFT,90.0) ;
		    rotate(CLOCKWISE,90.0) ;
		    sprintf(projection_type,"x") ;
		}
		else if(k == 1){
		    clear_rot() ;
		    rotate(UP,90.0) ;
		    rotate(LEFT,180.0) ;
		    sprintf(projection_type,"y") ;
		}
		else{
		    clear_rot() ;
		    sprintf(projection_type,"z") ;
		}
		if(strcmp(particle_type,"dark")==0 ||
			strcmp(particle_type,"darkandgas") == 0 ||
			strcmp(particle_type,"darkandstar") == 0 ||
			strcmp(particle_type,"all") == 0){
		    sprintf(hardfile.name,"%s%sdark",file_root,projection_type) ;
		    hardfile.ptr = fopen(hardfile.name,"w");
		    for (i = 0 ;i < boxlist[active_box].ndark ;i++) {
			dp = boxlist[active_box].dp[i] ;
			for (part_pos[0] = part_pos[1] = 0.0,j = 0 ;
				j < header.ndim ;j++) {
			    part_pos[0] += rot_matrix[0][j] * (dp->pos[j] -
				    boxes[active_box].center[j]) ;
			    part_pos[1] += rot_matrix[1][j] * (dp->pos[j] -
				    boxes[active_box].center[j]) ;
			}
			fprintf(hardfile.ptr,"%g %g\n",part_pos[0],part_pos[1]);
		    }
		}
		fclose(hardfile.ptr) ;
		if(strcmp(particle_type,"gas") == 0 ||
			strcmp(particle_type,"gasandstar") == 0 ||
			strcmp(particle_type,"darkandgas")== 0 ||
			strcmp(particle_type,"all") == 0){
		    sprintf(hardfile.name,"%s%sgas",
			    file_root,projection_type) ;
		    hardfile.ptr = fopen(hardfile.name,"w");
		    for (i = 0 ;i < boxlist[active_box].ngas ;i++) {
			gp = boxlist[active_box].gp[i] ;
			for (part_pos[0] = part_pos[1] = 0.0,j = 0 ;
				j < header.ndim ;j++) {
			    part_pos[0] += rot_matrix[0][j] * (gp->pos[j] -
				    boxes[active_box].center[j]) ;
			    part_pos[1] += rot_matrix[1][j] * (gp->pos[j] -
				    boxes[active_box].center[j]) ;
			}
			fprintf(hardfile.ptr,"%g %g\n",
				part_pos[0],part_pos[1]) ;
		    }
		    fclose(hardfile.ptr) ;
		}
		if(strcmp(particle_type,"star") == 0 ||
			strcmp(particle_type,"gasandstar") == 0 ||
			strcmp(particle_type,"darkandstar") == 0 ||
			strcmp(particle_type,"all") == 0){
		    sprintf(hardfile.name,"%s%sstar",
			    file_root,projection_type) ;
		    hardfile.ptr = fopen(hardfile.name,"w");
		    for (i = 0 ;i < boxlist[active_box].nstar ;i++) {
			sp = boxlist[active_box].sp[i] ;
			for (part_pos[0] = part_pos[1] = 0.0,j = 0 ;
				j < header.ndim ;j++) {
			    part_pos[0] += rot_matrix[0][j] * (sp->pos[j] -
				    boxes[active_box].center[j]) ;
			    part_pos[1] += rot_matrix[1][j] * (sp->pos[j] -
				    boxes[active_box].center[j]) ;
			}
			fprintf(hardfile.ptr,"%g %g\n",
				part_pos[0],part_pos[1]) ;
		    }
		    fclose(hardfile.ptr) ;
		}
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
