/* $Header$
 * $Log$
 * Revision 1.1.1.1.6.2  2000/07/13 01:19:34  nsk
 * Fixed error in variable substitution--despite original declaration,
 *   "job" in command_interp is not of length MAXCOMM.  Need to copy
 *    values to new string instead.  (maf)
 *
 * Revision 1.1.1.1.6.1  2000/07/07  03:27:54  nsk
 * Implement variable substitution. allow macro execution without
 *   "macro" prefix.  (cm/maf)
 *
 * Revision 1.1.1.1  1995/01/10  22:57:24  trq
 * Import to CVS
 *
 * Revision 2.3  94/04/20  08:45:27  trq
 * Added title variable.
 * 
 */
#include "defs.h"
#include "fdefs.h"
#include "comm.h"

void
command_interp(job)
    char *job ;
{
    char jobcpy[MAXCOMM], command[MAXCOMM],macro_job[MAXCOMM] ;
    int i;
    struct macro_list *macro;

    if(sscanf(job,"%s",command) != 1) {
	return ;
    }

    for(i=0;i<ncomm;i++) {
        if(strcmp(command,c_list[i].name) == 0) {
	    if(c_list[i].display && !display) {
		printf("<sorry not in display mode, %s>\n",title) ;
	    }
	    else {
	      /* don't replace for variable manipulation commands */
	      if ((strcmp(command,"set") != 0) && 
		  (strcmp(command,"delete") != 0)) {
		if (sub_val_for_var(job, jobcpy)) 
		  (*(c_list[i].funk))(jobcpy) ;
	      } else {
		(*(c_list[i].funk))(job) ;
	      }
	    } 
	    return;
	}
    }

    /* search macro list to allow macro execution without 'macro' prefix */
    /* (backwards compatibility above) */

    strcpy(macro_job,"macro ");
    for (macro=macros;macro != NULL;macro=macro->next) {
      if ( strcmp(macro->name,command) == 0) {
	if (!sub_val_for_var(job, jobcpy)) return;
	strcat(macro_job,jobcpy);
	run_macro(macro_job);
	return;
      }
    }
      
    printf("<sorry %s is not a command, %s>\n",command,title) ;
}

