#include "defs.h"

static char version_string[] = "2.1.10";

void
version(job)
    char job[MAXCOMM] ;
{
    printf("version %s:  30 JULY 1996\n", version_string) ;
    printf("MAXBOX = %d\n", MAXBOX) ;
}
