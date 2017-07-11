#ifndef MATH_UTILITY_H
#define MATH_UTILITY_H

#include "geometry.h"

template <typename T>
T determinant(const Vector3<T> &a, const Vector3<T> &b, const Vector3<T> &c)
{
	T ret = a[0] * (b[1] * c[2] - c[1] * b[2])
		  - a[1] * (b[0] * c[2] - c[0] * b[2])
		  + a[2] * (b[0] * c[1] - c[0] * b[1]);

	return ret;
}

// According to Camer's rule
template <typename T>
Vector3<T> solveLinearEquation(const Vector3<T> &a, const Vector3<T> &b, const Vector3<T> &c, const Vector3<T> &d)
{
	T x, y, z, A, e;
	A = determinant(a, b, c);
	e = determinant(d, b, c); x = e / A;
	e = determinant(a, d, c); y = e / A;
	e = determinant(a, b, d); z = e / A;

	return Vector3<T>(x,y,z);
}
#endif //end file 
