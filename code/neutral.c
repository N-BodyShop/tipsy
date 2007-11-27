/* $Header$
 * $Log$
 * Revision 1.9  2007/11/27 22:58:34  adrienne
 * Changed some syntax when tipsy prompts you for finding the neutral
 * fraction.
 *
 * Revision 1.8  2007/10/05 19:11:08  trq
 * Adrienne Stilp:
 *
 * Added "cube" command to produce velocity cubes for HI. (cube.c, comm.h,
 * arguments.c, Makefile.in)
 *
 * fits.c:  added comments and fits3d subroutine.
 *
 * neutral.c: added option to read in ionization fractions from gasoline
 * outputs.
 *
 * vista.c: automatically add "fits" to file names if not present.
 *
 * Revision 1.7  2005/12/17 00:25:00  trq
 * Load redshift information if needed.
 *
 * Revision 1.6  2000/01/12 22:55:19  nsk
 * Fixed bugs in cooling routines, added cooling damping,
 * fixed bugs in starformation,  regularized dependencies.
 *
 * Revision 1.5  1997/09/25  18:35:18  nsk
 * added uv command and discrete uv sources
 *
 * Revision 1.4  1997/09/05  01:02:23  nsk
 * streamlined vista (I hope it works), added neutralize command, added SZ
 * effect to vista.
 *
 * Revision 1.3  1995/12/11  20:00:48  nsk
 * added helium, dark absorb,  integral for elcetronic heating,
 * and read in 6 numbers for background
 *
 * Revision 1.2  1995/07/24  20:51:16  trq
 * Fixed bug in load_cool.c.
 * vista now has a velocity cut.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:37  trq
 * Import to CVS
 *
 * Revision 2.1  1994/09/22  16:20:14  trq
 * Changed to calculate neutral fraction of all particles in box 0.
 *
 */
#include <stdlib.h>
#include "defs.h"
#include "fdefs.h"

void
hneutral_func()
{
    struct gas_particle *gp ;
    int i,j ;
    double hneutral_p ;
    double heneutral_p ;
    double heII_p ;
    char* input;
    char file[MAXCOMM];
    char dummy[MAXCOMM];
    char in2[MAXCOMM];
    int option = 0;
    char type[MAXCOMM];
    int num_read;
    char ion[MAXCOMM];
    char ionFile[MAXCOMM] = "junk ";
    double *dummyptr;

    forever {
      /*
       * get either a filename (for .HI, .HeI, .HeII output from gasoline)
       * or calculate from uv field.
      */
      printf("<enter \"binary <filename> <type>\" without .HI extension ");
      printf("for binary arrays>\n") ;
      printf("<or \"ascii <filename>\" without .HI extension ");
      printf("for ascii arrays>\n");
      printf("<or enter \"calc\" to calculate neutral fraction ");
      printf("from uv field>\n") ;

      input = my_gets(" ");

      /* read in a binary array? */
      if (sscanf(input,"%s %s %s",dummy, file, type) == 3) {
	if ((strcmp(dummy, "binary") == 0 || strcmp(dummy, "b") == 0)&&
	    (strcmp(type, "float") == 0 || strcmp(type, "double") == 0)){
	  option = 1;
	  break;
	}	
      }
      /* read in an ascii array? */
      else if (sscanf(input, "%s %s", dummy, file) == 2) {
	if (strcmp(dummy, "ascii") == 0 || strcmp(dummy, "a") == 0) {
	  option = 2;
	  break;
	}
      }
      /* calculate from uv field */
      else if (sscanf(input, "%s", dummy) == 1) {
	if (strcmp(dummy, "calc") == 0 || strcmp(dummy, "c") == 0 || 
	    strcmp(dummy, "calculate") == 0) {
	  option = 3;
	  break;
	}
      }
      else {
	printf("<incorrect input");
      }
    }


    /* read in from a file, either binary or ascii */
    if (option == 1 || option == 2) {
            if (!redshift_loaded){
	load_redshift() ;
      }
      if (!cool_loaded ){
	load_cool() ;
      }

      /* free everything */
      if(hneutral != NULL) free(hneutral);
      if(heneutral != NULL) free(heneutral);
      if(heII != NULL) free(heII);
      if(header.nsph != 0) {
	hneutral = (double *)malloc(header.nsph*sizeof(*hneutral));
	heneutral = (double *)malloc(header.nsph*sizeof(*heneutral));
	heII = (double *)malloc(header.nsph*sizeof(*heII));
	if(hneutral == NULL || heneutral == NULL || heII == NULL)  {
	  printf("<sorry, no memory for hneutral, %s>\n",title) ;
	  free(hneutral) ;
	  free(heneutral) ;
	  free(heII) ;
	  return ;
	}
      }
      else {
	hneutral = NULL;
	heneutral = NULL;
	heII = NULL;
      }


      for (i = 0; i < 3; i++) {

	char ionFile[MAXCOMM] = "junk ";
	strcat(ionFile, file);

	/* figure out which file it is.. */
	if (i == 0) {
	  strcpy(ion,".HI ");
	  dummyptr = hneutral;
	} else if (i == 1) {
	  strcpy(ion, ".HeI ");
	  dummyptr = heneutral;
	} else if (i == 2) {
	  strcpy(ion, ".HeII ");
	  dummyptr = heII;
	}

	strcat(ionFile, ion);

	/* figure out whether to run readbinarray or readarray */
	if (option == 1) {
	  strcat(ionFile, type);
	  readbinarray(ionFile);
	}
	else if (option == 2) {
	  readarray(ionFile);
	}
	
	/* assign array values to hneutral */
	if (array != NULL) {
	  for (j = 0; j < header.nsph; j++) {
	    dummyptr[j] = array[j];
	  }
	} else {
	  printf("<sorry, array could not be read, %s>", title);
	  return;
	}
      }

      hneutral_loaded = YES ;

    }


    
    
    /* calc from UV field */
    else if (option == 3) {
    
      if (!redshift_loaded){
	load_redshift() ;
      }
      if (!cool_loaded ){
	load_cool() ;
      }
      if (!uv_loaded ){
	load_uv() ;
      }
      max_temp_old = -HUGE ;
      min_rho_old = HUGE ;
      if(hneutral != NULL) free(hneutral);
      if(heneutral != NULL) free(heneutral);
      if(heII != NULL) free(heII);
      if(header.nsph != 0) {
	hneutral = (double *)malloc(header.nsph*sizeof(*hneutral));
	heneutral = (double *)malloc(header.nsph*sizeof(*heneutral));
	heII = (double *)malloc(header.nsph*sizeof(*heII));
	if(hneutral == NULL || heneutral == NULL || heII == NULL)  {
	  printf("<sorry, no memory for hneutral, %s>\n",title) ;
	  free(hneutral) ;
	  free(heneutral) ;
	  free(heII) ;
	  return ;
	}
      }
      else {
	hneutral = NULL;
	heneutral = NULL;
	heII = NULL;
      }
      
      printf("<calculating neutral fraction, please be patient, %s>\n",
	     title);
      
      for (i = 0 ;i < boxlist[0].ngas ;i++) {
	gp = boxlist[0].gp[i] ;
	if(!uniform){
	  calc_uv(gp) ;
	}
	calc_hneutral(gp->temp, gp->rho, &hneutral_p, &heneutral_p, &heII_p);
	hneutral[gp-gas_particles] = hneutral_p ;
	heneutral[gp-gas_particles] = heneutral_p ;
	heII[gp-gas_particles] = heII_p ;
      }
      printf("<finished calculating neutral fraction, %s>\n",
	     title);
      hneutral_loaded = YES ;
    }
}
