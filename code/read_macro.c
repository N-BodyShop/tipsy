#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>

void
read_macro(job)
    char *job;
{
    char command[MAXCOMM] ;
    char macro_name[MAXCOMM] ;
    struct command_list **plist;
    struct macro_list *macro;
    struct macro_list *prev;
    int i ;
    int j ;

    if (sscanf(job,"%s %s",command,hardfile.name) == 2) {
	if( (hardfile.ptr = fopen(hardfile.name,"r")) 
		   == NULL) {
	    printf("<sorry, macro file %s does not exist, %s>\n",
		    hardfile.name,title) ;
	    return ;
	}
	forever {
	    if(fscanf(hardfile.ptr,"%s",macro_name) != 1){
		break ;
	    }
	    prev = NULL;
	    for(macro = macros; macro != NULL ; macro = macro->next){
		if ( strcmp(macro->name,macro_name) == 0) {
		  if(prev == NULL)
		    macros = macro->next; /* move head down */
		  else
		    prev->next = macro->next; /* delete from chain */
		  delete_macro_sub(macro);
		  macro = prev;
		  if(macro == NULL)
		      break;
		  else
		    continue;
		}
	    }
	    if(macro == NULL)
	      {
		if((macro = (struct macro_list *)malloc(sizeof(struct macro_list))) == NULL)
		  {
		    printf("<sorry %s, no memory for macros>\n",title) ;
		    return;
		  }
		macro->next = macros;
		macros = macro;
	      }
	    if((macro->name = malloc(1+strlen(macro_name))) == NULL) 
	      {
		printf("<sorry, %s.  No memory for macro name>\n",title);
		return;
	      }
	    strcpy(macro->name,macro_name) ;
	    macro->start = NULL;
	    plist = &(macro->start);
	    fgetc(hardfile.ptr) ;
	    forever {
		for(i = 0 ; i < MAXCOMM ; i++){
		    j = fgetc(hardfile.ptr) ;
		    if(j == '\n'){
			command[i] = '\0' ;
			break ;
		    }
		    else if( j == EOF){
			printf("<sorry, no end of macro %s>\n",title) ;
			return ;
		    }
		    else{
			command[i] = j ;
		    }
		}
		if ( strcmp(command,"end") == 0) {
		    break ;
		}
		else {
		    if((*plist = (struct command_list *)malloc(sizeof(struct command_list)))
		       == NULL) {
			printf("<sorry, no memory for commands %s>\n",title) ;
			break ;
		    }
		    if(((*plist)->command = malloc(1+strlen(command)))
		       == NULL) {
			printf("<sorry, no memory for commands %s>\n",title) ;
			free(*plist);
			*plist = NULL;
			break ;
		    }
		    strcpy((*plist)->command,command) ;
		    (*plist)->next = NULL;
		    plist = &((*plist)->next);
		}
	    }
	    j = fgetc(hardfile.ptr) ;
	    if(j == EOF){
		break ;
	    }
	    else{
		ungetc(j,hardfile.ptr) ;
	    }
	}
	fclose(hardfile.ptr) ;
    }
    else {
	input_error(command) ;
    }
}
