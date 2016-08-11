#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

class Vector{
public:
	Vector() :x_(0.0), y_(0.0), z_(0.0), w_(0.0){}
	explicit Vector(float x, float y, float z):x_(x), y_(y), z_(z), w_(0.0f){}
	Vector(const Vector& vector);
	~Vector() = default;
	
	Vector& operator= (const Vector& vector);

	Vector Vector::operator+(const Vector& vector) const{ 
		return Vector(x_ + vector.x(), y_ + vector.y(), z_ + vector.z());
	}
	Vector Vector::operator-(const Vector& vector) const{
		return Vector(x_ - vector.x(), y_ - vector.y(), z_ - vector.z());
	}
	void Vector::operator+=(const Vector& vector) {
		*this = *this + vector;
	}
	void Vector::operator-=(const Vector& vector){
		*this = *this - vector;
	}

	// scale multiply
	//Vector  operator*(const float scale) const { return Vector(x_ *scale, y_ *scale, z_ *scale); }
	
	// magnitute
	float magnitute() const { return 	sqrt(x_*x_ + y_*y_ + z_*z_); }
	// normalize
	void normalize(){ 
		float mag = magnitute();
		x_ /= mag; y_ /= mag; z_ /= mag;
	}

	// dot product
	float   dot(const Vector& vector) const{
		return x_*vector.x() + y_*vector.y() + z_*vector.z();
	}
	// cross product
	Vector cross(const Vector& vector) const;


	float x() const { return x_; }
	float y() const { return y_; }
	float z() const { return z_; }
	void x(float x){ x_ = x; }
	void y(float y){ y_ = y; }
	void z(float z){ z_ = z; }

private:
	float x_, y_, z_, w_;
};

#endif