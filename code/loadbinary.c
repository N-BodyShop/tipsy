#include "defs.h"
#include <stdlib.h>


void
loadbinary(infile,time)
    FILE *infile;
    double time;
{
    static double currtime = 0.0;
    static long currpos = 0L ;
    static long lastpos = 0L ;
    int old_nstar;
    int i;

    if ((float)currtime > (float)time){
	fseek(infile,0L,0);
	currtime=0.0;
	currpos=0;
    }
    old_nstar = header.nstar;
    forever {
	if(fread((char *)&header,sizeof(header),1,infile) 
	   != 1) {
	    printf("<sorry time too large %s, using %f>\n",title,
			     (float)currtime) ;
	    break ;
	}
	currtime = header.time ;
	currpos = ftell(infile) - sizeof(header);
	if ( (float)header.time >= (float)time ) 
	    break ;
	fseek(infile,
	    sizeof(gas_particles[0])*header.nsph +
	    sizeof(dark_particles[0])*header.ndark +
	    sizeof(star_particles[0])*header.nstar,
	    1) ;
    }	
    fseek(infile,currpos,0) ;
    lastpos = currpos ;
    fread((char *)&header,sizeof(header),1,infile) ;

    if(gas_particles != NULL) free(gas_particles);
    if(header.nsph != 0) {
	gas_particles = (struct gas_particle *)
			    malloc(header.nsph*sizeof(*gas_particles));
	if(gas_particles == NULL) {
	    printf("<sorry, no memory for gas particles, %s>\n",title) ;
	    return ;
	}
    }
    else
      gas_particles = NULL;
	    
    if(dark_particles != NULL) free(dark_particles);
    if(header.ndark != 0) {
	dark_particles = (struct dark_particle *)
			    malloc(header.ndark*sizeof(*dark_particles));
	if(dark_particles == NULL) {
	    printf("<sorry, no memory for dark particles, %s>\n",title) ;
	    return ;
	}
    }
    else
      dark_particles = NULL;

    if(star_particles != NULL) free(star_particles);
    if(header.nstar != 0) {
	star_particles = (struct star_particle *)
			    malloc(header.nstar*sizeof(*star_particles));
	if(star_particles == NULL) {
	    printf("<sorry, no memory for star particles, %s>\n",title) ;
	    return ;
	}
    }
    else
      star_particles = NULL;

    if(mark_gas == NULL && header.nsph != 0)
    	mark_gas = (short *)calloc(header.nsph, sizeof(*mark_gas));
    if(mark_gas == NULL && header.nsph != 0) {
	printf("<sorry, no memory for gas particle markers, %s>\n",title) ;
	return ;
    }
    if(mark_dark == NULL && header.ndark != 0)
    	mark_dark = (short *) calloc(header.ndark, sizeof(*mark_dark));
    if(mark_dark == NULL && header.ndark != 0) {
	printf("<sorry, no memory for dark particle markers, %s>\n",title) ;
	return ;
    }
    if(mark_star == NULL && header.nstar != 0)
    	mark_star = (short *)calloc(header.nstar, sizeof(*mark_star));
    else if(old_nstar < header.nstar) {
	mark_star = (short *)realloc(mark_star,
		header.nstar*sizeof(*mark_star));
	for (i = old_nstar; i < header.nstar; i++) mark_star[i] = 0;
    }
    if(mark_star == NULL && header.nstar != 0) {
	printf("<sorry, no memory for star particle markers, %s>\n",title) ;
	return ;
    }

    if(box0_smx) {
	kdFinish(box0_smx->kd);
	smFinish(box0_smx);
	box0_smx = NULL;
    }

    fread((char *)gas_particles,sizeof(struct gas_particle),
		     header.nsph,infile) ;
    fread((char *)dark_particles,sizeof(struct dark_particle),
		     header.ndark,infile) ;
    fread((char *)star_particles,sizeof(struct star_particle),
		     header.nstar,infile) ;
    currpos = lastpos ;
    fseek(infile,currpos,0) ;
    currtime = header.time ;
    if ((float)time != (float)currtime){
	printf("<used time %f, hope you don't mind %s>\n",
	       (float)currtime,title);
    }
}
