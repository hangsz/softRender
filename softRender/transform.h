#ifndef Transform_H
#define Transform_H

#include "vector.h"
#include "point.h"

static const float PI = 3.1415926f;

class Transform{

public:
	Transform();
	Transform(const Transform& transform);
	~Transform() = default;
	Transform& operator=(const Transform& transform);

	float (*operator[](int index))[4] ;
	const float(*operator[](int index) const) [4] ;

	Transform operator*(const Transform& transform) ;

	void operator*( Point& point) const;

	void operator*(Vector& vector) const;

	//
	void identity();

	void scale(float sx, float sy, float sz);
	void scale(const Vector& vector);
	void translate(float tx, float ty, float tz);
	void translate(const Vector& vector);
	void rotate(const Vector& vector, float angle);

	// model coordinates can be determined by a position( translate) and a vector and an angle(rotate).
	void world(){ identity(); };

	// determine the transform from view coordinates to world coordinates.
	void view(const Point& eye, const Point&at, const Vector& up);

	// determine the transform from viewing volume coordinates to view coordinates.
	// vv is a cuboid, cvv is a cube whose edge' length equals to 2. 
	void perspective(float angle, float aspect, float near, float far);

	void screen(int width, int height);

private:
	float mat_[4][4];

};



#endif