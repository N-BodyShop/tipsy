#include "defs.h"
void
load_visc() 
{
    char *visc_const ;

    forever{
	printf("<enter: alpha beta>\n") ;
	visc_const = my_gets(" ") ;
	if(sscanf(visc_const,"%lf %lf", &alpha,&beta) == 2){
	    break ;
	}
	printf("<sorry, inconsistent input, %s>\n",title) ;
    }
    visc_loaded = YES ;
    dudt_loaded = NO ;
    starform_loaded = NO ;
}
