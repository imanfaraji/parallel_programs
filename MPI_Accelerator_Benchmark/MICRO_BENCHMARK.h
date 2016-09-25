#ifndef __BENCH_H__
#define __BENCH_H__

//#define __IMI_DEBUG_ALL__ //disable this to avoid debugging printouts
//#define __IMI_DEBUG_NOW__ // use this for debugging specific parts of the code

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <cuda_runtime.h>


#define __MAX_BENCH__ 10 // Sets maximum number of benchmarks that can be passed as a command line argument
#define __REQ_MAX__ 256
#define __REQ_MAX_COLL__ 10

#ifdef __GLOBAL_VAR__
	#define EXTERN
#else
	#define EXTERN extern
#endif

EXTERN MPI_Request req[__REQ_MAX__]; // used for querying non-blocking point-to-point calls
EXTERN MPI_Status stat[__REQ_MAX__];
EXTERN int req_count;

EXTERN MPI_Request req_coll[__REQ_MAX_COLL__]; // used for querying non-blocking collective calls
EXTERN MPI_Status stat_coll[__REQ_MAX_COLL__];
EXTERN int req_count_coll;
EXTERN int bench_cycle;

typedef struct // This type is used to store benchmarks that are read from the cmd.
{
    char name[20]; // name = 2DSTENCIL/3DTORUS/COLLSUBCOMM
    char type[5]; //type = cpu/gpu
    int size;
    void *src;
    void *dst;
    int dim_wght;
    int dim_x;
    int dim_y;
    int dim_z;
    MPI_Comm sub_comm;
    int bench_count;
}benchmark;

void print_usage();
int isnumber(char *);

#endif







