#include "defs.h"
void
setsize(box,xmax,xmin)
    Real xmax[MAXDIM] ;
    Real xmin[MAXDIM] ;
    int box ;
{
    boxes[box].x1[0] = xmin[0] ;
    boxes[box].x1[1] = xmax[1] ;
    boxes[box].x1[2] = xmax[2] ;
    boxes[box].x2[0] = xmin[0] ;
    boxes[box].x2[1] = xmin[1] ;
    boxes[box].x2[2] = xmax[2] ;
    boxes[box].x3[0] = xmax[0] ;
    boxes[box].x3[1] = xmax[1] ;
    boxes[box].x3[2] = xmax[2] ;
    boxes[box].x4[0] = xmax[0] ;
    boxes[box].x4[1] = xmin[1] ;
    boxes[box].x4[2] = xmax[2] ;
    boxes[box].x5[0] = xmax[0] ;
    boxes[box].x5[1] = xmax[1] ;
    boxes[box].x5[2] = xmin[2] ;
    boxes[box].x6[0] = xmax[0] ;
    boxes[box].x6[1] = xmin[1] ;
    boxes[box].x6[2] = xmin[2] ;
}
