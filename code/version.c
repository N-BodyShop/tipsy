#include "defs.h"

static char version_string[] = "2.2";

void
version(job)
    char job[MAXCOMM] ;
{
    printf("version %s:  31 AUGUST 1999\n", version_string) ;
    printf("MAXBOX = %d\n", MAXBOX) ;
}
