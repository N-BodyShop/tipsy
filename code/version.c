#include "defs.h"

static char version_string[] = "2.1.3";

version(job)
    char job[MAXCOMM] ;
{
    printf("version %s:  22 MARCH 1994\n", version_string) ;
    printf("MAXBOX = %d\n", MAXBOX) ;
}
