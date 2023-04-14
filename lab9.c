#include <stdio.h>
#include <stdlib.h>

// RecordType
typedef struct RecordType
{
	int		id;
	char	name;
	int		order; 
  struct RecordType *next;
}RecordType;

// Fill out this structure
typedef struct HashType
{
  int size;
  RecordType **table;
}HashType;

// Compute the hash function
int hash(int x, int size)
{
return x % size;
}

void insert(HashType* hashTable, RecordType* record) {
  int hashIndex = hash(record->id, hashTable->size);

  RecordType* newRecord = (RecordType*)malloc(sizeof(RecordType)); // Create new node for the record
  newRecord->id = record->id;
  newRecord->name = record->name;
  newRecord->order = record->order;
  newRecord->next = NULL;

  
  if (hashTable->table[hashIndex] == NULL) {
    hashTable->table[hashIndex] = newRecord;// If the index is empty, add the record
  } else {
    RecordType* cur = hashTable->table[hashIndex];
    while (cur->next != NULL) {
      cur = cur->next;// Traverse the linked list at the index
    }

    cur->next = newRecord;// add the new record to the end of the linked list
  }
}


// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
      			pRecord->next = NULL;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;
    for (i = 0; i < hashSz; ++i)
    {
        RecordType *pRecord = pHashArray->table[i];
        if (pRecord != NULL)
        {
            printf("Index %d -> ", i);
            while (pRecord != NULL)
            {
                printf("%d, %c, %d", pRecord->id, pRecord->name, pRecord->order);//prints the data for every node in the linked list
                pRecord = pRecord->next;//moves to next item in the linked list
                if (pRecord != NULL) printf(" -> ");
            }
            printf("-> NULL");
            printf("\n");
        }
    }
}




int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	HashType* HashTable = (HashType*) malloc(sizeof(HashType));//initialize the hashtable
	int hashsz = 11;
	HashTable->size = hashsz;
	HashTable->table = (RecordType**)calloc(hashsz,sizeof(RecordType*));//initialize the linked lists

	for(int i=0;i<recordSz;i++)
	{
		insert(HashTable, &pRecords[i]);//inserts data into the hashtable for every struct in records
	}

	displayRecordsInHash(HashTable, hashsz);

	// Free memory
	for(int i=0; i < hashsz ;++i)
	{
		RecordType *tmp = HashTable->table[i];
		while(tmp != NULL)
		{
			RecordType *Tnext = tmp->next;
			free(tmp);
			tmp = Tnext;
		}
	}
	free(HashTable->table);
	free(HashTable);
	free(pRecords);

	return 0;
}
