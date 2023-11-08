#include <mpi.h>
#include <stdio.h>

#define MAXSIZE 4			/* change the size of matrices as required*/

int X[MAXSIZE][MAXSIZE], Y[MAXSIZE][MAXSIZE], Z[MAXSIZE][MAXSIZE];

void fill_matrix(int mat[MAXSIZE][MAXSIZE]){
	srand(time(NULL));
	/*TASK: Write code to fill the values in mat by making it equal to rand()%10*/
	int i,j;
	for(i=0; i < MAXSIZE; i++){
		for(j = 0; j < MAXSIZE; j++){
			mat[i][j] = rand() % 10;
		}
	}
}

void print_matrix(int mat[MAXSIZE][MAXSIZE]){
	int i, j;
	for(i = 0; i < MAXSIZE; i++){
		for(j = 0; j < MAXSIZE; j++){
			printf("%d " , mat[i][j]);
		}
		printf("\n");
	}
}


int main(int argc, char *argv[]){
	int my_rank, comm_size, from, to;
	// Init comm_size at the beginer, to make sure we can use the right comm_size as a condition
	MPI_Init (&argc, &argv);
  	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	// Judge if the MAXSIZE can be devided by comm_size
	if(MAXSIZE % comm_size != 0){
		exit(-1);
	}else{
		printf("Total Maxsize of the matrix: %d\nComm_size: %d\n",MAXSIZE,comm_size);
	}

  	int part_size = MAXSIZE*MAXSIZE/comm_size;
  	int localX[part_size][MAXSIZE], localY[part_size][MAXSIZE], localZ[part_size][MAXSIZE];
  	int i, j, k;
  	int root = 0;
  
	
	/*TASK: If the process is the root rank, fill both X and Y matrices*/
	if(my_rank == 0){
		fill_matrix(X);
		fill_matrix(Y);
		
		printf("The original matrix X:\n");
		print_matrix(X);
		printf("The original matrix Y:\n");
		print_matrix(Y);
	}
	
	from = my_rank * MAXSIZE/comm_size;
	to = (my_rank+comm_size) * MAXSIZE/comm_size;

	/*Root process sends the whole of Y to every process.*/
	MPI_Bcast (Y, MAXSIZE*MAXSIZE, MPI_INT, root, MPI_COMM_WORLD);
  
	/*Root process distributes parts of X to different processes*/
	MPI_Scatter (X, part_size, MPI_INT, localX, part_size, MPI_INT, root, MPI_COMM_WORLD);

  	for (i=from; i<to; i++){
	    	for (j=0; j<MAXSIZE; j++) {
	      		localZ[i][j] = 0;
	      		for (k=0; k<MAXSIZE; k++){
	        		localZ[i][j] += localX[i][k] * Y[k][j];
	    		}
		}
	}

	/*TASK: Use MPI_Gather to pull all localZ from each process and store it in Z at the root process*/
	MPI_Gather(localZ, part_size, MPI_INT, Z, part_size, MPI_INT, root, MPI_COMM_WORLD);
	
  	/*Task: If root print mat Z*/
	if(my_rank == 0){
		printf("The final matrix Z:\n");
		print_matrix(Z);
	}
	
	MPI_Finalize();
  	return 0;
}
