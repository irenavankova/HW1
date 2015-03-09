#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
//#include "util.h"

int main(int argc, char *argv[]) {

if (2 != argc){
	printf("Incorrect number of args");
return 0;
}

int T = atoi(argv[1]); //get number of iterations
printf("Number of iterations: %i\n", T);

int rank, size; //size is number of processors
int msg_send, msg_recv;

MPI_Status status;
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

printf("STARTING ITERATION\n");
int k = 0;
while ( k < T ) {
	//special case: processor 0
	if (rank == 0){
		//first iteration doesn't expect to receive anything
		if (k == 0){
			msg_send = rank;
			//send
			MPI_Send(&msg_send, 1, MPI_INT, rank+1, 998, MPI_COMM_WORLD);
			//report(rank,msg_send,rank+1);
		}
		else{
			//receive
			MPI_Recv(&msg_recv, 1, MPI_INT, size-1, 998, MPI_COMM_WORLD, &status);
			//add rank to message
			msg_send = msg_recv + rank;
			//send
			MPI_Send(&msg_send, 1, MPI_INT, rank+1, 998, MPI_COMM_WORLD);
			//report(rank,msg_send,rank+1);
		}
	}
	//special case: end
	else if (rank == size-1){
		//Last iteration doesn't send anything anywhere anymore
		if (k == T-1){
			//receive
			MPI_Recv(&msg_recv, 1, MPI_INT, rank-1, 998, MPI_COMM_WORLD, &status);
			//add rank to message
			msg_send = msg_recv + rank;
			printf("Finished final iteration #: %i, this is the result: %i\n", k+1,msg_send);
		}
		else{
			//receive
			MPI_Recv(&msg_recv, 1, MPI_INT, rank-1, 998, MPI_COMM_WORLD, &status);
			//add rank to message
			msg_send = msg_recv + rank;
			//printf("Finished iteration #: %i, this is the result: %i\n", k+1,msg_send);
			//send
			MPI_Send(&msg_send, 1, MPI_INT, 0, 998, MPI_COMM_WORLD);
			//report(rank,msg_send,0);
		}
	}
	//middle part of array
	else{
		//receive
		MPI_Recv(&msg_recv, 1, MPI_INT, rank-1, 998, MPI_COMM_WORLD, &status);
		//add rank to message
		msg_send = msg_recv + rank;
		//send right
		MPI_Send(&msg_send, 1, MPI_INT, rank+1, 998, MPI_COMM_WORLD);
		//report(rank,msg_send,rank+1);
	}
	k++;
}

//end iteration
printf("HAPPY END\n");

MPI_Finalize();

return 0;
}

void report(int sender, int msg, int receiver){
printf("I am processor # %i, and I am sending number %i to processor # %i\n",sender,msg,receiver);
}
