#include "defs.h"
#include "fdefs.h"

void
load_form() 
{
    char *star_const ;

    forever{
	printf("<please enter: cstar dtime temppar softpar>\n") ;
	star_const = my_gets(" ") ;
	if(sscanf(star_const,"%lf %lf %lf %lf",&cstar,&dtime,&temppar,&softpar) 
		== 4){
	    break ;
	}
	printf("<sorry, inconsistent input, %s>\n",title) ;
    }
    form_loaded = YES ;
}
