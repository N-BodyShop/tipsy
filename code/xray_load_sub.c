#include "defs.h"
#include "fdefs.h"

void
xray_load_sub(job)
    char job[MAXCOMM] ;
{
    xray_loaded = NO ;
    xray_lum_load() ;
}
