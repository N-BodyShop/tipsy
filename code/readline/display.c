/* display.c -- readline redisplay facility. */

/* Copyright (C) 1987, 1989, 1992 Free Software Foundation, Inc.

   This file is part of the GNU Readline Library, a library for
   reading lines of text with interactive input and history editing.

   The GNU Readline Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 1, or
   (at your option) any later version.

   The GNU Readline Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   The GNU General Public License is often shipped with GNU software, and
   is generally kept in a file called COPYING or LICENSE.  If you do not
   have a copy of the license, write to the Free Software Foundation,
   675 Mass Ave, Cambridge, MA 02139, USA. */
#define READLINE_LIBRARY

#include <stdio.h>
#include <sys/types.h>

#if defined (HAVE_UNISTD_H)
#  include <unistd.h>
#endif /* HAVE_UNISTD_H */

#if defined (HAVE_STDLIB_H)
#  include <stdlib.h>
#else
#  include "ansi_stdlib.h"
#endif /* HAVE_STDLIB_H */

#include "posixstat.h"

/* System-specific feature definitions and include files. */
#include "rldefs.h"

/* Some standard library routines. */
#include "readline.h"
#include "history.h"

#if !defined (strchr) && !defined (__STDC__)
extern char *strchr (), *strrchr ();
#endif /* !strchr && !__STDC__ */

/* Global and pseudo-global variables and functions
   imported from readline.c. */
extern char *rl_prompt;
extern int readline_echoing_p;
extern char *term_clreol, *term_im, *term_ic,  *term_ei, *term_DC;
/* Termcap variables. */
extern char *term_up, *term_dc, *term_cr, *term_IC;
extern int screenheight, screenwidth, screenchars;
extern int terminal_can_insert, term_xn;

extern void _rl_output_some_chars ();
extern int _rl_output_character_function ();

extern int _rl_output_meta_chars;
extern int _rl_horizontal_scroll_mode;
extern int _rl_mark_modified_lines;
extern int _rl_prefer_visible_bell;

/* Pseudo-global functions (local to the readline library) exported
   by this file. */
void _rl_move_cursor_relative (), _rl_output_some_chars ();
void _rl_move_vert ();

static void update_line (), clear_to_eol (), space_to_eol ();
static void delete_chars (), insert_some_chars ();

extern char *xmalloc (), *xrealloc ();

/* Heuristic used to decide whether it is faster to move from CUR to NEW
   by backing up or outputting a carriage return and moving forward. */
#define CR_FASTER(new, cur) (((new) + 1) < ((cur) - (new)))

/* **************************************************************** */
/*								    */
/*			Display stuff				    */
/*								    */
/* **************************************************************** */

/* This is the stuff that is hard for me.  I never seem to write good
   display routines in C.  Let's see how I do this time. */

/* (PWP) Well... Good for a simple line updater, but totally ignores
   the problems of input lines longer than the screen width.

   update_line and the code that calls it makes a multiple line,
   automatically wrapping line update.  Careful attention needs
   to be paid to the vertical position variables. */

/* Keep two buffers; one which reflects the current contents of the
   screen, and the other to draw what we think the new contents should
   be.  Then compare the buffers, and make whatever changes to the
   screen itself that we should.  Finally, make the buffer that we
   just drew into be the one which reflects the current contents of the
   screen, and place the cursor where it belongs.

   Commands that want to can fix the display themselves, and then let
   this function know that the display has been fixed by setting the
   RL_DISPLAY_FIXED variable.  This is good for efficiency. */

/* Global variables declared here. */
/* What YOU turn on when you have handled all redisplay yourself. */
int rl_display_fixed = 0;

/* The stuff that gets printed out before the actual text of the line.
   This is usually pointing to rl_prompt. */
char *rl_display_prompt = (char *)NULL;

/* Pseudo-global variables declared here. */
/* The visible cursor position.  If you print some text, adjust this. */
int _rl_last_c_pos = 0;
int _rl_last_v_pos = 0;

/* Number of lines currently on screen minus 1. */
int _rl_vis_botlin = 0;

/* Variables used only in this file. */
/* The last left edge of text that was displayed.  This is used when
   doing horizontal scrolling.  It shifts in thirds of a screenwidth. */
static int last_lmargin = 0;

/* The line display buffers.  One is the line currently displayed on
   the screen.  The other is the line about to be displayed. */
static char *visible_line = (char *)NULL;
static char *invisible_line = (char *)NULL;

/* A buffer for `modeline' messages. */
static char msg_buf[128];

/* Non-zero forces the redisplay even if we thought it was unnecessary. */
static int forced_display = 0;

/* Default and initial buffer size.  Can grow. */
static int line_size = 1024;

static char *last_prompt_string = (char *)NULL;
static char *local_prompt, *local_prompt_prefix;
static int visible_length, prefix_length;

/* The number of invisible characters in the line currently being
   displayed on the screen. */
