#ifndef TRILATERATION_H_
#define TRILATERATION_H_

/* Largest nonnegative number still considered zero */
#define   MAXZERO  0.0

typedef struct coordinate coordinate;
struct coordinate {
	double x;
	double y;
	double z;
};

/* Return the difference of two vectors, (vector1 - vector2). */
coordinate vdiff(const coordinate vector1, const coordinate vector2);

/* Return the sum of two vectors. */
coordinate vsum(const coordinate vector1, const coordinate vector2);

/* Multiply vector by a number. */
coordinate vmul(const coordinate vector, const double n);

coordinate vadd(const coordinate vector, const double n);

/* Divide vector by a number. */
coordinate vdiv(const coordinate vector, const double n);

/* Return the Euclidean norm. */
double vnorm(const coordinate vector);

/* Return the dot product of two vectors. */
double dot(const coordinate vector1, const coordinate vector2);

/* Replace vector with its cross product with another vector. */
coordinate cross(const coordinate vector1, const coordinate vector2);

int trilateration(coordinate * const result1, coordinate * const result2,
		const coordinate p1, const double r1, const coordinate p2,
		const double r2, const coordinate p3, const double r3,
		const double maxzero);

struct coordinate getResult(const coordinate result1, const coordinate result2,
		const coordinate oldPosition);


#endif /* TRILATERATION_H_ */

