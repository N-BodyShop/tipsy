#include "defs.h"
#include <stdlib.h>

static double currtime = 0.0;
static long currpos = 0L ;
static long lastpos = 0L ;

void
loadbinary(infile,time)
    FILE *infile;
    double time;
{
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

    if(header.ndim < 2 || header.ndim > 3) {
	    printf("<sorry, file has crazy dimension, %s>\n",title) ;
	    return;
    }
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
    if(box0_pi) {
      free(box0_pi);
      box0_pi = NULL;
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

void
loadbin_box(infile,time, xmin, xmax)
    FILE *infile;
    double time;
    Real *xmin;
    Real *xmax;
{
    int old_nstar;
    int i;
    int k;
    int in;
    int max_part = 1000;
    int max_gas = 1000;
    int max_dark = 1000;
    int max_star = 1000;
    struct gas_particle *gp;
    struct dark_particle *dp;
    struct star_particle *sp;
    int ngas, ndark, nstar;

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
    if(header.ndim < 2 || header.ndim > 3) {
	    printf("<sorry, file has crazy dimension, %s>\n",title) ;
	    return;
    }

    if(gas_particles != NULL) free(gas_particles);
    if(header.nsph != 0) {
	gas_particles = (struct gas_particle *)
			    malloc(max_gas*sizeof(*gas_particles));
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
			    malloc(max_dark*sizeof(*dark_particles));
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
			    malloc(max_star*sizeof(*star_particles));
	if(star_particles == NULL) {
	    printf("<sorry, no memory for star particles, %s>\n",title) ;
	    return ;
	}
    }
    else
      star_particles = NULL;

    if(box0_smx) {
	kdFinish(box0_smx->kd);
	smFinish(box0_smx);
	box0_smx = NULL;
    }
    if(box0_pi) {
      free(box0_pi);
    }
    box0_pi = (int *) malloc(max_part*sizeof(*box0_pi));
    if(box0_pi == NULL) {
	    printf("<sorry, no memory for particle indices, %s>\n",title) ;
	    return ;
    }
      
    for(i = 0, ngas = 0; i < header.nsph; i++) {
	gp = &gas_particles[ngas];
	fread((char *)gp, sizeof(struct gas_particle),
	      1,infile) ;
	in = 1;
	for(k = 0; k < header.ndim; k++) {
	    if(gp->pos[k] < xmin[k] || gp->pos[k] >= xmax[k])
		in = 0;
	}
	if(in) {
	    box0_pi[ngas] = i;
	    ngas++;
	    if(ngas >= max_gas) {
		max_gas *= 1.4;
		gas_particles = realloc(gas_particles,
					max_gas*sizeof(*gas_particles));
		if(gas_particles == NULL) {
		    printf("<sorry, no memory for gas particles, %s>\n",title);
		    return ;
		}
	    }
	    if(ngas >= max_part) {
		max_part *= 1.4;
		box0_pi = realloc(box0_pi, max_part*sizeof(*box0_pi));
		if(box0_pi == NULL) {
		    printf("<sorry, no memory for particle indices, %s>\n",title);
		    return ;
		}
	    }
	}
    }

    for(i = 0, ndark = 0; i < header.ndark; i++) {
        dp = &dark_particles[ndark];
	fread((char *)dp, sizeof(struct dark_particle),
	      1,infile) ;
	in = 1;
	for(k = 0; k < header.ndim; k++) {
	    if(dp->pos[k] < xmin[k] || dp->pos[k] >= xmax[k])
		in = 0;
	}
	if(in) {
	    box0_pi[ndark+ngas] = i + header.nsph;
	    ++ndark;
	    if(ndark >= max_dark) {
		max_dark *= 1.4;
		dark_particles = realloc(dark_particles,
					 max_dark*sizeof(*dark_particles));
		if(dark_particles == NULL) {
		    printf("<sorry, no memory for dark particles, %s>\n",title);
		    return ;
		}
	    }
	    if(ngas+ndark >= max_part) {
		max_part *= 1.4;
		box0_pi = realloc(box0_pi, max_part*sizeof(*box0_pi));
		if(box0_pi == NULL) {
		    printf("<sorry, no memory for particle indices, %s>\n",title);
		    return ;
		}
	    }
	}
    }

    for(i = 0, nstar = 0; i < header.nstar; i++) {
        sp = &star_particles[nstar];
	fread((char *)sp, sizeof(struct star_particle),
	      1,infile) ;
	in = 1;
	for(k = 0; k < header.ndim; k++) {
	    if(sp->pos[k] < xmin[k] || sp->pos[k] >= xmax[k])
		in = 0;
	}
	if(in) {
	    box0_pi[nstar+ndark+ngas] = i + header.nsph + header.ndark;
	    ++nstar;
	    if(nstar >= max_star) {
		max_star *= 1.4;
		star_particles = realloc(star_particles,
					 max_star*sizeof(*star_particles));
		if(star_particles == NULL) {
		    printf("<sorry, no memory for star particles, %s>\n",title);
		    return ;
		}
	    }
	    if(ngas+ndark+nstar >= max_part) {
		max_part *= 1.4;
		box0_pi = realloc(box0_pi, max_part*sizeof(*box0_pi));
		if(box0_pi == NULL) {
		    printf("<sorry, no memory for particle indices, %s>\n",title);
		    return ;
		}
	    }
	}
    }
    header.nsph = ngas;
    header.ndark = ndark;
    header.nstar = nstar;

    header.nbodies = header.nsph + header.ndark + header.nstar;

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
    currpos = lastpos ;
    fseek(infile,currpos,0) ;
    currtime = header.time ;
    if ((float)time != (float)currtime){
	printf("<used time %f, hope you don't mind %s>\n",
	       (float)currtime,title);
    }
}