static int visible_wrap_offset = 0;

/* The length (buffer offset) of the first line of the last (possibly
   multi-line) buffer displayed on the screen. */
static int visible_first_line_len = 0;

/* Expand the prompt string S and return the number of visible
   characters in *LP, if LP is not null.  This is currently more-or-less
   a placeholder for expansion. */

/* Current implementation:
	\001 (^A) start non-visible characters
	\002 (^B) end non-visible characters
   all characters except \001 and \002 (following a \001) are copied to
   the returned string; all characters except those between \001 and
   \002 are assumed to be `visible'. */	

static char *
expand_prompt (pmt, lp)
     char *pmt;
     int *lp;
{
  char *r, *ret, *p;
  int l, rl, ignoring;

  /* Short-circuit if we can. */
  if (strchr (pmt, RL_PROMPT_START_IGNORE) == 0)
    {
      r = savestring (pmt);
      if (lp)
	*lp = strlen (r);
      return r;
    }

  l = strlen (pmt);
  r = ret = xmalloc (l + 1);
  
  for (rl = ignoring = 0, p = pmt; p && *p; p++)
    {
      /* This code strips the invisible character string markers
	 RL_PROMPT_START_IGNORE and RL_PROMPT_END_IGNORE */
      if (*p == RL_PROMPT_START_IGNORE)
	{
	  ignoring++;
	  continue;
	}
      else if (ignoring && *p == RL_PROMPT_END_IGNORE)
	{
	  ignoring = 0;
	  continue;
	}
      else
	{
	  *r++ = *p;
	  if (!ignoring)
	    rl++;
	}
    }

  *r = '\0';
  if (lp)
    *lp = rl;
  return ret;
}

/*
 * Expand the prompt string into the various display components, if
 * necessary.
 *
 * local_prompt = expanded last line of string in rl_display_prompt
 *		  (portion after the final newline)
 * local_prompt_prefix = portion before last newline of rl_display_prompt,
 *			 expanded via expand_prompt
 * visible_length = number of visible characters in local_prompt
 * prefix_length = number of visible characters in local_prompt_prefix
 *
 * This function is called once per call to readline().  It may also be
 * called arbitrarily to expand the primary prompt.
 *
 * The return value is the number of visible characters on the last line
 * of the (possibly multi-line) prompt.
 */
int
rl_expand_prompt (prompt)
     char *prompt;
{
  char *p, *t;
  int c;

  /* Clear out any saved values. */
  if (local_prompt)
    free (local_prompt);
  if (local_prompt_prefix)
    free (local_prompt_prefix);
  local_prompt = local_prompt_prefix = (char *)0;

  p = strrchr (prompt, '\n');
  if (!p)
    {
      /* The prompt is only one line. */
      local_prompt = expand_prompt (prompt, &visible_length);
      local_prompt_prefix = (char *)0;
      return (visible_length);
    }
  else
    {
      /* The prompt spans multiple lines. */
      t = ++p;
      local_prompt = expand_prompt (p, &visible_length);
      c = *t; *t = '\0';
      /* The portion of the prompt string up to and including the
	 final newline is now null-terminated. */
      local_prompt_prefix = expand_prompt (prompt, &prefix_length);
      *t = c;
      return (prefix_length);
    }
}

