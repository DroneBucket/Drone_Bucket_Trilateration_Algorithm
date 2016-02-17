#include <stdio.h>
#include <math.h>

/* Largest nonnegative number still considered zero */
#define   MAXZERO  0.0

typedef struct coordinate coordinate;
struct coordinate {
	double	x;
	double	y;
	double	z;
};

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
coordinate vadd(const coordinate vector, const double n)
{
	coordinate v;
	v.x = vector.x + n;
	v.y = vector.y + n;
	if(vector.z + n < 0) {
		v.z = vector.z + fabs(n);
	}
	else {
		v.z = vector.z + n;
	}

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

double ComputeDistance(struct coordinate a, struct coordinate b) {
	double result  = 0.0;
	double x  = pow(a.x - b.x,2);
	double y  = pow(a.y - b.y,2);
	double z  = pow(a.z - b.z,2);
	result = sqrt(x + y + z);
	return result;
}

int main(void)
{
	/*coordinate	p1, p2, p3, o1, o2;
	double	r1, r2, r3;
	int	result;

	while (fscanf(stdin, "%lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg",
	                     &p1.x, &p1.y, &p1.z, &r1,
	                     &p2.x, &p2.y, &p2.z, &r2,
	                     &p3.x, &p3.y, &p3.z, &r3) == 12) {
		printf("Sphere 1: %g %g %g, radius %g\n", p1.x, p1.y, p1.z, r1);
		printf("Sphere 2: %g %g %g, radius %g\n", p2.x, p2.y, p2.z, r2);
		printf("Sphere 3: %g %g %g, radius %g\n", p3.x, p3.y, p3.z, r3);
		result = trilateration(&o1, &o2, p1, r1, p2, r2, p3, r3, MAXZERO);
		if (result)
			printf("No solution (%d).\n", result);
		else {
			printf("Solution 1: %g %g %g\n", o1.x, o1.y, o1.z);
			printf("  Distance to sphere 1 is %g (radius %g)\n", vnorm(vdiff(o1, p1)), r1);
			printf("  Distance to sphere 2 is %g (radius %g)\n", vnorm(vdiff(o1, p2)), r2);
			printf("  Distance to sphere 3 is %g (radius %g)\n", vnorm(vdiff(o1, p3)), r3);
			printf("Solution 2: %g %g %g\n", o2.x, o2.y, o2.z);
			printf("  Distance to sphere 1 is %g (radius %g)\n", vnorm(vdiff(o2, p1)), r1);
			printf("  Distance to sphere 2 is %g (radius %g)\n", vnorm(vdiff(o2, p2)), r2);
			printf("  Distance to sphere 3 is %g (radius %g)\n", vnorm(vdiff(o2, p3)), r3);
		}
	}*/
	srand(time(NULL));
	int maxValue = 320;
	struct coordinate target, terms[3], oldTarget;
	double distance[3];
	int i;
	struct coordinate	o1, o2, result;
	target.x = rand() % maxValue;
	target.y = rand() % maxValue;
	target.z = rand() % maxValue;
	printf(" %d \n" ,i);
	int count = 0;
	int k = 0;
	for(k = 0; k < 3; k++) {

		terms[k].x = rand() % maxValue;
		terms[k].y = rand() % maxValue;
		terms[k].z = rand() % maxValue;
		distance[k] = ComputeDistance(target,terms[k]);
		printf("generation = ( %g , %g , %g )\n", terms[k].x, terms[k].y, terms[k].z);

	}
	oldTarget = target;
	for(i = 0; i < 1000000; i++) {

		trilateration(&o1, &o2, terms[0], distance[0], terms[1], distance[1], terms[2], distance[2], MAXZERO);
		result = getResult(o1,o2, target);
		printf("res1 = ( %g , %g , %g )\n", o1.x, o1.y, o1.z);
		printf("res2 = ( %g , %g , %g )\n", o2.x, o2.y, o2.z);
		printf("roldTarget = ( %f , %f , %f )\n", oldTarget.x, oldTarget.y, oldTarget.z);
		printf("target = ( %f , %f , %f)\n", target.x, target.y, target.z);
		printf("result = ( %f , %f , %f )\n \n \n", result.x, result.y, result.z);

		if((round(result.x) == round(target.x)) && (round(result.y) == round(target.y)) && (round(result.z) == round(target.z))) {
			count++;
			printf(" === %d \n", count);
		}
		oldTarget = target;
		int translate = rand() % 1000;
		if((translate % 2) == 0 ) {
			target = vadd(target,translate);
			terms[0] = vadd(terms[0],translate);
			terms[1] = vadd(terms[1],translate);
			terms[2] = vadd(terms[2],translate);
			for(k = 0; k < 3; k++) {
				distance[k] = ComputeDistance(target,terms[k]);
			}

		}
		else {
			target = vadd(target,-translate);
			terms[0] = vadd(terms[0],-translate);
			terms[1] = vadd(terms[1],-translate);
			terms[2] = vadd(terms[2],-translate);
			for(k = 0; k < 3; k++) {
				distance[k] = ComputeDistance(target,terms[k]);
			}
		}


	}
	printf("%d reussis",count);

	return 0;
}
