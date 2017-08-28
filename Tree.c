#include<stdio.h>
#include<stdlib.h>
#include <string.h>

/*************************************************
* Discription: binary tree navigation
* 
* Author: John Green
* Time: 11/8/2014
**************************************************/ 

#define LEN 100

typedef struct TreeNode{
	char data;		  // data
	struct TreeNode* lChild; // left child
	struct TreeNode* rChild; // right child
} TreeNode;

TreeNode* BuildTree(char* treeArray, int*i, TreeNode* curTreeNode);
void PrintPreOrde(TreeNode* curTreeNode);
char* Path(char ch, TreeNode* root);
char FindLowestCommonAncestor(char a, char b, TreeNode* root); 
char FindKLowestCommonAncestor(char* nodeList, TreeNode* root);

// append char to end of string
char* stradd(const char* a, const char* b){
    size_t len = strlen(a) + strlen(b);
    char *ret = (char*)malloc(len * sizeof(char) + 1);
    *ret = '\0';
    return strcat(strcat(ret, a) ,b);
}

/*
* Find the path between root and the input node ch.  
*/ 
char* Path(char ch, TreeNode* root)
{
	char *path = NULL;
	char *lpath= NULL, *rpath= NULL;
	// traverse the path	
	if(root->data == ch)
	{
		path = &(root->data);
	}
	if(root->lChild != NULL)
	{
		lpath = Path(ch,root->lChild);
	}
	if(root->rChild != NULL)
	{
		rpath = Path(ch,root->rChild);
	}
	// create path string
	if(path != NULL)
	{
		return path;
	}
	else if(lpath != NULL)
	{
		return stradd(&(root->data),lpath);
	}
	else if(rpath != NULL)
	{
		return stradd(&(root->data),rpath);
	}
	return NULL;
}

/*
* Find lowest common ancestor of node a and b in a 
* binary tree root.
*/
char FindLowestCommonAncestor(char a, char b, TreeNode* root) 
{
	int pos = 0;
	char *apath, *bpath;
	// get paths
	apath = Path(a,root);
	bpath = Path(b,root);
	// check if same
	if(a == b)
		return a;
	// find common ancestor
	while(apath[pos] == bpath[pos])
	{
		pos++;
	}
	return apath[pos-1];
}

/*
* Find lowest common ancestor of nodes in array nodeList 
* from a binary tree root. 
*/
char FindKLowestCommonAncestor(char* nodeList, TreeNode* root)
{
	char **set;
	int i, pos = 0;
	
	set = malloc(strlen(nodeList)*sizeof(char*));
	// create array of paths
	for(i = 0; i < strlen(nodeList); i++)
	{
		set[i] = Path(nodeList[i], root);
	}
	while(1)
	{
		// check for common ancestor
		for(i = 1; i < strlen(nodeList); i++)
		{
			if(set[0][pos] != set[i][pos])
				return set[0][pos-1];
		}
		pos++;
	}
	
	return root->data;
}

TreeNode* BuildTree(char* treeArray, int* pos, TreeNode* curTreeNode){	
	if(treeArray[*pos]=='#') {
		curTreeNode = NULL;
		*pos = *pos + 1;
	}
    else{
		if(!(curTreeNode = (TreeNode *)malloc(sizeof(TreeNode))))
			printf("Error!");

		curTreeNode->data=treeArray[*pos];
		*pos = *pos + 1;

		curTreeNode->lChild=BuildTree(treeArray, pos, curTreeNode->lChild);
		curTreeNode->rChild=BuildTree(treeArray, pos, curTreeNode->rChild);
    }

    return curTreeNode;
}


void PrintPreOrde(TreeNode* curTreeNode)
{
 if(curTreeNode)
 {
  printf("%c  ",curTreeNode->data);
  PrintPreOrde(curTreeNode->lChild);
  PrintPreOrde(curTreeNode->rChild);
 }
}


void main()
{
	TreeNode* root = NULL;

	char treeArray[] = {'3', '5', '6', '#', '#', '2', '7', '#', '#', '4', 
		'#', '#', '1', '0', '#', '#', '8', '#', '#' }; 	
	char nodeList[LEN];
	int i = 0;
	char firstNode;
	char secondNode;

	root = BuildTree(treeArray, &i, root);
	PrintPreOrde(root);

	// test case 1. -- 2
	firstNode = '7';
	secondNode = '4';
	printf("%c and %c's lowest common ancestor is %c. \n", 
		firstNode, secondNode, FindLowestCommonAncestor(firstNode, secondNode, root));

	// test case 2. -- 5
	firstNode = '6';
	secondNode = '4';
	printf("%c and %c's lowest common ancestor is %c. \n", 
		firstNode, secondNode, FindLowestCommonAncestor(firstNode, secondNode, root));
	
	// test case 3. -- 3
	firstNode = '3';
	secondNode = '8';
	printf("%c and %c's lowest common ancestor is %c. \n", 
		firstNode, secondNode, FindLowestCommonAncestor(firstNode, secondNode, root));

	// test case 4. -- 4
	firstNode = '4';
	secondNode = '4';
	printf("%c and %c's lowest common ancestor is %c. \n", 
		firstNode, secondNode, FindLowestCommonAncestor(firstNode, secondNode, root));

	// test case 5. -- 5
	strcpy(nodeList, "6574");
	printf("%s's lowest common ancestor is %c. \n", nodeList, FindKLowestCommonAncestor(nodeList, root));

	// test case 6. -- 3
	strcpy(nodeList, "640");
	printf("%s's lowest common ancestor is %c. \n", nodeList, FindKLowestCommonAncestor(nodeList, root));

} 
