#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "defs.h"
#include "fdefs.h"
#include "smooth.h"
#include "kd.h"

PROTO(void, smSetBall, (SMX smx, double ball_size));

void smooth_sub(job)
    char *job ;
{
    char command[MAXCOMM] ;

    if (sscanf(job,"%s %f %d",command,&ball_size, &n_smooth) == 3 ) {
	if(ball_size >= 0.0)
	    ball_size_loaded = YES ;
	balls_loaded = NO;
    }
    else {
	input_error(command) ;
    }
}

void calc_density(psmx, bDark, bGas, bStar)
     SMX *psmx;
     int bDark;
     int bGas;
     int bStar;
{
    KD kd;
    SMX smx = *psmx;
    float period[MAXDIM];

    if(smx) {
	kdFinish(smx->kd);
	smFinish(smx);
	smx = NULL;
    }
    if(boxlist[0].ndark != header.ndark || boxlist[0].ngas != header.nsph
       || boxlist[0].nstar != header.nstar) {
	printf("<Warning, box 0 does not contain all particles, %s>\n", title);
	printf("<Reloading box 0, %s>\n", title);
	loadall();
    }
	
    printf("<Building tree, %s>\n", title);
	kdInit(&kd);
	kdReadBox(kd, &boxlist[0], bDark, bGas, bStar);
	kdBuildTree(kd);
    if(periodic)
	period[0] = period[1] = period[2] = period_size;
    else
	period[0] = period[1] = period[2] = 1e37;
    printf("<Calculating density, %s>\n", title);
    smInit(&smx,kd,n_smooth,period);
    if(!ball_size_loaded)
	smSmooth(smx,smDensitySym);
    else {
	smSetBall(smx, ball_size);
	smReSmooth(smx,smDensitySym);
    }
    kdOrder(kd);
    *psmx = smx;
}

void calc_balls(psmx, bDark, bGas, bStar)
     SMX *psmx;
     int bDark;
     int bGas;
     int bStar;
{
    KD kd;
    SMX smx = *psmx;
    float period[MAXDIM];

    if(smx) {
	kdFinish(smx->kd);
	smFinish(smx);
	smx = NULL;
    }
    if(boxlist[0].ndark != header.ndark || boxlist[0].ngas != header.nsph
       || boxlist[0].nstar != header.nstar) {
	printf("<Warning, box 0 does not contain all particles, %s>\n", title);
	printf("<Reloading box 0, %s>\n", title);
	loadall();
    }
	
    printf("<Building tree, %s>\n", title);
	kdInit(&kd);
	kdReadBox(kd, &boxlist[0], bDark, bGas, bStar);
	kdBuildTree(kd);
    if(periodic)
	period[0] = period[1] = period[2] = period_size;
    else
	period[0] = period[1] = period[2] = 1e37;
    printf("<Finding balls, %s>\n", title);
    smInit(&smx,kd,n_smooth,period);
    if(!ball_size_loaded)
	smSmooth(smx,smNull);
    else
	smSetBall(smx, ball_size);
    kdOrder(kd);
    printf("<Done, %s>\n", title);
    *psmx = smx;
}

int smInit(psmx, kd, nSmooth, fPeriod)
     SMX *psmx;
     KD kd;
     int nSmooth;
     float *fPeriod;
{
	SMX smx;
	int pi,j;

	assert(nSmooth <= kd->nActive);
	smx = (SMX)malloc(sizeof(struct smContext));
	assert(smx != NULL);
	smx->kd = kd;
	smx->nSmooth = nSmooth;
	smx->pq = (PQ *)malloc(nSmooth*sizeof(PQ));
	assert(smx->pq != NULL);
	PQ_INIT(smx->pq,nSmooth);
	smx->iMark = (char *)malloc(kd->nActive*sizeof(char));
	assert(smx->iMark);
	smx->nListSize = smx->nSmooth+RESMOOTH_SAFE;
	smx->fList = (float *)malloc(smx->nListSize*sizeof(float));
	assert(smx->fList != NULL);
	smx->pList = (int *)malloc(smx->nListSize*sizeof(int));
	assert(smx->pList != NULL);
	/*
	 ** Set for Periodic Boundary Conditions.
	 */
	for (j=0;j<3;++j) smx->fPeriod[j] = fPeriod[j];
	/*
	 ** Initialize arrays for calculated quantities.
	 */
	for (pi=0;pi<smx->kd->nActive;++pi) {
		smx->kd->p[pi].fDensity = 0.0;
/*
                for (j=0;j<3;++j) smx->kd->p[pi].vMean[j] = 0.0;
		smx->kd->p[pi].fVelDisp2 = 0.0;
*/
		}
	*psmx = smx;	
	return(1);
	}


