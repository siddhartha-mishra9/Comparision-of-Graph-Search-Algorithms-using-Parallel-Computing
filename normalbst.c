#include<stdio.h>
#include<stdlib.h>
#include<time.h>
typedef struct treeNode
{
int data;
struct treeNode *left;
struct treeNode *right;

}treeNode;

treeNode * Insert(treeNode *node,int data)
{
if(node==NULL)
{
treeNode *temp;
temp = (treeNode *)malloc(sizeof(treeNode));
temp -> data = data;
temp -> left = temp -> right = NULL;
return temp;
}

if(data >(node->data))
{
node->right = Insert(node->right,data);
}
else if(data < (node->data))
{
node->left = Insert(node->left,data);
}

printf(" %d \n",node->data);
return node;

}
treeNode * Find(treeNode *node, int data)
{
if(node==NULL)
{
return NULL;
}
if(data > node->data)
{
return Find(node->right,data);
}
else if(data < node->data)
{
return Find(node->left,data);
}
else
{
return node;
}
}

int main()
{
int item;
printf("Enter your Item number : ");
scanf("%d",&item);
clock_t t;
t = clock();
treeNode *root = NULL;
for(int i=0;i<1000;i++){
root = Insert(root, (rand()%100));
}
treeNode * temp;
temp = Find(root,item);
if(temp==NULL)
{
printf("Element not found\n");
}
else
{
printf("Element Found\n");
}
t = clock() - t;
double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

printf("sequential BST  took %f seconds to execute \n", time_taken);
}