/* Basic redisplay algorithm. */
void
rl_redisplay ()
{
  register int in, out, c, linenum;
  register char *line = invisible_line;
  int c_pos = 0, inv_botlin = 0, wrap_offset, wrap_column;
  char *prompt_this_line;

  if (!readline_echoing_p)
    return;

  if (!rl_display_prompt)
    rl_display_prompt = "";

  if (!invisible_line)
    {
      visible_line = xmalloc (line_size);
      invisible_line = xmalloc (line_size);
      line = invisible_line;
      for (in = 0; in < line_size; in++)
	{
	  visible_line[in] = 0;
	  invisible_line[in] = 1;
	}
      rl_on_new_line ();
    }

  /* Draw the line into the buffer. */
  c_pos = -1;

  /* Mark the line as modified or not.  We only do this for history
     lines. */
  out = 0;
  if (_rl_mark_modified_lines && current_history () && rl_undo_list)
    {
      line[out++] = '*';
      line[out] = '\0';
    }

  /* If someone thought that the redisplay was handled, but the currently
     visible line has a different modification state than the one about
     to become visible, then correct the caller's misconception. */
  if (visible_line[0] != invisible_line[0])
    rl_display_fixed = 0;

  /* If the prompt to be displayed is the `primary' readline prompt (the
     one passed to readline()), use the values we have already expanded.
     If not, use what's already in rl_display_prompt.  WRAP_OFFSET is the
     number of non-visible characters in the prompt string. */
  if (rl_display_prompt == rl_prompt)
    {
      int local_len = local_prompt ? strlen (local_prompt) : 0;
      if (local_prompt_prefix && forced_display)
	_rl_output_some_chars (local_prompt_prefix, strlen (local_prompt_prefix));

      if (local_len > 0)
	strncpy (line + out, local_prompt, local_len);
      out += local_len;
      line[out] = '\0';
      wrap_offset = local_len - visible_length;
    }
  else
    {
      int pmtlen;
      prompt_this_line = strrchr (rl_display_prompt, '\n');
      if (!prompt_this_line)
	prompt_this_line = rl_display_prompt;
      else
	{
	  prompt_this_line++;
	  if (forced_display)
	    _rl_output_some_chars (rl_display_prompt, prompt_this_line - rl_display_prompt);
	}

      pmtlen = strlen (prompt_this_line);
      strncpy (line + out,  prompt_this_line, pmtlen);
      out += pmtlen;
      line[out] = '\0';
      wrap_offset = 0;
    }

  for (in = 0; in < rl_end; in++)
    {
      c = (unsigned char)rl_line_buffer[in];

      if (out + 8 >= line_size)		/* XXX - 8 for \t */
	{
	  line_size *= 2;
	  visible_line = xrealloc (visible_line, line_size);
	  invisible_line = xrealloc (invisible_line, line_size);
	  line = invisible_line;
	}

      if (in == rl_point)
	c_pos = out;

      if (META_CHAR (c))
	{
	  if (_rl_output_meta_chars == 0)
	    {
	      sprintf (line + out, "\\%o", c);
	      out += 4;
	    }
	  else
	    line[out++] = c;	  
	}
#if defined (DISPLAY_TABS)
      else if (c == '\t')
	{
	  register int newout = (out | (int)7) + 1;
	  while (out < newout)
	    line[out++] = ' ';
	}
#endif
      else if (c < ' ')
	{
	  line[out++] = '^';
	  line[out++] = UNCTRL (c);	/* XXX was c ^ 0x40 */
	}
      else if (c == 127)
	{
	  line[out++] = '^';
	  line[out++] = '?';
	}
      else
	line[out++] = c;
    }
  line[out] = '\0';
  if (c_pos < 0)
    c_pos = out;

  /* C_POS == position in buffer where cursor should be placed. */

  /* PWP: now is when things get a bit hairy.  The visible and invisible
     line buffers are really multiple lines, which would wrap every
     (screenwidth - 1) characters.  Go through each in turn, finding
     the changed region and updating it.  The line order is top to bottom. */

  /* If we can move the cursor up and down, then use multiple lines,
     otherwise, let long lines display in a single terminal line, and
     horizontally scroll it. */

  if (!_rl_horizontal_scroll_mode && term_up && *term_up)
    {
      int total_screen_chars = screenchars;
      int nleft, cursor_linenum, pos, changed_screen_line;

      if (!rl_display_fixed || forced_display)
	{
	  forced_display = 0;

	  /* If we have more than a screenful of material to display, then
	     only display a screenful.  We should display the last screen,
	     not the first.  I'll fix this in a minute. */
	  if (out >= total_screen_chars)
	    out = total_screen_chars - 1;

	  /* Number of screen lines to display.  The first line wraps at
	     (screenwidth + wrap_offset) chars, the rest of the lines have
	     screenwidth chars. */
	  nleft = out - wrap_offset + term_xn - 1;
	  inv_botlin = (nleft > 0) ? nleft / screenwidth : 0;

	  /* The first line is at character position 0 in the buffer.  The
	     second and subsequent lines start at N * screenwidth, offset by
	     OFFSET.  OFFSET is wrap_offset for the invisible line and
	     visible_wrap_offset for the line currently displayed. */

#define W_OFFSET(line, offset) ((line) == 0 ? offset : 0)
#define L_OFFSET(n, offset) ((n) > 0 ? ((n) * screenwidth) + (offset) : 0)
#define VIS_CHARS(line) &visible_line[L_OFFSET((line), visible_wrap_offset)]
#define VIS_LINE(line) ((line) > _rl_vis_botlin) ? "" : VIS_CHARS(line)
#define INV_LINE(line) &invisible_line[L_OFFSET((line), wrap_offset)]

	  /* For each line in the buffer, do the updating display. */
	  for (linenum = 0; linenum <= inv_botlin; linenum++)
	    {
	      update_line (VIS_LINE(linenum), INV_LINE(linenum), linenum,
			   screenwidth + W_OFFSET(linenum, visible_wrap_offset),
			   screenwidth + W_OFFSET(linenum, wrap_offset),
			   inv_botlin);

	      /* If this is the line with the prompt, we might need to
		 compensate for invisible characters in the new line. Do
		 this only if there is not more than one new line (which
		 implies that we completely overwrite the old visible line)
		 and the new line is shorter than the old. */
	      if (linenum == 0 &&
		  inv_botlin == 0 &&
		  (wrap_offset > visible_wrap_offset) &&
		  (_rl_last_c_pos < visible_first_line_len))
		{
		  nleft = screenwidth + wrap_offset - _rl_last_c_pos;
		  clear_to_eol (nleft);
		}

	      /* Since the new first line is now visible, save its length. */
	      if (linenum == 0)
		visible_first_line_len = (inv_botlin > 0) ? screenwidth : out - wrap_offset;
	    }

	  /* We may have deleted some lines.  If so, clear the left over
	     blank ones at the bottom out. */
	  if (_rl_vis_botlin > inv_botlin)
	    {
	      char *tt;
	      for (; linenum <= _rl_vis_botlin; linenum++)
		{
		  tt = VIS_CHARS (linenum);
		  _rl_move_vert (linenum);
		  _rl_move_cursor_relative (0, tt);
		  clear_to_eol
		    ((linenum == _rl_vis_botlin) ? strlen (tt) : screenwidth);
		}
	    }
	  _rl_vis_botlin = inv_botlin;

	  /* Move the cursor where it should be. */
	  /* Which line? */
	  nleft = c_pos - wrap_offset - term_xn + 1;
	  cursor_linenum = (nleft > 0) ? nleft / screenwidth : 0;

	  /* CHANGED_SCREEN_LINE is set to 1 if we have moved to a
	     different screen line during this redisplay. */
	  changed_screen_line = _rl_last_v_pos != cursor_linenum;
	  if (changed_screen_line)
	    {
	      _rl_move_vert (cursor_linenum);
	      /* If we moved up to the line with the prompt using term_up,
	         the physical cursor position on the screen stays the same,
	         but the buffer position needs to be adjusted to account
	         for invisible characters. */
	      if (cursor_linenum == 0 && wrap_offset)
	        _rl_last_c_pos += wrap_offset;
	    }

	  /* We have to reprint the prompt if it contains invisible
	     characters, since it's not generally OK to just reprint
	     the characters from the current cursor position. */
	  nleft = visible_length + wrap_offset;
	  if (cursor_linenum == 0 && wrap_offset > 0 && _rl_last_c_pos > 0 &&
	      _rl_last_c_pos <= nleft && local_prompt)
	    {
	      if (term_cr)
		tputs (term_cr, 1, _rl_output_character_function);
	      _rl_output_some_chars (local_prompt, nleft);
	      _rl_last_c_pos = nleft;
	    }

	  /* Where on that line?  And where does that line start
	     in the buffer? */
	  pos = L_OFFSET(cursor_linenum, wrap_offset);
	  /* nleft == number of characters in the line buffer between the
	     start of the line and the cursor position. */
	  nleft = c_pos - pos;

	  /* Since backspace() doesn't know about invisible characters in the
	     prompt, and there's no good way to tell it, we compensate for
	     those characters here and call backspace() directly. */
	  if (wrap_offset && cursor_linenum == 0 && nleft < _rl_last_c_pos)
	    {
	      backspace (_rl_last_c_pos - nleft);
	      _rl_last_c_pos = nleft;
	    }

	  if (nleft != _rl_last_c_pos)
	    _rl_move_cursor_relative (nleft, &invisible_line[pos]);
	}
    }
  else				/* Do horizontal scrolling. */
    {
#define M_OFFSET(margin, offset) ((margin) == 0 ? offset : 0)
      int lmargin, ndisp, nleft, phys_c_pos, t;

      /* Always at top line. */
      _rl_last_v_pos = 0;

      /* Compute where in the buffer the displayed line should start.  This
	 will be LMARGIN. */

      /* The number of characters that will be displayed before the cursor. */
      ndisp = c_pos - wrap_offset;
      nleft  = visible_length + wrap_offset;
      /* Where the new cursor position will be on the screen.  This can be
         longer than SCREENWIDTH; if it is, lmargin will be adjusted. */
      phys_c_pos = c_pos - (last_lmargin ? last_lmargin : wrap_offset);
      t = screenwidth / 3;

      /* If the number of characters had already exceeded the screenwidth,
         last_lmargin will be > 0. */

      /* If the number of characters to be displayed is more than the screen
         width, compute the starting offset so that the cursor is about
         two-thirds of the way across the screen. */
      if (phys_c_pos > screenwidth - 2)
	{
	  lmargin = c_pos - (2 * t);
	  if (lmargin < 0)
	    lmargin = 0;
	  /* If the left margin would be in the middle of a prompt with
	     invisible characters, don't display the prompt at all. */
	  if (wrap_offset && lmargin > 0 && lmargin < nleft)
	    lmargin = nleft;
	}
      else if (ndisp < screenwidth - 2)		/* XXX - was -1 */
        lmargin = 0;
      else if (phys_c_pos < 1)
	{
	  /* If we are moving back towards the beginning of the line and
	     the last margin is no longer correct, compute a new one. */
	  lmargin = ((c_pos - 1) / t) * t;	/* XXX */
	  if (wrap_offset && lmargin > 0 && lmargin < nleft)
	    lmargin = nleft;
	}
      else
        lmargin = last_lmargin;

      /* If the first character on the screen isn't the first character
	 in the display line, indicate this with a special character. */
      if (lmargin > 0)
	line[lmargin] = '<';

      /* If SCREENWIDTH characters starting at LMARGIN do not encompass
         the whole line, indicate that with a special characters at the
         right edge of the screen.  If LMARGIN is 0, we need to take the
         wrap offset into account. */
      t = lmargin + M_OFFSET (lmargin, wrap_offset) + screenwidth;
      if (t < out)
        line[t - 1] = '>';

      if (!rl_display_fixed || forced_display || lmargin != last_lmargin)
	{
	  forced_display = 0;
	  update_line (&visible_line[last_lmargin],
		       &invisible_line[lmargin],
		       0,
		       screenwidth + visible_wrap_offset,
		       screenwidth + (lmargin ? 0 : wrap_offset),
		       0);

	  /* If the visible new line is shorter than the old, but the number
	     of invisible characters is greater, and we are at the end of
	     the new line, we need to clear to eol. */
	  t = _rl_last_c_pos - M_OFFSET (lmargin, wrap_offset);
	  if ((M_OFFSET (lmargin, wrap_offset) > visible_wrap_offset) &&
	      (_rl_last_c_pos == out) &&
	      t < visible_first_line_len)
	    {
	      nleft = screenwidth - t;
	      clear_to_eol (nleft);
	    }
	  visible_first_line_len = out - lmargin - M_OFFSET (lmargin, wrap_offset);
	  if (visible_first_line_len > screenwidth)
	    visible_first_line_len = screenwidth;

	  _rl_move_cursor_relative (c_pos - lmargin, &invisible_line[lmargin]);
	  last_lmargin = lmargin;
	}
    }
  fflush (rl_outstream);

  /* Swap visible and non-visible lines. */
  {
    char *temp = visible_line;
    visible_line = invisible_line;
    invisible_line = temp;
    rl_display_fixed = 0;
    /* If we are displaying on a single line, and last_lmargin is > 0, we
       are not displaying any invisible characters, so set visible_wrap_offset
       to 0. */
    if (_rl_horizontal_scroll_mode && last_lmargin)
      visible_wrap_offset = 0;
    else
      visible_wrap_offset = wrap_offset;
  }
}

