#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>
void
writeiv(job)
    char *job;
{
  char command[MAXCOMM] ;
  char file[MAXCOMM] ;
  int i,j ;
  int offset ;
  int tmp_color;
  struct gas_particle *gp ;
  struct dark_particle *dp ;
  struct star_particle *sp ;
  double fraction ;
  extern void writeAxes();
  extern void writeWireframe();
  int *used_dark = NULL;
  int *used_gas = NULL;
  int *used_star = NULL;
  int first_flag ;
  int npart ;

  if (sscanf(job,"%s %lf %s",command,&fraction,file) == 3) {
    if(binary_loaded == UNLOADED || boxes_loaded[0] == NO){
      printf("<sorry, box 0 is not loaded, %s>\n",title) ;
      return;
    }
    if(!current_project || !current_color) {
	printf("<plot does not represent the present internal state") ;
	printf(", %s>\n",title) ;
	return;
    }
    sprintf(hardfile.name,"%s",file) ;
    hardfile.ptr = fopen(hardfile.name,"w");
    if(hardfile.ptr == NULL){
      printf("<sorry,  can't open file %s, %s>\n",hardfile.name,title);
      return;
    }
    srand(0) ;
    fprintf(hardfile.ptr,"#Inventor V2.1 ascii\n\n");
    writeAxes();
    writeWireframe();
    fprintf(hardfile.ptr,"Separator {\n PackedColor {\n  orderedRGBA [ ");
    first_flag = 1 ;
    npart = 0 ;
    if(color_type == DARK || (color_type == ALL && showdark)) {
      used_dark = (int *)malloc(boxlist[active_box].ndark*sizeof(*used_dark));
      if(used_dark == NULL) {
	  printf("<sorry, not enough memory, %s>\n",title) ;  
	  return ;
      }
      for (i = 0 ; i < boxlist[active_box].ndark; i++) {
        if(((double)rand()/(double)RAND_MAX) <= fraction){
	  npart++ ;
	  if (first_flag){
	    first_flag = 0 ;
	  }
	  else{
	    fprintf(hardfile.ptr,",\n   ");
	  }
	  if(mark_dark[boxlist[active_box].dpi[i]]){
	    tmp_color = particle_color[i];
	    particle_color[i] = 1;
	  }
	  j = particle_color[i] ;
	  fprintf(hardfile.ptr,"0x%x", (unsigned int)(
	      (clist[j].red & 0xff00) << 16
	      | (clist[j].green & 0xff00) << 8
	      | (clist[j].blue & 0xff00) | 0xff));
	  if(mark_dark[boxlist[active_box].dpi[i]])
	    particle_color[i] = tmp_color;
	  used_dark[i] = 1 ;
	}
	else{
	  used_dark[i] = 0 ;
	}
      }
    }
    if(color_type == GAS || (color_type == ALL && showgas)) {
      used_gas = (int *)malloc(boxlist[active_box].ngas*sizeof(*used_gas));
      if(used_gas == NULL) {
	  printf("<sorry, not enough memory, %s>\n",title) ;  
	  return ;
      }
      if (color_type == ALL) {
	  offset = boxlist[active_box].ndark ;
      }
      else {
	  offset = 0 ;
      }
      for (i = 0 ; i < boxlist[active_box].ngas; i++) {
        if(((double)rand()/(double)RAND_MAX) <= fraction){
	  npart++ ;
	  if (first_flag){
	    first_flag = 0 ;
	  }
	  else{
	    fprintf(hardfile.ptr,",\n   ");
	  }
	  if(mark_gas[boxlist[active_box].gpi[i]]){
	    tmp_color = particle_color[i+offset];
	    particle_color[i+offset] = 1;
	  }
	  j = particle_color[i+offset] ;
	  fprintf(hardfile.ptr,"0x%x", (unsigned int)(
	      (clist[j].red & 0xff00) << 16
	      | (clist[j].green & 0xff00) << 8
	      | (clist[j].blue & 0xff00) | 0xff));
	  if(mark_gas[boxlist[active_box].gpi[i]])
	    particle_color[i+offset] = tmp_color;
	  used_gas[i] = 1 ;
	}
	else{
	  used_gas[i] = 0 ;
	}
      }
    }
    if(color_type == STAR || (color_type == ALL && showstar)) {
      used_star = (int *)malloc(boxlist[active_box].nstar*sizeof(*used_star));
      if(used_star == NULL) {
	  printf("<sorry, not enough memory, %s>\n",title) ;  
	  return ;
      }
      if (color_type == ALL) {
	offset = boxlist[active_box].ndark + boxlist[active_box].ngas ;
      }
      else{
	offset = 0 ;
      }
      for (i = 0 ; i < boxlist[active_box].nstar; i++) {
        if(((double)rand()/(double)RAND_MAX) <= fraction){
	  npart++ ;
	  if (first_flag){
	    first_flag = 0 ;
	  }
	  else{
	    fprintf(hardfile.ptr,",\n   ");
	  }
	  if(mark_star[boxlist[active_box].spi[i]]){
	    tmp_color = particle_color[i+offset];
	    particle_color[i+offset] = 1;
	  }
	  j = particle_color[i+offset] ;
	  fprintf(hardfile.ptr,"0x%x", (unsigned int)(
	      (clist[j].red & 0xff00) << 16
	      | (clist[j].green & 0xff00) << 8
	      | (clist[j].blue & 0xff00) | 0xff));
	  if(mark_star[boxlist[active_box].spi[i]])
	    particle_color[i+offset] = tmp_color;
	  used_star[i] = 1 ;
	}
	else{
	  used_star[i] = 0 ;
	}
      }
    }
    fprintf(hardfile.ptr," ]\n  }\n");
    fprintf(hardfile.ptr,
	" MaterialBinding {\n  value PER_VERTEX_INDEXED\n }\n");
    fprintf(hardfile.ptr," Coordinate3 {\n  point [ ");
    first_flag = 1 ;
    if(color_type == DARK || (color_type == ALL && showdark)) {
      for (i = 0 ; i < boxlist[active_box].ndark; i++) {
	if(used_dark[i]){
	  if (first_flag){
	    first_flag = 0 ;
	  }
	  else{
	    fprintf(hardfile.ptr,",\n   ");
	  }
	  dp = boxlist[active_box].dp[i] ;
	  fprintf(hardfile.ptr,"%g %g %g",dp->pos[0],dp->pos[1],dp->pos[2]);
	}
      }
    }
    if(color_type == GAS || (color_type == ALL && showgas)) {
      for (i = 0 ; i < boxlist[active_box].ngas; i++) {
	if(used_gas[i]){
	  if (first_flag){
	    first_flag = 0 ;
	  }
	  else{
	    fprintf(hardfile.ptr,",\n   ");
	  }
	  gp = boxlist[active_box].gp[i] ;
	  fprintf(hardfile.ptr,"%g %g %g",gp->pos[0],gp->pos[1],gp->pos[2]);
	}
      }
    }
    if(color_type == STAR || (color_type == ALL && showstar)) {
      for (i = 0 ; i < boxlist[active_box].nstar; i++) {
	if(used_star[i]){
	  if (first_flag){
	    first_flag = 0 ;
	  }
	  else{
	    fprintf(hardfile.ptr,",\n   ");
	  }
	  sp = boxlist[active_box].sp[i] ;
	  fprintf(hardfile.ptr,"%g %g %g",sp->pos[0],sp->pos[1],sp->pos[2]);
	}
      }
    }
    fprintf(hardfile.ptr," ]\n  }\n");
    fprintf(hardfile.ptr," LightModel {\n  model BASE_COLOR\n }\n");
    fprintf(hardfile.ptr," PointSet {\n  numPoints %d\n }\n}\n", npart);
    fclose(hardfile.ptr) ;
    if(used_dark != NULL)free(used_dark) ;
    if(used_gas != NULL)free(used_gas) ;
    if(used_star != NULL)free(used_star) ;
  }
  else {
    input_error(command) ;
  }
}

