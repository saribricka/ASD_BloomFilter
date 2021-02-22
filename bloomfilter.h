#ifndef BLOOMFILTER_H_INCLUDED
#define BLOOMFILTER_H_INCLUDED

#include <stdbool.h>

#include "hash.h"

typedef struct
{
	int k;			//numero di funzioni hash
	int m;			//numero di bit del filtro
	int* hashSeeds;	//seed delle k funzioni hash
	bool* filter;	//filtro
} BloomFilter;

BloomFilter createBloomFilter(int k,int m);
void freeBloomFilter(BloomFilter bloomFilter);

void bfInsertion(BloomFilter* bloomFilter, const char* elem, int elemLen);
void bfConstruction(BloomFilter* bloomFilter, const char** dataset, const int* datasetElemLen, int n);
bool bfSearch(BloomFilter bloomFilter, const char* elem, int elemLen);
int countDatasetMembership(BloomFilter bloomFilter, const char** dataset, const int* datasetElemLen, int n);

#endif // BLOOMFILTER_H_INCLUDED
