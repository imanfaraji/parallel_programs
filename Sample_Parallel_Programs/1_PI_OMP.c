/*********************************************************************************** 
* ---Program: 
* Measuring PI using OpenMP

* ---Program Description:
* In this program we calculate PI using Monte Carlo Method. 
* Parallel region distributes the work among threads.
* Critical section is used to add up partial sums.

* --- Developer: 
* Iman Faraji
**********************************************************************************/

#include <stdio.h>
#include "omp.h"

int main(int argc, char **argv)
{
  int nt, n, i, my_id;
  double sum, x, h, pi=0.0; 
  printf("Enter the number of intervals:\n");
  scanf("%d",&n);
  nt = omp_get_num_threads();
  h=1.0/(double)n;
#pragma omp parallel private(x,sum)
  {
    nt = omp_get_num_threads();
    sum = 0.0;
    my_id = omp_get_thread_num();
    for(i = my_id + 1; i <= n; i += nt)
    {
		x = h * ((double)i - 0.5);
		sum+ = 4.0 / (1.0 + x * x);
    }
#pragma omp critical 
    pi += sum * h;
  }
    printf("PI is approximately %20.16f\n",pi);
    return 0;
}