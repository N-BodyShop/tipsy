#include "defs.h"
#include "fdefs.h"
void
input_error(command)
    char *command ;
{
    char job[MAXCOMM] ;
    printf("<inconsistent input for command %s, %s>\n",command,title) ;
    sprintf(job,"arg %s",command) ;
    arguments(job) ;
}
