#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "mat.h"


static void
swap(POINT *arr, int a, int b)
{
	POINT temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;
}

/*
*        Name:  getdist
* Description:  计算两点间的距离
*
*/
static float
getdist(POINT p1, POINT p2)
{
	return sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
}

/*
*        Name:  getcross
* Description:  计算叉积z方向的值，用于判断两个向量的转向
*
*/
static float
getcross(POINT p0, POINT p1, POINT p2)
{
	return (p1.x - p0.x)*(p2.y - p0.y) - (p2.x - p0.x)*(p1.y - p0.y);
}

/*
*        Name:  getpod
* Description:  计算两个向量的点积
*
*/
static float
getdot(POINT p0, POINT p1, POINT p2)
{
	return (p1.x - p0.x)*(p2.x - p0.x) + (p1.y - p0.y)*(p2.y - p0.y);
}

/*
*        Name:  anglecmp
* Description:  比较两个向量的逆时针转角方向，p1大于p2时返回大于0，等于时返回等于0
*
*/
static float
anglecmp(POINT p0, POINT p1, POINT p2)
{
	float cross = getcross(p0, p1, p2);
	if (cross == 0) {
		return getdist(p0, p2) - getdist(p0, p1);
	}
	return cross;
}

/*
*        Name:  vectorsort
* Description:  根据基点进行向量排序, 快速排序递归实现
*
*/
static void
vectorsort(POINT *arr, int left, int right)
{
	int i, mid, last;

	if (left >= right) {
		return;
	}

	mid = (left + right) / 2;
	swap(arr, left, mid);
	last = left;
	for (i = left + 1; i <= right; i++) {
		if (anglecmp(arr[0], arr[i], arr[left])>0) {
			swap(arr, i, ++last);
		}
	}
	swap(arr, left, last);
	vectorsort(arr, left, last - 1);
	vectorsort(arr, last + 1, right);
}

/*
*        Name:  getconvex
* Description:  计算凸包
*
*/
void
getconvex(POINT *arr, int len, int *n)
{
	int i, base, top;

	/* 小于4个点的不计算凸包 */
	if (len < 4) {
		*n = len;
		return;
	}
	/* 计算基点，交换到0位置 */
	base = 0;
	for (i = 0; i<len; i++) {
		if (arr[i].y == arr[base].y && arr[i].x < arr[base].x) {
			base = i;
		}
		else if (arr[i].y < arr[base].y) {
			base = i;
		}
	}
	swap(arr, base, 0);

	/* 排序 */
	vectorsort(arr, 1, len - 1);

	/* 计算凸包 */
	top = 1;
	for (i = 2; i<len; i++) {
		while (top>0 && getcross(arr[top - 1], arr[top], arr[i]) <= 0) {
			top--;
		}
		arr[++top] = arr[i];
	}
	*n = top;
}

void
rotatingcalipers(POINT *arr, int len, POINT *rectangle)
{
	int top, down, right = 1, up = 0, left = 0, downlast, rightlast, uplast, leftlast;
	float area = FLT_MAX, dist, X, Y, k;
	POINT temp;

	getconvex(arr, len, &top);
	arr[++top] = arr[0];

	for (down = 0; down<top; down++) {
		// find right
		while (getdot(arr[down], arr[down + 1], arr[right]) <= getdot(arr[down], arr[down + 1], arr[right + 1])) {
			right = (right + 1) % top;
		}

		// find up
		if (down == 0) {
			up = right;
		}
		while (getcross(arr[down], arr[down + 1], arr[up]) <= getcross(arr[down], arr[down + 1], arr[up + 1])) {
			up = (up + 1) % top;
		}

		// find down
		if (down == 0) {
			left = up;
		}
		while (getdot(arr[down], arr[down + 1], arr[left]) >= getdot(arr[down], arr[down + 1], arr[left + 1])) {
			left = (left + 1) % top;
		}

		dist = getdist(arr[down], arr[down + 1]);
		X = getcross(arr[down], arr[down + 1], arr[up]) / dist;
		temp.x = arr[right].x + arr[down].x - arr[left].x;
		temp.y = arr[right].y + arr[down].y - arr[left].y;
		Y = getdot(arr[down], arr[down + 1], temp);

		if (area > X*Y) {
			area = X*Y;
			downlast = down;
			rightlast = right;
			uplast = up;
			leftlast = left;
		}
	}

	// 计算外接矩形
	if (arr[downlast + 1].y == arr[downlast].y) {
		rectangle[0].x = arr[leftlast].x;
		rectangle[0].y = arr[downlast].y;

		rectangle[1].x = arr[rightlast].x;
		rectangle[1].y = arr[downlast].y;

		rectangle[2].x = arr[rightlast].x;
		rectangle[2].y = arr[uplast].y;

		rectangle[3].x = arr[leftlast].x;
		rectangle[3].y = arr[uplast].y;

	}
	else if (arr[downlast + 1].x == arr[downlast].x) {
		rectangle[0].x = arr[downlast].x;
		rectangle[0].y = arr[leftlast].y;

		rectangle[1].x = arr[downlast].x;
		rectangle[1].y = arr[rightlast].y;

		rectangle[2].x = arr[uplast].x;
		rectangle[2].y = arr[rightlast].y;

		rectangle[3].x = arr[uplast].x;
		rectangle[3].y = arr[leftlast].y;

	}
	else {
		k = (arr[downlast + 1].y - arr[downlast].y) / (arr[downlast + 1].x - arr[downlast].x);

		rectangle[0].x = (k*arr[leftlast].y + arr[leftlast].x - k*arr[downlast].y + k*k*arr[downlast].x) / (k*k + 1.0);
		rectangle[0].y = k*rectangle[0].x + arr[downlast].y - k*arr[downlast].x;

		rectangle[1].x = (k*arr[rightlast].y + arr[rightlast].x - k*arr[downlast].y + k*k*arr[downlast].x) / (k*k + 1.0);
		rectangle[1].y = k*rectangle[1].x + arr[downlast].y - k*arr[downlast].x;

		rectangle[2].x = (k*arr[rightlast].y + arr[rightlast].x - k*arr[uplast].y + k*k*arr[uplast].x) / (k*k + 1.0);
		rectangle[2].y = k*rectangle[2].x + arr[uplast].y - k*arr[uplast].x;

		rectangle[3].x = (k*arr[leftlast].y + arr[leftlast].x - k*arr[uplast].y + k*k*arr[uplast].x) / (k*k + 1.0);
		rectangle[3].y = k*rectangle[3].x + arr[uplast].y - k*arr[uplast].x;
	}
}

