#include "MICRO_BENCHMARK.h"

static int my_args = 1;

read_init(int *procs, int *iter, int *skip, int argc, char ** argv)
{
	 *procs = atoi(argv[my_args++]);
	 *iter =  atoi(argv[my_args++]);
	 *skip =  atoi(argv[my_args++]);
}


void create_sub_comm(MPI_Comm old_comm, benchmark bench, MPI_Comm *new_sub_comm)
{
	/* This function builds a 3D mesh from the set of
	 * processes and then creates sub-communicators over
	 * which a collective communication is called. Dimension
	 * sizes of the mesh are set by MPI_Dims_create which
	 * tries to create as uniform sizes as possible. The
	 * dim_keep array is used to determine the sub-communicators.
	 * Each dim_keep element is set to either 0 or 1. Having dim_keep[i]=1
	 * means that dimension 'i' should be in each sub-communicator.
	 * For instance, setting dim_keep[0]=0, dim_keep[1]=1, dim_keep[2]=1
	 * will create multiple 2D sub-communicators. Setting all dim_keep
	 * elements to 1 will correspond to one sub-communicator which is same
	 * as the original 3D communicator.
	 */
	int ndims = 3;
    int dims[ndims], periodic[ndims];
    int i;
    for(i = 0; i < ndims; i++)
    {
		dims[i] = 0;
		periodic[i] = 0;
    }

    int old_comm_size;
    MPI_Comm_size(old_comm, &old_comm_size);

    int err;
    MPI_Comm cart_comm;
    err = MPI_Dims_create(old_comm_size, ndims, dims);
    err = MPI_Cart_create(old_comm, ndims, dims, periodic, 0, &cart_comm);
//FIXME err checking?
    int dim_keep[ndims];
    dim_keep[0] = bench.dim_x;
    dim_keep[1] = bench.dim_y;
    dim_keep[2] = bench.dim_z;
    err = MPI_Cart_sub(cart_comm, dim_keep, (MPI_Comm *) new_sub_comm);
    return;
}
//this function reads all the benchmarks from the input arguments
int read_bench(benchmark *bench, int argc, char **argv)
{
	int bench_count = 0;
    cudaError_t cuda_err = cudaSuccess;

	while (my_args != argc)
	{
		if (bench_count == __MAX_BENCH__ - 1)
		{
		   fprintf(stderr,"ABORT: MAX number of Benchmark is reached -- only up to %d benchmarks are supported\n", __MAX_BENCH__);
		   exit(1);
		}
//--------------------------------READING GENERAL ARGUMENTS---------
//***********************Assigning Benchmark NAME*****************************
		if (!strcmp(argv[my_args],"2DSTENCIL") || !strcmp(argv[my_args],"3DTORUS") || !strcmp(argv[my_args],"COLLSUBCOMM"))
		{
		  if (my_args != argc) strcpy(bench[bench_count].name,argv[my_args++]); //benchmark name is assigned
		}
		else
		{
			fprintf(stderr,"ABORT: Invalid Benchmark name: %s\n ******** use -h for detail\n",argv[my_args]);
			exit(1);
		}
//***********************Assigning Benchmark TYPE*****************************
		if (!strcmp(argv[my_args],"cpu") || !strcmp(argv[my_args],"gpu"))
		{
			if (my_args != argc) strcpy(bench[bench_count].type, argv[my_args++]); //benchmark type is assigned
		}
		 else
		{
			fprintf(stderr,"ABORT: Invalid Benchmark Type: %s\n ******** use -h for detail\n", argv[my_args]);
			exit(1);
		}
//***********************Assigning Benchmark SIZE*****************************
		if (!isnumber(argv[my_args]))
		{
			if (my_args != argc) bench[bench_count].size = atoi(argv[my_args++]);//benchmark size is assigned
		}
		else
		{
			fprintf(stderr,"ABORT: Invalid Benchmark SIZE: %s\n ******** use -h for detail\n",argv[my_args]);
			exit(1);
		}
		//***********************Assigning Benchmark Weight *****************************
		if (!isnumber(argv[my_args]))
		{
			if (my_args != argc) bench[bench_count].dim_wght = atoi(argv[my_args++]);//benchmark dimension is assigned
		}
		else
		{
			fprintf(stderr,"ABORT: Invalid Benchmark Weight: %s\n ******** use -h for detail\n",argv[my_args]);
			exit(1);
		}
//--------------------------------READING Benchmark Specific ARGUMENTS---------
		switch (bench[bench_count].name[0])
		{
			case '2': //representing "2DSTENCIL"
				//***********************Reading Dimensions*****************************
				if (!isnumber(argv[my_args]))
				{
					if (my_args != argc) bench[bench_count].dim_x = atoi(argv[my_args++]);//benchmark dim_x is assigned
				}
				else
				{
					fprintf(stderr,"ABORT: Invalid Benchmark dim_x: %s\n ******** use -h for detail\n",argv[my_args]);
					exit(1);
				}
				if (!isnumber(argv[my_args]))
				{
					if (my_args != argc) bench[bench_count].dim_y = atoi(argv[my_args++]);//benchmark dim_y is assigned
				}
				else
				{
					fprintf(stderr,"ABORT: Invalid Benchmark dim_y: %s\n ******** use -h for detail\n",argv[my_args]);
					exit(1);
				}
				//***********************Allocating BUFFERS*****************************
				if (!strcmp(bench[bench_count].type,"cpu"))
				{
					bench[bench_count].src = malloc(bench[bench_count].dim_wght * bench[bench_count].size);
					bench[bench_count].dst = malloc(bench[bench_count].dim_wght * bench[bench_count].size);
				}
				else if (!strcmp(bench[bench_count].type, "gpu"))
				{
					cuda_err = cudaMalloc(&(bench[bench_count].src), bench[bench_count].dim_wght * bench[bench_count].size);
					if (cuda_err != cudaSuccess)
					{
						fprintf(stderr, "ABORT: CUDA cannot allocate src memory\n");
						exit(1);
					}
					cuda_err = cudaMalloc(&(bench[bench_count].dst), bench[bench_count].dim_wght * bench[bench_count].size);
					if (cuda_err != cudaSuccess)
					{
						fprintf(stderr, "ABORT: CUDA cannot allocate dst memory\n");
						exit(1);
					}
				}
				break;
			case '3': //representing "3DTORUS"
				//***********************Reading Dimension*****************************
				if (!isnumber(argv[my_args]))
				{
					if (my_args != argc) bench[bench_count].dim_x = atoi(argv[my_args++]);//benchmark dim_x is assigned
				}
				else
				{
					fprintf(stderr,"ABORT: Invalid Benchmark dim_x: %s\n ******** use -h for detail\n",argv[my_args]);
					exit(1);
				}

				if (!isnumber(argv[my_args]))
				{
					if (my_args != argc) bench[bench_count].dim_y = atoi(argv[my_args++]);//benchmark dim_y is assigned
				}
				else
				{
					fprintf(stderr,"ABORT: Invalid Benchmark dim_y: %s\n ******** use -h for detail\n",argv[my_args]);
					exit(1);
				}

				if (!isnumber(argv[my_args]))
				{
					if (my_args != argc) bench[bench_count].dim_z = atoi(argv[my_args++]);//benchmark dim_z is assigned
				}
				else
				{
					fprintf(stderr,"ABORT: Invalid Benchmark dim_z: %s\n ******** use -h for detail\n",argv[my_args]);
					exit(1);
				}
				//***********************Allocating BUFFERS*****************************
				if (!strcmp(bench[bench_count].type,"cpu"))
				{
					bench[bench_count].src = malloc(bench[bench_count].dim_wght * bench[bench_count].size);
					bench[bench_count].dst = malloc(bench[bench_count].dim_wght * bench[bench_count].size);
				}
				else if (!strcmp(bench[bench_count].type, "gpu"))
				{
					cuda_err = cudaMalloc(&(bench[bench_count].src), bench[bench_count].dim_wght * bench[bench_count].size);
					if (cuda_err != cudaSuccess)
					{
						fprintf(stderr, "ABORT: CUDA cannot allocate src memory\n");
						exit(1);
					}
					cuda_err = cudaMalloc(&(bench[bench_count].dst), bench[bench_count].dim_wght * bench[bench_count].size);
					if (cuda_err != cudaSuccess)
					{
						fprintf(stderr, "ABORT: CUDA cannot allocate dst memory\n");
						exit(1);
					}
				}
				break;
			case 'C': //representing "COLLSUBCOM":
				//***********************Read Dimensions*****************************
				if (!isnumber(argv[my_args]))
				{
					if (my_args != argc) bench[bench_count].dim_x = atoi(argv[my_args++]); //This value indicates whether to use dim_x (1) or not (0) on  the subcom
				}
				else
				{
					fprintf(stderr,"ABORT: Invalid Benchmark dim_x: %s\n ******** use -h for detail\n",argv[my_args]);
					exit(1);
				}
				if (!isnumber(argv[my_args]))
				{
					if (my_args != argc) bench[bench_count].dim_y = atoi(argv[my_args++]);//This value indicates whether to use dim_y (1) or not (0) on  the subcom
				}
				else
				{
					fprintf(stderr,"ABORT: Invalid Benchmark dim_y: %s\n ******** use -h for detail\n",argv[my_args]);
					exit(1);
				}
				if (!isnumber(argv[my_args]))
				{
					if (my_args != argc) bench[bench_count].dim_z = atoi(argv[my_args++]);//This value indicates whether to use dim_z (1) or not (0) on  the subcom
				}
				else
				{
					fprintf(stderr,"ABORT: Invalid Benchmark dim_z: %s\n ******** use -h for detail\n",argv[my_args]);
					exit(1);
				}
				//***********************Creating Sub_communicator*****************************
				MPI_Comm sub_comm, comm = MPI_COMM_WORLD;
				create_sub_comm(comm, bench[bench_count], &sub_comm); //create a sub_communicator for comm communicator based on bench input arguments
				bench[bench_count].sub_comm = sub_comm;

				int comm_size, comm_rank;
				MPI_Comm_size(comm, &comm_size);
				MPI_Comm_rank(comm, &comm_rank);

				int sub_comm_size, sub_comm_rank;
				MPI_Comm_size(sub_comm, &sub_comm_size);
				MPI_Comm_rank(sub_comm, &sub_comm_rank);

#ifdef __IMI_DEBUG__
				printf("****** RANK %2d -- SIZE %2d  ---- sub_Rank %2d -- sub_Size %2d\n",
						comm_rank, comm_size, sub_comm_rank, sub_comm_size);
				fflush(stdout);

#endif
				//***********************Allocating BUFFERS*****************************
				if (!strcmp(bench[bench_count].type,"cpu"))
				{
					bench[bench_count].src = malloc(sub_comm_size * bench[bench_count].size * bench[bench_count].dim_wght);
					bench[bench_count].dst = malloc(sub_comm_size * bench[bench_count].size * bench[bench_count].dim_wght);
				}
				else if (!strcmp(bench[bench_count].type, "gpu"))
				{
					cuda_err = cudaMalloc(&(bench[bench_count].src), sub_comm_size * bench[bench_count].size * bench[bench_count].dim_wght);
					if (cuda_err != cudaSuccess)
					{
						fprintf(stderr, "ABORT: CUDA cannot allocate src memory\n");
						exit(1);
					}
					cuda_err = cudaMalloc(&(bench[bench_count].dst), sub_comm_size * bench[bench_count].size * bench[bench_count].dim_wght);
					if (cuda_err != cudaSuccess)
					{
						fprintf(stderr, "ABORT: CUDA cannot allocate dst memory\n");
						exit(1);
					}
				}
				break;

			default:
			  fprintf(stderr,"ABORT: Invalid name for benchmark # %d\n ******** use -h for detail\n",bench_count);
			  exit(1);
		}
		bench_count++;
	}
	return bench_count;
}
