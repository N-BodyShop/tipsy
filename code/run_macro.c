/* $Header$
 * $Log$
 * Revision 1.1.1.1.6.1  2000/07/07 03:31:20  nsk
 * Changes to substitute macro parameters, use for-loops, and
 *   expand variables.   (maf/cm)
 *
 * Revision 1.1.1.1  1995/01/10  22:57:34  trq
 * Import to CVS
 *
 * Revision 2.3  94/04/20  08:57:00  trq
 * Added title variable
 * 
 * Revision 2.1  93/10/19  10:51:26  trq
 * Use command instead of job
 * 
 */


#define MAXARG 25
#define MAXTOK 80

#include "defs.h"
#include "fdefs.h"

int sub_val_for_param(char job[], 
		  struct macro_list *macro, 
		  char macro_arg[][MAXTOK], 
		  char jobcpy[]);
extern int assign_var(char vname[], char val[]);



void
run_macro(job)
     char *job;
{
  int i, len, remaining, status;
  char command[MAXCOMM]; 
  char macro_name[MAXCOMM];
  char jobcpy[MAXCOMM];
  char token[MAXTOK]; 
  char rest[MAXCOMM]; 
  char macro_arg[MAXARG][MAXTOK];
  char *arg_index;
  struct command_list *list;
  struct macro_list *macro;


  /* parse macro call */
  if (sscanf(job,"%s %s", command, macro_name) < 2) {
    input_error(command) ;
    return;
  }

  for (macro = macros; macro != NULL ; macro = macro->next) {
    if ( strcmp(macro->name,macro_name) == 0) break;
  }
  if (macro == NULL)
    {
      printf("<sorry, %s is not a macro, %s>\n",macro_name,title) ;
      return ;
    }

  if (macro->n_params > 0) {

    /* parse job into tokens to get macro parameters */
    strcpy(jobcpy, job);
    i = -2;
    remaining = 1;
    while ( remaining > 0) {
      remaining = sscanf(jobcpy, "%s %[^\n]", token, rest);
      /*printf("remaining: %d\n", remaining);*/
      if (remaining > 0) {
	/*printf("token: |%s|\n", token);
	printf("remainder: |%s|\n", rest);*/
	strcpy(jobcpy, rest);
	if (i > -1) {
	  strcpy(macro_arg[i], token);
	  /*printf("macro arg: |%s|\n", macro_arg[i]);*/
	}
      }
      i++;
      remaining--;
    }

    if (i != macro->n_params) {
      printf("<sorry, macro %s requires %d arguments, %s>\n",
	     macro_name,macro->n_params,title);
      return;
    }
  }


  /* execute commands in macro */
  execute_command_list(macro->start, macro, macro_arg);
  /*
  for (list = macro->start ; list != NULL ; list = list->next) {

    if ( !sub_val_for_param(list->command, macro, macro_arg, jobcpy) ) {
      printf("<Error parsing macro line, %s:>\n<%s>\n", title, list->command);
      return;
    }
    printf("about to execute following: \n|%s|\n", jobcpy);
    command_interp(jobcpy) ;
  
    }*//*end macro command loop*/

}


/* execute a list of commands.  If a for loop encountered, compose
   a new list of commands and spawn a recursive call to execute those.
   Returns 1 for success, 0 for failure.
   */
