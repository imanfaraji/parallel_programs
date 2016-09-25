/*********************************************************************************** 
* ---Program: 
* Measuring PI using MPI

* ---Program Description:
* In this program we calculate PI using Monte Carlo Method. 
* At first Process 0 broadcasts the number of intervals to the other processes, 
* then each process calculates a portion of the PI. 
* Finally, Process 0 gathers and adds up the partial sums by using MPI_Gather
* routine and prints the output.

* --- Developer: 
* Iman Faraji
**********************************************************************************/

#include <stdio.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char **argv)
{
	int my_rank, size, n, i;
	double sum, h, x, my_pi, pi;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	if(my_rank == 0)
	{
		printf("Enter the number of intervals: \n");
		scanf("%d",&n);
	}	
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	sum = 0.0;
	h = 1.0 / (double)n;
	for (i = my_rank + 1; i <= n; i += size)
	{
		x = h * ((double) i - 0.5);
		sum + = 4.0 / (1.0 + x * x);
	}
	my_pi = h * sum;
	MPI_Reduce(&my_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (my_rank == 0)
	{
		printf("PI is aproximately %20.16f\n", pi);
	}
	MPI_Finalize();
	return 0;
}