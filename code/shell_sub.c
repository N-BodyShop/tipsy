/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:36  trq
 * Initial revision
 *
 * Revision 1.1  94/04/19  17:56:03  trq
 * Initial revision
 * 
 */

#include "defs.h"
#include "fdefs.h"
#include <ctype.h>

void
shell_sub(job)
    char *job;
{
  char *command;
  
  command = job;
  while(!isspace((int) *command))
    command++;
  if(*command)
    {
      system(command);
    }
  else
    {
      input_error("shell");
    }
}
