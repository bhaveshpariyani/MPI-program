//MPI code for scatter Gather

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <unistd.h>
int sum(int*,int);
void print(int*,int);
int main(int argc, char** argv) {

        MPI_Init(NULL, NULL);
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        int world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        int *temp =NULL;
        int array_size = 4;
        int ele_per_proc = array_size/world_size;
        if(world_rank==0){
                int *gather_subset_sum = malloc(sizeof(int)*world_size);
                int *compute_subset_sum = malloc(sizeof(int)* ele_per_proc);           
                int a[] = {10,20,60,80};

                MPI_Scatter(a, ele_per_proc, MPI_INT, compute_subset_sum,
ele_per_proc, MPI_INT, 0,MPI_COMM_WORLD);
                int local_sum = sum(compute_subset_sum,ele_per_proc);
                MPI_Gather(&local_sum, 1, MPI_INT, gather_subset_sum, 1, MPI_INT,
0,MPI_COMM_WORLD);
                int global_sum = sum(gather_subset_sum,ele_per_proc);

                printf("Total:%d\n",global_sum);
        }
        else{
                int *compute_subset_sum = malloc(sizeof(int)* ele_per_proc);
                MPI_Scatter(temp, 0,MPI_INT, compute_subset_sum, ele_per_proc,
MPI_INT, 0,MPI_COMM_WORLD);
                int local_sum = sum(compute_subset_sum,ele_per_proc);
                printf("Local Sum:%d Process:%d\n", local_sum,world_rank);
                MPI_Gather(&local_sum, 1, MPI_INT, temp, 0, MPI_INT, 0,MPI_COMM_WORLD);
        }
        MPI_Finalize();
}
int sum(int *arr,int size){
        int i,sum=0;
        for(i=0;i<size;i++){
                sum = sum + arr[i];
        }
        return sum;
}
void print(int *arr,int size){
        int i,sum=0;
        for(i=0;i<size;i++){
                printf("%d\n",arr[i]);
        }
}