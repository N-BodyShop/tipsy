#include "defs.h"

static char version_string[] = "2.1.6";

version(job)
    char job[MAXCOMM] ;
{
    printf("version %s:  24 JULY 1995\n", version_string) ;
    printf("MAXBOX = %d\n", MAXBOX) ;
}
