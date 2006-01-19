#define _LARGEFILE_SOURCE
#define _FILE_OFFSET_BITS 64
#include "defs.h"
#include "fdefs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

static double currtime = 0.0;
static off_t currpos = 0L ;
static off_t lastpos = 0L ;

int
loadbinary(infile,time)
    FILE *infile;
    double time;
{
    int old_nstar;
    int i;
    int nread;
    off_t tempn;
    off_t temppos;
    size_t tsize;

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
	if(header.ndim < 2 || header.ndim > 3) {
		printf("<sorry, file has crazy dimension, %s>\n",title) ;
		fseek(infile,0L,0);
		currtime=0.0;
		currpos=0;
		header.nstar = 0;
		return FALSE;
	}
	currtime = header.time ;
	currpos = ftell(infile) - sizeof(header);
	if ( (float)header.time >= (float)time ) 
	    break ;
	tempn = header.nsph;
        tempn *= sizeof(gas_particles[0]);
	temppos = tempn;
	tempn = header.ndark;
        tempn *= sizeof(dark_particles[0]);
	temppos += tempn;
	tempn = header.nstar;
        tempn *= sizeof(star_particles[0]);
	temppos += tempn;
	fseek(infile,temppos,1);
    }	
    fseek(infile,currpos,0) ;
    lastpos = currpos ;
    fread((char *)&header,sizeof(header),1,infile) ;

    if(header.ndim < 2 || header.ndim > 3) {
	    printf("<sorry, file has crazy dimension, %s>\n",title) ;
	    fseek(infile,0L,0);
	    currtime=0.0;
	    currpos=0;
	    header.nstar = 0;
	    return FALSE;
    }
    if(gas_particles != NULL) free(gas_particles);
    if(header.nsph != 0) {
      tsize = header.nsph;
      tsize *= sizeof(*gas_particles);
	gas_particles = (struct gas_particle *)malloc(tsize);
	if(gas_particles == NULL) {
	    printf("<sorry, no memory for gas particles, %s>\n",title) ;
	    return FALSE;
	}
    }
    else
      gas_particles = NULL;
	    
    if(dark_particles != NULL) free(dark_particles);
    if(header.ndark != 0) {
      tsize = header.ndark;
      tsize *= sizeof(*dark_particles);
	dark_particles = (struct dark_particle *)malloc(tsize);
	if(dark_particles == NULL) {
	    printf("<sorry, no memory for dark particles, %s>\n",title) ;
	    return FALSE;
	}
    }
    else
      dark_particles = NULL;

    if(star_particles != NULL) free(star_particles);
    if(header.nstar != 0) {
      tsize = header.nstar;
      tsize *= sizeof(*star_particles);
	star_particles = (struct star_particle *)malloc(tsize);
	if(star_particles == NULL) {
	    printf("<sorry, no memory for star particles, %s>\n",title) ;
	    return FALSE;
	}
    }
    else
      star_particles = NULL;

    if(mark_gas == NULL && header.nsph != 0)
    	mark_gas = (short *)calloc(header.nsph, sizeof(*mark_gas));
    if(mark_gas == NULL && header.nsph != 0) {
	printf("<sorry, no memory for gas particle markers, %s>\n",title) ;
	return FALSE;
    }
    if(mark_dark == NULL && header.ndark != 0)
    	mark_dark = (short *) calloc(header.ndark, sizeof(*mark_dark));
    if(mark_dark == NULL && header.ndark != 0) {
	printf("<sorry, no memory for dark particle markers, %s>\n",title) ;
	return FALSE;
    }
    if(mark_star == NULL && header.nstar != 0)
    	mark_star = (short *)calloc(header.nstar, sizeof(*mark_star));
    else if(old_nstar < header.nstar) {
      tsize = header.nstar;
      tsize *= sizeof(*mark_star);
      mark_star = (short *)realloc(mark_star,tsize);
	for (i = old_nstar; i < header.nstar; i++) mark_star[i] = 0;
    }
    if(mark_star == NULL && header.nstar != 0) {
	printf("<sorry, no memory for star particle markers, %s>\n",title) ;
	return FALSE;
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

    nread = fread((char *)gas_particles,sizeof(struct gas_particle),
		     header.nsph,infile) ;
    if(nread != header.nsph) {
	printf("<sorry, short read of gas particles: %d vs %d, %s>\n",
	       nread, header.nsph, title);
    }
	
    nread = fread((char *)dark_particles,sizeof(struct dark_particle),
		     header.ndark,infile) ;
    if(nread != header.ndark) {
	printf("<sorry, short read of dark particles: %d vs %d, %s>\n",
	       nread, header.ndark, title);
    }
    nread = fread((char *)star_particles,sizeof(struct star_particle),
		     header.nstar,infile) ;
    if(nread != header.nstar) {
	printf("<sorry, short read of star particles: %d vs %d, %s>\n",
	       nread, header.nstar, title);
    }
    currpos = lastpos ;
    fseek(infile,currpos,0) ;
    currtime = header.time ;
    if ((float)time != (float)currtime){
	printf("<used time %f, hope you don't mind %s>\n",
	       (float)currtime,title);
    }
    return TRUE;
}

