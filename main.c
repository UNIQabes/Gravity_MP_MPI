#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "grav_data/data_util_bin.c"
int main()
{
	int np = 1000;
	double *ms = malloc(sizeof(double) * np);
	read_data("grav_data/n1000/m.double", ms, np);
	for (int i = 0; i < np; i++)
	{
		printf("%f/", ms[i]);
	}
	printf("\n");
}