#include "defs.h"

static char version_string[] = "2.2.2";

void
version(job)
    char job[MAXCOMM] ;
{
    printf("version %s:  28 AUG 2003\n", version_string) ;
    printf("MAXBOX = %d\n", MAXBOX) ;
}
