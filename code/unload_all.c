#include "defs.h"
void
unload_all()
{
    int i ;

    for (i = 1 ;i <= MAXBOX ;i++) {
	boxes_loaded[i] = UNLOADED ;
	mark_box[i] = NO ;
    }
    hneutral_loaded = NO ;
}
