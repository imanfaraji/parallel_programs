/*********************************************************************************** 
* ---Program: 
* Parallelelizing Mandelbrot program using Dynamic Assignment in MPI

* ---Program Description:
* In this part the sequential Mandelbrot calculation program is parallelized using 
* dynamic task assignment. The master process (Process 0) each time sends one row to
* the other processes and then gets the result related to that row.
* Once the slave process finishes its task it will ask for the new remained row. 
* This process continues until there is no row left to be sent by the master process. 
* At this point the master process generates Mandelbrot output file.
* This will be used later by the Mandelbrot-display program to generate the image    
* --- Developer: 
* Iman Faraji
**********************************************************************************/

#include <stdio.h>
//IMAN****************************
#include "sys/time.h"
#include "mpi.h"
//********************************
#define     X_RESN  800       /* x resolution */
#define     Y_RESN  800       /* y resolution */

typedef struct complextype
{
    float real, imag;
} Compl;

//IMAN ADDED   This routine returns the elapsed time between the start and finish time
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return (diff<0);
}
//IMAN ENDS
int main (int argc, char **argv)
{

	//IMAN ADDED
	struct timeval tvBegin, tvEnd, tvDiff;
	gettimeofday(&tvBegin, NULL);
	int my_rank, my_row, size, terminator_tag = 99, data_tag=100, source_tag=101, result_tag=102, c1=1;
	MPI_Status stat;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int buf[(2 * Y_RESN) +1]; 
	//IMAN ENDS

	/* Mandlebrot variables */
	int i,j, k[X_RESN][Y_RESN];
	Compl   z, c;
	float   lengthsq, temp;
	   
	/* Calculate and save points into array */
	//IMAN MODIFIED
	//Process zero is a master processor which divides the calculation between other processes and gather their results
	if (my_rank == 0)
	{   int c0 = 1;
		int row = 0,count = 0; 
		for (i = 1; i < size; i++, row++)
		{
			MPI_Send(&row, 1, MPI_INT, i, data_tag, MPI_COMM_WORLD);
			count++;
			row++; 
		}
		do 
		{
			//We just need the coordinates and the results we don't need which process is sending the message
			MPI_Recv(buf, (2 * Y_RESN) + 1, MPI_INT, MPI_ANY_SOURCE, result_tag, MPI_COMM_WORLD, &stat);
			for (c0=1; c0 < 2 * Y_RESN+1;) 
				k[buf[0]][buf[++c0]]=buf[++c0];
			
			count--;
			if ( row < X_RESN)
			{
				//send the next row to who we just received from
				MPI_Send(&row, 1, MPI_INT, stat.MPI_SOURCE, data_tag, MPI_COMM_WORLD); 
				row++;
				count++;
			}
			else MPI_Send(&row, 1, MPI_INT, stat.MPI_SOURCE, terminator_tag, MPI_COMM_WORLD);
		   
		} while (count>0);

		/* save array of points out to file */
		FILE * output;
		output = fopen("mandelbrot_data.txt","w+");
		if(output == NULL) 
		{
			printf("error opening file\n");
			return -1;
		}

		for(i=0; i < X_RESN; i++) 
			for(j=0; j < Y_RESN; j++)
			fprintf(output,"%d\n",k[i][j]);

		fclose(output);

		gettimeofday(&tvEnd, NULL);
		timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
		printf("elapsed time is %ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
	}
	else 
	{ 
		//SLAVE Part ******************************************************

		MPI_Recv(&my_row, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		while (stat.MPI_TAG==data_tag)
		{ buf[0]=my_row;c1=1;
		 for(j=0; j < Y_RESN; j++)
		{
			z.real = z.imag = 0.0;
			c.real = ((float) j - 400.0)/200.0;               /* scale factors for 800 x 800 window */
			c.imag = ((float) my_row - 400.0)/200.0;
			k[my_row][j] = 0;

			do  
			{  /* iterate for pixel color */
			   temp = z.real * z.real - z.imag * z.imag + c.real;
			   z.imag = 2.0 * z.real * z.imag + c.imag;
			   z.real = temp;
			   lengthsq = z.real * z.real + z.imag * z.imag;
			   k[my_row][j]++;
			 } while (lengthsq < 4.0 && k[my_row][j] < 100);

			buf[++c1] = j;
			buf[++c1] = k[my_row][j];
		}
			  
			MPI_Send(buf, (2* Y_RESN)+ 1, MPI_INT, 0, result_tag, MPI_COMM_WORLD);	
			//Send the Calculated results along with the coordinates
			MPI_Recv(&my_row, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		}
	}  
	 //IMAN ENDS	
	MPI_Finalize();
	return 0;
}

