/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:33  trq
 * Initial revision
 *
 * Revision 2.3  94/04/20  08:57:00  trq
 * Added title variable
 * 
 * Revision 2.1  93/10/19  10:51:26  trq
 * Use command instead of job
 * 
 */

#include "defs.h"
#include "fdefs.h"

void
run_macro(job)
    char *job;
{
    char command[MAXCOMM] ;
    char macro_name[MAXCOMM] ;
    struct command_list *list;
    struct macro_list *macro;

    if (sscanf(job,"%s %s",command,macro_name) == 2) {
	for(macro = macros; macro != NULL ; macro = macro->next){
	    if ( strcmp(macro->name,macro_name) == 0) {
	      break;
	    }
	}
	if(macro == NULL)
	  {
	    printf("<sorry, %s is not a macro, %s>\n",macro_name,title) ;
	    return ;
	  }
	for(list = macro->start ; list != NULL ; list = list->next){
	    command_interp(list->command) ;
	}
    }
    else {
	input_error(command) ;
    }
}
