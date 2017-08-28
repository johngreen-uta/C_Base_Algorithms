#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/*************************************************
* Discription: this program takes a file containing the locations of
* stars and sorts them based on distance from earth as the file is being
* read into the program.
* the program prints out the k closest stars to earth based on the 
* sorting algorithims insertion sort, heap sort, and quick sort.
* 
* Author: John Green
* Time: 10/9/2014
**************************************************/

// this does not work. X is a pointer to an array in this program
#define NELEMS(x) (sizeof(x) / sizeof(x[0]))

typedef struct Star{
	int x; // value on x-axis
	int y; // value on y-axis
	int z; // value on z-axis
} Star;

double Distance(Star a, Star b);
Star* SelectStarsBasedInsertSort(char* fileName, int k);
Star* SelectStarsBasedMinHeap(char* fileName, int k);
Star* SelectStarsBasedQuickSort(char* fileName, int k);
void PrintStar(Star a);
void InsertSort(Star* star_ls, int size);
void HeapSort(Star *star_ls, int size);
void ShiftDown(Star *a, int start, int end);
void QuickSort(Star* star_ls, int start, int end);
double CompareStars(Star A, Star B);

/*
* Calculate the euclidean distance between star a and star b. 
*/
// The earth's coordinate is (0,0,0).
double Distance(Star a, Star b)
{
	return pow(	pow( (b.x - a.x), 2) + 
				pow( (b.y - a.y), 2) + 
				pow( (b.z - a.z), 2), .5);
}

/*
* Select top k closest stars based on the insert sort. 
*/
Star* SelectStarsBasedInsertSort(char* fileName, int k)
{	
	FILE* file;
	int count = 0, flag = 0;
	char *line, buffer[1000];
	Star *set, *star;
	
	set = malloc(k*sizeof(Star)); // initialize the array
	
	if( (file = fopen(fileName, "r") ) == NULL ) // check if file exists
    {
		printf("failed to load file");
        exit(1);
    }
    // loop through file grabing each lin for proccesing
    while( fgets(buffer, sizeof(buffer), file) != NULL )
    {
		flag = 0;
		// grab and store the values for x, y, z
		star = malloc(sizeof(Star));
		line = strtok(buffer, "(,)\n");
		star->x = atoi(line);
		line = strtok(NULL, "(,)\n");
		star->y = atoi(line);
		line = strtok(NULL, "(,)\n");
		star->z = atoi(line);
		
		// check if array is full
		if(count < k-1) 
		{
			set[count] = *star;
		}
		// check if last member of array
		else if( count == k-1 ) 
		{
			set[count] = *star;
			// set flag for initial sort
			flag = 1;
		}
		// check if new star is less than largest star in the current array
		else if( CompareStars(*star,set[k-1]) < 0 ) 
		{
			// replace larges star and flag for sorting 
			set[k-1] = *star;
			flag = 1;
		}
		// check if the array needs to be sorted
		if ( flag == 1 )
		{
			InsertSort(set, k);
		}
		count++;
	}
	fclose(file);
	return set;
}

/*
* Select top k closest stars based on the min heap. 
*/
Star* SelectStarsBasedMinHeap(char* fileName, int k)
{
	FILE* file;
	int count = 0, flag = 0;
	char *line, buffer[1000];
	Star *set, *star;
	
	set = malloc(k*sizeof(Star)); // initialize the array
	
	if( (file = fopen(fileName, "r") ) == NULL ) // check if file exists
    {
		printf("failed to load file");
        exit(1);
    }
    // loop through file grabing each lin for proccesing
    while( fgets(buffer, sizeof(buffer), file) != NULL )
    {
		flag = 0;
		// grab and store the values for x, y, z
		star = malloc(sizeof(Star));
		line = strtok(buffer, "(,)\n");
		star->x = atoi(line);
		line = strtok(NULL, "(,)\n");
		star->y = atoi(line);
		line = strtok(NULL, "(,)\n");
		star->z = atoi(line);
		
		// check if array is full
		if(count < k-1) 
		{
			set[count] = *star;
		}
		// check if last member of array
		else if( count == k-1 ) 
		{
			set[count] = *star;
			// set flag for initial sort
			flag = 1;
		}
		// check if new star is less than largest star in the current array
		else if( CompareStars(*star,set[k-1]) < 0 ) 
		{
			// replace larges star and flag for sorting 
			set[k-1] = *star;
			flag = 1;
		}
		// check if the array needs to be sorted
		if ( flag == 1 )
		{
			HeapSort(set,k);
		}
		count++;
	}
	fclose(file);
	return set;
}

