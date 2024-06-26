#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 37

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType* record;
	struct HashType* next;
};

// Compute the hash function
int hash(int x)
{
	return x % TABLE_SIZE;
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
			printf("%d ", hash(pRecord->id));
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
void displayRecordsInHash(struct HashType *pHashArray[], int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		if (pHashArray[i] != NULL) {
			printf("index %d -> ", i);
			struct HashType* temp = pHashArray[i];
			while (temp != NULL) {
				printf("%d, %c, %d -> ", temp->record->id, temp->record->name, temp->record->order);
				temp = temp->next;
			}
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
	// Your hash implementation

	// Creates and initializes the hash table
	struct HashType* table[TABLE_SIZE] = { NULL };


	for(int i = 0; i < recordSz; i++) {

		int record_index = hash(pRecords[i].id);

		// If there is no value stored at the current index, add a HashType and point it towards the record
		if (table[record_index] == NULL) {
			table[record_index] = (struct HashType*) malloc(sizeof(struct HashType));
			if (table[record_index] == NULL) {
				return -1;
			}
			table[record_index]->record = &pRecords[i];
			table[record_index]->next = NULL;
		}

		// Otherwise, if there is a collision, insert at the end of the linked list of HashTypes our new
		// HashType and store the pointer to the record there.
		else {
			struct HashType* temp = table[record_index];
			while (temp->next != NULL) {
				temp = temp->next;
			}
			temp->next = (struct HashType*) malloc(sizeof(struct HashType));
			if (temp->next == NULL) {
				return -1;
			}
			temp = temp->next;
			temp->record = &pRecords[i];
			temp->next = NULL; 
		}
	}

	displayRecordsInHash(table, TABLE_SIZE);
}