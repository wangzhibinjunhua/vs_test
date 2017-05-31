
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "readCSVFile.h"
#include <math.h>
#include "findSmallestPolygon.h"
#include "mat.h"

void test_findsp()
{
	char *filename = "e:/python-project/test/num/xy_522_2.txt";
	ReadCsvData(filename);
	printf("giNumRow=%d,giNumCol=%d\n", giNumRow, giNumCol);
	//ShowCsvData();
	double *polyX, *polyY, *x, *y;
	long *polyCorners;
	int N = giNumRow*sizeof(double);
	//int N = giNumRow;
	x = (double*)malloc(N);
	y = (double*)malloc(N);
	polyX = (double*)malloc(N);
	polyY = (double*)malloc(N);

	polyCorners = (long*)malloc(giNumRow*sizeof(long));
	memset(x, 0, N);
	memset(y, 0, N);
	memset(polyX, 0, N);
	memset(polyY, 0, N);
	int i = 0;
	for (i = 0; i < giNumRow; i++) {
		printf("%f,%f\n", giCsvData[i*giNumCol + 0], giCsvData[i*giNumCol + 1]);
		//memcpy(x + i*sizeof(giCsvData[i*giNumCol + 0]), &(giCsvData[i*giNumCol + 0]), sizeof(giCsvData[i*giNumCol + 0]));
		//memcpy(y + i*sizeof(giCsvData[i*giNumCol + 1]), &(giCsvData[i*giNumCol + 1]), sizeof(giCsvData[i*giNumCol + 1]));
		x[i] = giCsvData[i*giNumCol + 0];
		y[i] = giCsvData[i*giNumCol + 1];
	}

	for (i = 0; i < giNumRow; i++) {
		printf("x=%f,y=%f\n", x[i], y[i]);
	}

	findSmallestPolygon(x, y, giNumRow, polyX, polyY, polyCorners);

	for (i = 0; i < giNumRow; i++) {
		printf("px=%f,py=%f\n", polyX[i], polyY[i]);
	}
	printf("polyCorners=%ld\n", *polyCorners);
	FreeCsvData();
	free(x);
	free(y);
	free(polyY);
	free(polyX);
	free(polyCorners);
}

void test_mat()
{
	int n, i, length;
	char *filename = "e:/python-project/test/num/xy_522_2.txt";
	ReadCsvData(filename);
	printf("giNumRow=%d,giNumCol=%d\n", giNumRow, giNumCol);

	POINT a[] = { { 1.0, 2.0 },{ 2.0, 0.5 },{ 2.5, 1.0 },{ 2.0, 0.0 },{ 4.0, 2.0 } };

	POINT *old_data;
	POINT b[4];
	int N = giNumRow*sizeof(POINT);
	printf("N=%d\n", N);

	old_data = (POINT*)malloc(N);
	//old_data->x = (float*)malloc(giNumRow*sizeof(float));
	memset(b, 0, sizeof(b));
	memset(old_data, 0, sizeof(old_data));
	printf("sizeof(old_data)=%d,sizeof(POINT)=%d\n", sizeof(old_data), sizeof(POINT));
	for (i = 0; i < giNumRow; i++) {
		printf("%f,%f\n", giCsvData[i*giNumCol + 0], giCsvData[i*giNumCol + 1]);
		//memcpy(x + i*sizeof(giCsvData[i*giNumCol + 0]), &(giCsvData[i*giNumCol + 0]), sizeof(giCsvData[i*giNumCol + 0]));
		//memcpy(y + i*sizeof(giCsvData[i*giNumCol + 1]), &(giCsvData[i*giNumCol + 1]), sizeof(giCsvData[i*giNumCol + 1]));
		old_data[i].x = giCsvData[i*giNumCol + 0];
		old_data[i].y = giCsvData[i*giNumCol + 1];
	}

	printf("####################\n");

	for (int i = 0; i <giNumRow; i++) {

		printf("%f,%f\n", old_data[i].x, old_data[i].y);
	}
	
	//length = sizeof(a) / sizeof(POINT);
	//printf("length=%d\n", length);
	rotatingcalipers(old_data, giNumRow, b);

	for (i = 0; i < 4; i++) {
		printf("[%f, %f] ", b[i].x, b[i].y);
	}
	printf("\n");
	FreeCsvData();
}

int main(int argc, char **argv)
{
	//test_findsp();
	test_mat();
	getchar();
	return 0;
}