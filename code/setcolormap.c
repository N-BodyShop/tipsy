#include "defs.h"

void
setcolormap_sub(job)
    char job[MAXCOMM] ;
{
  char command[MAXCOMM] ;
  char map[MAXCOMM] ;

  if (sscanf(job,"%s %s ",command, map) == 2) {
    if (strcmp(map, "none") == 0 ) {
      cmap = CM_NONE ;
    } else if ((strcmp(map, "default") == 0 ) || (strcmp(map, "d") == 0 ) ) {
      cmap = CM_DEFAULT ;
    } else if ((strcmp(map, "rainbow") == 0)
	       || (strcmp(map, "r") == 0) ) {
      cmap = CM_RAINBOW ;
    } else if ( (strcmp(map, "reverse_rainbow") == 0)
		|| (strcmp(map, "revrain") == 0)
		|| (strcmp(map, "rr") == 0) ) {
      cmap = CM_REVRAIN ;
    } else if ( (strcmp(map, "wrbb") == 0) 
		|| (strcmp(map, "w") == 0) ) {
      cmap = CM_WRBB ;
    } else if ( (strcmp(map, "temperature") == 0 ) 
           || (strcmp(map, "temp") == 0 ) ) {
      cmap = CM_TEMP ;
    } else {
      printf("<sorry, %s is not a known map, %s>\n",map, title) ;
      return;
    }
    load_color_table();
  }
  else {
    input_error(command) ;
  }
}