int
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
    off_t tempn,temppos;
    size_t tsize;

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
	if(header.ndim < 2 || header.ndim > 3) {
		printf("<sorry, file has crazy dimension, %s>\n",title) ;
		fseek(infile,0L,0);
		currtime=0.0;
		currpos=0;
		header.nstar = 0;
		return FALSE;
	}
	currtime = header.time ;
	currpos = ftell(infile) - sizeof(header);
	if ( (float)header.time >= (float)time ) 
	    break ;
	tempn = header.nsph;
        tempn *= sizeof(gas_particles[0]);
	temppos = tempn;
	tempn = header.ndark;
        tempn *= sizeof(dark_particles[0]);
	temppos += tempn;
	tempn = header.nstar;
        tempn *= sizeof(star_particles[0]);
	temppos += tempn;
	fseek(infile,temppos,1);
    }	
    fseek(infile,currpos,0) ;
    lastpos = currpos ;
    fread((char *)&header,sizeof(header),1,infile) ;
    if(header.ndim < 2 || header.ndim > 3) {
	    printf("<sorry, file has crazy dimension, %s>\n",title) ;
	    header.nstar = 0;
	    return FALSE;
    }

    if(gas_particles != NULL) free(gas_particles);
    if(header.nsph != 0) {
      tsize = max_gas;
      tsize *= sizeof(*gas_particles);
	gas_particles = (struct gas_particle *)malloc(tsize);
	if(gas_particles == NULL) {
	    printf("<sorry, no memory for gas particles, %s>\n",title) ;
	    return FALSE;
	}
    }
    else
      gas_particles = NULL;
	    
    if(dark_particles != NULL) free(dark_particles);
    if(header.ndark != 0) {
      tsize = max_dark;
      tsize *= sizeof(*dark_particles);
	dark_particles = (struct dark_particle *)malloc(tsize);
	if(dark_particles == NULL) {
	    printf("<sorry, no memory for dark particles, %s>\n",title) ;
	    return FALSE;
	}
    }
    else
      dark_particles = NULL;

    if(star_particles != NULL) free(star_particles);
    if(header.nstar != 0) {
      tsize = max_star;
      tsize *= sizeof(*star_particles);
	star_particles = (struct star_particle *)malloc(tsize);
	if(star_particles == NULL) {
	    printf("<sorry, no memory for star particles, %s>\n",title) ;
	    return FALSE;
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
    tsize = max_part;
    tsize *= sizeof(*box0_pi);
    box0_pi = (int *)malloc(tsize);
    if(box0_pi == NULL) {
	    printf("<sorry, no memory for particle indices, %s>\n",title) ;
	    return FALSE;
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
		tsize = max_gas;
		tsize *= sizeof(*gas_particles);		
		gas_particles = realloc(gas_particles,tsize);
		if(gas_particles == NULL) {
		    printf("<sorry, no memory for gas particles, %s>\n",title);
		    return FALSE;
		}
	    }
	    if(ngas >= max_part) {
		max_part *= 1.4;
		tsize = max_part;
		tsize *= sizeof(*box0_pi);
		box0_pi = realloc(box0_pi,tsize);
		if(box0_pi == NULL) {
		    printf("<sorry, no memory for particle indices, %s>\n",title);
		    return FALSE;
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
		tsize = max_dark;
		tsize *= sizeof(*dark_particles);
		dark_particles = realloc(dark_particles,tsize);
		if(dark_particles == NULL) {
		    printf("<sorry, no memory for dark particles, %s>\n",title);
		    return FALSE;
		}
	    }
	    if(ngas+ndark >= max_part) {
		max_part *= 1.4;
		tsize = max_part;
		tsize *= sizeof(*box0_pi);
		box0_pi = realloc(box0_pi,tsize);
		if(box0_pi == NULL) {
		    printf("<sorry, no memory for particle indices, %s>\n",title);
		    return FALSE;
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
		tsize = max_star;
		tsize *= sizeof(*star_particles);
		star_particles = realloc(star_particles,tsize);
		if(star_particles == NULL) {
		    printf("<sorry, no memory for star particles, %s>\n",title);
		    return FALSE;
		}
	    }
	    if(ngas+ndark+nstar >= max_part) {
		max_part *= 1.4;
		tsize = max_part;
		tsize *= sizeof(*box0_pi);		
		box0_pi = realloc(box0_pi,tsize);
		if(box0_pi == NULL) {
		    printf("<sorry, no memory for particle indices, %s>\n",title);
		    return FALSE;
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
	return FALSE;
    }
    if(mark_dark == NULL && header.ndark != 0)
    	mark_dark = (short *) calloc(header.ndark, sizeof(*mark_dark));
    if(mark_dark == NULL && header.ndark != 0) {
	printf("<sorry, no memory for dark particle markers, %s>\n",title) ;
	return FALSE;
    }
    if(mark_star == NULL && header.nstar != 0)
    	mark_star = (short *)calloc(header.nstar, sizeof(*mark_star));
    else if(old_nstar < header.nstar) {
      tsize = header.nstar;
      tsize *= sizeof(*mark_star);
      mark_star = (short *)realloc(mark_star,tsize);
	for (i = old_nstar; i < header.nstar; i++) mark_star[i] = 0;
    }
    if(mark_star == NULL && header.nstar != 0) {
	printf("<sorry, no memory for star particle markers, %s>\n",title) ;
	return FALSE;
    }
    currpos = lastpos ;
    fseek(infile,currpos,0) ;
    currtime = header.time ;
    if ((float)time != (float)currtime){
	printf("<used time %f, hope you don't mind %s>\n",
	       (float)currtime,title);
    }
    return TRUE;
}

#include <rpc/types.h>
#include <rpc/xdr.h>

static XDR xdrs;

int xdr_header()
{
  int pad;
  
  if(xdr_double(&xdrs, &header.time) != TRUE)
    return 0;
  if(xdr_int(&xdrs, &header.nbodies) != TRUE)
    return 0;
  if(xdr_int(&xdrs, &header.ndim) != TRUE)
    return 0;
  if(xdr_int(&xdrs, &header.nsph) != TRUE)
    return 0;
  if(xdr_int(&xdrs, &header.ndark) != TRUE)
    return 0;
  if(xdr_int(&xdrs, &header.nstar) != TRUE)
    return 0;
  if(xdr_int(&xdrs, &pad) != TRUE)
    return 0;
  return 1;
}

int xdr_gas(gas)
struct gas_particle *gas;
{
  if(sizeof(Real) == sizeof(float))
    {
      if(xdr_float(&xdrs, &gas->mass) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &gas->pos[0]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &gas->pos[1]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &gas->pos[2]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &gas->vel[0]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &gas->vel[1]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &gas->vel[2]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &gas->rho) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &gas->temp) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &gas->hsmooth) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &gas->metals) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &gas->phi) != TRUE)
	return 0;
      return 1;
    }
    return 0;
}  

