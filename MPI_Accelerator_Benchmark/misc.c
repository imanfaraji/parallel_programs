#include "MICRO_BENCHMARK.h"

void print_usage()
{
	char USAGE[500];
	//USAGE -h:
	strcpy(USAGE, "********************************************HELP MESSAGE********************************************\n");
	strcat(USAGE, "./exe #_OF_PROCESSORS #_OF_ITERATIONS #_OF_SKIPS [benchmark_name#1] <benchmark_inputs#1> [benchmark_name#2] <benchmark_inputs#2> [bencmark_name#n] <benchmark_inputs#n>\n");
	strcat(USAGE, "benchmark_name:\n");
	strcat(USAGE, "\t+2DSTENCIL:\n");
	strcat(USAGE, "\t\tinput:\n");
	strcat(USAGE, "\t\t\t-type: \"cpu\" or \"gpu\"\n");
	strcat(USAGE, "\t\t\t-size\n");
	strcat(USAGE, "\t\t\t-dim_wght\n");
	strcat(USAGE, "\t\t\t-dim_x\n");
	strcat(USAGE, "\t\t\t-dim_y\n");
	strcat(USAGE, "\t+3DTORUS:\n");
	strcat(USAGE, "\t\tinput:\n");
	strcat(USAGE, "\t\t\t-type: \"cpu\" or \"gpu\"\n");
	strcat(USAGE, "\t\t\t-size\n");
	strcat(USAGE, "\t\t\t-dim_wght\n");
	strcat(USAGE, "\t\t\t-dim_x\n");
	strcat(USAGE, "\t\t\t-dim_y\n");
	strcat(USAGE, "\t\t\t-dim_z\n");
	strcat(USAGE, "\t+COLLSUBCOMM:\n");
	strcat(USAGE, "\t\tinput:\n");
	strcat(USAGE, "\t\t\t-type: \"cpu\" or \"gpu\"\n");
	strcat(USAGE, "\t\t\t-size\n");
	strcat(USAGE, "\t\t\t-dim_x_keep\n");
	strcat(USAGE, "\t\t\t-dim_y_keep\n");
	strcat(USAGE, "\t\t\t-dim_z_keep\n");
	printf("%s", USAGE);
	return;
}

int isnumber(char *str) //checks whether input string is a number (return 0) or not (return 1)
{
    int str_len = strlen(str);
    int i;

    for (i = 0; i < str_len; i++)
    {
        if (!isdigit(str[i]))
        {
            return 1; //input is not a number
        }
    }
    return 0; //input is a number
}
