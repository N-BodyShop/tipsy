#include "defs.h"

static char version_string[] = "2.1.7";

version(job)
    char job[MAXCOMM] ;
{
    printf("version %s:  27 DECEMBER 1995\n", version_string) ;
    printf("MAXBOX = %d\n", MAXBOX) ;
}