/* PWP: update_line() is based on finding the middle difference of each
   line on the screen; vis:

			     /old first difference
	/beginning of line   |	      /old last same       /old EOL
	v		     v	      v		    v
old:	eddie> Oh, my little gruntle-buggy is to me, as lurgid as
new:	eddie> Oh, my little buggy says to me, as lurgid as
	^		     ^	^			   ^
	\beginning of line   |	\new last same	   \new end of line
			     \new first difference

   All are character pointers for the sake of speed.  Special cases for
   no differences, as well as for end of line additions must be handeled.

   Could be made even smarter, but this works well enough */
static void
update_line (old, new, current_line, omax, nmax, inv_botlin)
     register char *old, *new;
     int current_line, omax, nmax;
{
  register char *ofd, *ols, *oe, *nfd, *nls, *ne;
  int temp, lendiff, wsatend, od, nd;

  /* If we're at the right edge of a terminal that supports xn, we're
     ready to wrap around, so do so.  This fixes problems with knowing
     the exact cursor position and cut-and-paste with certain terminal
     emulators.  In this calculation, TEMP is the physical screen
     position of the cursor. */
  temp = _rl_last_c_pos - W_OFFSET(_rl_last_v_pos, visible_wrap_offset);
  if (temp == screenwidth && term_xn && !_rl_horizontal_scroll_mode
      && _rl_last_v_pos == current_line - 1)
    {
      if (new[0])
	putc (new[0], rl_outstream);
      else
	putc (' ', rl_outstream);
      _rl_last_c_pos = 1;		/* XXX */
      _rl_last_v_pos++;
      if (old[0])
        old[0] = new[0];
    }
      
  /* Find first difference. */
  for (ofd = old, nfd = new;
       (ofd - old < omax) && *ofd && (*ofd == *nfd);
       ofd++, nfd++)
    ;

  /* Move to the end of the screen line.  ND and OD are used to keep track
     of the distance between ne and new and oe and old, respectively, to
     move a subtraction out of each loop. */
  for (od = ofd - old, oe = ofd; od < omax && *oe; oe++, od++);
  for (nd = nfd - new, ne = nfd; nd < nmax && *ne; ne++, nd++);

  /* If no difference, continue to next line. */
  if (ofd == oe && nfd == ne)
    return;

  wsatend = 1;			/* flag for trailing whitespace */
  ols = oe - 1;			/* find last same */
  nls = ne - 1;
  while ((ols > ofd) && (nls > nfd) && (*ols == *nls))
    {
      if (*ols != ' ')
	wsatend = 0;
      ols--;
      nls--;
    }

  if (wsatend)
    {
      ols = oe;
      nls = ne;
    }
  else if (*ols != *nls)
    {
      if (*ols)			/* don't step past the NUL */
	ols++;
      if (*nls)
	nls++;
    }

  _rl_move_vert (current_line);

  /* If this is the first line and there are invisible characters in the
     prompt string, and the prompt string has not changed, then redraw
     the entire prompt string.  We can only do this reliably if the
     terminal supports a `cr' capability.

     This is more than just an efficiency hack -- there is a problem with
     redrawing portions of the prompt string if they contain terminal
     escape sequences (like drawing the `unbold' sequence without a
     corresponding `bold') that manifests itself on certain terminals. */

  lendiff = strlen (local_prompt);
  if (current_line == 0 && !_rl_horizontal_scroll_mode &&
      lendiff > visible_length &&
      _rl_last_c_pos > 0 && (ofd - old) >= lendiff && term_cr)
    {
      tputs (term_cr, 1, _rl_output_character_function);
      _rl_output_some_chars (local_prompt, lendiff);
      _rl_last_c_pos = lendiff;
    }

  _rl_move_cursor_relative (ofd - old, old);

  /* if (len (new) > len (old)) */
  lendiff = (nls - nfd) - (ols - ofd);

  /* Insert (diff (len (old), len (new)) ch. */
  temp = ne - nfd;
  if (lendiff > 0)
    {
      /* Non-zero if we're increasing the number of lines. */
      int gl = current_line >= _rl_vis_botlin && inv_botlin > _rl_vis_botlin;
      /* Sometimes it is cheaper to print the characters rather than
	 use the terminal's capabilities.  If we're growing the number
	 of lines, make sure we actually cause the new line to wrap
	 around on auto-wrapping terminals. */
      if (terminal_can_insert && ((2 * temp) >= lendiff || term_IC) && (!term_xn || !gl))
	{
	  /* If lendiff > visible_length and _rl_last_c_pos == 0 and
	     _rl_horizontal_scroll_mode == 1, inserting the characters with
	     term_IC or term_ic will screw up the screen because of the
	     invisible characters.  We need to just draw them. */
	  if (*ols && (!_rl_horizontal_scroll_mode || _rl_last_c_pos > 0 ||
			lendiff <= visible_length))
	    {
	      insert_some_chars (nfd, lendiff);
	      _rl_last_c_pos += lendiff;
	    }
	  else
	    {
	      /* At the end of a line the characters do not have to
		 be "inserted".  They can just be placed on the screen. */
	      _rl_output_some_chars (nfd, lendiff);
	      _rl_last_c_pos += lendiff;
	    }
	  /* Copy (new) chars to screen from first diff to last match. */
	  temp = nls - nfd;
	  if ((temp - lendiff) > 0)
	    {
	      _rl_output_some_chars (nfd + lendiff, temp - lendiff);
	      _rl_last_c_pos += temp - lendiff;
	    }
	}
      else
	{
	  /* cannot insert chars, write to EOL */
	  _rl_output_some_chars (nfd, temp);
	  _rl_last_c_pos += temp;
	}
    }
  else				/* Delete characters from line. */
    {
      /* If possible and inexpensive to use terminal deletion, then do so. */
      if (term_dc && (2 * temp) >= -lendiff)
	{
	  /* If all we're doing is erasing the invisible characters in the
	     prompt string, don't bother.  It screws up the assumptions
	     about what's on the screen. */
	  if (_rl_horizontal_scroll_mode && _rl_last_c_pos == 0 &&
	      -lendiff == visible_wrap_offset)
	    lendiff = 0;

	  if (lendiff)
	    delete_chars (-lendiff); /* delete (diff) characters */

	  /* Copy (new) chars to screen from first diff to last match */
	  temp = nls - nfd;
	  if (temp > 0)
	    {
	      _rl_output_some_chars (nfd, temp);
	      _rl_last_c_pos += temp;
	    }
	}
      /* Otherwise, print over the existing material. */
      else
	{
	  if (temp > 0)
	    {
	      _rl_output_some_chars (nfd, temp);
	      _rl_last_c_pos += temp;
	    }
	  lendiff = (oe - old) - (ne - new);
	  if (term_xn && current_line < inv_botlin)
	    space_to_eol (lendiff);
	  else
	    clear_to_eol (lendiff);
	}
    }
}

