#include "defs.h"
packfit(c,ibuf,nbyte)

    short *c ;	
    register short *ibuf ;
    int nbyte ;

{
    int i;
    unsigned short high, low;

    for (i = nbyte/2; i > 0; i--) {
	high = ( *c & 0xff00) ;
	low = ( *c++ & 0x00ff) ;
	*ibuf++ = ( ( high >> 8 ) & 0x00ff) | ( ( low << 8 ) & 0xff00 ) ;
    }
    return ;
}
