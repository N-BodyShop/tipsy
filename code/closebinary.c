#include "defs.h"

void
closebinary(job)
    char *job ;
{
    if (!binaryopen){
	printf("<sorry, binary file not open, %s>\n",title) ;
    }
    else {
	fclose(binaryfile.ptr);
	binaryopen = CLOSED ;
	binary_loaded = UNLOADED ;
        if(mark_gas != NULL) {
 	   free(mark_gas);
           mark_gas = NULL;
	}
        if(mark_dark != NULL) {
 	   free(mark_dark);
           mark_dark = NULL;
	}
        if(mark_star != NULL) {
 	   free(mark_star);
           mark_star = NULL;
	}
        if(acsmooth != NULL) {
 	   free(acsmooth);
           acsmooth = NULL;
	}
        if(dwsmooth != NULL) {
 	   free(dwsmooth);
           dwsmooth = NULL;
	}
        if(hsmdivv != NULL) {
 	   free(hsmdivv);
           hsmdivv = NULL;
	}
        if(cooling != NULL) {
 	   free(cooling);
           cooling = NULL;
	}
        if(hneutral != NULL) {
 	   free(hneutral);
           hneutral = NULL;
	}
        if(meanmwt != NULL) {
 	   free(meanmwt);
           meanmwt = NULL;
	}
        if(starform != NULL) {
 	   free(starform);
           starform = NULL;
	}
        if(xray_lum != NULL) {
 	   free(xray_lum);
           xray_lum = NULL;
	}
    }
}
