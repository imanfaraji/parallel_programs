#include "MICRO_BENCHMARK.h"
#include "cuda.h"

static void rank2xyz(int dim_x, int dim_y, int dim_z, int rank, int *x, int *y, int *z)
{
  *x = rank / (dim_y * dim_z);
  *y = (rank - (*x * dim_y * dim_z)) / dim_z;
  *z = rank - (*x * dim_y * dim_z) - (*y * dim_z);
}
static void xyz2rank(int dim_x, int dim_y, int dim_z, int x, int y, int z, int *rank)
{
  *rank = x * dim_y * dim_z + y * dim_z + z;
}

/*
 * This Function implements 3D Torus Communication
 * Pattern
 */
void _3DTORUS_func(void *src, void *dst, int my_rank, char *type, int msg_size, int dim_wght, int dim_x, int dim_y, int dim_z)
{
	int j, mx, my, mz, tgt;
	rank2xyz(dim_x, dim_y,dim_z, my_rank, &mx, &my, &mz);
	
	if (bench_cycle != 0) //consider the benchmark to be cyclic -- no condition
	{
		//in x dimension:
		//1st
				xyz2rank(dim_x, dim_y, dim_z, ((mx+1)%dim_x), my, mz, &tgt);
			#ifdef __IMI_DEBUG_NOW__
				  printf("src %d --> dst: %d \n", my_rank,tgt);
			#endif

				  MPI_Isend(src, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
				  MPI_Irecv(dst, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
		//2nd
				xyz2rank(dim_x, dim_y, dim_z, ( (mx-1 + dim_x) % dim_x ), my, mz, &tgt);
			#ifdef __IMI_DEBUG_NOW__
				  printf("src %d --> dst: %d \n", my_rank,tgt);
			#endif

				  MPI_Isend(src, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
				  MPI_Irecv(dst, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;


			//in y dimension:
			//1st
			  xyz2rank(dim_x, dim_y, dim_z, mx, ((my+1)%dim_y), mz, &tgt);
		#ifdef __IMI_DEBUG_NOW__
			  printf("src %d --> dst: %d \n", my_rank,tgt);
		#endif
			  MPI_Isend(src, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			  req_count++;
			  MPI_Irecv(dst, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			  req_count++;
			//2nd
				
              xyz2rank(dim_x, dim_y, dim_z, mx, ( (my-1 + dim_y) % dim_y ), mz, &tgt);
			#ifdef __IMI_DEBUG_NOW__
				  printf("src %d --> dst: %d \n", my_rank,tgt);
			#endif
				  MPI_Isend(src, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
				  MPI_Irecv(dst, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
				
                  // in z dimension:
				//1st
				xyz2rank(dim_x, dim_y, dim_z, mx, my, ((mz+1)%dim_z), &tgt);
			#ifdef __IMI_DEBUG_NOW__
				  printf("src %d --> dst: %d \n", my_rank,tgt);
			#endif
				  MPI_Isend(src, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
				  MPI_Irecv(dst, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
			//2nd

				xyz2rank(dim_x, dim_y, dim_z, mx, my, ( (mz-1 + dim_z) % dim_z ), &tgt);
			#ifdef __IMI_DEBUG_NOW__
				  printf("src %d --> dst: %d \n", my_rank,tgt);
				  fflush(stdout);
			#endif
				  MPI_Isend(src, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
				  MPI_Irecv(dst, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
		#ifdef __IMI_DEBUG_ALL__
			  printf("2D rank %d req_count = %d \n", my_rank, req_count);
		#endif

	}
	else
	{
		//in x dimension:
		//1st
		if ((mx+1) < dim_x)
		{
				xyz2rank(dim_x, dim_y, dim_z, ((mx+1)%dim_x), my, mz, &tgt);
			#ifdef __IMI_DEBUG_NOW__
				  printf("src %d --> dst: %d \n", my_rank,tgt);
			#endif

				  MPI_Isend(src, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
				  MPI_Irecv(dst, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;		
		}
		
		
		
		//2nd
		if ((mx-1) >= 0)
		{
				xyz2rank(dim_x, dim_y, dim_z, ( (mx-1 + dim_x) % dim_x ), my, mz, &tgt);
			#ifdef __IMI_DEBUG_NOW__
				  printf("src %d --> dst: %d \n", my_rank,tgt);
			#endif
				  MPI_Isend(src, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
				  MPI_Irecv(dst, dim_wght * msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
		}

		if ((my+1) < dim_y)
		{
			//in y dimension:
			//1st
			  xyz2rank(dim_x, dim_y, dim_z, mx, ((my+1)%dim_y), mz, &tgt);
		#ifdef __IMI_DEBUG_NOW__
			  printf("src %d --> dst: %d \n", my_rank,tgt);
		#endif
			  MPI_Isend(src, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			  req_count++;
			  MPI_Irecv(dst, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
			  req_count++;
			//2nd
		}
		if ((my-1) >= 0)
		{
				xyz2rank(dim_x, dim_y, dim_z, mx, ( (my-1 + dim_y) % dim_y ), mz, &tgt);
			#ifdef __IMI_DEBUG_NOW__
				  printf("src %d --> dst: %d \n", my_rank,tgt);
			#endif
				  MPI_Isend(src, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
				  MPI_Irecv(dst, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
		}
		if ((mz+1) < dim_z)
		{
				// in z dimension:
				//1st
				xyz2rank(dim_x, dim_y, dim_z, mx, my, ((mz+1)%dim_z), &tgt);
			#ifdef __IMI_DEBUG_NOW__
				  printf("src %d --> dst: %d \n", my_rank,tgt);
			#endif
				  MPI_Isend(src, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
				  MPI_Irecv(dst, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
		}
			//2nd

		if ((mz-1) >= 0)
		{
				xyz2rank(dim_x, dim_y, dim_z, mx, my, ( (mz-1 + dim_z) % dim_z ), &tgt);
			#ifdef __IMI_DEBUG_NOW__
				  printf("src %d --> dst: %d \n", my_rank,tgt);
				  fflush(stdout);
			#endif
				  MPI_Isend(src, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
				  MPI_Irecv(dst, msg_size, MPI_CHAR, tgt, 0, MPI_COMM_WORLD, &req[req_count]);
				  req_count++;
		}
		#ifdef __IMI_DEBUG_ALL__
			  printf("2D rank %d req_count = %d \n", my_rank, req_count);
		#endif
		
	}
	  return;
}