void smFinish(smx)
     SMX smx;
{
	free(smx->iMark);
	free(smx->pq);
	free(smx->fList);
	free(smx->pList);
	free(smx);
	}


void smBallSearch(smx, fBall2, ri)
     SMX smx;
     double fBall2;
     float *ri;
{
	KDN *c;
	PARTICLE *p;
	int cell,cp,ct,pj;
	float fDist2,dx,dy,dz,lx,ly,lz,sx,sy,sz,x,y,z;
	PQ *pq;

	c = smx->kd->kdNodes;
	p = smx->kd->p;
	pq = smx->pqHead;
	x = ri[0];
	y = ri[1];
	z = ri[2];
	lx = smx->fPeriod[0];
	ly = smx->fPeriod[1];
	lz = smx->fPeriod[2];
	cell = ROOT;
	/*
	 ** First find the "local" Bucket.
	 ** This could mearly be the closest bucket to ri[3].
	 */
	while (cell < smx->kd->nSplit) {
		if (ri[c[cell].iDim] < c[cell].fSplit) cell = LOWER(cell);
		else cell = UPPER(cell);
		}
	/*
	 ** Now start the search from the bucket given by cell!
	 */
	for (pj=c[cell].pLower;pj<=c[cell].pUpper;++pj) {
		dx = x - p[pj].p.dp->pos[0];
		dy = y - p[pj].p.dp->pos[1];
		dz = z - p[pj].p.dp->pos[2];
		fDist2 = dx*dx + dy*dy + dz*dz;
		if (fDist2 < fBall2) {
			if (smx->iMark[pj]) continue;
			smx->iMark[pq->p] = 0;
			smx->iMark[pj] = 1;
			pq->fKey = fDist2;
			pq->p = pj;
			pq->ax = 0.0;
			pq->ay = 0.0;
			pq->az = 0.0;
			PQ_REPLACE(pq);
			fBall2 = pq->fKey;
			}
		}
	while (cell != ROOT) {
		cp = SIBLING(cell);
		ct = cp;
		SETNEXT(ct);
		while (1) {
			INTERSECT(c,cp,fBall2,lx,ly,lz,x,y,z,sx,sy,sz);
			/*
			 ** We have an intersection to test.
			 */
			if (cp < smx->kd->nSplit) {
				cp = LOWER(cp);
				continue;
				}
			else {
				for (pj=c[cp].pLower;pj<=c[cp].pUpper;++pj) {
					dx = sx - p[pj].p.dp->pos[0];
					dy = sy - p[pj].p.dp->pos[1];
					dz = sz - p[pj].p.dp->pos[2];
					fDist2 = dx*dx + dy*dy + dz*dz;
					if (fDist2 < fBall2) {
						if (smx->iMark[pj]) continue;
						smx->iMark[pq->p] = 0;
						smx->iMark[pj] = 1;
						pq->fKey = fDist2;
						pq->p = pj;
						pq->ax = sx - x;
						pq->ay = sy - y;
						pq->az = sz - z;
						PQ_REPLACE(pq);
						fBall2 = pq->fKey;
						}
					}
				}
		GetNextCell:
			SETNEXT(cp);
			if (cp == ct) break;
			}
		cell = PARENT(cell);
		}
	smx->pqHead = pq;
	}


int smBallGather(smx, fBall2, ri)
     SMX smx;
     double fBall2;
     float *ri;
{
	KDN *c;
	PARTICLE *p;
	int pj,nCnt,cp,nSplit;
	float dx,dy,dz,x,y,z,lx,ly,lz,sx,sy,sz,fDist2;

	c = smx->kd->kdNodes;
	p = smx->kd->p;
	nSplit = smx->kd->nSplit;
	lx = smx->fPeriod[0];
	ly = smx->fPeriod[1];
	lz = smx->fPeriod[2];
	x = ri[0];
	y = ri[1];
	z = ri[2];
	nCnt = 0;
	cp = ROOT;
	while (1) {
		INTERSECT(c,cp,fBall2,lx,ly,lz,x,y,z,sx,sy,sz);
		/*
		 ** We have an intersection to test.
		 */
		if (cp < nSplit) {
			cp = LOWER(cp);
			continue;
			}
		else {
			for (pj=c[cp].pLower;pj<=c[cp].pUpper;++pj) {
				dx = sx - p[pj].p.dp->pos[0];
				dy = sy - p[pj].p.dp->pos[1];
				dz = sz - p[pj].p.dp->pos[2];
				fDist2 = dx*dx + dy*dy + dz*dz;
				if (fDist2 < fBall2) {
					smx->fList[nCnt] = fDist2;
					smx->pList[nCnt++] = pj;
					if(nCnt >= smx->nListSize) {
					    smx->nListSize *= 2;
					    smx->fList = realloc(smx->fList, smx->nListSize*sizeof(*smx->fList));
					    smx->pList = realloc(smx->pList, smx->nListSize*sizeof(*smx->pList));
					    }
					}
				}
			}
	GetNextCell:
		SETNEXT(cp);
		if (cp == ROOT) break;
		}
	assert(nCnt <= smx->nListSize);
	return(nCnt);
	}


