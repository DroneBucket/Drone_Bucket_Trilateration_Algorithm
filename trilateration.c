#include "trilateration.h"
/* Return the difference of two vectors, (vector1 - vector2). */
coordinate vdiff(const coordinate vector1, const coordinate vector2)
{
	coordinate v;
	v.x = vector1.x - vector2.x;
	v.y = vector1.y - vector2.y;
	v.z = vector1.z - vector2.z;
	return v;
}

/* Return the sum of two vectors. */
coordinate vsum(const coordinate vector1, const coordinate vector2)
{
	coordinate v;
	v.x = vector1.x + vector2.x;
	v.y = vector1.y + vector2.y;
	v.z = vector1.z + vector2.z;
	return v;
}

/* Multiply vector by a number. */
coordinate vmul(const coordinate vector, const double n)
{
	coordinate v;
	v.x = vector.x * n;
	v.y = vector.y * n;
	v.z = vector.z * n;
	return v;
}

/* Divide vector by a number. */
coordinate vdiv(const coordinate vector, const double n)
{
	coordinate v;
	v.x = vector.x / n;
	v.y = vector.y / n;
	v.z = vector.z / n;
	return v;
}

/* Return the Euclidean norm. */
double vnorm(const coordinate vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

/* Return the dot product of two vectors. */
double dot(const coordinate vector1, const coordinate vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

/* Replace vector with its cross product with another vector. */
coordinate cross(const coordinate vector1, const coordinate vector2)
{
	coordinate v;
	v.x = vector1.y * vector2.z - vector1.z * vector2.y;
	v.y = vector1.z * vector2.x - vector1.x * vector2.z;
	v.z = vector1.x * vector2.y - vector1.y * vector2.x;
	return v;
}

/* Return zero if successful, negative error otherwise.
 * The last parameter is the largest nonnegative number considered zero;
 * it is somewhat analoguous to machine epsilon (but inclusive).
 */
int trilateration(coordinate *const result1, coordinate *const result2,
		const coordinate p1, const double r1,
		const coordinate p2, const double r2,
		const coordinate p3, const double r3,
		const double maxzero)
{
	coordinate	ex, ey, ez, t1, t2;
	double	h, i, j, x, y, z, t;

	/* h = |p2 - p1|, ex = (p2 - p1) / |p2 - p1| */
	ex = vdiff(p2, p1);
	h = vnorm(ex);
	if (h <= maxzero) {
		/* p1 and p2 are concentric. */
		return -1;
	}
	ex = vdiv(ex, h);

	/* t1 = p3 - p1, t2 = ex (ex . (p3 - p1)) */
	t1 = vdiff(p3, p1);
	i = dot(ex, t1);
	t2 = vmul(ex, i);

	/* ey = (t1 - t2), t = |t1 - t2| */
	ey = vdiff(t1, t2);
	t = vnorm(ey);
	if (t > maxzero) {
		/* ey = (t1 - t2) / |t1 - t2| */
		ey = vdiv(ey, t);

		/* j = ey . (p3 - p1) */
		j = dot(ey, t1);
	} else
		j = 0.0;

	/* Note: t <= maxzero implies j = 0.0. */
	if (fabs(j) <= maxzero) {
		/* p1, p2 and p3 are colinear. */

		/* Is point p1 + (r1 along the axis) the intersection? */
		t2 = vsum(p1, vmul(ex, r1));
		if (fabs(vnorm(vdiff(p2, t2)) - r2) <= maxzero &&
				fabs(vnorm(vdiff(p3, t2)) - r3) <= maxzero) {
			/* Yes, t2 is the only intersection point. */
			if (result1)
				*result1 = t2;
			if (result2)
				*result2 = t2;
			return 0;
		}

		/* Is point p1 - (r1 along the axis) the intersection? */
		t2 = vsum(p1, vmul(ex, -r1));
		if (fabs(vnorm(vdiff(p2, t2)) - r2) <= maxzero &&
				fabs(vnorm(vdiff(p3, t2)) - r3) <= maxzero) {
			/* Yes, t2 is the only intersection point. */
			if (result1)
				*result1 = t2;
			if (result2)
				*result2 = t2;
			return 0;
		}

		return -2;
	}

	/* ez = ex x ey */
	ez = cross(ex, ey);

	x = (r1*r1 - r2*r2) / (2*h) + h / 2;
	y = (r1*r1 - r3*r3 + i*i) / (2*j) + j / 2 - x * i / j;
	z = r1*r1 - x*x - y*y;
	if (z < -maxzero) {
		/* The solution is invalid. */
		return -3;
	} else
		if (z > 0.0)
			z = sqrt(z);
		else
			z = 0.0;

	/* t2 = p1 + x ex + y ey */
	t2 = vsum(p1, vmul(ex, x));
	t2 = vsum(t2, vmul(ey, y));

	/* result1 = p1 + x ex + y ey + z ez */
	if (result1)
		*result1 = vsum(t2, vmul(ez, z));

	/* result1 = p1 + x ex + y ey - z ez */
	if (result2)
		*result2 = vsum(t2, vmul(ez, -z));

	return 0;
}

struct coordinate getResult(const coordinate result1, const coordinate result2, const coordinate oldPosition) {
	if (result1.z < 0) {
		return result2;
	}
	else if (result2.z < 0) {
		return result1;
	}
	else {
		double dist1 = sqrt(pow(result1.x - oldPosition.x,2) + pow(result1.y - oldPosition.y, 2) + pow(result1.z - oldPosition.z, 2));
		double dist2 = sqrt(pow(result2.x - oldPosition.x,2) + pow(result2.y - oldPosition.y, 2) + pow(result2.z - oldPosition.z, 2));
		if (dist1 > dist2) {
			return result2;
		}
		else {
			return result1;
		}
	}
}
