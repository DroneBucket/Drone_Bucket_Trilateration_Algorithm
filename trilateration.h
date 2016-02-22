#ifndef TRILATERATION_H_
#define TRILATERATION_H_

/* Largest nonnegative number still considered zero */
#define   MAXZERO  0.0

typedef struct coordinate coordinate;
struct coordinate {
	float  x;
	float  y;
	float  z;
};

/* Return the difference of two vectors, (vector1 - vector2). */
coordinate vdiff(const coordinate vector1, const coordinate vector2);

/* Return the sum of two vectors. */
coordinate vsum(const coordinate vector1, const coordinate vector2);

/* Multiply vector by a number. */
coordinate vmul(const coordinate vector, const float  n);

coordinate vadd(const coordinate vector, const float  n);

/* Divide vector by a number. */
coordinate vdiv(const coordinate vector, const float  n);

/* Return the Euclidean norm. */
float  vnorm(const coordinate vector);

/* Return the dot product of two vectors. */
float  dot(const coordinate vector1, const coordinate vector2);

/* Replace vector with its cross product with another vector. */
coordinate cross(const coordinate vector1, const coordinate vector2);

int trilateration(coordinate * const result1, coordinate * const result2,
		const coordinate p1, const float  r1, const coordinate p2,
		const float  r2, const coordinate p3, const float  r3,
		const float  maxzero);

struct coordinate getResult(const coordinate result1, const coordinate result2,
		const coordinate oldPosition);


#endif /* TRILATERATION_H_ */

