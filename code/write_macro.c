/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:33  trq
 * Initial revision
 *
 * Revision 2.3  94/04/20  08:46:23  trq
 * Added title variable.
 * 
 * Revision 2.1  93/10/19  10:46:27  trq
 * *** empty log message ***
 * 
 */

#include "defs.h"
#include "fdefs.h"

void
write_macro(job)
    char *job;
{
    char command[MAXCOMM] ;
    char macro_name[MAXCOMM] ;
    struct command_list *list;
    struct macro_list *macro;

    if (sscanf(job,"%s %s %s",command,macro_name,hardfile.name) == 3) {
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
	hardfile.ptr = fopen(hardfile.name,"w") ;
	fprintf(hardfile.ptr,"%s\n",macro->name) ;
	for(list = macro->start ; list != NULL ; list = list->next){
	    fprintf(hardfile.ptr,"%s\n",list->command) ;
	}
	fprintf(hardfile.ptr,"end\n") ;
	fclose(hardfile.ptr) ;
    }
    else {
	input_error(command) ;
    }
}
