/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:24  trq
 * Initial revision
 *
 * Revision 2.2  94/04/20  08:17:27  trq
 * changed ifdef sun to ifdef ultrix
 * 
 * Revision 2.1  93/10/19  14:58:30  trq
 * *** empty log message ***
 * 
 */

#include "defs.h"

void
help(job)
    char *job;
{
    char arg[MAXCOMM] ;
    char command[MAXCOMM] ;
    char sys_command[MAXCOMM] ;
    int number ;

    number = (sscanf(job,"%s %s",arg,command)) ;
    if(number == 1){
#ifdef ultrix
	sprintf(sys_command,"man -P $TIPSYHELPDIR tipsy") ;
#else
	sprintf(sys_command,"man -M $TIPSYHELPDIR tipsy") ;
#endif
    }
    else{
#ifdef ultrix
	sprintf(sys_command,"man -P $TIPSYHELPDIR %s",command) ;
#else
	sprintf(sys_command,"man -M $TIPSYHELPDIR %s",command) ;
#endif
    }
    system(sys_command) ;

}
