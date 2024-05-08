#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

#include "grav_data/data_util_bin.c"
#include "second.c"

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
	int c=0;
	for ( c = localSize * rank; c < min(localSize * (rank + 1), np); c++)
	{
		int local_c = c - localSize * rank;
		vxs_local[local_c] = vxs_old[c];
		vys_local[local_c] = vys_old[c];
		vzs_local[local_c] = vzs_old[c];
	}

	MPI_Barrier(MPI_COMM_WORLD);
	double startTime = second();

	double dt = 1;
	double G = 1;
	int stepNum = 10;
	int stepCount = 0;
	for (stepCount = 0; stepCount < stepNum; stepCount++)
	{
		int i = localSize * rank;
		//工夫:各点の次のステップの速度と座標の算出はEPのため、この部分をスレッド/プロセス並列にしている。
#pragma omp parallel for
		for (i = localSize * rank; i < min(localSize * (rank + 1), np); i++) // i番目のvelocityとpositionを求める
		{
			// 工夫:配列に格納されていて頻繁にアクセスする値は、privateな一時変数に値をコピーし、計算にはその変数を使う。
			// そうすることでスレッド間の排他制御の回数が減り、その分速くなる。
			double x = xs_old[i];
			double y = ys_old[i];
			double z = zs_old[i];

			// 工夫:また、上と同様の理由で、計算途中の値についてもprivateな一時変数に入れるようにし、
			// 最終的な値を一時変数から配列にコピーするようにしている。
			double vx_new = 0;
			double vy_new = 0;
			double vz_new = 0;

			// 工夫:1つ目のループがすでに今回の実験の最大コア数(84)より十分大きい(約12倍)為、内側のループは並列化しない。
			// これによって、スレッドの初期化や、スレッド間の排他制御の回数を減らし、reduction処理を行わずに済む為、その分速くなる。
			int j = 0;
			for (j = 0; j < np; j++) // i番目に対して働くv番目の重力を求める。
			{
				if (j == i)
				{
					continue;
				}
				double rij = sqrt(pow(xs_old[j] - x, 2) + pow(ys_old[j] - y, 2) + pow(zs_old[j] - z, 2));

				//工夫:ここで計算した値は、j=現在のi i=現在のj の処理でも使われるため、キャッシュしておく方法もあるが、
				//その場合、計算量を半分に抑える代わりに他のプロセスとの通信を行う必要がある。
				//通信の遅延に加えて、(データ数)*(通信スループットの逆数)の通信時間が増加してしまう。
				//通信するよりも、逐一演算を行なった方が早いと判断した。
				double axij = (-G) * ms[j] / pow(rij, 2) * (x - xs_old[j]) / rij;
				double ayij = (-G) * ms[j] / pow(rij, 2) * (y - ys_old[j]) / rij;
				double azij = (-G) * ms[j] / pow(rij, 2) * (z - zs_old[j]) / rij;

				vx_new += axij * dt;
				vy_new += ayij * dt;
				vz_new += azij * dt;
			}
			// 工夫:速度のデータは次のステップのその質点の位置を計算する時にしか使わないので、新しい質点の値にすぐ更新する
			// こうすることで、配列への不要なコピーを無くせるor配列のキャッシュヒット率を上げる。
			//(一気に更新した方が排他制御のオーバーヘッドが小さくなったりしますか?)
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
		
		//この部分は、プロセス数に比例して通信のオーバーヘッドが発生するため、ノード数*プロセス数分の時間がかかると思われる。
		//また、受け取ったデータのコピーに関しては、メモリやキャッシュに対するプロセス数に比例して時間がかかると思われる。
		//そのため、この部分はノードあたりのプロセス数に比例して時間が増えると思われる。
		MPI_Allgather(xs_new_local, localSize, MPI_DOUBLE, xs_old, localSize, MPI_DOUBLE, MPI_COMM_WORLD);
		MPI_Allgather(ys_new_local, localSize, MPI_DOUBLE, ys_old, localSize, MPI_DOUBLE, MPI_COMM_WORLD);
		MPI_Allgather(zs_new_local, localSize, MPI_DOUBLE, zs_old, localSize, MPI_DOUBLE, MPI_COMM_WORLD);
		// 工夫:速度のデータはその質点の位置を計算するプロセスしか使わないため、各ステップにおける質点の速度は他のプロセスに共有しない。
		// それにより、データ通信及びコピーの時間が削減される
	}

	MPI_Barrier(MPI_COMM_WORLD);
	double endTime = second();
	double calcTime = endTime - startTime;
	double maxCalcTime;

	MPI_Reduce(&calcTime, &maxCalcTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("%f\n", calcTime);
	}

	if (rank == 0)
	{
		write_data(xs_old, "grav_data/result1000/outx.double", np);
		write_data(ys_old, "grav_data/result1000/outy.double", np);
		write_data(zs_old, "grav_data/result1000/outz.double", np);
	}

	MPI_Finalize();
}