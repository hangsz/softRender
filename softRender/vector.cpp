#include "vector.h"


//Vector operator*(const float scale, const Vector& vector)  { return vector*scale; }

Vector::Vector(const Vector& vector){
	x_ = vector.x();
	y_ = vector.y();
	z_ = vector.z();
	//w_ = vector.w();
}
Vector& Vector::operator=(const Vector& vector){	
	if (this != &vector) {
	    x_ = vector.x();
	    y_ = vector.y();
	    z_ = vector.z();
	   //w = vector.w;	
	}
	return *this;
}

Vector Vector::cross(const Vector& vector) const{
	return Vector( y_*vector.z() - z_*vector.y(),  
		           z_*vector.x() - x_*vector.z(),
		           x_*vector.y() - y_*vector.x() ) ;
		  
}