int
execute_command_list(struct command_list *list, 
		     struct macro_list *macro, 
		     char macro_arg[][MAXTOK]) {

  int status;
  struct command_list *item;
  char jobcpy[MAXCOMM];
  char vname[MAXVAR];
  char val[MAXCOMM];
  char command[MAXCOMM]; 
  char expr[MAXVAR];
  char *edum, *dum; 
  double loopvar, start, stop, step;
  struct command_list *loopstart, *loopitem, *prev, *save;
  

  for (item = list; item != NULL; item = item->next) {

    /* check whether "for" command */
    if(sscanf(item->command,"%s",command) != 1) {
      printf("<Couldn't read line, %s>\n",title);
      return 0;
    }

    if (strcmp(command,"for") != 0) {

      /* if not, just execute the command*/
      if ( !sub_val_for_param(item->command, macro, macro_arg, jobcpy) ) {
	printf("<Error parsing macro line, %s:>\n<%s>\n", 
	       title, item->command);
	return 0;
      }
      command_interp(jobcpy) ;

    } else {

      /* if so, parse line to get loop parameters*/
      if ( (dum=strchr(item->command,'$')) == NULL) {
	printf("<Incorrect variable syntax: missing $ >\n");
	return;
      }
      if ( (edum=strchr(item->command,'=')) == NULL || edum < dum ) {
	printf("<Incorrect loop syntax: missing = >\n");
	return 0;
      }    
      dum++;
      strncpy(expr, dum, edum-dum);
      expr[edum-dum] = '\0';
      if (sscanf(expr, "%[0-9a-zA-z]", vname) < 1) {
	printf("<Error in loop variable, %s>\n", title);
	printf("Received: <%s>\n", item->command);
	return 0;
      }

      dum = edum+1;
      strcpy(jobcpy, dum);
      if ( !sub_val_for_var(jobcpy) ) {
	printf("<Error parsing for command, %s:>\n<%s>\n", 
	       title, item->command);
	return 0;
      }

      if (sscanf(jobcpy, "%lf", &start) < 1) {
	printf("<Error in loop variable, %s>\n", title);
	printf("Received: <%s>\n", item->command);
	return 0;
      }

      if ( (edum=strchr(jobcpy,',')) == NULL ) {
	printf("<Incorrect expression syntax: missing , >\n");
	return 0;
      }    
      dum = edum+1;
      if (sscanf(dum, "%lf", &stop) < 1) {
	printf("<Error in loop variable, %s>\n", title);
	printf("Received: <%s>\n", item->command);
	return 0;
      }

      if ( (edum=strchr(dum,',')) == NULL ) {
	step = 1.;
      } else {
	dum = edum+1;
	if (sscanf(dum, "%lf", &step) < 1) {
	  printf("<Error in loop variable, %s>\n", title);
	  printf("Received: <%s>\n", item->command);
	  return 0;
	}
      }
	
      if ( (stop-start)*step < 0 ) {
	printf("<Sorry, nonsensical loop parameters, %s>\n", title);
	printf("<start=%g, stop=%g, step=%g>\n", start, stop, step);
	return 0;
      }


      /*read in statements until loop end reached*/
      prev = NULL;
      while ((item = item->next) != NULL) {
	sscanf(item->command, "%s", command);
	if (strcmp(command, "done") != 0) {
	  if ( (loopitem = 
		(struct command_list *) malloc(sizeof(struct command_list)) )
	       == NULL) {
	    printf("<sorry, no memory for commands %s>\n",title) ;
	    return 0;
	  }
	  if ( (loopitem->command = (char *) malloc(1+strlen(item->command)) )
	       == NULL) {
	    printf("<sorry, no memory for commands %s>\n",title) ;
	    return 0;
	  }
	  strcpy(loopitem->command, item->command);
	  if (prev == NULL) {
	    loopstart = loopitem;
	  } else {
	    prev->next = loopitem;
	  }
	  prev = loopitem;
	} else {
	  loopitem->next == NULL;
	  break;
	}
      }
      if (item == NULL) {
	printf("<sorry, loop not properly terminated, %s>\n");
	return 0;
      }

      /* now loop and execute statements */
      for (loopvar = start; (stop-loopvar)*step >= 0; loopvar += step) {
	sprintf(val, "%g", loopvar);
	assign_var(vname, val);
	status = execute_command_list(loopstart, macro, macro_arg);
	if (!status) return 0;
      }

      /* reclaim memory */
      loopitem = loopstart;
      while (loopitem != NULL) {
	save = loopitem->next;
	free(loopitem->command);
	free(loopitem);
	loopitem = save;
      }

    }/* end "for" branch*/
  }/*end command list loop*/
  return 1;

}





/* 
   Substitute macro arguments into command.  
   Macro arguments are denoted by a dollar sign and an integer ($1, $2, etc).  
   Something like $label is a variable instead, so leave it alone.  
   returns 1 for parsing success, 0 for failure.
*/

int 
sub_val_for_param(char job[], 
		  struct macro_list *macro, 
		  char macro_arg[][MAXTOK], 
		  char jobcpy[]) {
  
  int len, len0, index;
  char token[MAXTOK];
  char *remainder, *dum;

  strcpy(jobcpy, "");

  remainder = job;
  while ( (dum=strchr(remainder,'$')) != NULL) {
    /* copy part before variable */
    len = dum-remainder;
    len0 = strlen(jobcpy);
    strncat(jobcpy, remainder, len);
    jobcpy[len0+len] = '\0';
    dum++;
    remainder += len+1;

      /* get variable name */
    if (strlen(dum) == 0) {
      printf("<Error in variable syntax, %s>\n", title);
      printf("Received: <%s>\n", job);
      return 0;
    }
    if (*dum != '{') {
      /* standard method */
      if (sscanf(dum, "%[0-9a-zA-z]", token) < 1) {
	printf("<Error in variable syntax, %s>\n", title);
	printf("Received: <%s>\n", job);
	return 0;
      }
      remainder += strlen(token);  

      /* replace parameter $i with macro_arg[i] */
      index=atoi(token)-1;
      if (index < 0) {
	/* failed to find index, assume it's a variable */ 
	strcat(jobcpy, "$");
	strcat(jobcpy, token);
      } else if (index >= macro->n_params) {
	printf("<sorry, only %d parameters in macro %s, %s>\n", 
	       macro->n_params, macro->name, title);
	return 0;
      } else {
	/* found macro index, write argument*/
	strcat(jobcpy, macro_arg[index]);
      }
    } else {
      /* braces method */
      remainder++;
      if ( (dum = strchr(remainder,'}')) == NULL) {
	printf("<Error in variable syntax, %s>\n", title);
	printf("Received: <%s>\n", job);
	return;
      }
      len = dum-remainder;
      strncpy(token, remainder, len);
      token[len] = '\0';
      remainder += len+1;

      /* replace parameter $i with macro_arg[i] */
      index=atoi(token)-1;
      if (index < 0) {
	/* failed to find index, assume it's a variable */ 
	strcat(jobcpy, "${");
	strcat(jobcpy, token);
	strcat(jobcpy, "}");
      } else if (index >= macro->n_params) {
	printf("<sorry, only %d parameters in macro %s, %s>\n", 
	       macro->n_params, macro->name, title);
	return;
      } else {
	/* found macro index, write argument*/
	strcat(jobcpy, macro_arg[index]);
      }
    }


  }/*end loop over dollar signs*/

  /* copy any additional part */
  strcat(jobcpy, remainder);
}

