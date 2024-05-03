#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <math.h>
#include <omp.h>

#include "grav_data/data_util_bin.c"
#include "second.c"

int main()
{
	int np = 1000;
	double *ms = malloc(sizeof(double) * np);
	read_data("grav_data/n1000/m.double", ms, np);

	double *xs_old = malloc(sizeof(double) * np);
	read_data("grav_data/n1000/x.double", xs_old, np);

	double *ys_old = malloc(sizeof(double) * np);
	read_data("grav_data/n1000/y.double", ys_old, np);

	double *zs_old = malloc(sizeof(double) * np);
	read_data("grav_data/n1000/z.double", zs_old, np);

	double *vxs_old = malloc(sizeof(double) * np);
	read_data("grav_data/n1000/vx.double", vxs_old, np);

	double *vys_old = malloc(sizeof(double) * np);
	read_data("grav_data/n1000/vy.double", vys_old, np);

	double *vzs_old = malloc(sizeof(double) * np);
	read_data("grav_data/n1000/vz.double", vzs_old, np);

	// for (int i = 0; i < 3; i++){printf("%f/", xs_old[i]);}printf("\n");

	double *xs_new = malloc(sizeof(double) * np);
	double *ys_new = malloc(sizeof(double) * np);
	double *zs_new = malloc(sizeof(double) * np);
	// double *vxs_new = malloc(sizeof(double) * np);
	// double *vys_new = malloc(sizeof(double) * np);
	// double *vzs_new = malloc(sizeof(double) * np);

	double dt = 1;
	double G = 1;
	int stepNum = 10;

	double starttime = second();
	int stepCount=0;
	for (stepCount = 0; stepCount < stepNum; stepCount++)
	{
		// printf("pos[0]:(%f,%f,%f) v[0](%f,%f,%f)\n", xs_old[0], ys_old[0], zs_old[0], vxs_old[0], vys_old[0], vzs_old[0]);
		int i=0;
#pragma omp parallel for
		for (i = 0; i < np; i++) // i番目のvelocityとpositionを求める
		{
			double x = xs_old[i];
			double y = ys_old[i];
			double z = zs_old[i];

			double vx_new = 0;
			double vy_new = 0;
			double vz_new = 0;
			int j = 0;
			for (j = 0; j < np; j++) // i番目に対して働くv番目の重力を求める。
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
			vx_new += vxs_old[i];
			vy_new += vys_old[i];
			vz_new += vzs_old[i];

			xs_new[i] = x + vx_new * dt;
			ys_new[i] = y + vy_new * dt;
			zs_new[i] = z + vz_new * dt;
			vxs_old[i] = vx_new;
			vys_old[i] = vy_new;
			vzs_old[i] = vz_new;
		}

		double *temp;
		temp = xs_old;
		xs_old = xs_new;
		xs_new = temp;

		temp = ys_old;
		ys_old = ys_new;
		ys_new = temp;

		temp = zs_old;
		zs_old = zs_new;
		zs_new = temp;
	}

	double endtime = second();
	printf("%f\n", endtime - starttime);

	// for (int i = 0; i < 3; i++){printf("%f/", xs_old[i]);}printf("\n");

	write_data(xs_old, "grav_data/result1000/outx.double", np);
	write_data(ys_old, "grav_data/result1000/outy.double", np);
	write_data(zs_old, "grav_data/result1000/outz.double", np);
}