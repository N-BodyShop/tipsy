#include "defs.h"
commands(job)
    char job[MAXCOMM] ;
{
    char sys_command[MAXCOMM] ;
    char option[MAXCOMM] ;
    char command[MAXCOMM] ;

    if(sscanf(job,"%s %s",command,option) == 2 
            && strcmp(option,"print") == 0){
        sprintf(sys_command,"lpr \"$TIPSYHELPDIR\"/whatis") ;
        system(sys_command) ;
    }
    else{
        sprintf(sys_command,"more \"$TIPSYHELPDIR\"/whatis") ;
        system(sys_command) ;
    }
}
