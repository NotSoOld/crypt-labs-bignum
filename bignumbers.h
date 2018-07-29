#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define uint64 unsigned __int64
#define uint32 unsigned int
#define uint16 unsigned short
#define uint8 unsigned char
#define UINT64_MAX 0xffffffffffffffff
#define DEC_MAX 1000000000


union Big
{
	uint64 aa[2];
	uint32 bb[4];
	uint16 cc[8];
	uint8 dd[17];
};


void printBig(Big *);
Big *bigLShift(Big *, int);
Big *bigRShift(Big *, int);
Big *bigAdd(Big *, Big *, uint8 &);
Big *bigAdd(Big *, Big *);
int bigCompare(Big *, Big *);
Big *bigNegate(Big *);
Big *bigSNeg(Big *);
Big *bigSAdd(Big *, Big *);
void bigAssign(Big *, Big *);
Big *loadBigNumber(char *);
char *readNumber(char *);
int *stringToIntArray(char *, int &);
Big *parseIntoBigNumber(int *, int);
Big *bigModSum(Big *, Big *, Big *);
Big *bigModMult(Big *, Big *, Big *);
Big *bigModPwr(Big *, Big *, Big *);
void extendedBinaryEuclidian(Big *, Big *, Big *, Big *);
void printBigDecimal(Big *);
void checkTypes();