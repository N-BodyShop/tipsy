#include "defs.h"
void
load_epsgas()
{
    char *grav_const ;

    forever{
	printf("<enter gas gravitational softening, %s>\n",title) ;
	grav_const = my_gets(" ") ;
	if(sscanf(grav_const,"%lf",&epsgas_grav) == 1){
	    break ;
	}
	printf("<sorry, inconsistent input, %s>\n",title) ;
    }
    epsgas_loaded = YES ;
    starform_loaded = NO ;
}
