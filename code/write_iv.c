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
  int *used_dark ;
  int *used_gas ;
  int *used_star ;
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
	    particle_color[i] = tmp_color;
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
	    particle_color[i+offset] = tmp_color;
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
	    particle_color[i+offset] = tmp_color;
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

  fprintf(hardfile.ptr,"  Separator {\n");
  fprintf(hardfile.ptr,"    DrawStyle {\n");
  fprintf(hardfile.ptr,"      lineWidth     2\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    Separator {\n");
  fprintf(hardfile.ptr,"      BaseColor {\n");
  fprintf(hardfile.ptr,"        rgb	1 0 0\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Coordinate3 {\n");
  fprintf(hardfile.ptr,"        point  [   -0.1 -0.55 -0.55,\n");
  fprintf(hardfile.ptr,"                   0.1  -0.55 -0.55 ]\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      LineSet {\n");
  fprintf(hardfile.ptr,"        numVertices [ 2 ]\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Translation {\n");
  fprintf(hardfile.ptr,"        translation 0.075 -0.55 -0.55\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      RotationXYZ {\n");
  fprintf(hardfile.ptr,"        axis    Z\n");
  fprintf(hardfile.ptr,"        angle   -1.5708\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Cone {\n");
  fprintf(hardfile.ptr,"        bottomRadius    0.02\n");
  fprintf(hardfile.ptr,"        height  0.05\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Translation {\n");
  fprintf(hardfile.ptr,"	translation	0 0.05 0\n");
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
  fprintf(hardfile.ptr,"        point  [   -0.55 -0.1 -0.55,\n");
  fprintf(hardfile.ptr,"                   -0.55  0.1 -0.55 ]\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      LineSet {\n");
  fprintf(hardfile.ptr,"        numVertices [ 2 ]\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Translation {\n");
  fprintf(hardfile.ptr,"        translation -0.55 0.075 -0.55\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Cone {\n");
  fprintf(hardfile.ptr,"        bottomRadius    0.02\n");
  fprintf(hardfile.ptr,"        height  0.05\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Translation {\n");
  fprintf(hardfile.ptr,"	translation	0 0.05 0\n");
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
  fprintf(hardfile.ptr,"        point  [   -0.55 -0.55 -0.1,\n");
  fprintf(hardfile.ptr,"                   -0.55 -0.55  0.1 ]\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      LineSet {\n");
  fprintf(hardfile.ptr,"        numVertices [ 2 ]\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Translation {\n");
  fprintf(hardfile.ptr,"        translation -0.55 -0.55 0.075 \n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      RotationXYZ {\n");
  fprintf(hardfile.ptr,"        axis    X\n");
  fprintf(hardfile.ptr,"        angle   1.5708\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Cone {\n");
  fprintf(hardfile.ptr,"        bottomRadius    0.02\n");
  fprintf(hardfile.ptr,"        height  0.05\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Translation {\n");
  fprintf(hardfile.ptr,"	translation    0 0.05 0 \n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"      Text2 { \n");
  fprintf(hardfile.ptr,"        string \"Z\"\n");
  fprintf(hardfile.ptr,"      }\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"  }\n");

}


void writeWireframe() {

  fprintf(hardfile.ptr,"  Separator {\n");
  fprintf(hardfile.ptr,"    Coordinate3 {\n");
  fprintf(hardfile.ptr,"	point	[ -0.5 -0.5 -0.5,\n");
  fprintf(hardfile.ptr,"                   0.5 -0.5 -0.5, \n");
  fprintf(hardfile.ptr,"                   0.5 0.5 -0.5, \n");
  fprintf(hardfile.ptr,"                   -0.5 0.5 -0.5, \n");
  fprintf(hardfile.ptr,"                   -0.5 -0.5 -0.5, \n");
  fprintf(hardfile.ptr,"                   -0.5 -0.5 0.5, \n");
  fprintf(hardfile.ptr,"                   0.5 -0.5 0.5, \n");
  fprintf(hardfile.ptr,"                   0.5 -0.5 -0.5 \n");
  fprintf(hardfile.ptr,"                 ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    LineSet {\n");
  fprintf(hardfile.ptr,"      numVertices [ 8 ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    Coordinate3 {\n");
  fprintf(hardfile.ptr,"	point	[ -0.5 -0.5 0.5,\n");
  fprintf(hardfile.ptr,"                   -0.5 0.5 0.5, \n");
  fprintf(hardfile.ptr,"                   -0.5 0.5 -0.5\n");
  fprintf(hardfile.ptr,"                ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    LineSet {\n");
  fprintf(hardfile.ptr,"      numVertices [ 3 ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    Coordinate3 {\n");
  fprintf(hardfile.ptr,"	point	[ 0.5 -0.5 0.5,\n");
  fprintf(hardfile.ptr,"                   0.5 0.5 0.5, \n");
  fprintf(hardfile.ptr,"                   0.5 0.5 -0.5\n");
  fprintf(hardfile.ptr,"                ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    LineSet {\n");
  fprintf(hardfile.ptr,"      numVertices [ 3 ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    Coordinate3 {\n");
  fprintf(hardfile.ptr,"	point	[ -0.5 0.5 0.5,\n");
  fprintf(hardfile.ptr,"                   0.5 0.5 0.5\n");
  fprintf(hardfile.ptr,"                ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"    LineSet {\n");
  fprintf(hardfile.ptr,"      numVertices [ 2 ]\n");
  fprintf(hardfile.ptr,"    }\n");
  fprintf(hardfile.ptr,"  }  \n");

}
