#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

/*************************************************
* Discription: calculation of the shortest route 
* through use of the Dijkstra algorithm
* 
* Author: john green
* Time: 11/30/14
**************************************************/
#define LEN 1000


// A structure to represent an adjacency list node
typedef struct AdjListNode
{
    char data;
	int distance; 
    struct AdjListNode* next;
} AdjListNode;
 
// A structure to represent an adjacency liat
typedef struct AdjList
{
	char data;
    struct AdjListNode* head;  // pointer to head node of list
} AdjList;
 
// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be the number of vertices in graph.
typedef struct Graph
{
    int NumberOfNodes;
    struct AdjList* array;
} Graph;

void InitGraph(Graph* G, char* NodesStr);
void AddEdge(Graph* G, char* edgeStr); 
int CalculateRouteDistance(Graph* G, char* routeNodesStr);
char* CalculateShortestRoute(Graph* G, char* fromTo);
int minDistance(int disList[], int checkList[], int size);
int getPos(Graph* G, char node);
void DestroyGraph(Graph* G);


/*
* Initialize a graph.   
* 
* NodesStr describes the node list in the graph. 
* For example, if NodesStr is "ABCDE", this means that 
* there are 5 nodes in the graph and nodes id is A,
* B, C, D, E respectively.   
*/ 
void InitGraph(Graph* G, char* NodesStr)
{
	int size, i;
	AdjList *tempList;
	
	size = strlen(NodesStr);
	G->NumberOfNodes = size;
	G->array = malloc(size*sizeof(AdjList));
	for(i=0; i<size; i++)
	{
		tempList = malloc(sizeof(AdjList));
		tempList->data = NodesStr[i];
		tempList->head = NULL;
		G->array[i] = *tempList;
	}
}

/*
* The format of the string is 1st Character = starting node name 2nd
* Character = ending node name Integer = edge length
* 
* For example, the String "AB5" should create 2 nodes, each labeled "A" and
* "B" respectively with a distance between them of 5 units.
*/ 
void AddEdge(Graph* G, char* edgeStr)
{
	int i;
	AdjListNode *tempNode, *tempArrayNode;
	
	for(i=0; i<G->NumberOfNodes; i++)
	{
		if(G->array[i].data == edgeStr[0])
		{
			tempArrayNode = G->array[i].head;
			tempNode = malloc(sizeof(AdjListNode));
			tempNode->data = edgeStr[1];
			tempNode->distance = atoi(&edgeStr[2]);
			tempNode->next = NULL;
			if(tempArrayNode == NULL)
			{
				G->array[i].head = tempNode;
				return;
			}
			while(tempArrayNode->next != NULL)
			{
				tempArrayNode = tempArrayNode->next;
			}
			tempArrayNode->next = tempNode;
			return;
		}
	}
}

/*
* Each character in the String represents an edge traversal over which the
* sum of the edge lengths is to be accumulated.
* 
* The route is defined by providing a sequence of characters that represent
* the nodes of the path.
* 
* For example, if a graph had been created as "AB5", "BC2" then the return
* value for various paths would be:
* 
* "AB" --> 5 "BC" --> 2 "ABC" --> 7 
*/
int CalculateRouteDistance(Graph* G, char* routeNodesStr)
{
	int i, j, distance = 0;
	AdjListNode *tempNode;
	
	for(i=0; i<strlen(routeNodesStr)-1; i++)
	{
		for(j=0; j<G->NumberOfNodes; j++)
		{
			if(routeNodesStr[i] == G->array[j].data)
			{
				tempNode = G->array[j].head;
				while(tempNode != NULL)
				{
					if(tempNode->data == routeNodesStr[i+1])
						distance += tempNode->distance;
					tempNode = tempNode->next;
				}
			}
		}
	}
	return distance;
}