void writeAxes() {

    Real box_coord[BOXPTS][MAXDIM] ;
    double bound_max[MAXDIM];
    double bound_min[MAXDIM];
    double bound_center[MAXDIM];
    double max_size ;
    double offset ;
    int j,k;

    setvec(&box_coord[0][0],boxes[active_box].x1) ;
    setvec(&box_coord[1][0],boxes[active_box].x2) ;
    setvec(&box_coord[2][0],boxes[active_box].x3) ;
    setvec(&box_coord[3][0],boxes[active_box].x4) ;
    setvec(&box_coord[4][0],boxes[active_box].x5) ;
    setvec(&box_coord[5][0],boxes[active_box].x6) ;
    diff_add_vec(&box_coord[6][0],boxes[active_box].x5,
	    boxes[active_box].x3,boxes[active_box].x1) ;
    diff_add_vec(&box_coord[7][0],boxes[active_box].x6,
	    boxes[active_box].x4,boxes[active_box].x2) ;
    for(k = 0; k < MAXDIM; k++) {
      bound_max[k] = box_coord[0][k];
      bound_min[k] = box_coord[0][k];
    }
    for (j = 1 ;j < BOXPTS ;j++) {
	for(k = 0; k < MAXDIM; k++) {
	  bound_max[k] = max(bound_max[k], box_coord[j][k]);
	  bound_min[k] = min(bound_min[k], box_coord[j][k]);
	}
    }
    max_size = bound_max[0] - bound_min[0] ;
    max_size = max(bound_max[1] - bound_min[1],max_size) ;
    max_size = max(bound_max[2] - bound_min[2],max_size) ;
    for(k = 0; k < MAXDIM; k++) {
      bound_center[k] = 0.5*(bound_min[k]+bound_max[k]);
    }
    offset = .05*max_size ;
  fprintf(hardfile.ptr,"  Separator {\n");
  fprintf(hardfile.ptr,"    DrawStyle {\n");
  fprintf(hardfile.ptr,"      lineWidth     2\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    Separator {\n");
  fprintf(hardfile.ptr,"      BaseColor {\n");
  fprintf(hardfile.ptr,"        rgb	1 0 0\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Coordinate3 {\n");
  fprintf(hardfile.ptr,"        point  [   %g %g %g,\n",bound_center[0]-
	2.*offset,bound_min[1]-offset,bound_min[2]-offset);
  fprintf(hardfile.ptr,"                   %g %g %g ]\n",bound_center[0]+
	2.*offset,bound_min[1]-offset,bound_min[2]-offset);
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      LineSet {\n");
  fprintf(hardfile.ptr,"        numVertices [ 2 ]\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Translation {\n");
  fprintf(hardfile.ptr,"        translation %g %g %g\n",bound_center[0]+
	1.5*offset,bound_min[1]-offset,bound_min[2]-offset);
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      RotationXYZ {\n");
  fprintf(hardfile.ptr,"        axis    Z\n");
  fprintf(hardfile.ptr,"        angle   -1.5708\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Cone {\n");
  fprintf(hardfile.ptr,"        bottomRadius    %g\n",offset/5.);
  fprintf(hardfile.ptr,"        height  %g\n",offset);
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Translation {\n");
  fprintf(hardfile.ptr,"	translation	0 %g 0\n",offset);
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Text2 { \n");
  fprintf(hardfile.ptr,"        string \"X\"\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    Separator {\n");
  fprintf(hardfile.ptr,"      BaseColor {\n");
  fprintf(hardfile.ptr,"        rgb	0 1 0\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Coordinate3 {\n");
  fprintf(hardfile.ptr,"        point  [   %g %g %g,\n",bound_min[0]-offset,
	bound_center[1]-2.*offset,bound_min[2]-offset);
  fprintf(hardfile.ptr,"                   %g %g %g ]\n",bound_min[0]-
	  offset,bound_center[1]+2.*offset,bound_min[2]-offset);
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      LineSet {\n");
  fprintf(hardfile.ptr,"        numVertices [ 2 ]\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Translation {\n");
  fprintf(hardfile.ptr,"        translation %g %g %g\n",bound_min[0]-
	  offset,bound_center[1]+1.5*offset,bound_min[2]-offset);
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Cone {\n");
  fprintf(hardfile.ptr,"        bottomRadius    %g\n",offset/5.);
  fprintf(hardfile.ptr,"        height  %g\n",offset);
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Translation {\n");
  fprintf(hardfile.ptr,"	translation	0 %g 0\n",offset);
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Text2 { \n");
  fprintf(hardfile.ptr,"        string \"Y\"\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    Separator {\n");
  fprintf(hardfile.ptr,"      BaseColor {\n");
  fprintf(hardfile.ptr,"        rgb	0.1 0.1 1\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Coordinate3 {\n");
  fprintf(hardfile.ptr,"        point  [   %g %g %g,\n",bound_min[0]-
	  offset,bound_min[1]-offset,bound_center[2]-2.*offset);
  fprintf(hardfile.ptr,"                   %g %g %g ]\n",bound_min[0]-
	  offset,bound_min[1]-offset,bound_center[2]+2.*offset);
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      LineSet {\n");
  fprintf(hardfile.ptr,"        numVertices [ 2 ]\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Translation {\n");
  fprintf(hardfile.ptr,"        translation %g %g %g\n",bound_min[0]-
	  offset,bound_min[1]-offset,bound_center[2]+1.5*offset);
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      RotationXYZ {\n");
  fprintf(hardfile.ptr,"        axis    X\n");
  fprintf(hardfile.ptr,"        angle   1.5708\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Cone {\n");
  fprintf(hardfile.ptr,"        bottomRadius    %g\n",offset/5.);
  fprintf(hardfile.ptr,"        height  %g\n",offset);
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Translation {\n");
  fprintf(hardfile.ptr,"	translation	0 %g 0\n",offset);
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Text2 { \n");
  fprintf(hardfile.ptr,"        string \"Z\"\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"  }\n");

}


void writeWireframe() {

  Real x7[MAXDIM] ;
  Real x8[MAXDIM] ;

  fprintf(hardfile.ptr,"  Separator {\n");
  fprintf(hardfile.ptr,"    Coordinate3 {\n");
  diff_add_vec(&x7[0],boxes[active_box].x5,boxes[active_box].x3,
	  boxes[active_box].x1) ;
  diff_add_vec(&x8[0],boxes[active_box].x6,boxes[active_box].x4,
	  boxes[active_box].x2) ;
  fprintf(hardfile.ptr,"	point	[ %g %g %g,\n",x8[0],x8[1],x8[2]);
  fprintf(hardfile.ptr,"	           %g %g %g,\n",boxes[active_box].x6[0],
	boxes[active_box].x6[1], boxes[active_box].x6[2]) ;
  fprintf(hardfile.ptr,"	           %g %g %g,\n",boxes[active_box].x5[0],
	boxes[active_box].x5[1], boxes[active_box].x5[2]) ;
  fprintf(hardfile.ptr,"	           %g %g %g,\n",x7[0],x7[1],x7[2]) ;
  fprintf(hardfile.ptr,"	           %g %g %g,\n",x8[0],x8[1],x8[2]) ;
  fprintf(hardfile.ptr,"	           %g %g %g,\n",boxes[active_box].x2[0],
	boxes[active_box].x2[1], boxes[active_box].x2[2]) ;
  fprintf(hardfile.ptr,"	           %g %g %g,\n",boxes[active_box].x4[0],
	boxes[active_box].x4[1], boxes[active_box].x4[2]) ;
  fprintf(hardfile.ptr,"	           %g %g %g\n",boxes[active_box].x6[0],
	boxes[active_box].x6[1], boxes[active_box].x6[2]) ;
  fprintf(hardfile.ptr,"                 ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    LineSet {\n");
  fprintf(hardfile.ptr,"      numVertices [ 8 ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    Coordinate3 {\n");
  fprintf(hardfile.ptr,"	point	[ %g %g %g,\n",boxes[active_box].x2[0],
	boxes[active_box].x2[1], boxes[active_box].x2[2]) ;
  fprintf(hardfile.ptr,"	           %g %g %g,\n",boxes[active_box].x1[0],
	boxes[active_box].x1[1], boxes[active_box].x1[2]) ;
  fprintf(hardfile.ptr,"	           %g %g %g\n",x7[0],x7[1],x7[2]) ;
  fprintf(hardfile.ptr,"                ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    LineSet {\n");
  fprintf(hardfile.ptr,"      numVertices [ 3 ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    Coordinate3 {\n");
  fprintf(hardfile.ptr,"	point	[ %g %g %g,\n",boxes[active_box].x4[0],
	boxes[active_box].x4[1], boxes[active_box].x4[2]) ;
  fprintf(hardfile.ptr,"	           %g %g %g,\n",boxes[active_box].x3[0],
	boxes[active_box].x3[1], boxes[active_box].x3[2]) ;
  fprintf(hardfile.ptr,"	           %g %g %g\n",boxes[active_box].x5[0],
	boxes[active_box].x5[1], boxes[active_box].x5[2]) ;
  fprintf(hardfile.ptr,"                ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    LineSet {\n");
  fprintf(hardfile.ptr,"      numVertices [ 3 ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    Coordinate3 {\n");
  fprintf(hardfile.ptr,"	point	[ %g %g %g,\n",boxes[active_box].x1[0],
	boxes[active_box].x1[1], boxes[active_box].x1[2]) ;
  fprintf(hardfile.ptr,"	           %g %g %g\n",boxes[active_box].x3[0],
	boxes[active_box].x3[1], boxes[active_box].x3[2]) ;
  fprintf(hardfile.ptr,"                ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    LineSet {\n");
  fprintf(hardfile.ptr,"      numVertices [ 2 ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"  }  \n");

}
