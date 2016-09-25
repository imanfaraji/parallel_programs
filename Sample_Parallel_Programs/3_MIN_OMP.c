/*********************************************************************************** 
* ---Program: 
* Finding the minimum value in a list of numbers using OpenMP

* ---Program Description:
* Pragma Parallel for is used to parallelize the list search 
* A critical section is used in updating the minimum value. 
* --- Developer: 
* Iman Faraji
**********************************************************************************/
#include <stdio.h>
#include "omp.h"

#define __ARRAY_SIZE__ 100

int main(int argc, char **argv)
{
	int my_min,min;
	int a[__ARRAY_SIZE__],i;
	
	for (i=0; i < __ARRAY_SIZE__; i++)
		a[__ARRAY_SIZE__ - 1 - i] = i;
	
	min = a[0];
	
#pragma omp parallel for shared (a,min) private(i)
	for (i=0;i<__ARRAY_SIZE__;i++)
		if (a[i]<min)
#pragma omp critical (min_compare)
			if (a[i]<min) min=a[i];
			
	printf("Minimum number is %d\n",min);
	return 0;
}
 
