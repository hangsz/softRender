#include "matrix.h"
#include <iostream>

Matrix::Matrix(){
	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)//{
		//std::cout << i << j << std::endl;	
		mat_[i][j] = 0.0;
	//}
		
}
Matrix::Matrix(const Matrix& matrix){
	const float(*mat)[4] = matrix[0];
	
	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		mat_[i][j] = mat[i][j];
} 

Matrix& Matrix::operator=(const Matrix& matrix){
	const float(*mat)[4] = matrix[0];
	if (this != &matrix) {
		for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			mat_[i][j] = mat[i][j];
	}
	return *this;
}

float(*Matrix::operator[](int index))[4] {
	return mat_ + index;
}

const float(*Matrix::operator[](int index) const)[4] {
	return mat_ + index;
}



Matrix& Matrix::operator*(const Matrix& matrix)  {
	
	Matrix temp = *this;

	float(*mat1)[4] = temp[0];
	const float(*mat2)[4] = matrix[0];

	for (int i = 0; i < 4; ++i)
	for (int j = 0; j < 4; ++j)
		mat_[i][j] = mat1[i][0] * mat2[0][j] + \
		             mat1[i][1] * mat2[1][j] + \
		             mat1[i][2] * mat2[2][j] + \
		             mat1[i][3] * mat2[3][j];
	return *this;
}

Point& Matrix::operator*(Point& point) const{

	float val[4];
	for (int i = 0; i < 4;++i)
	    val[i] =  mat_[i][0] * point.x() + \
		          mat_[i][1] * point.y() + \
		          mat_[i][2] * point.z() + \
		          mat_[i][3] * point.w();

	point = Point(val[0], val[1], val[2], val[3]);

	return point;
}

Vector& Matrix::operator*(Vector& vector) const{
	return vector;
}


