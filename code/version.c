#include "defs.h"

static char version_string[] = "2.2.3b";

void
version(job)
    char job[MAXCOMM] ;
{
    printf("version %s:  03 MAR 2007\n", version_string) ;
    printf("MAXBOX = %d\n", MAXBOX) ;
}
