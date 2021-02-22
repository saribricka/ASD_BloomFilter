#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bloomfilter.h"

#define LINE_LENGTH 120

//funzione che carica in memoria un dataset
char** loadDataset(const char* filePath, int* n, int** elem_lenghts);

//funzione di verifica dei file di output
bool filediff(FILE* f1, FILE* f2);

// Main
void main()
{
	static const char* CONSTRUCTION_DATASET = "C:\\Users\\Sara\\Desktop\\Fresh_Materiale\\dataset1.txt";
	static const char* VERIFICATION_DATASET = "C:\\Users\\Sara\\Desktop\\Fresh_Materiale\\non-elements.txt";
	static const char* OUTPUT_FILE = "C:\\Users\\Sara\\Desktop\\Fresh_Materiale\\output.txt";
	static const char* VERIFICATION_FILE = "C:\\Users\\Sara\\Desktop\\Fresh_Materiale\\OUTPUT_CORRETTO1.txt";

	static const int hash_number = 10;		//k
	static const int bit_number = 1048576;	//m

	FILE *fout,*foutver;
	int i, n, non_elements, true_positives, false_positives;
	char** construction_dataset;
	int* construction_dataset_elem_lenghts;
	char** verification_dataset;
	int* verification_dataset_elem_lenghts;
	BloomFilter bloomFilter;
	bool final_check;

	////////// LETTURA DEI DATASET E SALVATAGGIO NEGLI ARRAY //////////
	construction_dataset = loadDataset(CONSTRUCTION_DATASET, &n, &construction_dataset_elem_lenghts);
	if (construction_dataset == NULL)
		return;

	verification_dataset = loadDataset(VERIFICATION_DATASET, &non_elements, &verification_dataset_elem_lenghts);
	if (verification_dataset == NULL)
		return;

	fout = fopen(OUTPUT_FILE, "w");
	if (fout == NULL) 
		return;

	fprintf(fout, "Elementi del dataset di costruzione: %d\n", n);
	fprintf(fout, "Elementi del dataset di verifica: %d\n", non_elements);
	//---
	
	////////// COSTRUZIONE DEL FILTRO //////////
	bloomFilter = createBloomFilter(hash_number, bit_number);
	bfConstruction(&bloomFilter, construction_dataset, construction_dataset_elem_lenghts,n);
	//---

	//////////  SELF-CHECK ELEMENTI  //////////
	true_positives = countDatasetMembership(bloomFilter,construction_dataset,construction_dataset_elem_lenghts,n);
	fprintf(fout, "True positives (self-check): %d\n", true_positives);
	//---

	//////////  CHECK NON ELEMENTI  //////////
	false_positives= countDatasetMembership(bloomFilter, verification_dataset,verification_dataset_elem_lenghts ,non_elements);
	fprintf(fout, "True negatives: %d\n", non_elements - false_positives);
	fprintf(fout, "False positives: %d\n", false_positives);
	fprintf(fout, "FPR: %f", (double)false_positives / non_elements);
	//---

	fclose(fout);

	//////////  CONTROLLO DI CORRISPONDENZA DEI RISULTATI CON IL FILE FORNITO  //////////
	fout = fopen(OUTPUT_FILE, "r");
	if (fout == NULL)
		return;

	foutver = fopen(VERIFICATION_FILE, "r");
	if (foutver == NULL)
		return;

	final_check = filediff(fout, foutver);
	if (final_check)
		printf("Verifica dei risultati eseguita con successo.\n");
	else 
		printf("Verifica dei risultati fallita.\n");

	fclose(fout);
	fclose(foutver);
	//---

	freeBloomFilter(bloomFilter);
	
	for (i = 0; i < n; i++)
		free(construction_dataset[i]);
	free(construction_dataset);
	free(construction_dataset_elem_lenghts);

	for (i = 0; i < non_elements; i++)
		free(verification_dataset[i]);
	free(verification_dataset);
	free(verification_dataset_elem_lenghts);
}


//carica in memoria un dataset
char** loadDataset(const char* filePath,int* n,int** elem_lenghts)
{
	int i;
	char* line[LINE_LENGTH];
	char** dataset;
	
	FILE* f = fopen(filePath, "r");
	if (f == NULL)
		return NULL;

	*n = 0;
	while (fgets((char*)line, LINE_LENGTH, f) != NULL)
		(*n)++;

	rewind(f);

	dataset = (char**)malloc((*n) * sizeof(char *));
	*elem_lenghts = (int*)malloc((*n) * sizeof(int));

	i = 0;
	while (fgets((char*)line, LINE_LENGTH, f) != NULL)
	{
		(*elem_lenghts)[i] = strlen((char*)line)-1; //Rimuove il /n

		dataset[i] = (char*)malloc((*elem_lenghts)[i] * sizeof(char));

		memcpy(dataset[i], line, (*elem_lenghts)[i]);
		i++;
	}

	fclose(f);

	return dataset;
}

//confronta due file; ritorna true se il contenuto è identico, false altrimenti
bool filediff(FILE* fp1, FILE* fp2)
{
	char ch1, ch2;

	rewind(fp1);
	rewind(fp2);

	ch1 = getc(fp1);
	ch2 = getc(fp2);
	while (ch1 == '\r') ch1 = getc(fp1);
	while (ch2 == '\r') ch2 = getc(fp2);

	//legge un char da entrambi i file
	while (ch1 != EOF || ch2 != EOF)
	{
		if (ch1 != ch2) return false;
		ch1 = getc(fp1);
		ch2 = getc(fp2);
		while (ch1 == '\r') ch1 = getc(fp1);
		while (ch2 == '\r') ch2 = getc(fp2);
	}

	return true;
}
