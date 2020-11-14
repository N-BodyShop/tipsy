#include "defs.h"
#include "fdefs.h"
void
clearall(job)
    char job[MAXCOMM] ;
{
    int i ;

    unload_all() ;
    unmark_all() ;
    clear_rot() ;
}
