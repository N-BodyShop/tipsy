#include "defs.h"
double perp_distance(x1,x2,x3)
    Real x1[MAXDIM] ;
    Real x2[MAXDIM] ;
    Real x3[MAXDIM] ;
{
    double seperation ;
    Real cross[MAXDIM] ;
    Real radius[MAXDIM] ;
    double dot_product() ;

    sub_vec(radius,x3,x1) ;
    cross_product(cross,radius,x2) ;
    seperation = sqrt(dot_product(cross,cross) / dot_product(x2,x2)) ;

    return(seperation) ;
}
