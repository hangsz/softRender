#include "transform.h"
#include <iostream>

Transform::Transform(){
	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		mat_[i][j] = 0.0;
		
}
Transform::Transform(const Transform& transform){
	const float(*mat)[4] = transform[0];
	
	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		mat_[i][j] = mat[i][j];
} 

Transform& Transform::operator=(const Transform& transform){
	if (this != &transform) {
		const float(*mat)[4] = transform[0];
		for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			mat_[i][j] = mat[i][j];
	}
	return *this;
}

float(*Transform::operator[](int index))[4] {
	return mat_ + index;
}

const float(*Transform::operator[](int index) const)[4] {
	return mat_ + index;
}

Transform Transform::operator*(const Transform& transform)  {
	Transform temp;
	float(*mat)[4] = temp[0];
	const float(*mat2)[4] = transform[0];

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		mat[i][j] =  mat_[i][0] * mat2[0][j] + \
		             mat_[i][1] * mat2[1][j] + \
		             mat_[i][2] * mat2[2][j] + \
		             mat_[i][3] * mat2[3][j];
	return temp;
}

void Transform::operator*(Point& point) const{
	float val[4];
	for (int i = 0; i < 4;++i)
	    val[i] =  mat_[i][0] * point.x() + \
		          mat_[i][1] * point.y() + \
		          mat_[i][2] * point.z() + \
		          mat_[i][3] * point.w();

	point = Point(val[0], val[1], val[2], val[3]);
}

void Transform::operator*(Vector& vector) const{}

void Transform::identity(){

	std::cout << "identity" << std::endl;
	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		mat_[i][j] = 0.0f;

	mat_[0][0] = mat_[1][1] = mat_[2][2] = mat_[3][3] = 1.0f;
	std::cout << "identity" << std::endl;
}

void Transform::scale(float x, float y, float z){
	identity();
	mat_[0][0] = x;
	mat_[1][1] = y;
	mat_[2][2] = z;
}
void Transform::scale(const Vector& vector){
	identity();
	mat_[0][0] = vector.x();
	mat_[1][1] = vector.y();
	mat_[2][2] = vector.z();
}
void Transform::translate(float x, float y, float z){
	identity();
	mat_[0][3] = x;
	mat_[1][3] = y;
	mat_[2][3] = z;
}
void Transform::translate(const Vector& vector){
	identity();
	mat_[0][3] = vector.x();
	mat_[1][3] = vector.y();
	mat_[2][3] = vector.z();
}

//transform: first type - from point to point 
void Transform::rotate(const Vector& vector, float angle){
	float ux = vector.x() / vector.magnitute();
	float uy = vector.y() / vector.magnitute();
	float uz = vector.z() / vector.magnitute();

	float c = cos(angle / 180.0f*PI);
	float s = sin(angle / 180.0f*PI);

	mat_[0][0] = c + (1.0f - c) * ux*ux;
	mat_[0][1] = (1.0f - c)*ux*uy - s*uz;
	mat_[0][2] = (1.0f - c)*ux*uz + s*uy;
	mat_[0][3] = 0.0;

	mat_[1][0] = (1.0f - c)*ux*uy + s*uz;
	mat_[1][1] = c + (1.0f - c) * uy*uy;
	mat_[1][2] = (1.0f - c)*uy*uz - s*ux;
	mat_[1][3] = 0.0;

	mat_[2][0] = (1.0f - c)*ux*uz - s*uy;
	mat_[2][1] = (1.0f - c)*uy*uz + s*ux;
	mat_[2][2] = c + (1.0f - c) * uz*uz;
	mat_[2][3] = 0.0;

	mat_[3][0] = 0.0;
	mat_[3][1] = 0.0;
	mat_[3][2] = 0.0;
	mat_[3][3] = 1.0f;

}


// determine the transform from view coordinates to world coordinates.
void Transform::view(const Point& eye, const Point&at, const Vector& up){
	Vector ncoor = eye - at;
	Vector ucoor = up.cross(ncoor);
	Vector vcoor = ncoor.cross(ucoor);

	ncoor.normalize();
	ucoor.normalize();
	vcoor.normalize();

	Vector tmp = eye - Point(0.0, 0.0, 0.0, 1.0f);

	float dx = -tmp.dot(ucoor);
	float dy = -tmp.dot(vcoor);
	float dz = -tmp.dot(ncoor);


	mat_[0][0] = ucoor.x();
	mat_[0][1] = ucoor.y();
	mat_[0][2] = ucoor.z();
	mat_[0][3] = dx;

	mat_[1][0] = vcoor.x();
	mat_[1][1] = vcoor.y();
	mat_[1][2] = vcoor.z();
	mat_[1][3] = dy;

	mat_[2][0] = ncoor.x();
	mat_[2][1] = ncoor.y();
	mat_[2][2] = ncoor.z();
	mat_[2][3] = dz;

	mat_[3][0] = 0.0;
	mat_[3][1] = 0.0;
	mat_[3][2] = 0.0;
	mat_[3][3] = 1.0f;
}

// determine the transform from viewing volume coordinates to view coordinates.
// vv is a cuboid, cvv is a cube whose edge' length equals to 2. 
void Transform::perspective(float angle, float aspect, float near, float far){

	near = -near;
	far = -far;
	float top = near*tan(0.5f*angle / 180.0f*PI);

	float bottom = -top;
	float right = top*aspect;
	float left = -right;

	mat_[0][0] = 2.0f*near / (right - left);
	mat_[0][1] = 0.0;
	mat_[0][2] = (right + left) / (right - left);
	mat_[0][3] = 0.0;

	mat_[1][0] = 0.0;
	mat_[1][1] = 2.0f*near / (top - bottom);
	mat_[1][2] = (top + bottom) / (top - bottom);
	mat_[1][3] = 0.0;

	mat_[2][0] = 0.0;
	mat_[2][1] = 0.0;
	mat_[2][2] = -(far + near) / (far - near);
	mat_[2][3] = -2.0*far*near / (far - near);

	mat_[3][0] = 0.0;
	mat_[3][1] = 0.0;
	mat_[3][2] = -1.0f;
	mat_[3][3] = 0.0;
}

void Transform::screen(int width, int height){
	identity();
	mat_[0][0] = width / 2;
	mat_[0][3] = (width - 1) / 2;		//all the numbers are int type.
	mat_[1][1] = height / 2;		//-1 edges is ignored.
	mat_[1][3] = (height - 1) / 2;
}