/* Tell the update routines that we have moved onto a new (empty) line. */
rl_on_new_line ()
{
  if (visible_line)
    visible_line[0] = '\0';

  _rl_last_c_pos = _rl_last_v_pos = 0;
  _rl_vis_botlin = last_lmargin = 0;
  return 0;
}

/* Actually update the display, period. */
rl_forced_update_display ()
{
  if (visible_line)
    {
      register char *temp = visible_line;

      while (*temp) *temp++ = '\0';
    }
  rl_on_new_line ();
  forced_display++;
  rl_redisplay ();
  return 0;
}

/* Move the cursor from _rl_last_c_pos to NEW, which are buffer indices.
   DATA is the contents of the screen line of interest; i.e., where
   the movement is being done. */
void
_rl_move_cursor_relative (new, data)
     int new;
     char *data;
{
  register int i;

  /* If we don't have to do anything, then return. */
  if (_rl_last_c_pos == new) return;

  /* It may be faster to output a CR, and then move forwards instead
     of moving backwards. */
  /* i == current physical cursor position. */
  i = _rl_last_c_pos - W_OFFSET(_rl_last_v_pos, visible_wrap_offset);
  if (CR_FASTER (new, _rl_last_c_pos) || (term_xn && i == screenwidth))
    {
#if defined (__MSDOS__)
      putc ('\r', rl_outstream);
#else
      tputs (term_cr, 1, _rl_output_character_function);
#endif /* !__MSDOS__ */
      _rl_last_c_pos = 0;
    }

  if (_rl_last_c_pos < new)
    {
      /* Move the cursor forward.  We do it by printing the command
	 to move the cursor forward if there is one, else print that
	 portion of the output buffer again.  Which is cheaper? */

      /* The above comment is left here for posterity.  It is faster
	 to print one character (non-control) than to print a control
	 sequence telling the terminal to move forward one character.
	 That kind of control is for people who don't know what the
	 data is underneath the cursor. */
#if defined (HACK_TERMCAP_MOTION)
      extern char *term_forward_char;

      if (term_forward_char)
	for (i = _rl_last_c_pos; i < new; i++)
	  tputs (term_forward_char, 1, _rl_output_character_function);
      else
	for (i = _rl_last_c_pos; i < new; i++)
	  putc (data[i], rl_outstream);
#else
      for (i = _rl_last_c_pos; i < new; i++)
	putc (data[i], rl_outstream);
#endif /* HACK_TERMCAP_MOTION */
    }
  else if (_rl_last_c_pos != new)
    backspace (_rl_last_c_pos - new);
  _rl_last_c_pos = new;
}

