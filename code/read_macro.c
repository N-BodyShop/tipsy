#include "defs.h"
#include "fdefs.h"
#include <stdlib.h>
#include <malloc.h>



/* environment variable TIPSYMACROPATH should be a colon-separated string 
   of directories, generally starting with a . */
FILE *get_macro_file(char name[]) {

  char *path = getenv("TIPSYMACROPATH");
  char *dir, *str, *term, *p;
  char fullpath[100]; 
  FILE *fp = NULL;
  int i;

  if (name[0] == '/') {
    /* absolute path */
    fp = fopen(name, "r");
    return fp;
  } else {
    /* relative path */
    if ( (path == NULL) || (*path == '\0') ) path = ".";
    str = path;
    while (fp == NULL) {
      /* try to get path entry */
      term = strchr(str, ':');
      if (term == NULL) term = strchr(str, '\0');
      
      if ( (str == term) || (term-str > 99) ) return NULL;
      for (i = 0, p=str; i < term-str; i++) {
	fullpath[i] = *p;
	p++;
      }
      fullpath[i] = '\0';
      
      strncat(fullpath, "/", 99-strlen(fullpath));
      strncat(fullpath, name, 99-strlen(fullpath));
      /* try to load file */
      fp = fopen(fullpath, "r");
      /* point to next directory */
      if (term[0] == '\0') {
	str = term;
      } else {
	str = term+1;
      }
    }
    return fp;
  }
  
}




void
read_macro(job)
     char *job;
{
  char command[MAXCOMM] ;
  char macro_name[MAXCOMM] ;
  char junk[MAXCOMM] ;
  struct command_list **plist;
  struct macro_list *macro;
  struct macro_list *prev;
  int i ;
  int j ;
  int n_args,n_params;

  if (sscanf(job,"%s %s",command,hardfile.name) == 2) {
    hardfile.ptr = get_macro_file(hardfile.name);
    if (hardfile.ptr == NULL) {
      printf("<sorry, macro file %s does not exist, %s>\n",
             hardfile.name,title) ;
      return ;
    }
    forever {

      if( (n_args=fscanf(hardfile.ptr,"%s %d%[^\n]",
			 macro_name,&n_params,junk)) < 1 ) break ;
      if (n_args > 1) fgetc(hardfile.ptr);  /*read in the extra return*/
      /* stole code from delete_macro since old version appeared buggy */
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
      /* put in at head of list */
      if((macro = (struct macro_list *)malloc(sizeof(struct macro_list))) 
	 == NULL) {
	printf("<sorry %s, no memory for macros>\n",title) ;            
	fclose(hardfile.ptr);
	return;
      }
      macro->start = NULL; /* init now so can bail out if bad macro */
      macro->next = macros;
      if((macro->name = malloc(1+strlen(macro_name))) == NULL) 
        {
          printf("<sorry, %s.  No memory for macro name>\n",title);
	  fclose(hardfile.ptr);
          return;
        }
      strcpy(macro->name,macro_name) ;
      if (n_args>1) {
        macro->n_params=n_params;
      } else {
        macro->n_params=0;
      }
      printf("<macro %s has %d parameters>\n",
             macro->name,macro->n_params);

      /* could check for parameter usage in following segment */
      macro->start = NULL;
      plist = &(macro->start);
      forever {
        for(i = 0 ; i < MAXCOMM ; i++){
          j = fgetc(hardfile.ptr) ;
          if(j == '\n'){
            command[i] = '\0' ;
            break ;
          }
          else if( j == EOF){
            printf("<sorry, no end of macro %s>\n",title) ;
	    delete_macro_sub(macro);
	    fclose(hardfile.ptr);
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
          if((*plist = 
	     (struct command_list *)malloc(sizeof(struct command_list)))
               == NULL) {
            printf("<sorry, no memory for commands %s>\n",title) ;
	    delete_macro_sub(macro);
	    fclose(hardfile.ptr);
            return ;
          }
	  (*plist)->next = NULL;
	  if(((*plist)->command = malloc(1+strlen(command)))
	     == NULL) {
            printf("<sorry, no memory for commands %s>\n",title) ;
	    delete_macro_sub(macro);
	    fclose(hardfile.ptr);
            return;
          }
          strcpy((*plist)->command,command) ;
	  printf("command: |%s|\n");
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
    macros = macro;
  }
  else {
    input_error(command) ;
  }
}
