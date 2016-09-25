/*********************************************************************************** 
* ---Program: 
* Measuring PI using Posix Thread

* ---Program Description:
* In this program we calculate PI using Monte Carlo Method. 
* Parallel region distributes the work among threads.
* Each threads calls a routine to calculate the partial sum
* The partial sums are then addeded together using mutual exclusion. 

* --- Developer: 
* Iman Faraji
**********************************************************************************/
#include <stdio.h>
#include <math.h>
#include "pthread.h"
#define NUM_THREADS    8
pthread_mutex_t mutex_pi;
double pi;

struct thread_data_st
{
	int st_my_rank;
	int st_n;
	double st_my_pi;
};

struct thread_data_st thread_data_st_array[NUM_THREADS];

void *calc_pi_routine(void *routine_data)
{
	int n, my_rank, i;
	double x, h, sum;
	struct thread_data_st *r_data;
	r_data = (struct thread_data_st *) routine_data;
	n = r_data->st_n;
	my_rank = r_data->st_my_rank;
	h = 1.0 / (double)n;
	sum = 0.0;
	for (i = my_rank + 1; i <= n; i += NUM_THREADS)
	{
		x = h * ((double)i - 0.5);
		sum += 4.0 / (1.0 + x * x);
	}
	r_data->st_my_pi = h * sum;
	pthread_mutex_lock(&mutex_pi);
	pi += r_data->st_my_pi;
	pthread_mutex_unlock(&mutex_pi);
}

int main(int argc, char **argv)
{
  int i, j, n;
  pthread_t threads[NUM_THREADS];
  printf("Enter the number of intervals: \n");
  scanf("%d", &n);
  for(i = 0; i < NUM_THREADS; i++)
  {
    thread_data_st_array[i].st_my_rank = i;
    thread_data_st_array[i].st_n = n;
	//Each thread calculates part of Pi
    pthread_create(&threads[i], NULL, calc_pi_routine, (void *)&thread_data_st_array[i]);
  }
  //Join all of the spawnned threads
  for (j=0; j < NUM_THREADS; j++)
  {
     pthread_join(threads[j], NULL);
  }
  printf("PI is approximately %20.16f\n", pi);
}