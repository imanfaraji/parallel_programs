#include "MICRO_BENCHMARK.h"

static void rank2xy(int dim_x, int dim_y, int rank, int *x, int *y)
{
  *x = rank / dim_y;
  rank = rank - *x * dim_y;
  *y = rank;
}
static void xy2rank(int dim_x, int dim_y, int x, int y, int *rank)
{
  *rank = x * dim_y + y;
}

/*
 * This Function implements 2D Torus Communication
 * Pattern
 */
void _2DSTENCIL_func(void *src, void *dst, int my_rank, char *type, int msg_size, int dim_wght, int dim_x, int dim_y)
{
	int j, mx, my, tgt;
	rank2xy(dim_x, dim_y, my_rank, &mx, &my);
	
	if (bench_cycle != 0) //consider this benchmark to be cyclic -- no condition
	{	

	//x dimension
			  xy2rank(dim_x, dim_y, (mx+dim_x+1)%dim_x, my, &tgt);
		#ifdef __IMI_DEBUG_ALL__
		//	printf("my_rank = %d mx = %d, my = %d\n", my_rank, mx, my);
		#endif

		#ifdef __IMI_DEBUG_ALL__
			  printf("src %d --> dst: %d \n", my_rank,tgt);
		#endif
			  MPI_Isend(src, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			  req_count++;
			  MPI_Irecv(dst, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			  req_count++;

			  xy2rank(dim_x, dim_y, (mx+dim_x-1)%dim_x, my, &tgt);
		#ifdef __IMI_DEBUG_ALL__
			  printf("src %d --> dst: %d \n", my_rank,tgt);
		#endif
			  MPI_Isend(src, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			  req_count++;
			  MPI_Irecv(dst, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			  req_count++;

	//y dimension 
			  xy2rank(dim_x, dim_y, mx, (my+dim_y+1)%dim_y, &tgt);
		#ifdef __IMI_DEBUG_ALL__
			   printf("src %d --> dst: %d \n", my_rank, tgt);
		#endif
			   MPI_Isend(src, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			   req_count++;
			   MPI_Irecv(dst, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			   req_count++;

			   
				xy2rank(dim_x, dim_y, mx, (my+dim_y-1)%dim_y, &tgt);
		#ifdef __IMI_DEBUG_ALL__
				printf("src %d --> dst: %d \n", my_rank, tgt);
		#endif
				MPI_Isend(src, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				req_count++;
				MPI_Irecv(dst, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				req_count++;

		#ifdef __IMI_DEBUG_ALL__
			 printf("req_count = %d\n", req_count);
			 printf("3D rank %d req_count = %d \n", my_rank, req_count);
		#endif
	}
	else // non-cyclic 
	{
	// x dimesnion 
			if ((mx+1) < dim_x)
			{
			  xy2rank(dim_x, dim_y, (mx+dim_x+1)%dim_x, my, &tgt);
		#ifdef __IMI_DEBUG_ALL__
		//	printf("my_rank = %d mx = %d, my = %d\n", my_rank, mx, my);
		#endif

		#ifdef __IMI_DEBUG_ALL__
			  printf("src %d --> dst: %d \n", my_rank,tgt);
		#endif
			  MPI_Isend(src, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			  req_count++;
			  MPI_Irecv(dst, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			  req_count++;
			}
			if ((mx-1) >= 0)
			{
			  xy2rank(dim_x, dim_y, (mx+dim_x-1)%dim_x, my, &tgt);
		#ifdef __IMI_DEBUG_ALL__
			  printf("src %d --> dst: %d \n", my_rank,tgt);
		#endif
			  MPI_Isend(src, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			  req_count++;
			  MPI_Irecv(dst, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			  req_count++;
			}
	// y dimension 
			if ((my+1) < dim_y)
			{
			   xy2rank(dim_x, dim_y, mx, (my+dim_y+1)%dim_y, &tgt);
		#ifdef __IMI_DEBUG_ALL__
			   printf("src %d --> dst: %d \n", my_rank, tgt);
		#endif
			   MPI_Isend(src, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			   req_count++;
			   MPI_Irecv(dst, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			   req_count++;
			}

			if ((my-1) >= 0)
			{
				xy2rank(dim_x, dim_y, mx, (my+dim_y-1)%dim_y, &tgt);
		#ifdef __IMI_DEBUG_ALL__
				printf("src %d --> dst: %d \n", my_rank, tgt);
		#endif
				MPI_Isend(src, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				req_count++;
				MPI_Irecv(dst, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				req_count++;
			}
		#ifdef __IMI_DEBUG_ALL__
			 printf("req_count = %d\n", req_count);
			 printf("3D rank %d req_count = %d \n", my_rank, req_count);
		#endif	
		
	}
	 return;
}