#include "defs.h"
void
loadall()
{
    Real xmax[MAXDIM] ;
    Real xmin[MAXDIM] ;

    maxpos(xmax,xmin) ;
    setsize(0,xmax,xmin) ;
    loadbox(0) ;
    unload_all() ;
}
