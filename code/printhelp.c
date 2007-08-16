#include "defs.h"
printhelp(job)
    char job[MAXCOMM] ;
{
    char arg[MAXCOMM] ;
    char command[MAXCOMM] ;
    char sys_command[MAXCOMM] ;
    int number ;

    number = (sscanf(job,"%s %s",arg,command)) ;
    if(number == 1){
	sprintf(sys_command,"lpr \"$TIPSYHELPDIR\"/fmt1/tipsy.ps") ;
    }
    else{
      if(strcmp(command, "all") == 0) {
	sprintf(sys_command,"lpr \"$TIPSYHELPDIR\"/fmt1/*.ps") ;
      }
      else {
	sprintf(sys_command,"lpr \"$TIPSYHELPDIR\"/fmt1/%s.ps", command) ;
      }
    }
    system(sys_command) ;

}
