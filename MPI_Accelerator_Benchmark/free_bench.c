#include "MICRO_BENCHMARK.h"

void free_bench(benchmark * bench, int bench_count)
{
	int bench_cnt = 0;
	cudaError_t cuda_err = cudaSuccess;
	while (bench_cnt != bench_count)
	{
		if (!strcmp(bench[bench_cnt].type, "cpu"))
		{
			free(bench[bench_cnt].src);
			free(bench[bench_cnt].dst);
		}

		else if (!strcmp(bench[bench_cnt].type, "gpu"))
		{
			cuda_err = cudaFree(bench[bench_cnt].src);
			if (cuda_err != cudaSuccess)
			{
				fprintf(stderr, "ABORT: CUDA cannot free src memory\n");
			}
			cuda_err = cudaFree(bench[bench_cnt].dst);
			if (cuda_err != cudaSuccess)
			{
				fprintf(stderr, "ABORT: CUDA cannot free dst memory\n");
			}
		}
		bench_cnt++;
	}
	return;
}
