
/*********************************************************************************** 
* ---Program: 
* Using Divide and Conquer Strategy to add a list of numbers - Parallel Mode

* ---Program Description:
* In this Program the whole list is divided between all processes 
* using divide and conquer method. On each level the problem size 
* - list of the array in this example - will be divided between two processes.
* When all of the processes get their own list, they will add up the elements
*  within their own list using divide and conquer strategy to calculate their
* own partial sum. Then the partial sums calculated by each process will be 
* added up using the summation technique 
* --- Developer: 
* Iman Faraji
**********************************************************************************/
#include <stdio.h>
#include <math.h>
#include "mpi.h"
#define __ARRAY_SIZE__ 1000
 
int a[__ARRAY_SIZE__];

//This function is used for dividing an array in to two arrays with the same size
void divide(int *x, int *y, int *z, int size)
{
	int i;
	for (i=0; i < size/2; i++)
	{
		*(y + i) = *(x + i);
		*(z + i) = *( x + i + size/2);
	}
	if (size % 2 != 0)
		*(z + i) = *(x + i + size/2); 
}
//sub-routine for adding elements of the array using divide and conquer
int add(int *x, int size)
{
	if (size == 1) 
		return *x;
	else if (size == 2) 
		return *x + *( x + 1);
	else
	{
		int add_part1, add_part2, *y, *z;
		y =(int *) malloc((size/2) * sizeof(*y));
		z = (int *) malloc(((size%2 == 0) ? size/2 : size/2+1) * sizeof(*z));
		divide(x, y , z, size);
		add_part1 = add(y, size/2);
		add_part2 = add(z, (size%2 == 0) ? size/2 : size/2+1);
		free(y);
		free(z);
		return add_part1 + add_part2;
	}
}

