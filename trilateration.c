#include <stdio.h>
#include <math.h>

#include "trilateration.h"

/* Return the difference of two vectors, (vector1 - vector2). */
void vdiff( coordinate vector1,  coordinate vector2, coordinate * result) {
	result->x = vector1.x - vector2.x;
	result->y = vector1.y - vector2.y;
	result->z = vector1.z - vector2.z;
}

/* Return the sum of two vectors. */
void vsum( coordinate vector1,  coordinate vector2, coordinate * result) {
	result->x = vector1.x + vector2.x;
	result->y = vector1.y + vector2.y;
	result->z = vector1.z + vector2.z;
}

/* Multiply vector by a number. */
void vmul(coordinate vector, coordinate * result, float n) {
	result->x = vector.x * n;
	result->y = vector.y * n;
	result->z = vector.z * n;
}

/* Divide vector by a number. */
void vdiv( coordinate * vector,  float n) {
	vector->x = vector->x / n;
	vector->y = vector->y / n;
	vector->z = vector->z / n;
}

/* Return the Euclidean norm. */
void vnorm( coordinate vector, float * result) {
	*result = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

/* Return the dot product of two vectors. */
void dot( coordinate vector1,  coordinate vector2, float * result) {
	*result = vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

/* Replace vector with its cross product with another vector. */
void cross( coordinate vector1,  coordinate vector2, coordinate * result) {
	result->x = vector1.y * vector2.z - vector1.z * vector2.y;
	result->y = vector1.z * vector2.x - vector1.x * vector2.z;
	result->z = vector1.x * vector2.y - vector1.y * vector2.x;
}

/*Calculate the distance between the point a an the point b*/
void computeDistance(struct coordinate a, struct coordinate b, float *distance) {
	*distance = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

/*add n for each value of a vector*/
void vadd(coordinate * vector,int n) {
	vector->x = vector->x + n;
	vector->y = vector->y + n;
	if (vector->z + n < 0) {
		vector->z = vector->z + fabs(n);
	} else {
		vector->z = vector->z + n;
	}
}

/* Return zero if successful, negative error otherwise.
 * The last parameter is the largest nonnegative number considered zero;
 * it is somewhat analoguous to machine epsilon (but inclusive).
 */
int trilateration(coordinate *  result1, coordinate *  result2,
		coordinate p1,  float r1,  coordinate p2,
		float r2,  coordinate p3,  float r3,
		float maxzero) {
	static coordinate ex, ey, ez;
	static float h, i, j;

	/* d = |p2 - p1|, ex = (p2 - p1) / |p2 - p1| */
	vdiff(p2, p1, &ex);
	vnorm(ex, &h);
	if (h <= maxzero) {
		/* p1 and p2 are concentric. */
		return -1;
	}
	vdiv(&ex, h);

	/* t1 = p3 - p1, t2 = ex (ex . (p3 - p1)) */
	vdiff(p3, p1,&ey);
	dot(ex, ey, &i);
	vmul(ex, &ez, i);

	/* ey = (t1 - t2), t = |t1 - t2| */
	vdiff(ey, ez, &ey);
	vnorm(ey, &j);
	if (j > maxzero) {
		/* ey = (t1 - t2) / |t1 - t2| */
		vdiv(&ey, j);

		/* j = ey . (p3 - p1) */
		vdiff(p3, p1, &ez);
		dot(ey, ez, &j);
	} else
		j = 0.0;

	/* Note: t <= maxzero implies j = 0.0. */
	//	if (fabs(j) <= maxzero) {
	/* p1, p2 and p3 are colinear. */

	/* Is point p1 + (r1 along the axis) the intersection? */
	/*vsum(p1, vmul(ex, r1), &ez);
		if (fabs(vnorm(vdiff(p2, ez)) - r2) <= maxzero
				&& fabs(vnorm(vdiff(p3, ez)) - r3) <= maxzero) {
			/* Yes, t2 is the only intersection point. */
	/*	if (result1)
	 *result1 = ez;
			if (result2)
	 *result2 = ez;
			return 0;
		}*/

	/* Is point p1 - (r1 along the axis) the intersection? */
	/*ez = vsum(p1, vmul(ex, -r1));
		if (fabs(vnorm(vdiff(p2, ez)) - r2) <= maxzero
				&& fabs(vnorm(vdiff(p3, ez)) - r3) <= maxzero) {*/
	/* Yes, t2 is the only intersection point. */
	/*if (result1)
	 *result1 = ez;
			if (result2)
	 *result2 = ez;
			return 0;
		}*/

	/*	return -2;
	}*/

	/* ez = ex x ey */
	cross(ex, ey,&ez);

	h = (r1 * r1 - r2 * r2) / (2 * h) + h / 2;
	j = (r1 * r1 - r3 * r3 + i * i) / (2 * j) + j / 2 - h * i / j;
	i = r1 * r1 - h * h - j * j;
	if (i < -maxzero) {
		/* The solution is invalid. */
		return -3;
	} else if (i > 0.0)
		i = sqrt(i);
	else
		i = 0.0;

	/* t2 = p1 + x ex + y ey */
	vmul(ex,&p2, h);
	vsum(p1, p2,result1);
	vmul(ey,result2, j);
	vsum(*result1,*result2 ,result1);
	*result2 = *result1;
	vmul(ez,&p1,i);
	/* result1 = p1 + x ex + y ey + z ez */
	vsum(*result1, p1 ,result1);
	vmul(ez,&p1, -i);
	/* result1 = p1 + x ex + y ey - z ez */
	vsum(*result2, p1, result2);

	return 0;
}

/*Choose the good result between two. The good result is selected by comparing the two results (which are passed as parameters ) with the old position */
struct coordinate getResult( coordinate result1,  coordinate result2,
		coordinate oldPosition) {
	if (result1.z < 0) {
		return result2;
	} else if (result2.z < 0) {
		return result1;
	} else {
		float dist1 = sqrt(
				pow(result1.x - oldPosition.x, 2)
				+ pow(result1.y - oldPosition.y, 2)
				+ pow(result1.z - oldPosition.z, 2));
		float dist2 = sqrt(
				pow(result2.x - oldPosition.x, 2)
				+ pow(result2.y - oldPosition.y, 2)
				+ pow(result2.z - oldPosition.z, 2));
		if (dist1 > dist2) {
			return result2;
		} else {
			return result1;
		}
	}
}

void test(coordinate terms[], coordinate * oldTarget, coordinate * target){
	static int  k;
	static float distance[3];
	static struct coordinate o1, o2;
	for (k = 0; k < 3; k++) {
		computeDistance(*target, terms[k], &distance[k]);
	}
	trilateration(&o1, &o2, terms[0], distance[0], terms[1], distance[1],
			terms[2], distance[2], MAXZERO);
	*oldTarget  = getResult(o1, o2, *target);
	int translate = target->x / 300;

	if ((translate % 2) == 0) {
		vadd(target, translate);
		vadd(&terms[0], translate);
		vadd(&terms[1], translate);
		vadd(&terms[2], translate);
	}
	else {
		vadd(target, -translate);
		vadd(&terms[0], -translate);
		vadd(&terms[1], -translate);
		vadd(&terms[2], -translate);
	}
}