/*
* The format of the fromTo input string is 1st Character = starting node
* name 2nd Character = ending node name
* 
* Any single shortest path from the starting node name to the ending node
* dame. If the graph had been created as "AB5", "BC2", "AD2", "DC5", and
* this method was called with "AC" then both paths "ABC7" and "ADC7" are of
* length 7 and either would be a valid return value.
* 
* The return value should be the node names followed by the length of the
* path.
* 
* Hint: Students can use the Dijkstra algorithm to implement the shortest 
* route on the graph, 
*/
char* CalculateShortestRoute(Graph* G, char* fromTo)
{
	AdjListNode *tempNode;
	char *string, *tempStr;
	int *parentList;
	int *disList;
	int *checkList;
	int i, start, newDis, parDis, index;
	
	parentList = malloc(G->NumberOfNodes*sizeof(int));
	disList = malloc(G->NumberOfNodes*sizeof(int));
	checkList = malloc(G->NumberOfNodes*sizeof(int));
	string = malloc(LEN*sizeof(char));
	tempStr = malloc(LEN*sizeof(char));
	
	for(i=0; i<G->NumberOfNodes; i++)
	{
		parentList[i] = -1;
		disList[i] = 999;
		checkList[i] = 0;
	}
	
	start = getPos(G,fromTo[0]);
	disList[start] = 0;
	for(i=0; i<G->NumberOfNodes; i++)
	{
		start = minDistance(disList, checkList, G->NumberOfNodes);
		checkList[start] = 1;
		tempNode = G->array[start].head;
		while(tempNode != NULL)
		{
			index = getPos(G, tempNode->data);
			parDis = disList[start];
			newDis = (tempNode->distance + parDis);
			if(newDis < disList[index] || disList[index] == 0)
			{
				disList[index] = newDis;
				parentList[index] = start;
			}
			tempNode = tempNode->next;
		}
	}
	
	sprintf(string,"%c%d",fromTo[1],disList[getPos(G,fromTo[1])]);
	index = parentList[getPos(G,fromTo[1])];
	while(index != getPos(G,fromTo[0]))
	{
		sprintf(tempStr,"%c%s",G->array[index].data,string);
		index = parentList[index];
		strcpy(string,tempStr);
	}
	sprintf(tempStr,"%c%s",G->array[index].data,string);
	strcpy(string,tempStr);
	free(parentList);
	free(disList);
	free(checkList);
	free(tempStr);
	
	return string;
}

int minDistance(int disList[], int checkList[], int size)
{
	int min = 999, min_index, i;
	for(i=0; i< size; i++)
		if (checkList[i] == 0 && disList[i] <= min)
		{
			min = disList[i];
			min_index = i;
		}
	return min_index;
}

int getPos(Graph* G, char node)
{
	int i;
	for(i=0; i<G->NumberOfNodes; i++)
	{
		if(G->array[i].data == node)
			return i;
	}
	return -1;
}

/*
* Destroy a graph.    
*/ 
void DestroyGraph(Graph* G)
{
	AdjListNode *tempNode;
	AdjListNode *freeNode;
	int i;
	for(i=0; i<G->NumberOfNodes; i++)
	{
		tempNode = G->array[i].head;
		while(tempNode->next != NULL)
		{
			freeNode = tempNode;
			tempNode = tempNode->next;
			free(freeNode);
		}
	}
	free(G->array);
}

int main()
{
	Graph G;
	char path[LEN];
	char fromTo[LEN];

	InitGraph(&G, "ABCDE");

	AddEdge(&G, "AB5");
	AddEdge(&G, "BC4");
	AddEdge(&G, "CD8");
	AddEdge(&G, "DC8");
	AddEdge(&G, "DE6");
	AddEdge(&G, "AD5");
	AddEdge(&G, "CE2");
	AddEdge(&G, "EB3");
	AddEdge(&G, "AE7");
	
	// Test 1 -- 9
	strcpy(path, "ABC");
	printf("The route distance of %s is %d. \n", path, CalculateRouteDistance(&G, path));

	// Test 2 -- 5
	strcpy(path, "AD");
	printf("The route distance of %s is %d. \n", path, CalculateRouteDistance(&G, path));

	// Test 3 -- 13
	strcpy(path, "ADC");
	printf("The route distance of %s is %d. \n", path, CalculateRouteDistance(&G, path));

	// Test 4 -- 22
	strcpy(path, "AEBCD");
	printf("The route distance of %s is %d. \n", path, CalculateRouteDistance(&G, path));

	// Test 5 -- "ABC9"
	strcpy(fromTo, "AC");
	printf("The shortest path of %s is %s. \n", fromTo, CalculateShortestRoute(&G, fromTo));

	// Test 6 -- "BCEB9"
	strcpy(fromTo, "BB");
	printf("The shortest path of %s is %s. \n", fromTo, CalculateShortestRoute(&G, fromTo));

	DestroyGraph(&G);
	
	return 0;
} 
