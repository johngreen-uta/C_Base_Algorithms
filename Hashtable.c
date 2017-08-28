#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

/*************************************************
* Discription: the program uses a seprate chaining
* hash table to catalog and count all words in the
* source file
* 
* Author: John Green
* Time: 11/8/2014
**************************************************/

#define FALSE (0)
#define TRUE (!FALSE)

typedef int STATUS; 

#define TABLE_LENGTH 1000000
#define NULL 0

typedef struct HashNode{
	char* key;
	int data; 
	struct HashNode* next;
} HashNode;

typedef struct Hashtable{
	int size;	
	struct HashNode* hashArray; 
} Hashtable;

void Init(Hashtable* hashtable);
STATUS IsEmpty(Hashtable* hashtable);
int Get(char* key, Hashtable* hashtable);
int Size(Hashtable* hashtable);
STATUS ContainsKey(char* key, Hashtable* hashtable);
void Put(char* key, int value, Hashtable* hashtable);
void Destroy(Hashtable* hashtable);
int HashKey(char* key, Hashtable* hashtable);
char** Keys(Hashtable* hashtable);
void CalculateWordFrequency(char* inputFile);

/*
* Initialize a hashtable. 
*/
void Init(Hashtable* hashtable)
{
	HashNode nullNode;
	int i;
	// create NullNode
	hashtable->size = TABLE_LENGTH;
	hashtable->hashArray = malloc(TABLE_LENGTH*sizeof(HashNode));
	nullNode.key = NULL;
	nullNode.data = -1;
	nullNode.next = NULL;
	for(i=0; i<TABLE_LENGTH; i++)
	{
		// initialize array to nullnodes
		hashtable->hashArray[i] = nullNode;
	}
}
/*
* Test if this hastable is empty. 
*/ 
STATUS IsEmpty(Hashtable* hashtable)
{
	int i;
	for(i=0; i<hashtable->size; i++)
	{
		// test if a key is present
		if(hashtable->hashArray[i].key != NULL)
			return FALSE;
	}
	return TRUE;
}

/*
* Returns the value to which the specified key is mapped, 
* or NULL if this map contains no mapping for the key. 
*/
int Get(char* key, Hashtable* hashtable)
{
	HashNode *tempNode;
	// get node were key would be
	tempNode = &(hashtable->hashArray[HashKey(key,hashtable)]);
	while(tempNode != NULL)
	{
		// se if a key exists at location
		if(tempNode->key == NULL)
		{
			return NULL;
		}
		// check if keys match
		else if(strcmp(tempNode->key, key) == 0)
		{
			tempNode->data+=1;
			return HashKey(key,hashtable);
		}
		tempNode = tempNode->next;
	}
	return NULL;
}

/*
* Returns the number of keys in this hashtable.
*/
int Size(Hashtable* hashtable)
{
	HashNode *tempNode;
	int i, count=0;
	// loop through hashtable counting each key
	for(i=0; i<hashtable->size; i++)
	{
		tempNode = &(hashtable->hashArray[i]);
		// test if key exists
		if(tempNode->key != NULL)
		{
			while(tempNode != NULL)
			{
				tempNode = tempNode->next;
				count++;
			}
		}
	}
	return count;
}

/*
* Tests if the specified object is a key in this hashtable.
*/
STATUS ContainsKey(char* key, Hashtable* hashtable)
{
	HashNode *tempNode;
	// get key location
	tempNode = &(hashtable->hashArray[HashKey(key,hashtable)]);
	if(tempNode->key != NULL)
	{
		// check if key is present
		while(tempNode != NULL)
		{
			if(strcmp(tempNode->key,key) == 0)
				return TRUE;
			tempNode = tempNode->next;
		}
	}
	return FALSE;
}

