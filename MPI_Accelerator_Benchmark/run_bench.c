#include "MICRO_BENCHMARK.h"

void run_bench(int rank, benchmark *bench, int bench_count){
	int cnt = 0;
	while (cnt != bench_count)
	{
		switch (bench[cnt].name[0])
		{
			case '2': //representing "2DSTENCIL"
#ifdef __IMI_DEBUG_ALL__
    if (rank == 0)
    {
				printf("%s\n", bench[cnt].name);
    }
#endif
				_2DSTENCIL_func(bench[cnt].src, bench[cnt].dst, rank, bench[cnt].type, bench[cnt].size, bench[cnt].dim_wght, bench[cnt].dim_x, bench[cnt].dim_y);
				break;

			case '3': //representing "3DTORUS"
#ifdef __IMI_DEBUG_ALL__
    if (rank == 0)
    {
				printf("%s\n", bench[cnt].name);
    }
#endif
				_3DTORUS_func(bench[cnt].src, bench[cnt].dst, rank, bench[cnt].type, bench[cnt].size, bench[cnt].dim_wght, bench[cnt].dim_x, bench[cnt].dim_y, bench[cnt].dim_z);
				break;

			case 'C': //representing "COLLSUBCOM":
#ifdef __IMI_DEBUG_ALL__
    if (rank == 0)
    {
				printf("%s\n", bench[cnt].name);
    }
#endif
    			_COLLSUBCOMM_func(bench[cnt].src, bench[cnt].dst, bench[cnt].size, bench[cnt].dim_wght, bench[cnt].sub_comm);
				break;

			default:
			    fprintf(stderr,"ABORT: Invalid name for benchmark # %d\n ******** use -h for detail\n"
			    		,cnt);
			    exit(1);
		}
		cnt++;
	 }
	return ;
}
