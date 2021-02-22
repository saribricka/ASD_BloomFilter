
#include "hash.h"

// funzione hash murmur 
unsigned int hashMurmur(const char* key, int unsigned len, unsigned int seed)
{
	unsigned int h = seed;

	//Key chunks
	if (len > 3)
	{
		const unsigned int* key_x4 = (const int*)key;
		int i = len >> 2;
		do {
			unsigned int k = *key_x4;
			k *= 0xCC9E2D51;
			k = (k << 15) | (k >> 17);
			k *= 0x1B873593;
			h ^= k;
			h = (h << 13) | (h >> 19);
			h = (h * 5) + 0xE6546B64;
			key_x4++;
		} while (--i);
		key = (const char*)key_x4;
	}
	//---

	//Trailing chunk
	if (len & 3) //len non è un multiplo di 4
	{
		int i = len & 3;
		unsigned int t = 0;
		key = &key[i - 1];
		do
		{
			t <<= 8;
			t |= *key;
			key--;
		} while (--i);
		t *= 0xCC9E2D51;
		t = (t << 15) | (t >> 17);
		t *= 0x1B873593;
		h ^= t;
	}
	//---

	//Avalanche
	h ^= len;
	h ^= h >> 16;
	h *= 0x85EBCA6B;
	h ^= h >> 13;
	h *= 0xC2B2AE35;
	h ^= h >> 16;
	//---

	return h;
}

// funzione hash utilizzata dal bloom filter che modula il digest in base alla dimensione
// del bloom filter, in modo che lo spazio di indirizzamento coincida con m
unsigned int hashFunction(const char* key, int keyLen,int seed,int m)
{
	return hashMurmur(key, keyLen, seed) % m;
}