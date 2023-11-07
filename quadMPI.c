#include<stdio.h>
#include<mpi.h>

double func(double);
double independentRankWork(int commSize, int myRank);
void integrationPTP(int commSize, int myRank);
void integrationCC(int commSize, int myRank);

int main(void){
	const int REPEAT = 5;

	double tStart;
	double tEnd;
	int myRank;
	int commSize;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &commSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	
	int count = 0;
	for(count; count < REPEAT; count++){
	
    		tStart = MPI_Wtime();
		integrationPTP(commSize, myRank);
		tEnd = MPI_Wtime();
	        printf("Time taken for ptp = %f milliseconds\n", 1000.0*(tEnd - tStart));	//print wallTimeTaken

	    	\\Add timings for integrationCC.
		integrationCC(commSize, myRank);
		
		

		
		printf("COUNT = %d\n", count);
		

	}

	MPI_Finalize();

}

void integrationPTP(int commSize, int myRank){
	double mySum = independentRankWork(commSize, myRank);

	if(myRank){
		MPI_Send(&mySum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	else{
		double totalSum = mySum;
		double recvSum;
		int i;
		for(i = 1; i < commSize; i++){
			MPI_Recv(&recvSum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			totalSum += recvSum;
		}
		printf("Final area is %f\n", totalSum);
	}

	MPI_Barrier(MPI_COMM_WORLD);
}

void integrationCC(int commSize, int myRank){
	/*Perform the same function as integrationPTP
	But use MPI_Reduce instead of point to point communications*/
}

double independentRankWork(int commSize, int myRank){
	/*Set up the number of quads*/
	const double a=0.0, b=200.0;
	const int  quads = 50000;
	double const width = (b-a) / (double) quads;
	
	int quadsPerRank = quads / commSize;

	if(myRank == commSize - 1){	//final rank leaving this here for ease
		quadsPerRank = quads - (quadsPerRank*myRank);
	}
	
	int startIter = myRank * quadsPerRank;
	int endIter = startIter + quadsPerRank - 1;
	
	double x, y;
    
	double sum;
	int i;

    for(i = startIter; i <= endIter; i++){
		x = a + i * width;
		y = x + width;
		sum += 0.5*width*(func(x) + func(y));
    }
	return sum;
}

double func (double x) {
  return pow(x,1.5)/3.1 - x/log(3.0);
}

