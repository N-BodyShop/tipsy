#include "defs.h"

void
whatis(job)
    char job[MAXCOMM] ;
{
    char arg[MAXCOMM] ;
    char command[MAXCOMM] ;
    char sys_command[MAXCOMM] ;
    int number ;

    number = (sscanf(job,"%s %s",arg,command)) ;
    if(number == 1){
	printf("<sorry, you must enter a command, %s>\n",title) ;
    }
    else{
#ifdef ultrix
	sprintf(sys_command,"man -P \"$TIPSYHELPDIR\" -f %s",command) ;
#else
	sprintf(sys_command,"man -M \"$TIPSYHELPDIR\" -f %s",command) ;
#endif
    }
    system(sys_command) ;

}
