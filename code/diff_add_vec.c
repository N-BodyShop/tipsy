/*
 * $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:23  trq
 * Initial revision
 *
 * Revision 2.2  94/04/20  08:24:11  trq
 * Included fdefs.h
 * 
 * Revision 2.1  94/02/18  10:45:49  trq
 * Change doubles to Reals.
 * 
 */
#include "defs.h"
#include "fdefs.h"

void
diff_add_vec(a,b,c,d)
    Real a[MAXDIM] ;
    Real b[MAXDIM] ;
    Real c[MAXDIM] ;
    Real d[MAXDIM] ;
{
    int i;

    for (i = 0 ;i < MAXDIM ;i++) {
	a[i] = b[i] - c[i] + d[i] ;
    }
}
