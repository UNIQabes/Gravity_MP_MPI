#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

#include "grav_data/data_util_bin.c"

int max(int v1, int v2)
{
	return v1 > v2 ? v1 : v2;
}
int min(int v1, int v2)
{
	return v1 < v2 ? v1 : v2;
}

int main(int argc, char *argv[])
{

	int rank, len, procNum;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &procNum);

	procNum = 8;

	// pointの数
	int np = 1000;

	// i番目のvelocityとpositionを求めるループで、それぞれのプロセスが担当する範囲の大きさ
	// np/procNumの小数部を切り上げた値
	int localSize = (np + procNum - 1) / procNum;

	double arySize = localSize * procNum;

	double *ms = malloc(sizeof(double) * np);

	read_data("grav_data/n1000/m.double", ms, np);

	double *xs_old = malloc(sizeof(double) * arySize);
	read_data("grav_data/n1000/x.double", xs_old, np);

	double *ys_old = malloc(sizeof(double) * arySize);
	read_data("grav_data/n1000/y.double", ys_old, np);

	double *zs_old = malloc(sizeof(double) * arySize);
	read_data("grav_data/n1000/z.double", zs_old, np);

	double *vxs_old = malloc(sizeof(double) * arySize);
	read_data("grav_data/n1000/vx.double", vxs_old, np);

	double *vys_old = malloc(sizeof(double) * arySize);
	read_data("grav_data/n1000/vy.double", vys_old, np);

	double *vzs_old = malloc(sizeof(double) * arySize);
	read_data("grav_data/n1000/vz.double", vzs_old, np);

	double *xs_new_local = malloc(sizeof(double) * localSize);
	double *ys_new_local = malloc(sizeof(double) * localSize);
	double *zs_new_local = malloc(sizeof(double) * localSize);

	double *vxs_local = malloc(sizeof(double) * localSize);
	double *vys_local = malloc(sizeof(double) * localSize);
	double *vzs_local = malloc(sizeof(double) * localSize);
	for (int i = localSize * rank; i < min(localSize * (rank + 1), np); i++)
	{
		int local_i = i - localSize * rank;
		vxs_local[local_i] = vxs_old[i];
		vys_local[local_i] = vys_old[i];
		vzs_local[local_i] = vzs_old[i];
	}

	double dt = 1;
	double G = 1;
	int stepNum = 10;

	for (int stepCount = 0; stepCount < stepNum; stepCount++)
	{
#pragma omp parallel for
		for (int i = localSize * rank; i < min(localSize * (rank + 1), np); i++) // i番目のvelocityとpositionを求める
		{

			double x = xs_old[i];
			double y = ys_old[i];
			double z = zs_old[i];

			double vx_new = 0;
			double vy_new = 0;
			double vz_new = 0;
			for (int j = 0; j < np; j++) // i番目に対して働くv番目の重力を求める。
			{
				if (j == i)
				{
					continue;
				}
				double rij = sqrt(pow(xs_old[j] - x, 2) + pow(ys_old[j] - y, 2) + pow(zs_old[j] - z, 2));

				double axij = (-G) * ms[j] / pow(rij, 2) * (x - xs_old[j]) / rij;
				double ayij = (-G) * ms[j] / pow(rij, 2) * (y - ys_old[j]) / rij;
				double azij = (-G) * ms[j] / pow(rij, 2) * (z - zs_old[j]) / rij;

				vx_new += axij * dt;
				vy_new += ayij * dt;
				vz_new += azij * dt;
			}
			int local_i = i - localSize * rank;
			vx_new += vxs_local[local_i];
			vy_new += vys_local[local_i];
			vz_new += vzs_local[local_i];

			xs_new_local[local_i] = x + vx_new * dt;
			ys_new_local[local_i] = y + vy_new * dt;
			zs_new_local[local_i] = z + vz_new * dt;
			vxs_local[local_i] = vx_new;
			vys_local[local_i] = vy_new;
			vzs_local[local_i] = vz_new;
		}
		MPI_Allgather(xs_new_local, localSize, MPI_DOUBLE, xs_old, localSize, MPI_DOUBLE, MPI_COMM_WORLD);
		MPI_Allgather(ys_new_local, localSize, MPI_DOUBLE, ys_old, localSize, MPI_DOUBLE, MPI_COMM_WORLD);
		MPI_Allgather(zs_new_local, localSize, MPI_DOUBLE, zs_old, localSize, MPI_DOUBLE, MPI_COMM_WORLD);
	}

	if (rank == 0)
	{
		write_data(xs_old, "grav_data/result1000/outx.double", np);
		write_data(ys_old, "grav_data/result1000/outy.double", np);
		write_data(zs_old, "grav_data/result1000/outz.double", np);
	}

	MPI_Finalize();
}