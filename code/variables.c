#include "defs.h"

int sub_val_for_var(char job[]);
int assign_var(char vname[], char val[]);

/* 
   add: 1) array variables; 2) multiple-variable printing;
   3) printing list of all variable names 
*/



void set_var(char job[])
{
  int i, len;
  char expression[MAXCOMM], vname[MAXVAR], temp[MAXVAR], *val;
  char *vdum, *edum, *calc(char*);

  /* should handle range of syntax */
  /* simple error checking: check for `$', '=' */
  /* apparently supercedes num_arg checking */
  /* other error handling comes in calc program */

  if ((vdum=strchr(job,'$')) == NULL) {
    printf("<Incorrect variable syntax: missing $ >\n");
    return;
  } else if ((edum=strchr(job,'=')) == NULL || edum < vdum) {
    printf("<Incorrect expression syntax: missing = >\n");
    return;
  }    

  vdum++;
  edum++;
  len=strlen(vdum)-strlen(edum)-1;
  
  /* parse name first */
  
  strncpy(temp,vdum,len);
  temp[len] = '\0';
  sscanf(temp,"%s",vname);  /* to avoid white space between [var] & [=] */

  /* now pass expression to calc */
  strcpy(expression,edum);
  if (!sub_val_for_var(expression)) {
    printf("<Error setting variable, %s>\n", title);
    return;
  }
  val = calc(expression);

  /* if valid return, allocate variable, etc. else report error and return */
  assign_var(vname, val);

  return;
}



/* assign the variable vname the value val
   returns 1 for success, 0 for failure
   for internal use only */
int assign_var(char vname[], char val[]) {
  
  struct var_list *var;

  /* reassignment of existing variable? */
  for (var=vars; var != NULL; var=var->next) {
    if (strcmp(var->name,vname)==0) {
      strcpy(var->value,val);
      return 1;
    }
  }

  /* if non-existent, then insert at beginning of list */
  if ((var=(struct var_list*)malloc(sizeof(struct var_list))) == NULL) {
    printf("<sorry %s, no memory for variables>\n",title);
    return;
  }
  var->next=vars;
  vars=var;
  if ((var->name=(char *)malloc(1+strlen(vname))) == NULL) {
    free(var);
    printf("<sorry, %s.  No memory for variable name>\n",title);
    return 0;
  }
  if ((var->value=(char *)malloc(1+strlen(val))) == NULL) {
    free(var->name);
    free(var);
    printf("sorry, %s.  No memory for variable value>\n",title);
    return 0;
  }

  /* error check for format string will go here */

  strcpy(var->name,vname);
  strcpy(var->value,val);
  return 1;

}




void delete_var(char job[])
{
  int len;
  char *vdum,vname[MAXVAR],temp[MAXVAR];
  struct var_list *var,*prev;

  if ((vdum=strchr(job,'$')) == NULL) {
    printf("<Incorrect variable syntax: missing $ >\n");
    return;
  }
  
  vdum++;
  len=strlen(vdum);
  strncpy(temp,vdum,len); 
  temp[len] = '\0';
  if ( sscanf(temp,"%s",vname) < 1) {
    printf("<could not understand variable name, %s>\n", title);
    return;
  }
  
  prev=NULL;
  for (var=vars; var != NULL; var=var->next) {

    if (strcmp(var->name,vname)==0) {
      if (prev == NULL) {                  /* check linked-list syntax */
	vars=var->next;
      } else {
	prev->next=var->next;
      }
      printf("\n\tDeleting $%s\n\n",var->name);
      
      free(var->name);
      free(var->value);
      free(var);
      
      return;
    }
    prev=var;
  }
  printf("\n\t$%s not defined\n\n",vname);
  return;
}


/* N. B. this routine does not handle variable substitution for `set': */
/* this is handled within `set' because the syntax differs */
/* returns 1 if succeeded, 0 if not*/
int sub_val_for_var(char job[])
{
  int len, len0, vfound;
  char jobcpy[MAXCOMM] = "";
  char vname[MAXCOMM], *remainder, *dum;
  struct var_list *var;

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
      if (sscanf(dum, "%[0-9a-zA-z]", vname) < 1) {
	printf("<Error in variable syntax, %s>\n", title);
	printf("Received: <%s>\n", job);
	return 0;
      }
      remainder += strlen(vname);  /*be careful--won't work with braces*/
    } else {
      /* braces method */
      remainder++;
      if ( (dum = strchr(remainder,'}')) == NULL) {
	printf("<Error in variable syntax, %s>\n", title);
	printf("Received: <%s>\n", job);
	return 0;
      }
      len = dum-remainder;
      strncpy(vname, remainder, len);
      vname[len] = '\0';
      remainder += len+1;
    }

    /* look up and copy variable value */
    vfound=0;
    for (var=vars;var != NULL; var=var->next) {
      if ( strcmp(var->name,vname) == 0) {
	strcat(jobcpy, var->value);
	vfound=1;
	break;
      }
    }
    if (vfound==0) {
      printf("<variable %s not defined>\n",vname);
      return 0;
    }

  }  
  /* copy any additional part */
  strcat(jobcpy, remainder);
  strcpy(job,jobcpy);
  return 1;

}