/*
* Select top k closest stars based on the quick sort. 
*/
Star* SelectStarsBasedQuickSort(char* fileName, int k)
{
	FILE* file;
	int count = 0, flag = 0;
	char *line, buffer[1000];
	Star *set, *star;
	
	set = malloc(k*sizeof(Star)); // initialize the array
	
	if( (file = fopen(fileName, "r") ) == NULL ) // check if file exists
    {
		printf("failed to load file");
        exit(1);
    }
    // loop through file grabing each lin for proccesing
    while( fgets(buffer, sizeof(buffer), file) != NULL )
    {
		flag = 0;
		// grab and store the values for x, y, z
		star = malloc(sizeof(Star));
		line = strtok(buffer, "(,)\n");
		star->x = atoi(line);
		line = strtok(NULL, "(,)\n");
		star->y = atoi(line);
		line = strtok(NULL, "(,)\n");
		star->z = atoi(line);
		
		// check if array is full
		if(count < k-1) 
		{
			set[count] = *star;
		}
		// check if last member of array
		else if( count == k-1 ) 
		{
			set[count] = *star;
			// set flag for initial sort
			flag = 1;
		}
		// check if new star is less than largest star in the current array
		else if( CompareStars(*star,set[k-1]) < 0 ) 
		{
			// replace larges star and flag for sorting 
			set[k-1] = *star;
			flag = 1;
		}
		// check if the array needs to be sorted
		if ( flag == 1 )
		{
			QuickSort(set, 0, k-1);
		}
		count++;
	}
	fclose(file);
	return set;
}

/*
 * takes an array and the size of the array
 * sorts the array in ascending order based on distance from earth
 */
void InsertSort(Star* star_ls, int size)
{
	double distance;
	int i, j;
	Star earth, temp_star;
	
	earth.x = 0; earth.y = 0; earth.z = 0;
	
	for (i = 1; i < size; i++) // loop through the array
    {
        distance = Distance(earth, star_ls[i]); // distance of item i from earth
        // loop backwards from index i 
        // checking if item j is closer to earth than item j-1
        for (j = i; j > 0 && distance < Distance(earth, star_ls[j-1]); j--) 
        {
			// swap item j and item j-1
            temp_star = star_ls[j];
            star_ls[j] = star_ls[j - 1];
            star_ls[j - 1] = temp_star;
        }
    }
}

/*
 * takes an array an the size of the array
 * loops throught the parent nodes building the heap
 * loops throught the array swaping the first and last nodes
 * then heapifying the array, placing the largest node at the end
 * decramenting the size of the unsorted array 
 */
void HeapSort(Star *star_ls, int size)
{
    int start, end;
    Star temp_star;
 
    // starts at the last parent node ( (size-2)/2 )
    // ends at the root of the tree 0
    // heapafies each parent node
    for (start = (size-2)/2; start >=0; start--) 
    {
        ShiftDown( star_ls, start, size);
    }
	// stars at last item and decraments the size fo the array as it is sorted
    for (end=size-1; end > 0; end--) 
    {
		// swaps first and last item of the unsorted array
		temp_star = star_ls[end];
		star_ls[end] = star_ls[0];
		star_ls[0] = temp_star;
		// heapafy so the last item in the array is sorted
        ShiftDown(star_ls, 0, end);
    }
}

