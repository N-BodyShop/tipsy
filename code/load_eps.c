#include "defs.h"
#include "fdefs.h"

void
load_eps()
{
    char *grav_const ;

    forever{
	printf("<enter dark gravitational softening, %s>\n",title) ;
	grav_const = my_gets(" ") ;
	if(sscanf(grav_const,"%lf",&eps_grav) == 1){
	    break ;
	}
	printf("<sorry, inconsistent input, %s>\n",title) ;
    }
    eps_loaded = YES ;
}