/* PWP: move the cursor up or down. */
void
_rl_move_vert (to)
     int to;
{
  register int delta, i;

  if (_rl_last_v_pos == to || to > screenheight)
    return;

#if defined (__GO32__)
  {
    int row, col;

    ScreenGetCursor (&row, &col);
    ScreenSetCursor ((row + to - _rl_last_v_pos), col);
  }
#else /* !__GO32__ */

  if ((delta = to - _rl_last_v_pos) > 0)
    {
      for (i = 0; i < delta; i++)
	putc ('\n', rl_outstream);
      tputs (term_cr, 1, _rl_output_character_function);
      _rl_last_c_pos = 0;
    }
  else
    {			/* delta < 0 */
      if (term_up && *term_up)
	for (i = 0; i < -delta; i++)
	  tputs (term_up, 1, _rl_output_character_function);
    }
#endif /* !__GO32__ */
  _rl_last_v_pos = to;		/* Now TO is here */
}

/* Physically print C on rl_outstream.  This is for functions which know
   how to optimize the display.  Return the number of characters output. */
rl_show_char (c)
     int c;
{
  int n = 1;
  if (META_CHAR (c) && (_rl_output_meta_chars == 0))
    {
      fprintf (rl_outstream, "M-");
      n += 2;
      c = UNMETA (c);
    }

#if defined (DISPLAY_TABS)
  if (c < 32 && c != '\t')
#else
  if (c < 32)
#endif /* !DISPLAY_TABS */
    {
      fprintf (rl_outstream, "C-");
      n += 2;
      c += 64;
    }

  putc (c, rl_outstream);
  fflush (rl_outstream);
  return n;
}