/*
 * takes an array, starting index, ending index
 * heapafying the nodes as it woks down the tree
 */
void ShiftDown(Star *star_ls, int start, int end)
{
    int root = start;
    int child;
    Star temp_star; 
 
    while ( root*2+1 < end ) 
    {
        child = 2*root + 1;
        // check if left child is less than right child
        if ((child + 1 < end) && 
			CompareStars(star_ls[child],star_ls[child+1]) < 0) 
        {
            child += 1;
        }
        // check if greater child is greater than parrent
        if (CompareStars(star_ls[root], star_ls[child]) < 0) 
        {
			// swap parent and child
			temp_star = star_ls[child];
			star_ls[child] = star_ls[root];
			star_ls[root] = temp_star;
			// move down the tree
            root = child;
        }
        else
            return;
    }
}

/*
 * recursive, in-place sort
 * takes an array, a starting index, and an ending index
 * finds the proper index of the pivot and swaps the pivot to the index
 * recursively calls QuickSort on the unsorted portions of the array
 */
void QuickSort(Star* star_ls, int start, int end)
{
	int left, right, pivot;
	Star temp_star;
	
	if(start < end) // ckeck for 1 item in the list
	{
		left = start; // set left index
		right = end;  // set right index
		pivot = end;  // set pivot index
		
		while(left < right) // check for left right crossover
		{
			// if left item is less than pivot item, increase left index
			while( CompareStars(star_ls[left], star_ls[pivot]) <= 0 && 
				left < end )
			{
				left++;
			}
			// if right item is greater than pivot item, decrease right index
			while( CompareStars(star_ls[right], star_ls[pivot]) > 0 )
			{
				right--;
			}
			if(left < right) // if left and right indexs have crossed each other
			{
				// swap left and right items
				temp_star = star_ls[left];
				star_ls[left] = star_ls[right];
				star_ls[right] = temp_star;
			}
		}
		// swap pivot and right items
		// pivot item is now sorted
		temp_star = star_ls[pivot];
		star_ls[pivot] = star_ls[right];
		star_ls[right] = temp_star;
		// sort the unsorted portions left over
		QuickSort(star_ls, start, right-1);
		QuickSort(star_ls, right+1, end);
	}
}

/*
 * compare distance from earth of two stars
 * return: negative if A<B, posative if A>B, zero if A=B
 */
double CompareStars(Star A, Star B)
{
	Star earth;
	earth.x = 0; earth.y = 0; earth.z = 0;
	return Distance(earth, A) - Distance(earth, B);
}

/*
* Print the star coordinates.
*/
void PrintStar(Star a)
{
	printf("%d, %d, %d", a.x, a.y, a.z);
}

int main(int argc, char *argv[])
{
	// argv[1] is the proper argument, argv[0] gives the programe name
     char* fileName = argv[1]; 
     Star* stars;
     int i;
     int k = 20;
     
     printf("\nPrint top %d closest stars (Insert Select): \n", k);
     stars = SelectStarsBasedInsertSort(fileName, k);
     // NELEMS(stars) does not work. stars is a pointer
     for(i = 0; i < k; i++){ 
          PrintStar(stars[i]);
          printf("\n");   
     }
     
     printf("\nPrint top %d closest stars (Min Heap Select): \n", k);
     stars = SelectStarsBasedMinHeap(fileName, k);
     // NELEMS(stars) does not work. stars is a pointer
     for(i = 0; i < k; i++){
          PrintStar(stars[i]);
          printf("\n");   
     }     
     
     printf("\nPrint top %d closest stars (Quick Select): \n", k);
     stars = SelectStarsBasedQuickSort(fileName, k);
     // NELEMS(stars) does not work. stars is a pointer
     for(i = 0; i < k; i++){
          PrintStar(stars[i]);
          printf("%d\n",i);  
     } 
     return 0;
} 
