#ifndef POINT_H
#define POINT_H

#include "vector.h"

class Point{
public:
	Point() :x_(0.0), y_(0.0), z_(0.0), w_(1.0f) {};
	Point(float x, float y, float z, float w) :x_(x), y_(y), z_(z), w_(w){}
	Point(const Point& point);
	~Point() = default;
	Point& operator= (const Point& point);

    Point operator+(const Vector& vector) const{
		return Point(x_ + w_*vector.x(), y_ + w_*vector.y(), z_ + w_*vector.z(),w_);
	}
	Point operator-(const Vector& vector) const{
		return Point(x_ - w_*vector.x(), y_ - w_*vector.y(), z_ - w_*vector.z(), w_);;
	}

	Vector operator-(const Point& point) const{
		return Vector( x_ / w_ - point.x() / point.w(),
			           y_ / w_ - point.y() / point.w(),
			           z_ / w_ - point.z() / point.w());	
	}
	void operator+=(const Vector& vector) {
		*this = *this + vector;
	}
	void operator-=(const Vector& vector) {
		*this = *this - vector;
	}

	float x() const{ return x_; }
	float y() const{ return y_; }
	float z() const{ return z_; }	
	float w() const{ return w_; }
	void x(float x){ x_ = x; } 
	void y(float y){ y_ = y; }
	void z(float z){ z_ = z; }
	void w(float w){ w_ = w; }

	void normalize(){ x_ /= w_; y_ /= w_; z_ /= w_; w_=1.0f; }

	void lerp(const Point& p1, const Point& p2, const Point& p3, float beta,float gamma);

private:
	float x_, y_, z_, w_;
};

#endif