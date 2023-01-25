/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#include "Matrix2.h"
#include "Maths.h"

using namespace NCL;
using namespace NCL::Maths;

Matrix2::Matrix2(void)	{
	array[0][0] = 1.0f;
	array[0][1] = 0.0f;
	array[1][0] = 0.0f;
	array[1][1] = 1.0f;
}

Matrix2::Matrix2(float elements[4]) {
	array[0][0] = elements[0];
	array[0][1] = elements[1];
	array[1][0] = elements[2];
	array[1][1] = elements[3];
}

Matrix2::Matrix2(const Vector2& a, const Vector2& b) {
	array[0][0] = a.x;
	array[0][1] = b.x;
	array[1][0] = a.y;
	array[1][1] = b.y;
}

void Matrix2::ToZero() {
	array[0][0] = 0.0f;
	array[0][1] = 0.0f;
	array[1][0] = 0.0f;
	array[1][1] = 0.0f;
}

Matrix2 Matrix2::Rotation(float degrees)	{
	Matrix2 mat;

	float radians = Maths::DegreesToRadians(degrees);
	float s = sin(radians);
	float c = cos(radians);

	mat.array[0][0] = c;
	mat.array[0][1] = s;
	mat.array[1][0] = -s;
	mat.array[1][1] = c;

	return mat;
}

void Matrix2::Invert() {
	float determinant = (array[0][0] * array[1][1]) - (array[0][1] * array[1][0]);
	float invDet = 1.0f / determinant; //Turn our divides into multiplies!

	array[0][0] = array[1][1] * invDet;
	array[0][1] = -array[1][0] * invDet;
	array[1][0] = -array[0][1] * invDet;
	array[1][1] = array[0][0] * invDet;
}

Matrix2 Matrix2::Inverse() const {
	Matrix2 newMatrix = *this;
	newMatrix.Invert();
	return newMatrix;
}