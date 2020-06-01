#include<stdio.h>
#include<math.h>
#include "mpi.h"
int get_source(int[],int,int);
int nary = 2;
int main(int argc, char ** argv)
{
	double start,end;
	int world_rank,i;
	int size = 600000;
	int input[size];
	int world_size;
	int chunk_size;
	MPI_Status status;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int leftover = size%world_size;
	chunk_size = size/world_size;
	for(i=1;i<=size;i++)
		input[i-1] = 1;	
	int levels = (log(world_size)/log(nary))+1,nonodes = 0,j=0,k=0;
	for (int i = 0; i < levels; ++i)
	{
			nonodes += pow(nary,i);
	}
	int tree[nonodes];
	i = world_size;
	while(i!=0) {
		for(k = 0;k < world_size; k = k + i) {
			tree[j] = k;	j++;
		}
		i /= nary;
	}
	if(world_rank == 0)
	{	
		start = MPI_Wtime();
		int procers_count = world_size,to_send,new_size = size;
		int sentto[levels],j=0;
		int recv_sum,my_sum = 0;
		int ext,rem_proc;
		while(procers_count!=1) {
			to_send = world_rank;
			rem_proc = (procers_count/nary);
			new_size = new_size/nary;
			for(i = 0; i<nary-1; i++) {
				to_send += rem_proc;
				MPI_Send(&rem_proc, 1, MPI_INT, to_send, 0, MPI_COMM_WORLD);
				MPI_Send(&new_size, 1, MPI_INT, to_send, 0, MPI_COMM_WORLD);
				//printf("Rank: %d Sent_to:%d\n",world_rank,to_send);
				sentto[j] = to_send;
				j++;
			}
			procers_count /= nary;
		}
		for(i = j-1;i >= 0; i--) {
			MPI_Recv(&recv_sum, 1, MPI_INT, sentto[i], 0, MPI_COMM_WORLD, &status);	
			my_sum += recv_sum;
			//printf("Rank: %d RecievedFrom:%d\n",world_rank,sentto[i]);
		}
		int offset = new_size*world_rank;
		for (i = offset; i < (offset+chunk_size); ++i)
		{
				my_sum += input[i];
		}
		if(leftover!=0) {
			for (int i = (size-leftover); i < size; ++i)
			{
				my_sum += input[i];
			}
		}
		end = MPI_Wtime();
		printf("Time:%lf\n", end - start);
	}
	else
	{
		int recv_size,to_recv_from,procers_count,rem_proc;
		to_recv_from = tree[get_source(tree,world_rank,nonodes)];
		//printf("Rank: %d WRecived_From:%d\n",world_rank,to_recv_from);
		MPI_Recv(&procers_count, 1, MPI_INT, to_recv_from, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(&recv_size, 1, MPI_INT, to_recv_from, 0, MPI_COMM_WORLD, &status);
		//printf("Rank: %d Recived_From:%d\n",world_rank,to_recv_from);
		int to_send,new_size = recv_size,my_sum=0;
		int sentto[100],j=0;
		int recv_sum;
		while(procers_count!=1) {
			to_send = world_rank;
			rem_proc = (procers_count/nary);
			new_size = new_size/nary;
			for(i = 0; i<nary-1; i++) {
				to_send += rem_proc;
				MPI_Send(&rem_proc, 1, MPI_INT, to_send, 0, MPI_COMM_WORLD);
				MPI_Send(&new_size, 1, MPI_INT, to_send, 0, MPI_COMM_WORLD);
				//printf("Rank: %d Sent_to:%d\n",world_rank,to_send);
				sentto[j] = to_send;
				j++;
			}
			procers_count /= nary;
		}
		if(j!=0) {
			for(i = j-1;i >= 0; i--) {
				MPI_Recv(&recv_sum, 1, MPI_INT, sentto[i], 0, MPI_COMM_WORLD, &status);
				my_sum += recv_sum;
				//printf("Rank: %d Rsum: %d RecievedFrom:%d\n",world_rank,recv_sum,sentto[i]);
			}
		}
		int offset = new_size*world_rank;
		for (i = offset; i < (offset+chunk_size); ++i)
		{
				my_sum += input[i];
		}
		MPI_Send(&my_sum, 1, MPI_INT, to_recv_from, 0, MPI_COMM_WORLD);
		//printf("Rank: %d Sum: %d Sent_to:%d\n",world_rank,my_sum,to_recv_from);
	}
	MPI_Finalize();
	return 0;
}
int get_source(int tree[],int proc, int size) {
	int i,index;
	for (i = 0; i < size; ++i)
	{
		if(tree[i]==proc) {
			index = i;
			break;
		}
	}
	float tep = (float)(index-1);
	float te = tep/(float) nary;
	return (int) (ceil(te)-1.0);
}