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
        if(hsmdivv != NULL) {
 	   free(hsmdivv);
           hsmdivv = NULL;
	   divv_loaded = NO ;
	}
        if(cooling != NULL) {
 	   free(cooling);
           cooling = NULL;
	   cooling_loaded = NO ;
	}
        if(hneutral != NULL) {
 	   free(hneutral);
           hneutral = NULL;
	   hneutral_loaded = NO ;
	}
        if(heneutral != NULL) {
 	   free(heneutral);
           heneutral = NULL;
	}
        if(heII != NULL) {
 	   free(heII);
           heII = NULL;
	}
        if(meanmwt != NULL) {
 	   free(meanmwt);
           meanmwt = NULL;
	   meanmwt_loaded = NO ;
	}
        if(starform != NULL) {
 	   free(starform);
           starform = NULL;
	   starform_loaded = NO ;
	}
        if(xray_lum != NULL) {
 	   free(xray_lum);
           xray_lum = NULL;
	   xray_loaded = NO ;
	}
	if(dudt != NULL) {
	   free(dudt) ;
	   dudt = NULL ;
	   dudt_loaded = NO ;
	}
    }
}
