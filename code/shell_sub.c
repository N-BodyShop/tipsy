/* $Header$
 * $Log$
 * Revision 1.1.1.1.6.1  2000/07/13 01:17:04  nsk
 * Fixed bug -- space at front added "shell" to command.  (maf)
 *
 * Revision 1.1.1.1  1995/01/10  22:57:37  trq
 * Import to CVS
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
  char junk[MAXCOMM], command[MAXCOMM];
  
  if (sscanf(job, "%s %[^\n]", junk, command) == 2)
    {
      system(command);
    }
  else
    {
      input_error("shell");
    }
}
