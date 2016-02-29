#ifndef TRILATERATION_H_
#define TRILATERATION_H_

/* Largest nonnegative number still considered zero */
#define   MAXZERO  0.0

typedef struct coordinate coordinate;
struct coordinate {
	float x;
	float y;
	float z;
};
/* Return the difference of two vectors, (vector1 - vector2). */
void vdiff( coordinate vector1,  coordinate vector2, coordinate * result);

/* Return the sum of two vectors. */
void vsum( coordinate vector1,  coordinate vector2, coordinate * result);

/* Multiply vector by a number. */
void vmul(coordinate vector, coordinate * result, float n);

/* Divide vector by a number. */
void vdiv( coordinate * vector,  float n);

/* Return the Euclidean norm. */
void vnorm( coordinate vector, float * result);

/* Return the dot product of two vectors. */
void dot( coordinate vector1,  coordinate vector2, float * result);

/* Replace vector with its cross product with another vector. */
void cross( coordinate vector1,  coordinate vector2, coordinate * result);

void computeDistance(struct coordinate a, struct coordinate b, float *distance);

void vadd(coordinate * vector,int n);

/* Return zero if successful, negative error otherwise.
 * The last parameter is the largest nonnegative number considered zero;
 * it is somewhat analoguous to machine epsilon (but inclusive).
 */
int trilateration(coordinate *  result1, coordinate *  result2,
		coordinate p1,  float r1,  coordinate p2,
		float r2,  coordinate p3,  float r3,
		float maxzero);

struct coordinate getResult( coordinate result1,  coordinate result2, coordinate oldPosition);

void test(coordinate terms[], coordinate * oldTarget, coordinate * target);


#endif /* TRILATERATION_H_ */
