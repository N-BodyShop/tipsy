/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:36  trq
 * Initial revision
 *
 * Revision 1.1  94/04/19  17:55:27  trq
 * Initial revision
 * 
 */

#include "nsktrqxbm.h"

void
get_nsktrq(bitP, widthP, heightP)
     char **bitP;
     int *widthP;
     int *heightP;
{
  *widthP = nsktrq_width;
  *heightP = nsktrq_height;
  *bitP = nsktrq_bits;
}

  
