#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>

void buildTree(int start,int end,int*,int,int,int);

void printInorder(int* arr,int start,int end) {
   if(start > end) {
      return; 
   }
   printInorder(arr, start*2 + 1, end); 
   printf("%d\t", arr[start]); 
   printInorder(arr, start*2 + 2, end); 
}

int main(int argc, char *argv[]) {
   int i,j,k,N=512,turns,procs;
   int tree[N];
   clock_t cpu_start=clock();
   printf("Inorder Child Tree Sequence followed by Child Tree Generated in array form- \n");
   #pragma omp parallel for firstprivate(tree,i,j,k) lastprivate(procs)
   for(turns=0;turns<omp_get_num_threads();turns++) {
      int offset=turns*N;
      procs=omp_get_num_threads();
      buildTree(0,N,tree,0,offset,N);
      #pragma omp critical 
      {
         printInorder(tree,0,N-1);
         printf("\n");
         for(k=0;k<N;k++) {
            printf("%d\t", tree[k]);
         }
         printf("\n\n");
      }
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
