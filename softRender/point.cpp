
#include "point.h"

Point::Point(const Point& point){
	x_ = point.x();
	y_ = point.y();
	z_ = point.z();
	w_ = point.w();
}
Point& Point::operator=(const Point& point){
	if (this != &point) {
		x_ = point.x();
		y_ = point.y();
		z_ = point.z();
		w_ = point.w();
	}
	return *this;
}

void Point::lerp(const Point& p1, const Point& p2, const Point& p3, float beta, float gamma){

	x_ = (1.0f - beta - gamma)*p1.x() + beta*(p2.x() - p1.x()) + gamma*(p3.x() - p1.x());
	y_ = (1.0f - beta - gamma)*p1.y() + beta*(p2.y() - p1.y()) + gamma*(p3.y() - p1.y());
	z_ = (1.0f - beta - gamma)*p1.z() + beta*(p2.z() - p1.z()) + gamma*(p3.z() - p1.z());
	w_ = (1.0f - beta - gamma)*p1.w() + beta*(p2.w() - p1.w()) + gamma*(p3.w() - p1.w());

}