void smSmooth(smx, fncSmooth)
     SMX smx;
     void (*fncSmooth)();
{
	KDN *c;
	PARTICLE *p;
    PQ *pq,*pqLast;
	int cell;
	int pi,pin,pj,pNext,nCnt,nSmooth;
	float dx,dy,dz,x,y,z,h2,ax,ay,az;


	for (pi=0;pi<smx->kd->nActive;++pi) {
		smx->kd->p[pi].fBall2 = -1.0;
		}
	for (pi=0;pi<smx->kd->nActive;++pi) {
		smx->iMark[pi] = 0;
		}
	pqLast = &smx->pq[smx->nSmooth-1];
	c = smx->kd->kdNodes;
	p = smx->kd->p;
	nSmooth = smx->nSmooth;
	/*
	 ** Initialize Priority Queue.
	 */
	pin = 0;
	pNext = 1;
	ax = 0.0;
	ay = 0.0;
	az = 0.0;
	for (pq=smx->pq,pj=0;pq<=pqLast;++pq,++pj) {
		smx->iMark[pj] = 1;
		pq->p = pj;
		pq->ax = ax;
		pq->ay = ay;
		pq->az = az;
		}
	while (1) {
		if (smx->kd->p[pin].fBall2 >= 0) {
			/*
			 ** Find next particle which is not done, and load the
			 ** priority queue with nSmooth number of particles.
			 */
			while (pNext < smx->kd->nActive 
			       && smx->kd->p[pNext].fBall2 >= 0) ++pNext;
			/*
			 ** Check if we are really finished.
			 */
			if (pNext == smx->kd->nActive) break;
			pi = pNext;
			++pNext;
			x = p[pi].p.dp->pos[0];
			y = p[pi].p.dp->pos[1];
			z = p[pi].p.dp->pos[2];
			/*
			 ** First find the "local" Bucket.
			 ** This could mearly be the closest bucket to ri[3].
			 */
			cell = ROOT;
			while (cell < smx->kd->nSplit) {
				if (p[pi].p.dp->pos[c[cell].iDim] < c[cell].fSplit)
					cell = LOWER(cell);
				else
					cell = UPPER(cell);
				}
			/*
			 ** Remove everything from the queue.
			 */
			smx->pqHead = NULL;
			for (pq=smx->pq;pq<=pqLast;++pq) smx->iMark[pq->p] = 0;
			/*
			 ** Add everything from pj up to and including pj+nSmooth-1.
			 */
			pj = c[cell].pLower;
			if (pj > smx->kd->nActive - nSmooth)
				pj = smx->kd->nActive - nSmooth;
			for (pq=smx->pq;pq<=pqLast;++pq) {
				smx->iMark[pj] = 1;
				dx = x - p[pj].p.dp->pos[0];
				dy = y - p[pj].p.dp->pos[1];
				dz = z - p[pj].p.dp->pos[2];
				pq->fKey = dx*dx + dy*dy + dz*dz;
				pq->p = pj++;
				pq->ax = 0.0;
				pq->ay = 0.0;
				pq->az = 0.0;
				}
			PQ_BUILD(smx->pq,nSmooth,smx->pqHead);
			}
		else {
			/*
			 ** Calculate the priority queue using the previous particles!
			 */
			pi = pin;
			x = p[pi].p.dp->pos[0];
			y = p[pi].p.dp->pos[1];
			z = p[pi].p.dp->pos[2];
			smx->pqHead = NULL;
			for (pq=smx->pq;pq<=pqLast;++pq) {
				pq->ax -= ax;
				pq->ay -= ay;
				pq->az -= az;
				dx = x + pq->ax - p[pq->p].p.dp->pos[0];
				dy = y + pq->ay - p[pq->p].p.dp->pos[1];
				dz = z + pq->az - p[pq->p].p.dp->pos[2];
				pq->fKey = dx*dx + dy*dy + dz*dz;
				}
			PQ_BUILD(smx->pq,nSmooth,smx->pqHead);
			ax = 0.0;
			ay = 0.0;
			az = 0.0;
			}
		smBallSearch(smx,smx->pqHead->fKey,p[pi].p.dp->pos);
		smx->kd->p[pi].fBall2 = smx->pqHead->fKey;
		/*
		 ** Pick next particle, 'pin'.
		 ** Create fList and pList for function 'fncSmooth'.
		 */
		pin = pi;
		nCnt = 0;
		h2 = smx->pqHead->fKey;
		for (pq=smx->pq;pq<=pqLast;++pq) {
			if (pq == smx->pqHead) continue;
			smx->pList[nCnt] = pq->p;
			smx->fList[nCnt++] = pq->fKey;
			if (smx->kd->p[pq->p].fBall2 >= 0) continue;
			if (pq->fKey < h2) {
				pin = pq->p;
				h2 = pq->fKey;
				ax = pq->ax;
				ay = pq->ay;
				az = pq->az;
				}
			}
		(*fncSmooth)(smx,pi,nCnt,smx->pList,smx->fList);
		}
	}


