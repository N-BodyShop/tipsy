/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:33  trq
 * Initial revision
 *
 * Revision 2.7  1994/10/12  21:26:39  trq
 * Cleaned up mallocs.
 *
 * Revision 2.6  1994/09/01  23:59:16  trq
 * Do not overwrite an existing macro.
 *
 * Revision 2.5  1994/09/01  22:18:09  nsk
 * fixed prompt during interactive phase
 *
 * Revision 2.4  1994/04/20  08:45:45  trq
 * Added title variable.
 *
 * Revision 2.2  93/10/18  15:50:09  trq
 * Added check for NULL
 * 
 */

#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>
#include <malloc.h>

void
make_macro(job)
    char *job;
{
    char command[MAXCOMM] ;
    char macro_name[MAXCOMM] ;
    char prompt[MAXCOMM];
    char *macro_com;
    struct command_list **plist;
    struct macro_list *macro;
    
    if (sscanf(job,"%s %s",command,macro_name) == 2){
	for(macro = macros; macro != NULL ; macro = macro->next){
	    if ( strcmp(macro->name,macro_name) == 0) {
	      printf("<sorry %s, macro exists,>\n",title) ;
	      return;
	    }
	}
	
	if((macro = (struct macro_list *)malloc(sizeof(struct macro_list))) == NULL)
	  {
	    printf("<sorry %s, no memory for macros>\n",title) ;
	    return;
	  }
	macro->next = macros;
	macros = macro;
	if((macro->name = malloc(1+strlen(macro_name))) == NULL) 
	  {
	    printf("<sorry, %s.  No memory for macro name>\n",title);
	    return;
	  }
	strcpy(macro->name,macro_name) ;
	macro->start = NULL;
	plist = &(macro->start);
	printf("<start macro input, %s>\n",title);
	sprintf(prompt, "<enter command, %s>",title) ;
	forever {
	    macro_com = my_gets(prompt) ;
	    if ( (macro_com == NULL) || (strcmp(macro_com,"end") == 0)) {
		break ;
	    }
	    else {
		if((*plist = (struct command_list *)malloc(sizeof(struct command_list))) == NULL) {
		    printf("<sorry, no memory for commands %s>\n",title) ;
		    break ;
		}
		if(((*plist)->command = malloc(1+strlen(macro_com))) == NULL) {
		    printf("<sorry, no memory for commands %s>\n",title) ;
		    free(*plist);
		    *plist = NULL;
		    break ;
		}
		strcpy((*plist)->command,macro_com) ;
		(*plist)->next = NULL;
		plist = &((*plist)->next);
	    }
	}
	printf("<ended macro input, %s>\n",title);
    }
    else {
	input_error(command) ;
    }
}
