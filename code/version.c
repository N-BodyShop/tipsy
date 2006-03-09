#include "defs.h"

static char version_string[] = "2.2.3a";

void
version(job)
    char job[MAXCOMM] ;
{
    printf("version %s:  09 AUG 2006\n", version_string) ;
    printf("MAXBOX = %d\n", MAXBOX) ;
}
