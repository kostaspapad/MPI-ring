#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>


int main( int argc, char **argv ) {
	int size;
	int rank;
	char message[ 100 ];
	MPI_Status status;
	int count;
	int tag = 1;

	/* Start MPI */
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );

	if( rank == 0 ) {		
		
		sprintf( message, "Hello from %d", rank );
		/*
		 * If this is rank 0 
		 * Send first message to process 1...
		 *
		 */
		MPI_Send( message, strlen( message ), MPI_CHAR, 1,
			tag, MPI_COMM_WORLD );
		/*
		 * ... after that receive the message
		 * from the last process.
		 *
		 */
		MPI_Recv( message, strlen( message ), MPI_CHAR, size-1,
		    tag, MPI_COMM_WORLD, &status );
		
		MPI_Get_count( &status, MPI_CHAR, &count );

		/* Append zero byte to terminate string */
		message[ count ] = 0;
		fprintf( stderr, "Process %d received: %s\n", rank, message );

		fflush( stdout );

	} else {

		MPI_Recv( message, 100, MPI_CHAR, rank-1,
			tag, MPI_COMM_WORLD, &status );

		/* Get the number of top level elements */
		MPI_Get_count( &status, MPI_CHAR, &count );

		/* Append zero byte to terminate string */
		message[ count ] = 0;
		fprintf( stderr, "Process %d received: %s\n", rank, message );

		fflush( stdout );

		/* Send message to next process in ring */
		sprintf( message, "Hello from %d", rank );
		
		MPI_Send( message, strlen( message ), MPI_CHAR, ( ( rank+1 ) % size ),
			tag, MPI_COMM_WORLD );
	}

	MPI_Finalize();
	return (0);
}
