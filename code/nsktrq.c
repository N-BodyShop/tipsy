/* $Header$
 * $Log$
 * Revision 1.3  1997/08/20 00:09:03  trq
 * Small changes to compile under Solaris.
 *
 * Revision 1.2  1996/08/19  20:56:02  trq
 * main.c, plot_all.c: allow use of pseudocolor visuals with depth >= 8.
 * nsktrq.c: include fdefs.h.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:37  trq
 * Import to CVS
 *
 * Revision 1.1  94/04/19  17:55:27  trq
 * Initial revision
 * 
 */

#include "nsktrqxbm.h"
#include "defs.h"
#include "fdefs.h"

void
get_nsktrq(bitP, widthP, heightP)
     char **bitP;
     int *widthP;
     int *heightP;
{
  *widthP = nsktrq_width;
  *heightP = nsktrq_height;
  *bitP = (char *)nsktrq_bits;
}

  
