#include "defs.h"

static char version_string[] = "2.1.11";

void
version(job)
    char job[MAXCOMM] ;
{
    printf("version %s:  25 JULY 1997\n", version_string) ;
    printf("MAXBOX = %d\n", MAXBOX) ;
}
