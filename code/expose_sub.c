
/* expose internal variables */


#include "defs.h"
#include "fdefs.h"

void hide_header();
void hide_box();


void
expose_sub(job)
    char *job;
{

  int status;
  char command[MAXCOMM] ;
  char reply[15] ;

  status = sscanf(job,"%s %s",command,reply);
  if (status == 2) {
    if        ( strcmp(reply,"yes") == 0 
		|| strcmp(reply,"y") == 0 
		|| strcmp(reply,"on") == 0 ) {
        exposed = ON;
	expose_header();
	expose_box();
    } else if ( strcmp(reply,"no") == 0 
		|| strcmp(reply,"n") == 0 
		|| strcmp(reply,"off") == 0 ) {
        if (exposed == ON) {
	  hide_header();
	  hide_box();
	  exposed = OFF;
	}
    }
  } else if (status == 1) {
    exposed = ON;
    expose_header();
    expose_box();
  } else {
    input_error(command) ;
  }
}


void expose_header() {
  char value[MAXCOMM];
  sprintf(value, "%g", header.time);
  assign_var("TIME", value);
  sprintf(value, "%d", header.nbodies);
  assign_var("NBODIES", value);
  sprintf(value, "%d", header.nsph);
  assign_var("NSPH", value);
  sprintf(value, "%g", header.ndark);
  assign_var("NDARK", value);
  sprintf(value, "%g", header.nstar);
  assign_var("NSTAR", value);
}


void expose_box() {

  char value[MAXCOMM];

  if (boxes_loaded[active_box]) {
    sprintf(value, "%g", boxes[active_box].center[0]);
    assign_var("BOX_CENTER_0", value);
    sprintf(value, "%g", boxes[active_box].center[1]);
    assign_var("BOX_CENTER_1", value);
    sprintf(value, "%g", boxes[active_box].center[2]);
    assign_var("BOX_CENTER_2", value);

    /* this is the weirdest notation ever */
    sprintf(value, "%g", (boxes[active_box].x3[0] - boxes[active_box].x1[0]) );
    assign_var("BOX_SIZE_0", value);
    sprintf(value, "%g", (boxes[active_box].x1[1] - boxes[active_box].x2[1]) );
    assign_var("BOX_SIZE_1", value);
    sprintf(value, "%g", (boxes[active_box].x1[2] - boxes[active_box].x5[2]) );
    assign_var("BOX_SIZE_2", value);

    sprintf(value, "%g", boxes[active_box].total_com[0]);
    assign_var("BOX_CM_0", value);
    sprintf(value, "%g", boxes[active_box].total_com[1]);
    assign_var("BOX_CM_1", value);
    sprintf(value, "%g", boxes[active_box].total_com[2]);
    assign_var("BOX_CM_2", value);

  } 

}



void hide_header() {
  delete_var("delete $TIME");
  delete_var("delete $NBODIES");
  delete_var("delete $NSPH");
  delete_var("delete $NDARK");
  delete_var("delete $NSTAR");
}

void hide_box() {
  delete_var("delete $BOX_CENTER_0");
  delete_var("delete $BOX_CENTER_1");
  delete_var("delete $BOX_CENTER_2");
  delete_var("delete $BOX_SIZE_0");
  delete_var("delete $BOX_SIZE_1");
  delete_var("delete $BOX_SIZE_2");
  delete_var("delete $BOX_CM_0");
  delete_var("delete $BOX_CM_1");
  delete_var("delete $BOX_CM_2");
}