void smReSmooth(smx, fncSmooth)
     SMX smx;
     void (*fncSmooth)();
{
	PARTICLE *p;
	int pi,nSmooth;

	p = smx->kd->p;
	for (pi=0;pi<smx->kd->nActive;++pi) {
		/*
		 ** Do a Ball Gather at the radius of the most distant particle
		 ** which is smDensity sets in smx->pBall[pi].
		 */
		nSmooth = smBallGather(smx,smx->kd->p[pi].fBall2,p[pi].p.dp->pos);
		(*fncSmooth)(smx,pi,nSmooth,smx->pList,smx->fList);
		}
 	}


void smSetBall(smx, ball_size)
     SMX smx;
     double ball_size;
{
    float fBall2;
    int pi;
    
    fBall2 = 4.0*ball_size*ball_size;

    for (pi=0;pi<smx->kd->nActive;++pi) {
	smx->kd->p[pi].fBall2 = fBall2;
	}
}
    
/*
 * Set search ball from sph smoothing length.
 */
void smSetBallh(smx)
     SMX smx;
{
    int pi;
    double fBall;
    
    assert(header.nsph == smx->kd->nActive);


    for (pi=0;pi<smx->kd->nActive;++pi) {
	/*
	 * Search ball is twice smoothing length for spline kernel.
	 */
	fBall = 2.0*smx->kd->p[pi].p.gp->hsmooth;
	smx->kd->p[pi].fBall2 = fBall*fBall;
	}
}
    
void smDensity(smx, pi, nSmooth, pList, fList)
     SMX smx;
     int pi;
     int nSmooth;
     int *pList;
     float *fList;
{
	float ih2,r2,rs,fDensity;
	int i,pj;

	ih2 = 4.0/smx->kd->p[pi].fBall2;
	fDensity = 0.0;
	for (i=0;i<nSmooth;++i) {
		pj = pList[i];
		r2 = fList[i]*ih2;
		rs = 2.0 - sqrt(r2);
		if (r2 < 1.0) rs = (1.0 - 0.75*rs*r2);
		else rs = 0.25*rs*rs*rs;
		fDensity += rs*smx->kd->p[pj].p.dp->mass;
		}
	smx->kd->p[pi].fDensity = M_1_PI*sqrt(ih2)*ih2*fDensity; 
	}


void smDensitySym(smx, pi, nSmooth, pList, fList)
     SMX smx;
     int pi;
     int nSmooth;
     int *pList;
     float *fList;
{
	float fNorm,ih2,r2,rs;
	int i,pj;

	ih2 = 4.0/smx->kd->p[pi].fBall2;
	fNorm = 0.5*M_1_PI*sqrt(ih2)*ih2;
	for (i=0;i<nSmooth;++i) {
		pj = pList[i];
		r2 = fList[i]*ih2;
		rs = 2.0 - sqrt(r2);
		if (r2 < 1.0) rs = (1.0 - 0.75*rs*r2);
		else rs = 0.25*rs*rs*rs;
		rs *= fNorm;
		smx->kd->p[pi].fDensity += rs*smx->kd->p[pj].p.dp->mass;
		smx->kd->p[pj].fDensity += rs*smx->kd->p[pi].p.dp->mass;
		}
	}

void smNull(smx, pi, nSmooth, pList, fList)
     SMX smx;
     int pi;
     int nSmooth;
     int *pList;
     float *fList;
{
	return;
}
