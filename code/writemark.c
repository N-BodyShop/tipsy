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

  if (sscanf(job,"%s %d %s",command, &box, filename) == 3) {
    if (boxes_loaded[box]) {
      infile = fopen(filename, "w");
      if(infile == NULL)
	{
	  printf("<Sorry %s, can't open file>\n",title);
	  return;
	}
	fprintf(infile, "%d %d %d\n" ,header.nbodies, header.nsph,
		header.nstar) ;
	    for (i = 0 ;i < boxlist[box].ngas ;i++) {
		gp = boxlist[box].gp[i] ;
		moff = gp - gas_particles;
		if(mark_gas[moff])
		  fprintf(infile, "%d\n", moff+1);
	    }
	    for (i = 0 ;i < boxlist[box].ndark ;i++) {
		dp = boxlist[box].dp[i] ;
		moff = dp - dark_particles;
		if(mark_dark[moff]) 
		  fprintf(infile, "%d\n", header.nsph+moff+1);
	    }
	    for (i = 0 ;i < boxlist[box].nstar ;i++) {
		sp = boxlist[box].sp[i] ;
		moff = sp - star_particles;
		if(mark_star[moff])
		  fprintf(infile, "%d\n", header.ndark+header.nsph+moff+1);
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
