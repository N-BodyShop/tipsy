#include "defs.h"
unmark_all()
{
    int i,j ;

    for (i = 1 ;i <= MAXBOX ;i++) {
	mark_box[i] = NO ;
    }
    for(j = 0 ;j < header.nsph ; j++){
	mark_gas[j] = (short)0 ;
    }
    for(j = 0 ;j < header.ndark ; j++){
	mark_dark[j] = (short)0 ;
    }
    for(j = 0 ;j < header.nstar ; j++){
	mark_star[j] = (short)0 ;
    }
}