int xdr_dark(dark)
struct dark_particle *dark;
{
  if(sizeof(Real) == sizeof(float))
    {
      if(xdr_float(&xdrs, &dark->mass) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &dark->pos[0]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &dark->pos[1]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &dark->pos[2]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &dark->vel[0]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &dark->vel[1]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &dark->vel[2]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &dark->eps) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &dark->phi) != TRUE)
	return 0;
      return 1;
    }
    return 0;
}  

int xdr_star(star)
struct star_particle *star;
{
  if(sizeof(Real) == sizeof(float))
    {
      if(xdr_float(&xdrs, &star->mass) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &star->pos[0]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &star->pos[1]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &star->pos[2]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &star->vel[0]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &star->vel[1]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &star->vel[2]) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &star->metals) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &star->tform) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &star->eps) != TRUE)
	return 0;
      if(xdr_float(&xdrs, &star->phi) != TRUE)
	return 0;
      return 1;
    }
    return 0;
}  
#define STD_HEADER_SIZE 32
#define STD_GAS_SIZE 48
#define STD_DARK_SIZE 36
#define STD_STAR_SIZE 44

int
loadstandard(infile,time)
    FILE *infile;
    double time;
{
    int old_nstar;
    int i;
    off_t tempn,temppos;
    size_t tsize;

    if ((float)currtime > (float)time){
	fseek(infile,0L,0);
	currtime=0.0;
	currpos=0;
    }
    old_nstar = header.nstar;
    xdrstdio_create(&xdrs, infile, XDR_DECODE);
    forever {
	if(xdr_header() != 1) {
	    printf("<sorry time too large %s, using %f>\n",title,
			     (float)currtime) ;
	    break ;
	}
	if(header.ndim < 2 || header.ndim > 3) {
		printf("<sorry, file has crazy dimension, %s>\n",title) ;
		fseek(infile,0L,0);
		currtime=0.0;
		currpos=0;
		header.nstar = 0;
		return FALSE;
	}
	currtime = header.time ;
	currpos = ftell(infile) - STD_HEADER_SIZE;
	if ( (float)header.time >= (float)time ) 
	    break ;
	tempn = header.nsph;
        tempn *= STD_GAS_SIZE;
	temppos = tempn;
	tempn = header.ndark;
        tempn *= STD_DARK_SIZE;
	temppos += tempn;
	tempn = header.nstar;
        tempn *= STD_STAR_SIZE;
	temppos += tempn;
	fseek(infile,temppos,1);
    }	
    fseek(infile,currpos,0) ;
    lastpos = currpos ;
    xdr_header();
    
    if(header.ndim < 2 || header.ndim > 3) {
	    printf("<sorry, file has crazy dimension, %s>\n",title) ;
	    header.nstar = 0;
	    fseek(infile,0L,0);
	    return FALSE;
    }
    if(gas_particles != NULL) free(gas_particles);
    if(header.nsph != 0) {		
      tsize = header.nsph;
      tsize *= sizeof(*gas_particles);
	gas_particles = (struct gas_particle *)malloc(tsize);
	if(gas_particles == NULL) {
	    printf("<sorry, no memory for gas particles, %s>\n",title) ;
	    return FALSE;
	}
    }
    else
      gas_particles = NULL;
	    
    if(dark_particles != NULL) free(dark_particles);
    if(header.ndark != 0) {
      tsize = header.ndark;
      tsize *= sizeof(*dark_particles);
	dark_particles = (struct dark_particle *)malloc(tsize);
	if(dark_particles == NULL) {
	    printf("<sorry, no memory for dark particles, %s>\n",title) ;
	    return FALSE;
	}
    }
    else
      dark_particles = NULL;

    if(star_particles != NULL) free(star_particles);
    if(header.nstar != 0) {
      tsize = header.nstar;
      tsize *= sizeof(*star_particles);
	star_particles = (struct star_particle *)malloc(tsize);
	if(star_particles == NULL) {
	    printf("<sorry, no memory for star particles, %s>\n",title) ;
	    return FALSE;
	}
    }
    else
      star_particles = NULL;

    if(mark_gas == NULL && header.nsph != 0)
    	mark_gas = (short *)calloc(header.nsph, sizeof(*mark_gas));
    if(mark_gas == NULL && header.nsph != 0) {
	printf("<sorry, no memory for gas particle markers, %s>\n",title) ;
	return FALSE;
    }
    if(mark_dark == NULL && header.ndark != 0)
    	mark_dark = (short *) calloc(header.ndark, sizeof(*mark_dark));
    if(mark_dark == NULL && header.ndark != 0) {
	printf("<sorry, no memory for dark particle markers, %s>\n",title) ;
	return FALSE;
    }
    if(mark_star == NULL && header.nstar != 0)
    	mark_star = (short *)calloc(header.nstar, sizeof(*mark_star));
    else if(old_nstar < header.nstar) {
      tsize = header.nstar;
      tsize *= sizeof(*mark_star);
      mark_star = (short *)realloc(mark_star,tsize);
      for (i = old_nstar; i < header.nstar; i++) mark_star[i] = 0;
    }
    if(mark_star == NULL && header.nstar != 0) {
	printf("<sorry, no memory for star particle markers, %s>\n",title) ;
	return FALSE;
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

    for(i = 0; i < header.nsph; ++i) {
	if(!xdr_gas(&gas_particles[i]))
	    break;
    }
    if(i != header.nsph)
	printf("<sorry, short read of gas: %d vs %d, %s>\n", i,
	       header.nsph, title);

    for(i = 0; i < header.ndark; ++i) {
	if(!xdr_dark(&dark_particles[i]))
	    break;
    }
    if(i != header.ndark)
	printf("<sorry, short read of dark: %d vs %d, %s>\n", i,
	       header.ndark, title);

    for(i = 0; i < header.nstar; ++i) {
	if(!xdr_star(&star_particles[i]))
	    break;
    }
    if(i != header.nstar)
	printf("<sorry, short read of star: %d vs %d, %s>\n", i,
	       header.nstar, title);
    
    currpos = lastpos ;
    fseek(infile,currpos,0) ;
    currtime = header.time ;
    if ((float)time != (float)currtime){
	printf("<used time %f, hope you don't mind %s>\n",
	       (float)currtime,title);
    }
    xdr_destroy(&xdrs);
    return TRUE;
}

int
loadstd_box(infile,time, xmin, xmax)
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
    off_t tempn,temppos;
    size_t tsize;

    if ((float)currtime > (float)time){
	fseek(infile,0L,0);
	currtime=0.0;
	currpos=0;
    }
    old_nstar = header.nstar;
    xdrstdio_create(&xdrs, infile, XDR_DECODE);
    forever {
	if(xdr_header() != 1) {
	    printf("<sorry time too large %s, using %f>\n",title,
			     (float)currtime) ;
	    break ;
	}
	currtime = header.time ;
	currpos = ftell(infile) - STD_HEADER_SIZE;
	if ( (float)header.time >= (float)time ) 
	    break ;
	tempn = header.nsph;
        tempn *= STD_GAS_SIZE;
	temppos = tempn;
	tempn = header.ndark;
        tempn *= STD_DARK_SIZE;
	temppos += tempn;
	tempn = header.nstar;
        tempn *= STD_STAR_SIZE;
	temppos += tempn;
	fseek(infile,temppos,1);
    }	
    fseek(infile,currpos,0) ;
    lastpos = currpos ;
    xdr_header();
    if(header.ndim < 2 || header.ndim > 3) {
	    printf("<sorry, file has crazy dimension, %s>\n",title) ;
	    header.nstar = 0;
	    return FALSE;
    }

    if(gas_particles != NULL) free(gas_particles);
    if(header.nsph != 0) {
      tsize = max_gas;
      tsize *= sizeof(*gas_particles);
	gas_particles = (struct gas_particle *)malloc(tsize);
	if(gas_particles == NULL) {
	    printf("<sorry, no memory for gas particles, %s>\n",title) ;
	    return FALSE;
	}
    }
    else
      gas_particles = NULL;
	    
    if(dark_particles != NULL) free(dark_particles);
    if(header.ndark != 0) {
      tsize = max_dark;
      tsize *= sizeof(*dark_particles);
	dark_particles = (struct dark_particle *)malloc(tsize);
	if(dark_particles == NULL) {
	    printf("<sorry, no memory for dark particles, %s>\n",title) ;
	    return FALSE;
	}
    }
    else
      dark_particles = NULL;

    if(star_particles != NULL) free(star_particles);
    if(header.nstar != 0) {
      tsize = max_star;
      tsize *= sizeof(*star_particles);
	star_particles = (struct star_particle *)malloc(tsize);
	if(star_particles == NULL) {
	    printf("<sorry, no memory for star particles, %s>\n",title) ;
	    return FALSE;
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
    tsize = max_part;
    tsize *= sizeof(*box0_pi);
    box0_pi = (int *) malloc(tsize);
    if(box0_pi == NULL) {
	    printf("<sorry, no memory for particle indices, %s>\n",title) ;
	    return FALSE;
    }
      
    for(i = 0, ngas = 0; i < header.nsph; i++) {
	gp = &gas_particles[ngas];
	xdr_gas(gp);
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
		tsize = max_gas;
		tsize *= sizeof(*gas_particles);
		gas_particles = realloc(gas_particles,tsize);
		if(gas_particles == NULL) {
		    printf("<sorry, no memory for gas particles, %s>\n",title);
		    return FALSE;
		}
	    }
	    if(ngas >= max_part) {
		max_part *= 1.4;
		tsize = max_part;
		tsize *= sizeof(*box0_pi);
		box0_pi = realloc(box0_pi,tsize);
		if(box0_pi == NULL) {
		    printf("<sorry, no memory for particle indices, %s>\n",title);
		    return FALSE;
		}
	    }
	}
    }

    for(i = 0, ndark = 0; i < header.ndark; i++) {
        dp = &dark_particles[ndark];
	xdr_dark(dp);
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
		tsize = max_dark;
		tsize *= sizeof(*dark_particles);
		dark_particles = realloc(dark_particles,tsize);
		if(dark_particles == NULL) {
		    printf("<sorry, no memory for dark particles, %s>\n",title);
		    return FALSE;
		}
	    }
	    if(ngas+ndark >= max_part) {
		max_part *= 1.4;
		tsize = max_part;
		tsize *= sizeof(*box0_pi);
		box0_pi = realloc(box0_pi,tsize);
		if(box0_pi == NULL) {
		    printf("<sorry, no memory for particle indices, %s>\n",title);
		    return FALSE;
		}
	    }
	}
    }

    for(i = 0, nstar = 0; i < header.nstar; i++) {
        sp = &star_particles[nstar];
	xdr_star(sp);
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
		tsize = max_star;
		tsize *= sizeof(*star_particles);
		star_particles = realloc(star_particles,tsize);
		if(star_particles == NULL) {
		    printf("<sorry, no memory for star particles, %s>\n",title);
		    return FALSE;
		}
	    }
	    if(ngas+ndark+nstar >= max_part) {
		max_part *= 1.4;
		tsize = max_part;
		tsize *= sizeof(*box0_pi);
		box0_pi = realloc(box0_pi,tsize);
		if(box0_pi == NULL) {
		    printf("<sorry, no memory for particle indices, %s>\n",title);
		    return FALSE;
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
	return FALSE;
    }
    if(mark_dark == NULL && header.ndark != 0)
    	mark_dark = (short *) calloc(header.ndark, sizeof(*mark_dark));
    if(mark_dark == NULL && header.ndark != 0) {
	printf("<sorry, no memory for dark particle markers, %s>\n",title) ;
	return FALSE;
    }
    if(mark_star == NULL && header.nstar != 0)
    	mark_star = (short *)calloc(header.nstar, sizeof(*mark_star));
    else if(old_nstar < header.nstar) {
      tsize = header.nstar;
      tsize *= sizeof(*mark_star);
      mark_star = (short *)realloc(mark_star,tsize);
	for (i = old_nstar; i < header.nstar; i++) mark_star[i] = 0;
    }
    if(mark_star == NULL && header.nstar != 0) {
	printf("<sorry, no memory for star particle markers, %s>\n",title) ;
	return FALSE;
    }
    currpos = lastpos ;
    fseek(infile,currpos,0) ;
    currtime = header.time ;
    if ((float)time != (float)currtime){
	printf("<used time %f, hope you don't mind %s>\n",
	       (float)currtime,title);
    }
    xdr_destroy(&xdrs);
    return TRUE;
}

