#include "defs.h"

static char version_string[] = "2.1.8";

void
version(job)
    char job[MAXCOMM] ;
{
    printf("version %s:  19 APRIL 1996\n", version_string) ;
    printf("MAXBOX = %d\n", MAXBOX) ;
}
