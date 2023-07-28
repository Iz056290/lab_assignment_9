#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order;
};

struct Node
{
    struct RecordType record;
    struct Node* next;
};

// Fill out this structure
struct HashType
{
    struct Node* head;
};

// Compute the hash function
int hash(int x, int hashSize)
{
    return x % hashSize;
}

void insertRecord(struct HashType* hashTable, int hashSize, struct RecordType record)
{
    int index = hash(record.id, hashSize);

    // Create a new node for the record
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->record = record;
    newNode->next = NULL;

    // If the slot in the hash table is empty, simply add the new node
    if (hashTable[index].head == NULL)
    {
        hashTable[index].head = newNode;
    }
    else // If the slot is not empty, add the new node to the front of the linked list
    {
        newNode->next = hashTable[index].head;
        hashTable[index].head = newNode;
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
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    for (int i = 0; i < hashSz; ++i)
    {
        printf("index %d ->", i);
        struct Node* current = pHashArray[i].head;
        while (current != NULL)
        {
            printf(" %d %c %d ->", current->record.id, current->record.name, current->record.order);
            current = current->next;
        }
        printf(" NULL\n");
    }
}

void freeHashTable(struct HashType* pHashArray, int hashSz)
{
    for (int i = 0; i < hashSz; ++i)
    {
        struct Node* current = pHashArray[i].head;
        while (current != NULL)
        {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(pHashArray);
}


int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    int hashSize = 15; // You can adjust this size according to your requirements

    // Create the hash table
    struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType) * hashSize);
    for (int i = 0; i < hashSize; ++i)
    {
        hashTable[i].head = NULL;
    }

    // Your hash implementation
    for (int i = 0; i < recordSz; ++i)
    {
        insertRecord(hashTable, hashSize, pRecords[i]);
    }

    // Display records in the hash table
    displayRecordsInHash(hashTable, hashSize);

    // Free memory used by the hash table and individual linked lists
    for (int i = 0; i < hashSize; ++i)
    {
        struct Node* current = hashTable[i].head;
        while (current != NULL)
        {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable);

    // Free memory used by the records array
    free(pRecords);

    return 0;
}