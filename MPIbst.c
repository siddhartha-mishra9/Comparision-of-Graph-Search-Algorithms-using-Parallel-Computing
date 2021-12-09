#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define ID 0
#define VALUE 1 
#define LEFT 2
#define RIGHT 3
#define rows 100

typedef struct
{
    int id;
    int Value;
}BTree;

//*
This function is used to allocate the node value and updating the left and right value of the node to find if the node have any extra node to its right or to its left
*// 

void locate(int index, int* node_array, BTree * node)
{
    node_array[index * 4 + ID] = node->id;
    node_array[index * 4 + VALUE] = node->Value;
    node_array[index * 4 + LEFT] = -1;
    node_array[index * 4 + RIGHT] = -1;
}

//*
InsertNode Will take the no of rows, tree as structure, integer node array

It checks 
if node value > value of node array at root * 4 + VALUE
If the right value of the node array is -1 then change the right of that node and allocate the value
	Else recursively call the insert node 
Else
If the left value of the node array is -1 then change the right of that node and allocate the value
	Else recursively call the insert node 

*//
void InsertNode(int root, BTree * node, int index, int * node_array)
{
    if (node->Value > node_array[root * 4 + VALUE])
    {
        if (node_array[root * 4 + RIGHT] == -1)
        {
            node_array[root * 4 + RIGHT] = index;
            locate(index, node_array, node);
        }
        else
        {
            InsertNode(node_array[root * 4 + RIGHT], node, index, node_array);
        }
    }   
    else
    {
        if (node_array[root * 4 + LEFT] == -1)
        {
            node_array[root * 4 + LEFT] = index;
            locate(index, node_array, node);
        }
        else
        {
            InsertNode(node_array[root * 4 + LEFT], node, index, node_array);
        }
    }
}

void  CreateTree(int size, int * node_array)
{
    int i;
    BTree * tree = (BTree *)malloc(sizeof(BTree));
    tree->id = rand() % 10000;
    tree->Value = rand() % 40 + 10;
    locate(0, node_array, tree);
    for (i = 1; i < size; i++)
    {
        BTree * node = (BTree *)malloc(sizeof(BTree));
        node->id = rand() % size;
        node->Value = rand() % 60 + 10;
        InsertNode(0, node, i, node_array);
        printf("id - %d, Value - %d\n",node->id,node->Value);
    }

}
//*
It takes the search value, the node array and the index of start node
Check if the value of the node array at index index * 4 + VALUE == search value
	Print 
If the left value of the node array is -1 then recursively call the search function 
Same as for the right value of the node array
*//
void search(int Value, int * node_array, int index)
{
    if (node_array[index * 4 + VALUE] == Value)
    {
        printf("ID = %d, VALUE= %d\n", node_array[index * 4 + ID], node_array[index * 4 + VALUE]);
    }
    if (node_array[index * 4 + LEFT] != -1)
    {
        search(Value, node_array, node_array[index * 4 + LEFT]);
    }
    if (node_array[index * 4 + RIGHT] != -1)
    {
        search(Value, node_array, node_array[index * 4 + RIGHT]);
    }

}

//*
It will check which value of node array == search value
*//
void check(int Value, int * node_array, int index)
{
    if (node_array[index * 4 + VALUE] == Value)
    {
        printf("ID = %d, VALUE= %d\n", node_array[index * 4 + ID], node_array[index * 4 + VALUE]);
    }
}


//*
Check if the work is == no of processor then end the execution of the function
Else
	Reduce the num
Call the check function with search value, nodearray and worklist of num index as parameter 
Check if left not equal to -1 
	Then insert left value to the worklist and increment the num value
Check if right not equal to -1 
	Then insert right value to the worklist and increment the num value
Recursively call get_more_work to search in other nodes
*//

void get_more_work(int * work_list, int * num, int numP, int Value, int * node_array)
{
    if ((*num) == numP)
    {
        return;
    }
    else
    {
        (*num)--;
        int root = work_list[*num];
        check(Value, node_array, root);
        int left = node_array[root * 4 + LEFT];
        int right = node_array[root * 4 + LEFT];
        if (left != -1)
        {
            work_list[*num] = left;
            (*num)++;
        }
        if (right != -1)
        {
            work_list[*num] = right;
            (*num)++;
        }
        get_more_work(work_list, num, numP, Value, node_array);
    }
}

void main(int argc, char **argv)
{

    int taskid, numtasks;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    int tree[rows][4]; // 0 :ID, 1 :Value, 2 :left , 3 : right
    
    if (!taskid)
    {
        CreateTree(rows, (int *)tree); //creating a tree with random node values
    }

    int trValuetVALUE =14;

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(tree, rows * 4, MPI_INT, 0, MPI_COMM_WORLD);
    int search_item;

    printf("\nEnter your Search Value : ");
    scanf("%d",&search_item); //taking input for the search item 
    printf("\n");
    
    double time;
    time = -MPI_Wtime();
    int* work_list = (int*)malloc(sizeof(int)* numtasks);

    if (!taskid) //if tasked==0 then get_more_work
    {
        int work = 1;
        work_list[0] = 0;
        get_more_work(work_list, &work, numtasks, trValuetVALUE, (int *)tree);
    }

    int begin_node;
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatter(work_list, 1, MPI_INT, &begin_node, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    
    search(search_item, (int *)tree, begin_node);
    MPI_Barrier(MPI_COMM_WORLD);
    time += MPI_Wtime();

    if (!taskid)
        printf("Time:%lf, numtasks:%d\n", time, numtasks);
    free(work_list);
    MPI_Finalize();

}