int main(int argc, char **argv)
{
  int size = __ARRAY_SIZE__, sum , i;
  for (i=0; i < size; i++) //Initializing the array
	  *(a+i) = i + 1; 

  int my_rank, a1[__ARRAY_SIZE__], tag = 100;
  MPI_Status *stat;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
 
  if (my_rank == 0)
  {
    int sum, part_sum, part_sum1, my_size=size, nxt_size;

    divide(a, a, a1, my_size); //divide the array into two arrays (array1 and array2)
    nxt_size = ((my_size % 2 == 0) ? my_size/2 : my_size/2 + 1); //size of array2
    my_size = my_size/2;//size of array1

    MPI_Send(&nxt_size, 1, MPI_INT, 4, tag, MPI_COMM_WORLD);  //Send the size of the message
    MPI_Send(a1, nxt_size, MPI_INT, 4, tag, MPI_COMM_WORLD); //Send the message
  
    divide(a, a, a1, my_size);
    nxt_size = ((my_size %2 == 0) ? my_size/2 : my_size/2 + 1);
    my_size = my_size / 2;

    MPI_Send(&nxt_size, 1, MPI_INT, 2, tag, MPI_COMM_WORLD); 
    MPI_Send(a1, nxt_size, MPI_INT, 2, tag, MPI_COMM_WORLD); 
  
    divide(a, a, a1, my_size);
    nxt_size = ((my_size % 2 == 0) ? my_size/2 : my_size/2 + 1);
    my_size = my_size/2;
 
    MPI_Send(&nxt_size, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);  
    MPI_Send(a1, nxt_size, MPI_INT, 1, tag, MPI_COMM_WORLD); 
 
    part_sum = add(a, my_size);//Calculating Partial sum using divide and conquer strategy

    MPI_Recv(&part_sum1, 1, MPI_INT, 1, tag, MPI_COMM_WORLD, stat);//Receiving remote calculated partial sum
    part_sum += part_sum1; //starts adding up partial sums

    MPI_Recv(&part_sum1, 1, MPI_INT, 2, tag, MPI_COMM_WORLD, stat);
    part_sum += part_sum1;

    MPI_Recv(&part_sum1, 1, MPI_INT, 4, tag, MPI_COMM_WORLD, stat);
    sum = part_sum + part_sum1; // Calculating final result
    
    printf("SUM of the array with %d elements is %d\n",__ARRAY_SIZE__,sum); // Printing final result
  }

  if (my_rank == 1)
  { 
    int part_sum, my_size;

    MPI_Recv(&my_size, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, stat);//Receiving the size of the message
    MPI_Recv(a, my_size, MPI_INT, 0, tag, MPI_COMM_WORLD, stat);//Receiving the message

    part_sum = add(a, my_size);
    MPI_Send(&part_sum, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
  } 

  if (my_rank == 2)
  {
	int part_sum, part_sum1, my_size, nxt_size; 

	MPI_Recv(&my_size, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, stat);
	MPI_Recv(a, my_size, MPI_INT, 0, tag, MPI_COMM_WORLD, stat);

	divide(a, a, a1, my_size);
	nxt_size = ((my_size %2 == 0) ? my_size/2 : my_size/2 + 1);
	my_size = my_size/2;

	MPI_Send(&nxt_size, 1, MPI_INT, 3, tag, MPI_COMM_WORLD);  
	MPI_Send(a1, nxt_size, MPI_INT, 3, tag, MPI_COMM_WORLD);

	part_sum = add(a, my_size);

	MPI_Recv(&part_sum1, 1, MPI_INT, 3, tag, MPI_COMM_WORLD, stat);
	part_sum += part_sum1;

	MPI_Send(&part_sum, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
  }
       
  if (my_rank == 3)
  { 
    int part_sum, my_size;

    MPI_Recv(&my_size, 1, MPI_INT, 2, tag, MPI_COMM_WORLD, stat);
    MPI_Recv(a, my_size, MPI_INT, 2, tag, MPI_COMM_WORLD, stat);

    part_sum = add(a, my_size);
    MPI_Send(&part_sum, 1, MPI_INT, 2, tag, MPI_COMM_WORLD);
  } 

  if (my_rank == 4)
  {
    int part_sum, part_sum1, my_size, nxt_size;

    MPI_Recv(&my_size, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, stat);
    MPI_Recv(a, my_size, MPI_INT, 0, tag, MPI_COMM_WORLD, stat);

    divide(a, a, a1, my_size);
    nxt_size = ((my_size%2==0) ? my_size/2 : my_size/2 + 1);
    my_size = my_size/2;

    MPI_Send(&nxt_size, 1, MPI_INT, 6, tag, MPI_COMM_WORLD);  
    MPI_Send(a1, nxt_size, MPI_INT, 6, tag,MPI_COMM_WORLD); 
  
    divide(a, a, a1, my_size);
    nxt_size = ((my_size%2==0) ? my_size/2 : my_size/2 + 1);
    my_size = my_size/2;
 
    MPI_Send(&nxt_size, 1, MPI_INT, 5, tag, MPI_COMM_WORLD); 
    MPI_Send(a1, nxt_size, MPI_INT, 5, tag,MPI_COMM_WORLD);  
    part_sum = add(a, my_size);

    MPI_Recv(&part_sum1, 1, MPI_INT, 5, tag, MPI_COMM_WORLD, stat);
    part_sum += part_sum1;

    MPI_Recv(&part_sum1, 1, MPI_INT, 6, tag,MPI_COMM_WORLD, stat);
    part_sum += part_sum1;

    MPI_Send(&part_sum, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
  }

  if (my_rank==5)
  {
    int part_sum,my_size;

    MPI_Recv(&my_size, 1, MPI_INT, 4, tag, MPI_COMM_WORLD, stat);
    MPI_Recv(a, my_size, MPI_INT ,4 , tag, MPI_COMM_WORLD, stat);

    part_sum = add(a, my_size);
    MPI_Send(&part_sum, 1, MPI_INT, 4, tag, MPI_COMM_WORLD);
  } 

  if (my_rank==6)
  {
	int part_sum, part_sum1, my_size, nxt_size; 

	MPI_Recv(&my_size, 1, MPI_INT, 4, tag,MPI_COMM_WORLD, stat);
	MPI_Recv(a,my_size,MPI_INT,4,tag,MPI_COMM_WORLD,stat);

	divide(a, a, a1, my_size);
	nxt_size = ((my_size %2 == 0) ? my_size/2 : my_size/2 + 1);
	my_size = my_size/2;

	MPI_Send(&nxt_size, 1, MPI_INT, 7, tag, MPI_COMM_WORLD); 
	MPI_Send(a1, nxt_size, MPI_INT, 7, tag, MPI_COMM_WORLD); 

	part_sum = add(a,my_size);

	MPI_Recv(&part_sum1, 1, MPI_INT, 7, tag, MPI_COMM_WORLD, stat);
	part_sum += part_sum1;

	MPI_Send(&part_sum, 1, MPI_INT, 4, tag, MPI_COMM_WORLD);
  }

  if (my_rank == 7)
  {
     int part_sum, my_size;

     MPI_Recv(&my_size, 1, MPI_INT, 6, tag, MPI_COMM_WORLD, stat);
     MPI_Recv(a, my_size, MPI_INT, 6, tag, MPI_COMM_WORLD, stat);

     part_sum = add(a, my_size);
     MPI_Send(&part_sum, 1, MPI_INT, 6, tag, MPI_COMM_WORLD);
  } 
   
  MPI_Finalize();
  return 0;
}
