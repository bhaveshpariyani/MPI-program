/*
//MPI send Receive Program

#include<stdio.h>
#include "mpi.h"
//using namespace std;
int main(int argc, char ** argv)
{
	int world_rank,res,world_size,dst = 0,tag = 0,size=100,i;
	MPI_Status status;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int chunk_size = size/world_size;
	if(world_rank == 0)
	{	int input[size];
		for(i=1;i<=size;i++)
			input[i-1] = i;
		int interval = 100;
		int offset = chunk_size;
		for (i = 1; i < world_size; ++i)
		{
				MPI_Send(&input, size, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
		offset = 0;
		int sum = 0,temp;
		for (i = 0; i < (offset+chunk_size); ++i)
		{
				sum += input[i];
		}
		for (i = 1; i < world_size; ++i)
		{
				MPI_Recv(&temp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
				sum += temp;
		}
		printf("Total:%d\n", sum);
	}
	else
	{
		int offset = chunk_size * world_rank;
		int input[size];
		MPI_Recv(&input, 100, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		int sum = 0,temp;
		for (i = offset; i < (offset+chunk_size); ++i)
		{
				sum += input[i];
		}
		printf("Local Sum:%d Process:%d\n", sum,world_rank);
		MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}
*/

/*
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
*/

/*

// Sum using Broadcast

#include<stdio.h>
#include "mpi.h"
//using namespace std;
int main(int argc, char ** argv)
{
	int world_rank,res,world_size,dst = 0,tag = 0,size=100,i;
	MPI_Status status;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int chunk_size = size/world_size;
	if(world_rank == 0)
	{	int input[size];
		for(i=1;i<=size;i++)
			input[i-1] = i;
		int interval = 100;
		int offset = chunk_size;
	
		MPI_Bcast(input, size, MPI_INT, 0, MPI_COMM_WORLD);
		//MPI_Bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
		offset = 0;
		int sum = 0,temp;
		for (i = 0; i < (offset+chunk_size); ++i)
		{
				sum += input[i];
		}
		for (i = 1; i < world_size; ++i)
		{
				MPI_Recv(&temp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
				sum += temp;
		}
		printf("Total:%d\n", sum);
	}
	else
	{
		int offset = chunk_size * world_rank;
		int input[size];
		MPI_Bcast(input, size, MPI_INT, 0, MPI_COMM_WORLD);
		int sum = 0,temp;
		for (i = offset; i < (offset+chunk_size); ++i)
		{
				sum += input[i];
		}
		printf("Local Sum:%d Process:%d\n", sum,world_rank);
		MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
} 
*/