/*********************************************************************************** 
* ---Program: 
* Finding the minimum value in a list of numbers using MPI

* ---Program Description:
* Each Process checks a part of the list and finds its minimum candidate.
* Next, process 0 calls MP_Reduce on the candidates to find the minimum value. 
* --- Developer: 
* Iman Faraji
**********************************************************************************/

#include <stdio.h>
#include <mpi.h>

#define __ARRAY_SIZE__ 100

int main(int argc , char **argv)
{
	int size, my_rank, my_min, min;
	int a[__ARRAY_SIZE__], i;
	
	MPI_Init(&argc , &argv);
	
	setvbuf(stdout, NULL,_IONBF, 0); // To reduce out-of-order printing
	
	MPI_Comm_rank(MPI_COMM_WORLD , &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD , &size);
	  
	for(i=0; i < __ARRAY_SIZE__; i++) // Initializing the array
		a[i] = i; 
	
	my_min = a[my_rank];
	
	for (i = my_rank + size; i < __ARRAY_SIZE__; i += size)
		if(my_min > a[i])
			my_min=a[i];

	printf("I am Process %d, and my Candidate for the min number is %d\n",my_rank,my_min);

	MPI_Reduce(&my_min, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD); //find the total minimum
	if (my_rank == 0) 
		printf("The min number is: %d\n", min);

	MPI_Finalize();
	return 0;
}
