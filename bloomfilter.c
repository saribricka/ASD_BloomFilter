//Sara Briccoli - 838847

#include <stdlib.h>

#include "bloomfilter.h"

//Creazione di un nuovo Bloom filter
BloomFilter createBloomFilter(int k, int m)
{
	BloomFilter bloomFilter;
	bloomFilter.k = k;
	bloomFilter.m = m;
	bloomFilter.hashSeeds = (int*)malloc(sizeof(int) * k);
	bloomFilter.filter = (bool*)malloc(sizeof(bool) * m);

	if (bloomFilter.filter == NULL || bloomFilter.hashSeeds == NULL)
	{
		printf("ERROR: memory weren't allocated!\n");
		exit(1);
	}
	printf("OK: memory allocated\n");
	//Set a number 1-k for each hash function
	for (int i = 1; i <= k; i++)
	{
		bloomFilter.hashSeeds[i - 1] = i;
	}
	//Set 0 every filters'bit
	for (int i = 0; i < m; i++)
	{
		bloomFilter.filter[i] = false;
	}
	return bloomFilter;
}

// Libera tutta la memoria allocata per il bloom filter
void freeBloomFilter(BloomFilter bloomFilter)
{
	free(bloomFilter.filter);
	free(bloomFilter.hashSeeds);
/*	if (bloomFilter.filter == NULL || bloomFilter.hashSeeds == NULL)
	{
		printf("OK: free function worked\n");
	}
	else 
	{
		printf("ERROR: free function didn't work\n");
		exit(1);
	}	*/

}

//Inserimento di un elemento all'interno del bloom filter
void bfInsertion(BloomFilter * bloomFilter, const char* elem, int elemLen)
{
	int i = 0;
	while (i < bloomFilter->k)
	{
		bloomFilter->filter[hashFunction(elem, elemLen, bloomFilter->hashSeeds[i], bloomFilter->m)] = true;
		i++;
	}
	//return bloomFilter;
}

//Inserimento di un dataset di elementi all'interno del bloom filter
void bfConstruction(BloomFilter * bloomFilter, const char** dataset, const int* datasetElemLen, int n)
{
	int i = 0;
	while (i < n)
	{
		bfInsertion(bloomFilter, dataset[i], datasetElemLen[i]);
		i++;
	}
}

//Ricerca di un elemento all'interno del bloom filter
bool bfSearch(BloomFilter bloomFilter, const char* elem, int elemLen)
{
	int i = 0;
	while (i < bloomFilter.k)
	{
		if (bloomFilter.filter[hashFunction(elem, elemLen, bloomFilter.hashSeeds[i], bloomFilter.m)] == false)
		{
			return false;
		}
		i += 1;
	}
	return true;
}

//Conteggio del numero di elementi di un dataset presenti nel bloom filter
int countDatasetMembership(BloomFilter bloomFilter, const char** dataset, const int* datasetElemLen, int n)
{
	int c = 0;	//counter
	for (int i = 0; i < n; i++)
	{
		if (bfSearch(bloomFilter, dataset[i], datasetElemLen[i]) == true)
		{
			c++;
		}
	}
	printf("Found %d elements!\n", &c);
	return c;
}