// C / C++ program for Prim's MST for adjacency list representation of graph
 
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include<algorithm>
 
//a node in neighbourhood list

struct BrotherNode
{
    int vertex;
    double weight;
    struct BrotherNode* next;
};
 
// a neighbourhood list

struct BrotherList
{
    struct BrotherNode *head;  // pointer to head node of list
};
 
// the graph stores the array of brotherhood lists.

// and V is the number of vertices

struct Graph
{
    int V;
    struct BrotherList* array;
};
 
// create a new brother list node

struct BrotherNode* newBrotherNode(int vertex, double weight)
{
    struct BrotherNode* newNode =
            (struct BrotherNode*) malloc(sizeof(struct BrotherNode));
    newNode->vertex = vertex;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}
 
// make a graph of V vertices

struct Graph* makenewGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
 
    // making array of brotherhood lists.  length of array=V
    graph->array = (struct BrotherList*) malloc(V * sizeof(struct BrotherList));
 
    for (int i = 0; i < V; ++i) // set the head of every list as NULL
        graph->array[i].head = NULL;
 
    return graph;
}
 
// if you want to add a new edge to your graph

void makenew_edge(struct Graph* graph, int point1, int point2, double weight)
{
    // The edge is going between point 1 & point2. point 2 is added as brother of point 1
    struct BrotherNode* newNode = newBrotherNode(point2, weight);
    newNode->next = graph->array[point1].head;
    graph->array[point1].head = newNode;
 
    // similarliy point 1 is also brother of point 2
    newNode = newBrotherNode(point1, weight);
    newNode->next = graph->array[point2].head;
    graph->array[point2].head = newNode;
}
 
// structure of a node in heap

struct nodeofheap
{
    int  value;
    int key;
};
 
// making a min heap

struct MinHeap
{
    // at any general time, number of elements in the heap
    int total;
    int max;  // the maximum limit of heap
    int *location;     //in order to change the key value
    struct nodeofheap **array;
};
 
// making a fresh node of heap

struct nodeofheap* freshnodeofheap(int value, int key)
{
    struct nodeofheap* nodeofheap =(struct nodeofheap*) malloc(sizeof(struct nodeofheap));
    nodeofheap->value = value;
    nodeofheap->key = key;
    return nodeofheap;
}
 
// making a fresh heap 

struct MinHeap* makefreshheap(int max)
{
    struct MinHeap* minHeap =(struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->location = (int *)malloc(max * sizeof(int));
    minHeap->total = 0;
    minHeap->max = max;
    minHeap->array =(struct nodeofheap**) malloc(max * sizeof(struct nodeofheap*));
    return minHeap;
}
 
// helping function for Heapify, swaps two nodes

void swapnodeofheap(struct nodeofheap** a, struct nodeofheap** b)
{
    struct nodeofheap* temp = *a;
    *a = *b;
    *b = temp;
}
 
// A standard function to heapify at given nodeindex
// This function also updates locationition of nodes when they are swapped.
// locationition is needed for updatekey()

void Heapifyfunc(struct MinHeap* minHeap, int nodeindex)
{
    int minimum, left, right;
    minimum = nodeindex;
    left = 2 * nodeindex + 1;
    right = 2 * nodeindex + 2;
 
    if ((left < minHeap->total) &&(minHeap->array[left]->key < minHeap->array[minimum]->key) )
      minimum = left;
 
    if ((right < minHeap->total) &&(minHeap->array[right]->key < minHeap->array[minimum]->key) )
      minimum = right;
 
    if (minimum != nodeindex)
    {
        // in the following manner, nodes will be swapped
        nodeofheap *minimumNode = minHeap->array[minimum];
        nodeofheap *nodeindexNode = minHeap->array[nodeindex];
        minHeap->location[minimumNode->value] = nodeindex;
        minHeap->location[nodeindexNode->value] = minimum;
        swapnodeofheap(&minHeap->array[minimum], &minHeap->array[nodeindex]);
 
        Heapifyfunc(minHeap, minimum);
    }
}
 
// check if the heap is empty or not
int checkifempty(struct MinHeap* minHeap)
{
    return (minHeap->total == 0);
}
 
// find the mininum node

struct nodeofheap* getminNode(struct MinHeap* minHeap)
{
    if (checkifempty(minHeap)==1)
        return NULL;
    struct nodeofheap* root = minHeap->array[0];
    struct nodeofheap* lastNode = minHeap->array[minHeap->total - 1];
    minHeap->array[0] = lastNode;
    minHeap->location[root->value] = minHeap->total-1;
    minHeap->location[lastNode->value] = 0;
 
