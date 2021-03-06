#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void printInorder(int* arr,int start,int end) {
   if(start > end) {
      return;
   }
   printInorder(arr, start*2 + 1, end); 
   printf("%d\t", arr[start]); 
   printInorder(arr, start*2 + 2, end); 
}

void buildTree(int start,int end,int*,int,int,int);

int main() {
   int i,j,N = 512;
   int tree[N],*FinalTree=NULL;
   int turns,children = 1;
   clock_t  cpu_start = clock();
   FinalTree=(int*)malloc(N*children*sizeof(int));
   printf("Inorder Tree - \n");
   for(turns=0;turns<children;turns++) {
      int offset = turns*N;
      buildTree(0,N,tree,0,offset,N);
      for(i=turns*N,j=0;i<((turns+1)*N-1),j<N;i++,j++) {
         FinalTree[i]=tree[j];
      }
      printInorder(tree,0,N-1);
   }
   printf("\n\nChild Tree Generated by processes in array form -");
   for(i=0;i<N*children;i++) {
      if(i%N==0) {
         printf("\nChild %d - ",(i/N+1));
      }
      printf("%d\t",FinalTree[i]);
   }
   clock_t cpu_finish = clock();
   printf("\nTime elapsed in traversing is = %f seconds\n",((double)(cpu_finish - cpu_start)/CLOCKS_PER_SEC));
   return 0;
}

void buildTree(int start,int end,int* tree,int k,int offset,int N) {
   if(start<=end && k<=N-1){
      int mid = (end+start+1)/2;
      tree[k] = mid+offset;
      buildTree(start,mid-1,tree, k*2+1,offset,N);
      buildTree(mid+1,end,tree,k*2+2,offset,N);
   }
}
