/*
 * concatenate two boxes, avoiding duplicating particles.
 */
#include "defs.h"
#include "fdefs.h"

PROTO(void, load_catbox, (int box1, int box2, int box3));

void
catbox(job)
     char *job;
{
    char command[MAXCOMM] ;
    int box1, box2, box3;
    
    if(sscanf(job, "%s %d %d %d", command, &box1, &box2, &box3) == 4) {
      if(box1 < 0 || box1 > MAXBOX || !boxes_loaded[box1]) {
	printf("<sorry, box %d is not loaded, %s>\n", box1, title);
	return;
      }
      if(box2 < 0 || box2 > MAXBOX || !boxes_loaded[box2]) {
	printf("<sorry, box %d is not loaded, %s>\n", box2, title);
	return;
      }
      if(!boxes_loaded[0]) {
	printf("<sorry, box 0 is not loaded, %s>\n", title);
	return;
      }
      if(box3 > MAXBOX || box3 < 0) {
	printf("<sorry, box %d is not available, %s>\n", box3, title);
	return;
      }
      /*
       * make box frame
       */
      boxes[box3].x1[0] = min(boxes[box1].x1[0], boxes[box2].x1[0]);
      boxes[box3].x1[1] = max(boxes[box1].x1[1], boxes[box2].x1[1]);
      boxes[box3].x1[2] = max(boxes[box1].x1[2], boxes[box2].x1[2]) ;
      boxes[box3].x2[0] = min(boxes[box1].x2[0], boxes[box2].x2[0]) ;
      boxes[box3].x2[1] = min(boxes[box1].x2[1], boxes[box2].x2[1]);
      boxes[box3].x2[2] = max(boxes[box1].x2[2], boxes[box2].x2[2]) ;
      boxes[box3].x3[0] = max(boxes[box1].x3[0], boxes[box2].x3[0]);
      boxes[box3].x3[1] = max(boxes[box1].x3[1], boxes[box2].x3[1]);
      boxes[box3].x3[2] = max(boxes[box1].x3[2], boxes[box2].x3[2]) ;
      boxes[box3].x4[0] = max(boxes[box1].x4[0], boxes[box2].x4[0]);
      boxes[box3].x4[1] = min(boxes[box1].x4[1], boxes[box2].x4[1]);
      boxes[box3].x4[2] = max(boxes[box1].x4[2], boxes[box2].x4[2]) ;
      boxes[box3].x5[0] = max(boxes[box1].x5[0], boxes[box2].x5[0]);
      boxes[box3].x5[1] = max(boxes[box1].x5[1], boxes[box2].x5[1]);
      boxes[box3].x5[2] = min(boxes[box1].x5[2], boxes[box2].x5[2]) ;
      boxes[box3].x6[0] = max(boxes[box1].x6[0], boxes[box2].x6[0]);
      boxes[box3].x6[1] = min(boxes[box1].x6[1], boxes[box2].x6[1]);
      boxes[box3].x6[2] = min(boxes[box1].x6[2], boxes[box2].x6[2]) ;
      load_catbox(box1, box2, box3);
    }
    else {
	input_error(command) ;
    }
}