int
rl_character_len (c, pos)
     register int c, pos;
{
  unsigned char uc;

  uc = (unsigned char)c;

  if (META_CHAR (uc))
    return ((_rl_output_meta_chars == 0) ? 4 : 1);

  if (uc == '\t')
    {
#if defined (DISPLAY_TABS)
      return (((pos | 7) + 1) - pos);
#else
      return (2);
#endif /* !DISPLAY_TABS */
    }

  return ((isprint (uc)) ? 1 : 2);
}

/* How to print things in the "echo-area".  The prompt is treated as a
   mini-modeline. */

#if defined (HAVE_VARARGS_H)
rl_message (va_alist)
     va_dcl
{
  char *format;
  va_list args;

  va_start (args);
  format = va_arg (args, char *);
  vsprintf (msg_buf, format, args);
  va_end (args);

  rl_display_prompt = msg_buf;
  rl_redisplay ();
  return 0;
}
#else /* !HAVE_VARARGS_H */
rl_message (format, arg1, arg2)
     char *format;
{
  sprintf (msg_buf, format, arg1, arg2);
  rl_display_prompt = msg_buf;
  rl_redisplay ();
  return 0;
}
#endif /* !HAVE_VARARGS_H */

/* How to clear things from the "echo-area". */
rl_clear_message ()
{
  rl_display_prompt = rl_prompt;
  rl_redisplay ();
  return 0;
}

