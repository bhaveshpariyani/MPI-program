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