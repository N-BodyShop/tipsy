#include "defs.h"
closeascii(job)
    char job[MAXCOMM] ;
{
    if (!asciiopen){
	printf("<sorry, ascii file not open, %s>\n",title) ;
    }
    else {
	fclose(bodfile.ptr);
	asciiopen = CLOSED ;
    }
}
