/* $Header$
 * $Log$
 * Revision 1.1  1995/01/10 22:57:33  trq
 * Initial revision
 *
 * Revision 2.4  1994/09/06  23:09:56  trq
 * Changed cast to match new readline library.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "defs.h"

extern XtAppContext app_con;
extern Display *baseframe_dpy;
extern int display;

/* A static variable for holding the line. */
static char *my_gets_line = (char *)NULL;
     

/* Read a string, and return a pointer to it.  Returns NULL on EOF. */
char *
my_gets (prompt)
char *prompt;
{
  /* If the buffer has already been allocated, return the memory
     to the free pool. */
  if (my_gets_line != (char *)NULL)
    free (my_gets_line);

  /* Get a line from the user. */
  my_gets_line = readline (prompt);

  /* If the line has any text in it, save it on the history. */
  if (my_gets_line && *my_gets_line)
    add_history (my_gets_line);

  return (my_gets_line);
}

void
initialize_readline ()
{
  extern Function *rl_event_hook;
  extern void check_X_input();
   char **tipsy_completion ();

   /* Allow conditional parsing of the ~/.inputrc file. */
   rl_readline_name = "tipsy";

  if(display)
    rl_event_hook = (Function *) check_X_input;

   /* Tell the completer that we want a crack first. */
   rl_attempted_completion_function = (CPPFunction *)tipsy_completion;
  using_history();
  read_history(".tipsy_history");
}

 /* Attempt to complete on the contents of TEXT.  START and END show the
    region of TEXT that contains the word to complete.  We can use the
    entire line in case we want to do some simple parsing.  Return the
    array of matches, or NULL if there aren't any. */

char **
tipsy_completion (text, start, end)
      char *text;
      int start, end;
{
   char **matches;
   char *command_generator ();

   matches = (char **)NULL;

   /* If this word is at the start of the line, then it is a command
      to complete.  Otherwise it is the name of a file in the current
      directory. */
   if (start == 0)
     matches = completion_matches (text, command_generator);

   return (matches);
}

 /* Generator function for command completion.  STATE lets us know whether
    to start from scratch; without any state (i.e. STATE == 0), then we
    start at the top of the list. */

char *
command_generator (text, state)
      char *text;
      int state;
{
   static int list_index, len;
   char *name;

   /* If this is a new word to complete, initialize now.  This includes
      saving the length of TEXT for efficiency, and initializing the index
      variable to 0. */
   if (!state)
     {
       list_index = 0;
       len = strlen (text);
     }

   /* Return the next name which partially matches from the command list. */

   while ((list_index < ncomm) && (name = c_list[list_index].name))
     {
       list_index++;

       if (strncmp (name, text, len) == 0)
	 {
	   char *temp = strcpy(malloc(1+strlen(name)), name);
	   return (temp);
	 }
     }

   /* If no names matched, then return NULL. */
   return ((char *)NULL);
}

static int tty_ready = 0;
static XtInputId tty_input;

void
set_tty_ready(client_data, fid, id)
     XtPointer client_data;
     int *fid;
     XtInputId id;
{
  tty_ready = 1;
  XtRemoveInput(tty_input);
}

void
check_X_input()
{
  XEvent event;

  tty_input = XtAppAddInput(app_con, 0, (XtPointer) XtInputReadMask,
			    (XtInputCallbackProc) set_tty_ready, NULL);
  while(!tty_ready)
    {
      if(XtAppPeekEvent(app_con, &event) == True)
	{
	  XtAppNextEvent(app_con, &event);
	  XtDispatchEvent(&event);
	}
      else
	{
	  XtRemoveInput(tty_input);
	  return;
	}
    }
  tty_ready = 0;
}
