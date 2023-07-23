#pragma once

#include "typedefs.h"
#include <cmath>

//Normalization of a given point/vector preserving signage
void normalize(float p[3]) {
	float d = 0.0f;
	int i;
	for (i = 0; i < 3; i++) {
		d += p[i] * p[i];
	}
	d = (float)sqrt((double)d);
	for (i = 0; i < 3; i++) {
		p[i] /= d;
	}
}

/*
*	Suppose three points p0, p1, p2.
*	The first parameter is assigned the cross product:
*		(p2 - p1) x (p0 - p1)
*	The next parameters are the coordinates of the three points.
*	To get the outer product of a triangle's sides, they must be
*	inserted clockwise in order for it to point out of the object.
*/
void cross_product(vector3f out,
	float p00, float p01, float p02,
	float p10, float p11, float p12,
	float p20, float p21, float p22)
{
	vector3f v2 = { p20 - p10, p21 - p11, p22 - p12 };
	vector3f v1 = { p00 - p10, p01 - p11, p02 - p12 };

	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void cross_product(vector3f out,
	point3f p0, point3f p1, point3f p2)
{
	vector3f v2 = { p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2] };
	vector3f v1 = { p0[0] - p1[0], p0[1] - p1[1], p0[2] - p1[2] };

	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void cross_product(vector3f out, vector3f v1, vector3f v2)
{
	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void multiply(float p[3], float c)
{
	p[0] *= c;
	p[1] *= c;
	p[2] *= c;
}