void 
data_analysis(POINT *xy, int len, POINT *pxy)
{
	int i;
	for (i = 0; i < len; i++) {
		if (xy[i].x > 28)xy[i].x = 28;
		if (xy[i].x < -28)xy[i].x = -28;
		if (xy[i].y > 28)xy[i].y = 28;
		if (xy[i].y < -28)xy[i].y = -28;
	}

	POINT b[4];
	memset(b, 0, sizeof(b));

	POINT *xy_temp;
	xy_temp = (POINT*)malloc(len*sizeof(POINT));
	memset(xy_temp, 0, len * sizeof(POINT));
	memcpy(xy_temp,xy,sizeof(POINT)*len);
	rotatingcalipers(xy_temp, len, b);
	free(xy_temp);
	xy_temp = NULL;
	for (i = 0; i < 4; i++) {
		printf("[%f, %f] ", b[i].x, b[i].y);
	}
	POINT max_y, min_x,min_y;
	max_y.y = b[0].y;
	max_y.x = b[0].x;
	min_x.x = b[0].x;
	min_x.y = b[0].y;
	min_y.y = b[0].y;
	min_y.x = b[0].x;
	for (i = 0; i < 4; i++) {
		if (max_y.y < b[i].y) {
			max_y.y = b[i].y;
			max_y.x = b[i].x;
		}

		if (min_x.x > b[i].x) {
			min_x.x = b[i].x;
			min_x.y = b[i].y;
		}

		if (min_y.y > b[i].y) {
			min_y.y = b[i].y;
			min_y.x = b[i].x;
		}
	}

	float sin_yaw = (max_y.x - min_x.x) / getdist(min_x, max_y);
	float cos_yaw = (max_y.y - min_x.y) / getdist(min_x, max_y);
	printf("sin yaw=%f,cos yaw=%f\n", sin_yaw, cos_yaw);
	for (i = 0; i < len; i++) {
		pxy[i].x = xy[i].x*cos_yaw - xy[i].y*sin_yaw;
		pxy[i].y = xy[i].y*cos_yaw + xy[i].x*sin_yaw;

	}

	POINT p_max_y, p_min_x, p_min_y;
	p_max_y.x = max_y.x*cos_yaw - max_y.y*sin_yaw;
	p_max_y.y= max_y.y*cos_yaw + max_y.x*sin_yaw;

	p_min_x.x = min_x.x*cos_yaw - min_x.y*sin_yaw;
	p_min_x.y = min_x.y*cos_yaw + min_x.x*sin_yaw;

	p_min_y.x = min_y.x*cos_yaw - min_y.y*sin_yaw;
	p_min_y.y = min_y.y*cos_yaw + min_y.x*sin_yaw;

	if ((p_max_y.y - p_min_x.y) > (p_min_y.x - p_min_x.x)) {
		for (i = 0; i < len; i++) {
			float temp;
			temp = pxy[i].x;
			pxy[i].x = pxy[i].y;
			pxy[i].y = temp;

			if (p_min_x.x < 0)pxy[i].x -= p_min_x.x - 0.000001f;
			if (p_min_x.y < 0)pxy[i].y -= p_min_x.y - 0.000001f;
		}
	}
	else {
		printf("1111\n");
		for (i = 0; i < len; i++) {
			if (p_min_x.x < 0)pxy[i].x -= p_min_x.x-0.000001f;
			if (p_min_x.y < 0)pxy[i].y -= p_min_x.y-0.000001f;
		}
	}

}

/*int
main(int argc, char *argv[])
{
	int n, i, length;
	POINT a[] = { { 1.0, 2.0 },{ 2.0, 0.5 },{ 2.5, 1.0 },{ 2.0, 0.0 },{ 4.0, 2.0 } };
	POINT b[4];
	length = sizeof(a) / sizeof(POINT);
	rotatingcalipers(a, length, b);
	for (i = 0; i<4; i++) {
		printf("[%f, %f] ", b[i].x, b[i].y);
	}
	printf("\n");
}*/