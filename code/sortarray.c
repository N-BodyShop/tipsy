/* sortarray: sorts a[left]...a[right] into increasing order based on 
   their values at array[]  */

#include "defs.h"
#include "fdefs.h"

PROTO(void, swapi, (int *a, int i, int j));
PROTO(int, arraygeti, (int i, int box, int particle_type));

void sortarray(a, left, right, box, p_type)
    int *a, left, right, box, p_type;
{
    int i, last;
 
    if (left >= right)
	return;
    swapi(a, left, (left + right)/2);
    last = left;
    for (i = left+1; i <= right; i++)
	if ( array[arraygeti(a[i],box,p_type)] > 
		array[arraygeti(a[left],box,p_type)] ) 
	    swapi(a, ++last, i);
    swapi(a, left, last);
    sortarray(a, left, last-1, box, p_type);
    sortarray(a, last+1, right, box, p_type);
}


void swapi(a, i, j)
    int *a, i, j;
{
    int temp;

    temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}




int arraygeti(i, box, particle_type)
    int i, box, particle_type;
{
    int ngas, ndark;


    ngas = boxlist[box].ngas;
    ndark = boxlist[box].ndark;
    if(particle_type == GAS ) {
        return boxlist[box].gpi[i];
    }
    else if(particle_type == DARK ) {
        return boxlist[box].dpi[i]+header.nsph; 
    }
    else if(particle_type == STAR ) {
        return boxlist[box].spi[i]+header.nsph+header.ndark;
    }
    else if(particle_type == ALL ) {
        if (i < ngas) {
            return boxlist[box].gpi[i];
        }
        else if (i < ngas + ndark) {
            return boxlist[box].dpi[i-ngas ]+header.nsph;
        }
        else {
            return boxlist[box].spi[i-ndark-ngas]+header.nsph+header.ndark;
        }
    }
    else {
	printf("<not a valid particle type for index>\n");
	return; 
    }

}

	
  
