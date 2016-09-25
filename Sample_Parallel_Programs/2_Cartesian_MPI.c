/*********************************************************************************** 
* ---Program: 
* Building 3-D mesh using cartesian communicator and using its sub-communicator in a
* broadast call.

* ---Program Description:
* This program builds a 3-dimensional Cartesian commmunicator from MPI_COMM_WROLD.
* It then prints the topology information for each process. 
* It then builds sub-communicator on rows of the Cartesian communicator.
* A Broadcast is called on the row commnicators and the resuls are printed.

* --- Developer: 
* Iman Faraji
**********************************************************************************/



#include <stdio.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char **argv)
{
	int my_rank, p, q, my_grid_rank, grid_rank, row_test;
	int dim_sizes[3], wrap_around[3], coordinates[3], free_coords[3];
	int reorder = 0;
	MPI_Comm grid_comm, row_comm;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	q = pow(p, 1/3.0);
	dim_sizes[0] = dim_sizes[1] = dim_sizes[2] = q;
	wrap_around[0] = wrap_around[1] = wrap_around[2]=0;
	
	MPI_Cart_create(MPI_COMM_WORLD, 3, dim_sizes, wrap_around,reorder, &grid_comm);
	MPI_Comm_rank(grid_comm, &my_grid_rank);
	MPI_Cart_coords(grid_comm, my_grid_rank, 3, coordinates);
	MPI_Cart_rank(grid_comm, coordinates, &grid_rank);
	
	printf("Process %d my_grid_rank = %d, coords = (%d,%d,%d), grid_rank = %d\n",
	my_rank, my_grid_rank, coordinates[0], coordinates[1], coordinates[2], grid_rank);

	free_coords[0] = 0;
	free_coords[1] = 0;
	free_coords[2] = 1;
	
	MPI_Cart_sub(grid_comm, free_coords, &row_comm);
	
	if(coordinates[2]==0) 
		row_test = my_grid_rank;
	else
		row_test = -1;

	MPI_Bcast(&row_test, 1, MPI_INT, 0, row_comm);

	printf("Process %d coords = (%d,%d,%d), row_test=%d\n",
	my_rank, coordinates[0], coordinates[1], coordinates[2], row_test);

	MPI_Finalize();
	return 0;
}
