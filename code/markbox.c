#include "defs.h"
markbox(job)
    char job[MAXCOMM] ;
{
    char command[MAXCOMM] ;
    int i,j ;

    if (boxes_loaded[0]) {
	if (sscanf(job,"%s %d",command,&i) == 2) {
	    if(i >= 0 && i <= MAXBOX) {
		if(boxes_loaded[i]) {
		    mark_box[i] = YES ;
		    for(j = 0 ;j < boxlist[i].ngas ; j++){
			mark_gas[boxlist[i].gpi[j]]++ ;
		    }
		    for(j = 0 ;j < boxlist[i].ndark ; j++){
			mark_dark[boxlist[i].dpi[j]]++ ;
		    }
		    for(j = 0 ;j < boxlist[i].nstar ; j++){
			mark_star[boxlist[i].spi[j]]++ ;
		    }
		}
		else {
		    printf("<box %d not loaded, %s>\n",i,title) ;
		}
	    }
	    else {
		printf("<box %d cannot be marked, %s>\n",i,title) ;
	    }
	}
	else {
	    input_error(command) ;
	}
    }
    else {
	printf("<sorry, no boxes are loaded, %s>\n",title) ;
    }
}
