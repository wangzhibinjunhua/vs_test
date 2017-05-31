#pragma once
#ifndef __MAT_H
#define  __MAT_H
typedef struct point_s {
	float x;
	float y;
} POINT;

void
rotatingcalipers(POINT *arr, int len, POINT *rectangle);
#endif
