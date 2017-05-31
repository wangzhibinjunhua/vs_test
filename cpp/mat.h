#pragma once
#ifndef __MAT_H
#define  __MAT_H
typedef struct point_s {
	float x;
	float y;
} POINT;

POINT *ori_data;
void
rotatingcalipers(POINT *arr, int len, POINT *rectangle);
void
data_analysis(POINT *xy, int len, POINT *pxy);
#endif