rl_reset_line_state ()
{
  rl_on_new_line ();

  rl_display_prompt = rl_prompt ? rl_prompt : "";
  forced_display = 1;
  return 0;
}

/* Quick redisplay hack when erasing characters at the end of the line. */
void
_rl_erase_at_end_of_line (l)
     int l;
{
  register int i;

  backspace (l);
  for (i = 0; i < l; i++)
    putc (' ', rl_outstream);
  backspace (l);
  for (i = 0; i < l; i++)
    visible_line[--_rl_last_c_pos] = '\0';
  rl_display_fixed++;
}

/* Clear to the end of the line.  COUNT is the minimum
   number of character spaces to clear, */
static void
clear_to_eol (count)
     int count;
{
#if !defined (__GO32__)
  if (term_clreol)
    {
      tputs (term_clreol, 1, _rl_output_character_function);
    }
  else
#endif /* !__GO32__ */
    space_to_eol (count);
}

/* Clear to the end of the line using spaces.  COUNT is the minimum
   number of character spaces to clear, */
static void
space_to_eol (count)
     int count;
{
  register int i;

  for (i = 0; i < count; i++)
   putc (' ', rl_outstream);

  _rl_last_c_pos += count;
}

/* Insert COUNT characters from STRING to the output stream. */
static void
insert_some_chars (string, count)
     char *string;
     int count;
{
#if defined (__GO32__)
  int row, col, width;
  char *row_start;

  ScreenGetCursor (&row, &col);
  width = ScreenCols ();
  row_start = ScreenPrimary + (row * width);

  memcpy (row_start + col + count, row_start + col, width - col - count);

  /* Place the text on the screen. */
  _rl_output_some_chars (string, count);
#else /* !_GO32 */

  /* If IC is defined, then we do not have to "enter" insert mode. */
  if (term_IC)
    {
      char *tgoto (), *buffer;
      buffer = tgoto (term_IC, 0, count);
      tputs (buffer, 1, _rl_output_character_function);
      _rl_output_some_chars (string, count);
    }
  else
    {
      register int i;

      /* If we have to turn on insert-mode, then do so. */
      if (term_im && *term_im)
	tputs (term_im, 1, _rl_output_character_function);

      /* If there is a special command for inserting characters, then
	 use that first to open up the space. */
      if (term_ic && *term_ic)
	{
	  for (i = count; i--; )
	    tputs (term_ic, 1, _rl_output_character_function);
	}

      /* Print the text. */
      _rl_output_some_chars (string, count);

      /* If there is a string to turn off insert mode, we had best use
	 it now. */
      if (term_ei && *term_ei)
	tputs (term_ei, 1, _rl_output_character_function);
    }
#endif /* !__GO32__ */
}

/* Delete COUNT characters from the display line. */
static void
delete_chars (count)
     int count;
{
#if defined (__GO32__)
  int row, col, width;
  char *row_start;

  ScreenGetCursor (&row, &col);
  width = ScreenCols ();
  row_start = ScreenPrimary + (row * width);

  memcpy (row_start + col, row_start + col + count, width - col - count);
  memset (row_start + width - count, 0, count * 2);
#else /* !_GO32 */

  if (count > screenwidth)	/* XXX */
    return;

  if (term_DC && *term_DC)
    {
      char *tgoto (), *buffer;
      buffer = tgoto (term_DC, count, count);
      tputs (buffer, count, _rl_output_character_function);
    }
  else
    {
      if (term_dc && *term_dc)
	while (count--)
	  tputs (term_dc, 1, _rl_output_character_function);
    }
#endif /* !__GO32__ */
}

void
_rl_update_final ()
{
  int full_lines;

  full_lines = 0;
  if (_rl_vis_botlin && visible_line[screenwidth * _rl_vis_botlin] == 0)
    {
      _rl_vis_botlin--;
      full_lines = 1;
    }
  _rl_move_vert (_rl_vis_botlin);
  if (full_lines && term_xn)
    {
      /* Remove final line-wrap flag in xterm. */
      char *last_line;
      last_line = &visible_line[screenwidth * _rl_vis_botlin];
      _rl_move_cursor_relative (screenwidth - 1, last_line);
      clear_to_eol (0);
      putc (last_line[screenwidth - 1], rl_outstream);
    }
  _rl_vis_botlin = 0;
  crlf ();
  fflush (rl_outstream);
  rl_display_fixed++;
}
