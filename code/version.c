#include "defs.h"

static char version_string[] = "2.2.3c";

void
version(job)
    char job[MAXCOMM] ;
{
    printf("version %s:  12 SEPT 2008\n", version_string) ;
    printf("MAXBOX = %d\n", MAXBOX) ;
}
