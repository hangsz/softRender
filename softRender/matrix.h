#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include "point.h"

class Matrix{

public:
	Matrix();
	Matrix(const Matrix& matrix);
	~Matrix() = default;
	Matrix& operator=(const Matrix& matrix);

	
	float (*operator[](int index))[4] ;
	const float(*operator[](int index) const) [4] ;

	// 	chain
	Matrix& operator*(const Matrix& matrix) ;

	Point& operator*( Point& point) const;

	Vector& operator*(Vector& vector) const;

private:
	float mat_[4][4];

};



#endif