/*********************************************************************************** 
* ---Program: 
* Finding the minimum value in a list of numbers using Pthread

* ---Program Description:
* Each thread calls a routine to check a part of the array to find the minimum candidate.
* After that, each thread compares its own candidate with the possible minimum value. 
* Mutual Exclusion is used to avoid simultaneous comparisons done by different threads. 
* --- Developer: 
* Iman Faraji
**********************************************************************************/

#include <stdio.h>
#include "pthread.h"

#define __NUM_THREADS__ 4
#define __ARRAY_SIZE__ 100


int a[__ARRAY_SIZE__];
int g_index = 0; //global index
int min;
pthread_mutex_t mutex1, mutex2;

void *min_routine(void *nothing) 
{
	int l_index;
	do
	{
		pthread_mutex_lock(&mutex1);
		
		l_index = g_index;
		g_index++;
		
		pthread_mutex_unlock(&mutex1);
		
		if (l_index < __ARRAY_SIZE__) // to skip the last extra rotation
		{
			if(*(a + l_index) < min)
			{
				pthread_mutex_lock(&mutex2);
				//another thread may have already passed the comparison with the old min so we check it again
				if(*(a + l_index) < min) min = *(a + l_index); 
				pthread_mutex_unlock(&mutex2);   
			}
		}
	}while(l_index < __ARRAY_SIZE__);
}

int main(int argc, char **argv)
{
	pthread_t thread[__NUM_THREADS__];
	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);
	
	int i;
	for (i=0; i < __ARRAY_SIZE__; i++) 
		a[__ARRAY_SIZE__ - i -1] = i;
	
	min = a[0];
	
	for (i=0; i < __NUM_THREADS__; i++) 
	{
		pthread_create(&thread[i], NULL, min_routine, NULL);
	}
	
	for(i=0; i < __NUM_THREADS__; i++) 
		pthread_join(thread[i], NULL);
	
	printf("Min value between 100 numbers is %d\n",min);
	return 0;
}