/*
* Maps the specified key to the specified value in this hashtable. 
* Neither the key nor the value can be null.
* The value can be retrieved by calling the get method with a key 
* that is equal to the original key.
*/
void Put(char* key, int value, Hashtable* hashtable)
{
	HashNode *tempNode, *newNode;
	// add key if hashtable location is empty
	if(hashtable->hashArray[value].key == NULL)
	{
		hashtable->hashArray[value].key = key;
		hashtable->hashArray[value].data = 1;
	}
	// add key to the end of key chain
	else
	{
		newNode = malloc(sizeof(HashNode));
		newNode->key = key;
		newNode->data = 1;
		newNode->next = NULL;
		tempNode = &(hashtable->hashArray[value]);
		while(tempNode->next != NULL)
			tempNode = tempNode->next;
		tempNode->next = newNode;
	}
}

/*
* Destroy this hashtable. Free the space for this hashtable.
*/
void Destroy(Hashtable* hashtable)
{
	int i;
	HashNode *tempNode;
	HashNode *deleteNode;
	
	for(i=0; i<hashtable->size; i++)
	{
		tempNode = &(hashtable->hashArray[i]);
		while(tempNode->next != NULL)
		{
			deleteNode = tempNode;
			tempNode = tempNode->next;
			free(deleteNode);
		}
		free(tempNode);
	}
	free(hashtable->hashArray);
	free(hashtable);
}

/*
* Calculate the hash value for a key
*/ 
int HashKey(char* key, Hashtable* hashtable)
{
	int hash = 0;
    int i;
    // loop through chars
    for (i = 0; i < (int)strlen(key); i++)
    {
		// calculate hash number
        hash = ((hash << 3) + abs(key[i])) % hashtable->size;
    }
    return hash;
}

/*
* Returns an array of keys in this hashtable.
*/ 
char** Keys(Hashtable* hashtable)
{
	HashNode *tempNode;
	char **keys;
	int i;
	int arrayPos = 0;
	
	keys = malloc(Size(hashtable)*sizeof(char*));
	
	for(i=0; i<hashtable->size; i++)
	{
		// check if key is present
		if(hashtable->hashArray[i].key != NULL)
		{
			tempNode = &(hashtable->hashArray[i]);
			while(tempNode != NULL)
			{
				// add keys to list
				keys[arrayPos] = hashtable->hashArray[i].key;
				arrayPos++;
				tempNode = tempNode->next;
			}
		}
	}
	
	return keys;
}

/* 
* Count the frequency of the word in a txt file.
* Print the word and frequency of this word. 
* For example:
* "hash", 18 
* "table", 7
* ... 
*/ 
void CalculateWordFrequency(char* inputFile){

	FILE* file;
	char *curWord, buffer[1000], *key;
	Hashtable *set;
	int value, i;
	set = malloc(sizeof(Hashtable)); // initialize the array
	Init(set);
	if( (file = fopen(inputFile, "r") ) == NULL ) // check if file exists
    {
		printf("failed to load file");
        exit(1);
    }
    // loop through file grabing each line for proccesing
    while( fgets(buffer, sizeof(buffer), file) != NULL )
    {		
		curWord = strtok(buffer, "  \"&@#$%^*/,.-:;!?()[]{}1234567890\n\t\r");
		while(curWord != NULL)
		{
			// check if key exists
			value = Get(curWord, set);
			if(value == NULL)
			{
				// add key to hashtable
				value = HashKey(curWord,set);
				key = malloc(strlen(curWord)*sizeof(char));
				strcpy(key,curWord);
				Put(key,value,set);
			}
			
			curWord = strtok(NULL, "  \"&@#$%^*/,.-:;!?()[]{}1234567890\n\t\r");
		}
	}
	fclose(file);

	// loop through table and print key and count
	for(i=0; i<set->size; i++)
	{
		if(set->hashArray[i].key != NULL)
			printf("%s, %d\n",set->hashArray[i].key, set->hashArray[i].data);
	}
}  

int main()
{
	char* filename = "lab3.txt"; 
	CalculateWordFrequency(filename); 
	
	return 0;
} 
