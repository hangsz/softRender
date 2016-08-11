#include "transform.h"

#define PI  3.141926


void Transform::identity(){

	std::cout << "identity" << std::endl;
	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)		
		pmat_[i][j] = 0.0f;
		
	pmat_[0][0]= pmat_[1][1] = pmat_[2][2] = pmat_[3][3] = 1.0f;
	std::cout << "identity" << std::endl;
}

void Transform::scale(float x, float y, float z){
	identity();
	pmat_[0][0] = x;
	pmat_[1][1] = y;
	pmat_[2][2] = z;
}
void Transform::scale(const Vector& vector){
	identity();
	pmat_[0][0] = vector.x();
	pmat_[1][1] = vector.y();
	pmat_[2][2] = vector.z();
}
void Transform::translate(float x, float y, float z){
	identity();
	pmat_[0][3] = x;
	pmat_[1][3] = y;
	pmat_[2][3] = z;
}
void Transform::translate(const Vector& vector){
	identity();
	pmat_[0][3] = vector.x();
	pmat_[1][3] = vector.y();
	pmat_[2][3] = vector.z();
}

//transform: first type - from point to point 
void Transform::rotate(const Vector& vector, float angle){
	float ux = vector.x() / vector.magnitute();
	float uy = vector.y() / vector.magnitute();
	float uz = vector.z() / vector.magnitute();

	float c = cos(angle / 180.0f*PI);
	float s = sin(angle / 180.0f*PI);

	pmat_[0][0] = c + (1.0f - c) * ux*ux;
	pmat_[0][1] = (1.0f - c)*ux*uy - s*uz;
	pmat_[0][2] = (1.0f - c)*ux*uz + s*uy;
	pmat_[0][3] = 0.0;

	pmat_[1][0] = (1.0f - c)*ux*uy + s*uz;
	pmat_[1][1] = c + (1.0f - c) * uy*uy;
	pmat_[1][2] = (1.0f - c)*uy*uz - s*ux;
	pmat_[1][3] = 0.0;

	pmat_[2][0] = (1.0f - c)*ux*uz - s*uy;
	pmat_[2][1] = (1.0f - c)*uy*uz + s*ux;
	pmat_[2][2] = c + (1.0f - c) * uz*uz;
	pmat_[2][3] = 0.0;

	pmat_[3][0] = 0.0;
	pmat_[3][1] = 0.0;
	pmat_[3][2] = 0.0;
	pmat_[3][3] = 1.0f;

}


// determine the matrix from view coordinates to world coordinates.
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


	pmat_[0][0] = ucoor.x();
	pmat_[0][1] = ucoor.y();
	pmat_[0][2] = ucoor.z();
	pmat_[0][3] = dx;

	pmat_[1][0] = vcoor.x();
	pmat_[1][1] = vcoor.y();
	pmat_[1][2] = vcoor.z();
	pmat_[1][3] = dy;

	pmat_[2][0] = ncoor.x();
	pmat_[2][1] = ncoor.y();
	pmat_[2][2] = ncoor.z();
	pmat_[2][3] = dz;

	pmat_[3][0] = 0.0;
	pmat_[3][1] = 0.0;
	pmat_[3][2] = 0.0;
	pmat_[3][3] = 1.0f;
}

// determine the matrix from viewing volume coordinates to view coordinates.
// vv is a cuboid, cvv is a cube whose edge' length equals to 2. 
void Transform::perspective(float angle, float aspect, float near, float far){

	near = -near;
	far = -far;
	float top = near*tan(0.5f*angle / 180.0f*PI);
	
	float bottom = -top;
	float right = top*aspect;
	float left = -right;

	pmat_[0][0] = 2.0f*near / (right - left);
	pmat_[0][1] = 0.0;
	pmat_[0][2] = (right + left) / (right - left);
	pmat_[0][3] = 0.0;

	pmat_[1][0] = 0.0;
	pmat_[1][1] = 2.0f*near / (top - bottom);
	pmat_[1][2] = (top + bottom) / (top - bottom);
	pmat_[1][3] = 0.0;	  

	pmat_[2][0] = 0.0;
	pmat_[2][1] = 0.0;
	pmat_[2][2] = -(far + near) / (far - near);
	pmat_[2][3] = -2.0*far*near / (far - near);

	pmat_[3][0] = 0.0;
	pmat_[3][1] = 0.0;
	pmat_[3][2] = -1.0f;
	pmat_[3][3] = 0.0;
}

void Transform::screen(int width, int height){
	identity();
	pmat_[0][0] = width / 2;
	pmat_[0][3] = (width-1)/2;		//all the numbers are int type.
	pmat_[1][1] = height / 2;		//-1 edges is ignored.
	pmat_[1][3] = (height - 1) / 2;
}

Point& Transform::operator*(Point& point) const{
	return Matrix::operator*(point);
}

Vector& Transform::operator*(Vector& vector) const{
	return Matrix::operator*(vector);
}

