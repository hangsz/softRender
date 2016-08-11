
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"
#include <iostream>

class Transform:public Matrix {
public:
	Transform() :Matrix() { 
		std::cout << "Transform" << std::endl;
		pmat_ = Matrix::operator[](0);
		//identity();
	}
	Transform(const Transform& trans): Matrix(trans) { 
		pmat_ = Matrix::operator[](0);
	}

	Transform& operator=(const Transform& transform){
		Matrix::operator=(transform);
		pmat_ = Matrix::operator[](0);
		return *this;
	}
	Transform& operator*(const Transform& transform){
		Matrix::operator*(transform);
		pmat_ = Matrix::operator[](0);
		return *this;
	}

	Point& Transform::operator*(Point& point) const;
	Vector& Transform::operator*(Vector& vector) const;

	float* operator[](int index){ return pmat_[0] + index;}
	const float* operator[](int index) const{ return pmat_[0] + index; }


	void identity();

	void scale(float sx, float sy, float sz);
	void scale(const Vector& vector);
	void translate(float tx, float ty, float tz);
	void translate(const Vector& vector);
	void rotate(const Vector& vector, float angle);

	//perspective * view(look at) * world 

	// model coordinates can be determined by a position( translate) and a vector and an angle(rotate).
	void world(){ identity(); };

	// determine the matrix from view coordinates to world coordinates.
	void view(const Point& eye, const Point&at, const Vector& up);

	// determine the matrix from viewing volume coordinates to view coordinates.
	// vv is a cuboid, cvv is a cube whose edge' length equals to 2. 
	void perspective(float angle, float aspect, float near, float far);

	void screen(int width, int height);

	

private:

	float (*pmat_)[4];

};

#endif
