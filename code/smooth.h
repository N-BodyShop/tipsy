#ifndef SMOOTH_HINCLUDED
#define SMOOTH_HINCLUDED

#include "kd.h"

#define RESMOOTH_SAFE  10


typedef struct pqNode {
	float fKey;
	struct pqNode *pqLoser;
	struct pqNode *pqFromInt;
	struct pqNode *pqFromExt;
	struct pqNode *pqWinner;	/* Only used when building initial tree */
	int p;
	float ax;
	float ay;
	float az;
	} PQ;


typedef struct smContext {
	KD kd;
	int nSmooth;
	float fPeriod[3];
	PQ *pq;
	PQ *pqHead;
	char *iMark;
	int nListSize;
	float *fList;
	int *pList;
	} * SMX;


#define PQ_INIT(pq,n)\
{\
	int PQ_j;\
	for (PQ_j=0;PQ_j<(n);++PQ_j) {\
		if (PQ_j < 2) (pq)[PQ_j].pqFromInt = NULL;\
		else (pq)[PQ_j].pqFromInt = &(pq)[PQ_j>>1];\
		(pq)[PQ_j].pqFromExt = &(pq)[(PQ_j+(n))>>1];\
		}\
	}


#define PQ_BUILD(pq,n,q)\
{\
	int PQ_i,PQ_j;PQ *PQ_t,*PQ_lt;\
	for (PQ_j=(n)-1;PQ_j>0;--PQ_j) {\
		PQ_i = (PQ_j<<1);\
		if (PQ_i < (n)) PQ_t = (pq)[PQ_i].pqWinner;\
		else PQ_t = &(pq)[PQ_i-(n)];\
		++PQ_i;\
		if (PQ_i < (n)) PQ_lt = (pq)[PQ_i].pqWinner;\
		else PQ_lt = &(pq)[PQ_i-(n)];\
		if (PQ_t->fKey < PQ_lt->fKey) {\
			(pq)[PQ_j].pqLoser = PQ_t;\
			(pq)[PQ_j].pqWinner = PQ_lt;\
			}\
		else {\
			(pq)[PQ_j].pqLoser = PQ_lt;\
			(pq)[PQ_j].pqWinner = PQ_t;\
			}\
		}\
	(q) = (pq)[1].pqWinner;\
	}


#define PQ_REPLACE(q)\
{\
	PQ *PQ_t,*PQ_lt;\
	PQ_t = (q)->pqFromExt;\
	while (PQ_t) {\
		if (PQ_t->pqLoser->fKey > (q)->fKey) {\
			PQ_lt = PQ_t->pqLoser;\
			PQ_t->pqLoser = (q);\
			(q) = PQ_lt;\
			}\
		PQ_t = PQ_t->pqFromInt;\
		}\
	}



PROTO(void, calc_density, (SMX *psmx, int bDark, int bGas, int bStar));
PROTO(void, calc_balls, (SMX *psmx, int bDark, int bGas, int bStar));
PROTO(int, smInit, (SMX *,KD,int,float *));
PROTO(void, smFinish, (SMX));
PROTO(void, smBallSearch, (SMX,double,float *));
PROTO(int,  smBallGather, (SMX,double,float *));
PROTO(void, smSmooth, (SMX,void (*)(SMX,int,int,int *,float *)));
PROTO(void, smReSmooth, (SMX,void (*)(SMX,int,int,int *,float *)));
PROTO(void, smDensity, (SMX,int,int,int *,float *));
PROTO(void, smDensitySym, (SMX,int,int,int *,float *));
PROTO(void, smMeanVel, (SMX,int,int,int *,float *));
PROTO(void, smMeanVelSym, (SMX,int,int,int *,float *));
PROTO(void, smVelDisp, (SMX,int,int,int *,float *));
PROTO(void, smVelDispSym, (SMX,int,int,int *,float *));
PROTO(void, smNull, (SMX,int,int,int *,float *));
PROTO(void, smOutDensity, (SMX,FILE *));
PROTO(void, smOutMeanVel, (SMX,FILE *));
PROTO(void, smOutVelDisp, (SMX,FILE *));
PROTO(void, smOutPhase, (SMX,FILE *));
PROTO(void, smOutMach, (SMX,FILE *));
PROTO(void, smOutSpeed, (SMX,FILE *));
PROTO(void, smSetBallh, (SMX smx));

#endif



