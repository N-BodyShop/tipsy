#include "defs.h"
unmarkbox(i)
    int i ;
{
    int j ;

    mark_box[i] = NO ;
    for(j = 0 ;j < boxlist[i].ngas ; j++){
	if(mark_gas[boxlist[i].gpi[j]]){
	    mark_gas[boxlist[i].gpi[j]]-- ;
	}
    }
    for(j = 0 ;j < boxlist[i].ndark ; j++){
	if(mark_dark[boxlist[i].dpi[j]]){
	    mark_dark[boxlist[i].dpi[j]]-- ;
	}
    }
    for(j = 0 ;j < boxlist[i].nstar ; j++){
	if(mark_star[boxlist[i].spi[j]]){
	    mark_star[boxlist[i].spi[j]]-- ;
	}
    }
}
