#include "defs.h"
#include "fdefs.h"

void
writemark(job)
     char *job;
     
{
  char command[MAXCOMM] ;
  char filename[MAXCOMM] ;
  FILE *infile;
  int i;
  int box;
    struct gas_particle *gp ;
    struct dark_particle *dp ;
    struct star_particle *sp ;
  int moff;
  int nbodies, nsph, ndark, nstar;

  if (sscanf(job,"%s %d %s",command, &box, filename) == 3) {
    if (boxes_loaded[box]) {
        infile = fopen(filename, "w");
	if(infile == NULL)
	    {
	      printf("<Sorry %s, can't open file>\n",title);
	      return;
	    }
        if(box0_pi) {		/* estimate total quantities */
	    nbodies = box0_pi[header.nbodies-1]+1;
	    if(header.nsph > 0)
	      nsph = box0_pi[header.nsph-1]+1;
	    else
	      nsph = 0;
	    if(header.nstar > 0)
	      nstar = nbodies - box0_pi[header.nsph+header.ndark];
	    else
	      nstar = 0;
	} else {
	    nbodies = header.nbodies;
	    nsph = header.nsph;
	    nstar = header.nstar;
	}
	fprintf(infile, "%d %d %d\n" ,nbodies, nsph, nstar) ;
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		moff = gp - gas_particles;
		if(mark_gas[moff]) {
		    if(box0_pi)	/* partial box loaded */
			moff = box0_pi[moff];
		    fprintf(infile, "%d\n", moff+1);
		}
	    }
	    for (i = 0 ;i < boxlist[box].ndark ;i++) {
		dp = boxlist[box].dp[i] ;
		moff = dp - dark_particles;
		if(mark_dark[moff]) {
		    moff += header.nsph;
		    if(box0_pi)	/* partial box loaded */
		        moff = box0_pi[moff];
		    fprintf(infile, "%d\n", moff+1);
		}
	    }
	    for (i = 0 ;i < boxlist[box].nstar ;i++) {
		sp = boxlist[box].sp[i] ;
		moff = sp - star_particles;
		if(mark_star[moff]) {
		    moff += header.nsph + header.ndark;
		    if(box0_pi)	/* partial box loaded */
		        moff = box0_pi[moff];
		    fprintf(infile, "%d\n", moff+1);
		}
	    }
        fclose(infile);
    }
    else
      {
	printf("<sorry, box %d is not loaded, %s>\n",box,title) ;
      }
    }
    else {
	input_error(command) ;
    }
}
