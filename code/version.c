#include "defs.h"

static char version_string[] = "2.2.1";

void
version(job)
    char job[MAXCOMM] ;
{
    printf("version %s:  02 JULY 2002\n", version_string) ;
    printf("MAXBOX = %d\n", MAXBOX) ;
}
