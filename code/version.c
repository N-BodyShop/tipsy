#include "defs.h"

static char version_string[] = "2.1.5";

version(job)
    char job[MAXCOMM] ;
{
    printf("version %s:  02 MARCH 1995\n", version_string) ;
    printf("MAXBOX = %d\n", MAXBOX) ;
}