void load_catbox(box1, box2, box3)
     int box1;
     int box2;
     int box3;
{
  int i, i1, i2;
  double d;

  if(boxlist_alloc(box3, boxlist[box1].ngas + boxlist[box2].ngas,
		    boxlist[box1].ndark + boxlist[box2].ndark,
		    boxlist[box1].nstar + boxlist[box2].nstar) == 0)
    return;
  for(i = 0, i1 = 0, i2 = 0;
      i1 < boxlist[box1].ngas || i2 < boxlist[box2].ngas;) {
    struct gas_particle *p1, *p2;
    if(i1 < boxlist[box1].ngas) {
      p1 = boxlist[box1].gp[i1];
    } else {
      boxlist[box3].gp[i] = boxlist[box2].gp[i2];
      boxlist[box3].gpi[i] = boxlist[box2].gpi[i2];
      i++;
      i2++;
      continue;
    }
    if(i2 < boxlist[box2].ngas) {
      p2 = boxlist[box2].gp[i2];
    } else {
      boxlist[box3].gp[i] = boxlist[box1].gp[i1];
      boxlist[box3].gpi[i] = boxlist[box1].gpi[i1];
      i++;
      i1++;
      continue;
    }
    if(p1 == p2) {
      boxlist[box3].gp[i] = boxlist[box1].gp[i1];
      boxlist[box3].gpi[i] = boxlist[box1].gpi[i1];
      i1++;
      i2++;
      i++;
    }
    else if(p1 < p2) {
      boxlist[box3].gp[i] = boxlist[box1].gp[i1];
      boxlist[box3].gpi[i] = boxlist[box1].gpi[i1];
      i1++;
      i++;
    }
    else if(p1 > p2) {
      boxlist[box3].gp[i] = boxlist[box2].gp[i2];
      boxlist[box3].gpi[i] = boxlist[box2].gpi[i2];
      i2++;
      i++;
    }
  }
  boxlist[box3].ngas = i;
  for(i = 0, i1 = 0, i2 = 0;
      i1 < boxlist[box1].ndark || i2 < boxlist[box2].ndark;) {
    struct dark_particle *p1, *p2;
    if(i1 < boxlist[box1].ndark) {
      p1 = boxlist[box1].dp[i1];
    } else {
      boxlist[box3].dp[i] = boxlist[box2].dp[i2];
      boxlist[box3].dpi[i] = boxlist[box2].dpi[i2];
      i++;
      i2++;
      continue;
    }
    if(i2 < boxlist[box2].ndark) {
      p2 = boxlist[box2].dp[i2];
    } else {
      boxlist[box3].dp[i] = boxlist[box1].dp[i1];
      boxlist[box3].dpi[i] = boxlist[box1].dpi[i1];
      i++;
      i1++;
      continue;
    }
    if(p1 == p2) {
      boxlist[box3].dp[i] = boxlist[box1].dp[i1];
      boxlist[box3].dpi[i] = boxlist[box1].dpi[i1];
      i1++;
      i2++;
      i++;
    }
    else if(p1 < p2) {
      boxlist[box3].dp[i] = boxlist[box1].dp[i1];
      boxlist[box3].dpi[i] = boxlist[box1].dpi[i1];
      i1++;
      i++;
    }
    else if(p1 > p2) {
      boxlist[box3].dp[i] = boxlist[box2].dp[i2];
      boxlist[box3].dpi[i] = boxlist[box2].dpi[i2];
      i2++;
      i++;
    }
  }
  boxlist[box3].ndark = i;
  for(i = 0, i1 = 0, i2 = 0;
      i1 < boxlist[box1].nstar || i2 < boxlist[box2].nstar;) {
    struct star_particle *p1, *p2;
    if(i1 < boxlist[box1].nstar) {
      p1 = boxlist[box1].sp[i1];
    } else {
      boxlist[box3].sp[i] = boxlist[box2].sp[i2];
      boxlist[box3].spi[i] = boxlist[box2].spi[i2];
      i++;
      i2++;
      continue;
    }
    if(i2 < boxlist[box2].nstar) {
      p2 = boxlist[box2].sp[i2];
    } else {
      boxlist[box3].sp[i] = boxlist[box1].sp[i1];
      boxlist[box3].spi[i] = boxlist[box1].spi[i1];
      i++;
      i1++;
      continue;
    }
    if(p1 == p2) {
      boxlist[box3].sp[i] = boxlist[box1].sp[i1];
      boxlist[box3].spi[i] = boxlist[box1].spi[i1];
      i1++;
      i2++;
      i++;
    }
    else if(p1 < p2) {
      boxlist[box3].sp[i] = boxlist[box1].sp[i1];
      boxlist[box3].spi[i] = boxlist[box1].spi[i1];
      i1++;
      i++;
    }
    else if(p1 > p2) {
      boxlist[box3].sp[i] = boxlist[box2].sp[i2];
      boxlist[box3].spi[i] = boxlist[box2].spi[i2];
      i2++;
      i++;
    }
  }
  boxlist[box3].nstar = i;
  boxlist_realloc(box3);
  boxes_loaded[box3] = LOADED ;
  boxes[box3].size = 0.0 ;
  d = distance(boxes[box3].x1, boxes[box3].x2) ;
  boxes[box3].volume = d ;
  boxes[box3].size = max(boxes[box3].size, d) ;
  d = distance(boxes[box3].x1, boxes[box3].x3) ;
  boxes[box3].volume *= d ;
  boxes[box3].size = max(boxes[box3].size, d) ;
  d = distance(boxes[box3].x5, boxes[box3].x3) ;
  boxes[box3].volume *= d ;
  boxes[box3].size = max(boxes[box3].size, d) ;
  for (i = 0 ; i < header.ndim ; i++ ) {
    boxes[box3].center[i] = (boxes[box3].x1[i] +boxes[box3].x6[i]) / 2.0 ;
  }
  box_cumulate(box3);
}
     