    // decrease the number of nodes in the heap
    --minHeap->total;
    Heapifyfunc(minHeap, 0);
 
    return root;
}
 
// updating key of any given vertex v

void updatekey(struct MinHeap* minHeap, int value, int key)
{
    // find index of v in the array
    int i = minHeap->location[value];
    minHeap->array[i]->key = key;
 
    // do until the heap is settled , if child is less than parent , swap them
    while (i && minHeap->array[i]->key < minHeap->array[(i - 1) / 2]->key)
    {
        minHeap->location[minHeap->array[i]->value] = (i-1)/2;
        minHeap->location[minHeap->array[(i-1)/2]->value] = i;
        swapnodeofheap(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}
 
// check if node is in min heap or not

int isInMinHeap(struct MinHeap *minHeap, int value)
{
   if (minHeap->location[value] < minHeap->total)
     return 1;
   return 0;
}
 
struct make_mstedge{
 int u,v; double w;
 };
 typedef struct make_mstedge mstedge;
 struct compare{
 bool operator()(const mstedge & lhs, const mstedge &rhs){return lhs.w < rhs.w;}
 };
 // the function that we will use finally to print our answer

void printMST(mstedge mst[], double sum, int n)
{
    printf("%.02lf\n",sum);
    for (int i = 1; i < n; ++i)
	   {
        printf("%d %d %.02lf\n",mst[i].u,mst[i].v, mst[i].w);
	 }
}
// applying the prims algorithm

void primalgo(struct Graph* graph)
{
    int V = graph->V;
    int parent[V];   // Array to store our minimum spanning tree
	double key[V];
	double sum=0;
 
    struct MinHeap* minHeap = makefreshheap(V);
 
    // except root , all vertices get the key as infinite
    for (int v = 1; v < V; ++v)
    {
        parent[v] = -1;
        key[v] = INT_MAX;
        minHeap->array[v] = freshnodeofheap(v, key[v]);
        minHeap->location[v] = v;
    }
 
    // root key is 0
    key[0] = 0;
    minHeap->array[0] = freshnodeofheap(0, key[0]);
    minHeap->location[0]   = 0;
 
    minHeap->total = V;
 
    while (!checkifempty(minHeap))
    {
        // get the vertex which has minimum key
        struct nodeofheap* nodeofheap = getminNode(minHeap);
        int u = nodeofheap->value;
 // check and update all neighbours of u
        struct BrotherNode* random = graph->array[u].head;
        while (random != NULL)
        {
            int v = random->vertex;
 
           //if the vertex v has not been considered, take and update it
            if (isInMinHeap(minHeap, v) && random->weight < key[v])
            {
                key[v] = random->weight;
                parent[v] = u;
                updatekey(minHeap, v, key[v]);
            }
            random = random->next;
        }
    }
	mstedge mst[V];
	for(int i=1;i<V;i++)
	{
	  mst[i].v = i;
	  mst[i].u = parent[i];
	  mst[i].w = key[i];
	  sum+=key[i];
	}
	std::sort(mst+1, mst+V,compare()); 
    // print edges of MST
    printMST(mst,sum,V);
}
 
// Driver program to test above functions

int main()
{
    // Let us create the graph given in above fugure
    int V;
    scanf("%d",&V);
    struct Graph* graph = makenewGraph(V);
   int u,v;double w;
   while (!feof(stdin) && scanf("%d %d %lf",&u,&v,&w)){ makenew_edge(graph, u, v, w);} 
    primalgo(graph);
 
    return 0;
}
