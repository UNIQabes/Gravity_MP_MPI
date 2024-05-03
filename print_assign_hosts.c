#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "second.c"

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

	

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	char *procName = malloc(sizeof(char) * 300);
	int len=0;
	MPI_Get_processor_name(procName, &len);
	
	printf("%s:%d/%d\n",procName,rank,size);

	
	MPI_Finalize();
}