#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>

void
delete_macro_sub(macro)
     struct macro_list *macro;
{
  struct command_list *list;
  
  list = macro->start;
  while(list != NULL)
    {
      struct command_list *save = list->next;
      free(list->command);
      free(list);
      list = save;
    }
  free(macro->name);
  free(macro);
}

void
delete_macro(job)
    char *job;
{
    char command[MAXCOMM] ;
    char macro_name[MAXCOMM] ;
    struct macro_list *macro;
    struct macro_list *prev;

    if (sscanf(job,"%s %s",command,macro_name) == 2) {
        prev = NULL;
	for(macro = macros; macro != NULL ; macro = macro->next){
	    if ( strcmp(macro->name,macro_name) == 0) {
	      if(prev == NULL)
		macros = macro->next; /* move head down */
	      else
		prev->next = macro->next; /* delete from chain */
	      delete_macro_sub(macro);
	      break;
	    }
	    prev = macro;
	}
	if(macro == NULL){
	    printf("<sorry, %s is not a macro, %s>\n",macro_name,title) ;
	    return ;
	}
    }
    else {
	input_error(command) ;
    }
}
